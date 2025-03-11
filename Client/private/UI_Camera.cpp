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

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CUI_Camera::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CUI_Camera::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CUI_Camera::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this);
}

HRESULT CUI_Camera::Render() // UI설정 전 필요한 각종 셋팅 몰아두기
{
	if (FAILED(Bind_Resource()))
		return E_FAIL;
	if (FAILED(m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE)))
		return E_FAIL;
	if (FAILED(m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 40)))
		return E_FAIL;
	if (FAILED(m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER)))
		return E_FAIL;

	return S_OK;
}

void CUI_Camera::Update_Projection_Matrix()
{
	m_ProjMatrix.MakeOrthoProjMat(FWINCX, FWINCY, m_fNear, m_fFar);
}

HRESULT CUI_Camera::Ready_Components(void* pArg)
{
	/* For.Prototype_Component_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom),pArg)))
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
		MSG_BOX("Failed to Clone : CUI_Camera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Camera::Free()
{
	__super::Free();
}
