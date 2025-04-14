#include "Gravity.h"
#include "Transform.h"
#include "GameInstance.h"
#include "Collider.h"

// 현재 이 그래비티와 연결된 트랜스폼의 Y 포지션
#define CUR_Y m_pTransformCom->m_WorldMatrix._42

// 기울기에 따른 Y축 보정
#define Y_UPPER_CORRECTION 20.f
#define Y_LOWER_CORRECTION 30.f

// 바닥 감지시 오차 보정
#define FLOOR_CORRECTION 5.f

vector<_uint>		CGravity::m_StandableColliderGroupID = {};

// 높이맵 기반 터레인의 흔적
#ifdef _GRV_TER
const _float3* CGravity::m_pTerrainVtxPos = { nullptr };
_uint					CGravity::m_iTerrainVtxNumX = {};
_uint					CGravity::m_iTerrainVtxNumZ = {};
_float3					CGravity::m_vTerrainScale = { 1.f,1.f,1.f };

void CGravity::Set_GravityStaticInfo(const DESC& Desc)
{
	m_pTerrainVtxPos = Desc.pTerrainVtxPos;
	m_iTerrainVtxNumX = Desc.iTerrainVtxNumX;
	m_iTerrainVtxNumZ = Desc.iTerrainVtxNumZ;
	m_vTerrainScale = Desc.vTerrainScale;
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
		m_fFloorY = fCurFloorY + m_fHalfHeight;

}

#endif

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
	{
		MSG_BOX("Gravity에 nullptr을 넣어주면 어떡해...");
		return E_FAIL;
	}

	DESC* pDesc = static_cast<DESC*>(pArg);
	m_pTransformCom = pDesc->pTransformCom;
	m_pTransformCom->AddRef();
	m_fHalfHeight = m_pTransformCom->Compute_Scaled().y * 0.5f;
	m_fTimeIncreasePerSec = pDesc->fTimeIncreasePerSec;
	m_fMaxFallSpeedperSec = pDesc->fMaxFallSpeedPerSec;

	return S_OK;
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

_float3 CGravity::Get_Terrain_Front_Dir()
{
	const _float3& vLook = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);

	_float3 vFrontOnTerrain = vLook - m_vCurNormal * vLook.Dot(m_vCurNormal);
	vFrontOnTerrain.Normalize();

	return vFrontOnTerrain;
}

_float3 CGravity::Get_Terrain_Right_Dir()
{
	const _float3& vRight = *m_pTransformCom->Get_State(CTransform::STATE_RIGHT);

	_float3 vRightOnTerrain = vRight - m_vCurNormal * vRight.Dot(m_vCurNormal);
	vRightOnTerrain.Normalize();

	return vRightOnTerrain;
}

void CGravity::Update(_float fTimeDelta)
{
	//Check_Terrain();
	m_bJumpEndFlag = FALSE;
	Raycast_StandAble_Obj();
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
		//m_bForceSetFloor = FALSE;
	}
}

void CGravity::Update_Height()
{
	m_fHalfHeight = reinterpret_cast<_float3*>(&m_pTransformCom->m_WorldMatrix._21)->Length() * 0.5f;
}

void CGravity::Raycast_StandAble_Obj()
{
	_float fResult{};
	for (auto& ID : m_StandableColliderGroupID)
	{
		fResult = m_pGameInstance->Raycast_Downward(
			*m_pTransformCom->Get_State(CTransform::STATE_POSITION) - _float3{ 0.f, m_fHalfHeight - FLOOR_CORRECTION ,0.f },
			ID);
		if (fResult > GRV_MIN_HEIGHT)
			m_vCurNormal = CCollider::Get_Last_Collision_Depth();	
	}
	m_fFloorY = fResult + m_fHalfHeight;
}

void CGravity::Jumping(_float fTimeDelta)
{
	if (m_bJump)
	{
		// 최대 낙하 속력 계산
		_float fMaxFallSpeed = m_fMaxFallSpeedperSec * fTimeDelta;

		// 시간 증가
		m_fTime += m_fTimeIncreasePerSec * fTimeDelta;

		// 현재 Y 위치 계산 (자유낙하 공식 적용)
		_float fCurrentY = m_fStartY + (m_fJumpPower * m_fTime) - (G_FORCE * SQUARE(m_fTime)) * 0.5f;

		// 최대 낙하 속도 초과 방지
		_float fFallSpeed = (fCurrentY - CUR_Y); // 현재 프레임에서 이동해야 할 거리 (속도 개념)

		// 최대 낙하 속도를 초과하면 제한
		if (fFallSpeed < -fMaxFallSpeed)
		{
			CUR_Y -= fMaxFallSpeed; // 최대 낙하 속도로 제한
		}
		else
		{
			CUR_Y = fCurrentY; // 정상 낙하
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
				m_bJumpEndFlag = TRUE;
				CUR_Y = m_fFloorY;
			}
		}
	}
	else if (m_fFloorY < CUR_Y) // 낙하
	{
 	//	if (abs(CUR_Y - m_fFloorY) < Y_LOWER_CORRECTION) // 기울기 보정
		//{
		//	CUR_Y = m_fFloorY;
		//}
		//else
		//{
			m_bJump = true;
			m_fTime = 0.f;
			m_fStartY = CUR_Y;
			m_fJumpPower = 0.f;
		//}
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
