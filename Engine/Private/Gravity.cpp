#include "Gravity.h"
#include "Transform.h"

// 현재 이 그래비티와 연결된 트랜스폼의 Y 포지션
#define CUR_Y m_pTransformCom->m_WorldMatrix._42

// 기울기에 따른 Y축 보정
#define Y_UPPER_CORRECTION 20.f
#define Y_LOWER_CORRECTION 30.f

const _float3*  CGravity::m_pTerrainVtxPos = { nullptr };
_uint			CGravity::m_iTerrainVtxNumX = {};
_uint			CGravity::m_iTerrainVtxNumZ = {};
_float3			CGravity::m_vTerrainScale{1.f,1.f,1.f};

CGravity::CGravity(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CComponent{pGraphic_Device}
{
}

CGravity::CGravity(const CGravity& Prototype)
	:CComponent( Prototype )
{
}

HRESULT CGravity::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGravity::Initialize(void* pArg)
{
	if (pArg == nullptr)
		return E_FAIL;

	m_pTransformCom = static_cast<CTransform*>(pArg);
	m_pTransformCom->AddRef();
	m_fHeight = m_pTransformCom->Compute_Scaled().y;

	return S_OK;
}

void CGravity::Set_TerrainInfo(const DESC& Desc)
{
	m_pTerrainVtxPos = Desc.pTerrainVtxPos;
	m_iTerrainVtxNumX = Desc.iTerrainVtxNumX;
	m_iTerrainVtxNumZ = Desc.iTerrainVtxNumZ;
	m_vTerrainScale = Desc.vTerrainScale;
}

void CGravity::Go_Straight_On_Terrain(_float fTimedelta)
{
	_float3 vLook = m_pTransformCom->Get_State(CTransform::STATE_RIGHT)->Cross(m_vCurNormal);

	_float3 vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	vPos += vLook.Normalize() * fTimedelta * m_pTransformCom->m_fSpeedPerSec;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
}

void CGravity::Go_Backward_On_Terrain(_float fTimedelta)
{
	_float3 vDir = -m_pTransformCom->Get_State(CTransform::STATE_RIGHT)->Cross(m_vCurNormal);

	_float3 vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	vPos += vDir.Normalize() * fTimedelta * m_pTransformCom->m_fSpeedPerSec;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
}

void CGravity::Go_Left_On_Terrain(_float fTimedelta)
{
	_float3 vDir = m_pTransformCom->Get_State(CTransform::STATE_LOOK)->Cross(m_vCurNormal);

	_float3 vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	vPos += vDir.Normalize() * fTimedelta * m_pTransformCom->m_fSpeedPerSec;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
}

void CGravity::Go_Right_On_Terrain(_float fTimedelta)
{
	_float3 vDir = -m_pTransformCom->Get_State(CTransform::STATE_LOOK)->Cross(m_vCurNormal);

	_float3 vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	vPos += vDir.Normalize() * fTimedelta * m_pTransformCom->m_fSpeedPerSec;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
}

void CGravity::Set_JumpOption(const JUMPDESC& JumpDESC)
{
	m_fTimeIncreasePerSec = JumpDESC.fTimeIncreasePerSec;
	m_fMaxFallSpeedperSec = JumpDESC.fMaxFallSpeedPerSec;
}

void CGravity::Update(_float fTimeDelta)
{
	Check_Terrain();
	Jumping(fTimeDelta);
}

void CGravity::Jump(_float fJumpPower)
{
	if (!m_bJump)
	{
		m_bJump = TRUE;
		m_fStartY = CUR_Y;
		m_fTime = 0.f;
		m_fJumpPower = fJumpPower;
	}
}

void CGravity::Check_Terrain()
{
	m_fFloorY = 0.f;

	auto vPosition = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_uint	iIndex = _uint(vPosition.z / m_vTerrainScale.z) * m_iTerrainVtxNumX
		+ _uint(vPosition.x / m_vTerrainScale.x);

	_float	fRatioX = vPosition.x - m_pTerrainVtxPos[iIndex + m_iTerrainVtxNumX].x * m_vTerrainScale.x;
	_float	fRatioZ = m_pTerrainVtxPos[iIndex + m_iTerrainVtxNumX].z - vPosition.z * m_vTerrainScale.z;

	D3DXPLANE	Plane;

	_float3 VtxPos[4]{
		m_pTerrainVtxPos[iIndex + m_iTerrainVtxNumX] * m_vTerrainScale,
		m_pTerrainVtxPos[iIndex + m_iTerrainVtxNumX + 1] * m_vTerrainScale,
		m_pTerrainVtxPos[iIndex + 1] * m_vTerrainScale,
		m_pTerrainVtxPos[iIndex] * m_vTerrainScale
	};

	// 우상단
	if (fRatioX > fRatioZ)
	{
		D3DXPlaneFromPoints(&Plane,
			&VtxPos[0],
			&VtxPos[1],
			&VtxPos[2]);
	}

	else
	{
		D3DXPlaneFromPoints(&Plane,
			&VtxPos[0],
			&VtxPos[2],
			&VtxPos[3]);
	}
	
	Update_NormalVector(Plane);

	_float fCurFloorY = (-Plane.a * vPosition.x - Plane.c * vPosition.z - Plane.d) / Plane.b;

	if (vPosition.y + Y_UPPER_CORRECTION >= fCurFloorY)
		m_fFloorY = fCurFloorY + m_fHeight * 0.5f;

}

void CGravity::Jumping(_float fTimeDelta)
{
	if (m_bJump)
	{
		_float fMaxFallSpeed = m_fMaxFallSpeedperSec * fTimeDelta;

		m_fTime += m_fTimeIncreasePerSec * fTimeDelta;
		float fCerrentY = m_fStartY + (m_fJumpPower * m_fTime) - (G_FORCE * SQUARE(m_fTime)) * 0.5f; // 현재의 Y 위치 계산

		if (fCerrentY < m_fStartY - fMaxFallSpeed) // 최대속력 낙하
		{
			CUR_Y -= fMaxFallSpeed;
		}
		else // 그냥 낙하
		{
			CUR_Y = fCerrentY;
		}


		if (m_fFloorY >= CUR_Y) // 점프 중지
		{
			//if (m_bMax_Falling) // 최대속도로 땅에 부딪혔다면 한번 튕겨라
			//{
			//	m_fJumpPower = 25.f;
			//	m_fTime = 0.f;
			//	m_fStartY = m_fFloorY;
			//	m_bBounce = true;
			//}
			//else
			{
				m_bJump = false;
				CUR_Y = m_fFloorY;
			}
		}
	}
	else if (m_fFloorY < CUR_Y) // 낙하
	{
 		if (abs(CUR_Y - m_fFloorY) < Y_LOWER_CORRECTION) // 기울기 보정
		{
			CUR_Y = m_fFloorY;
		}
		else
		{
			m_bJump = true;
			m_fStartY = CUR_Y;
			m_fJumpPower = 0.f;
		}
	}
}

void CGravity::Update_NormalVector(const D3DXPLANE& Plane)
{
	if (m_vCurNormal != Plane)
	{
		m_vCurNormal = Plane;
		m_bNormalChanged = TRUE;
	}
	else
	{
		m_bNormalChanged = FALSE;
	}
}

CGravity* CGravity::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGravity* pInstance = new CGravity(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CGravity");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CGravity::Clone(void* pArg)
{
	CGravity* pInstance = new CGravity(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CGravity");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGravity::Free()
{
	__super::Free();
	Safe_Release(m_pTransformCom);
}
