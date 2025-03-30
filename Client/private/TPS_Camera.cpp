#include "TPS_Camera.h"

CTPS_Camera::CTPS_Camera(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCamera{ pGraphic_Device }
{
}

CTPS_Camera::CTPS_Camera(const CTPS_Camera& Prototype)
	: CCamera(Prototype)
{
}

HRESULT CTPS_Camera::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTPS_Camera::Initialize(void* pArg)
{
	if (pArg == nullptr)
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CTPS_Camera::Priority_Update(_float fTimeDelta)
{
	if (KEY_PRESSING(DIK_A))
	{
		m_pTransformCom->Go_Left(fTimeDelta);
	}
	if (KEY_PRESSING(DIK_D))
	{
		m_pTransformCom->Go_Right(fTimeDelta);
	}
	if (KEY_PRESSING(DIK_W))
	{
		m_pTransformCom->Go_Straight(fTimeDelta);
	}
	if (KEY_PRESSING(DIK_S))
	{
		m_pTransformCom->Go_Backward(fTimeDelta);
	}

	if (KEY_PRESSING(DIK_UP))
	{
		m_pTransformCom->Go_Up(fTimeDelta);
	}

	Bind_Resource();
}

EVENT CTPS_Camera::Update(_float fTimeDelta)
{
	return EVENT();
}

void CTPS_Camera::Late_Update(_float fTimeDelta)
{
	//m_pGameInstance->Add_RenderGroup(CRenderer::RG_PRIORITY, this);
}

HRESULT CTPS_Camera::Render()
{
	if (FAILED(m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE)))
		return E_FAIL;

	return S_OK;
}

void CTPS_Camera::Update_Projection_Matrix()
{
	m_ProjMatrix.MakePerspectiveProjMat(m_fFov, m_fAspect, m_fNear, m_fFar);
}

HRESULT CTPS_Camera::Ready_Components(void* pArg)
{
	CTransform::DESC desc = {};
	desc.fSpeedPerSec = 300.f;
	desc.fRotationPerSec = 10.f;

	/* For.Prototype_Component_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &desc)))
		return E_FAIL;

	return S_OK;
}

CTPS_Camera* CTPS_Camera::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTPS_Camera* pInstance = new CTPS_Camera(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTPS_Camera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CCamera* CTPS_Camera::Clone(void* pArg)
{
	CTPS_Camera* pInstance = new CTPS_Camera(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CTPS_Camera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTPS_Camera::Free()
{
	__super::Free();
}
