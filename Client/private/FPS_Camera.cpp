// �� Ŭ���� �̸� : FPS_Camera
// �θ� Ŭ���� �̸� : Camera

#include "FPS_Camera.h"

CFPS_Camera::CFPS_Camera(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCamera{ pGraphic_Device }
{
}

CFPS_Camera::CFPS_Camera(const CFPS_Camera& Prototype)
	: CCamera(Prototype)
{
}

HRESULT CFPS_Camera::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFPS_Camera::Initialize(void* pArg)
{
	if (pArg == nullptr)
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	DESC* pDesc = static_cast<DESC*>(pArg);
	m_fMouseSensor = pDesc->fMouseSensor;
	m_vInitPos = pDesc->vEye;
	m_vInitLook = pDesc->vAt;

	return S_OK;
}

void CFPS_Camera::Priority_Update(_float fTimeDelta)
{
	if (KEY_DOWN(DIK_TAB))
	{
		m_bMouseFixMod = !m_bMouseFixMod;
		ShowCursor(!m_bMouseFixMod);
	}

	if (m_bMouseFixMod)
	{
		Mouse_Move();
		Mouse_Fix();
	}
	Bind_Resource();
}

EVENT CFPS_Camera::Update(_float fTimeDelta)
{
	return EVN_NONE;
}

void CFPS_Camera::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_PRIORITY, this);
}

HRESULT CFPS_Camera::Render()
{
	if (FAILED(m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE)))
		return E_FAIL;
	return S_OK;
}

void CFPS_Camera::Mouse_Move()
{
	_float		fMouseMoveX = { static_cast<_float>(m_pGameInstance->Get_DIMMoveState(DIMM_X)) };
	_float		fMouseMoveY = { static_cast<_float>(m_pGameInstance->Get_DIMMoveState(DIMM_Y)) };

	_float3		vRotationAxis = (*m_pTransformCom->Get_State(CTransform::STATE_RIGHT) * fMouseMoveY)
		+ (*m_pTransformCom->Get_State(CTransform::STATE_UP) * fMouseMoveX);

	_float fAngle = RADIAN(_float3(fMouseMoveX, fMouseMoveY, 0).Length() * m_fMouseSensor);

	_float3 vLook = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);

	_float4x4 matRot{ vRotationAxis,fAngle };
	vLook.TransformNormal(matRot);
	m_pTransformCom->LookAt(*m_pTransformCom->Get_State(CTransform::STATE_POSITION) + vLook);
}

void CFPS_Camera::Mouse_Fix()
{
	POINT		ptMouse{ g_iWinSizeX >> 1, g_iWinSizeY >> 1 };

	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}

void CFPS_Camera::StartShake(_float fIntensity, _float fDuration)
{
	m_fIntensity = fIntensity;
	m_fDuration = fDuration;
	m_fElapsed = 0.f;
}

void CFPS_Camera::Update_Projection_Matrix()
{
	m_ProjMatrix.MakePerspectiveProjMat(m_fFov, m_fAspect, m_fNear, m_fFar);
}

HRESULT CFPS_Camera::Ready_Components(void* pArg)
{
	/* For.Prototype_Component_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), pArg)))
		return E_FAIL;

	return S_OK;
}

CFPS_Camera* CFPS_Camera::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CFPS_Camera* pInstance = new CFPS_Camera(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CFPS_Camera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CCamera* CFPS_Camera::Clone(void* pArg)
{
	CFPS_Camera* pInstance = new CFPS_Camera(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CFPS_Camera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFPS_Camera::Free()
{
	__super::Free();
}