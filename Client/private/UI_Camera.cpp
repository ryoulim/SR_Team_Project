#include "UI_Camera.h"
#include "GameInstance.h"

CUI_Camera::CUI_Camera(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CCamera{pGraphicDevice}
{
}

CUI_Camera::CUI_Camera(const CUI_Camera& Prototype)
	:CCamera( Prototype )

{
}

HRESULT CUI_Camera::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Camera::Initialize(void* pArg)
{
	if (pArg == nullptr)
		return E_FAIL;

	m_CameraDesc = *static_cast<DESC*>(pArg);
	if (FAILED(Ready_Components()))
		return E_FAIL;

	//어짜피 항등행렬로 쓸거니..
	//m_pTransform->LookAt(m_CameraDesc.vAt);
	//m_pTransform->Set_State(CTransform::STATE_POSITION, m_CameraDesc.vEye);

	return S_OK;
}

void CUI_Camera::Priority_Update(_float fTimeDelta)
{
}

void CUI_Camera::Update(_float fTimeDelta)
{
}

void CUI_Camera::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this);
}

HRESULT CUI_Camera::Render() // UI설정 전 필요한 각종 셋팅 몰아두기
{
	if(FAILED(m_pTransform->Bind_View_Transform()))
		return E_FAIL;
	if (FAILED(Bind_Projection_Transform()))
		return E_FAIL;
	if (FAILED(m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE)))
		return E_FAIL;
	if (FAILED(m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 40)))
		return E_FAIL;
	if (FAILED(m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Camera::Bind_Projection_Transform()
{
	_float4x4 ProjMat{};

	D3DXMatrixOrthoLH(&ProjMat, g_iWinSizeX, g_iWinSizeY, m_CameraDesc.fNear, m_CameraDesc.fFar);

	return m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &ProjMat);
}

HRESULT CUI_Camera::Ready_Components()
{
	/* For.Prototype_Component_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransform))))
		return E_FAIL;

	return S_OK;
}

CUI_Camera* CUI_Camera::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_Camera* pInstance = new CUI_Camera(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Camera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CCamera* CUI_Camera::Clone(void* pArg)
{
	CUI_Camera* pInstance = new CUI_Camera(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Camera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Camera::Free()
{
	__super::Free();
}
