#include "UI.h"

CUI::CUI(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CUI::CUI(const CUI& Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CUI::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI::Initialize(void* pArg)
{
	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (pArg != nullptr)
	{
		DESC* pDesc = static_cast<DESC*>(pArg);
		m_vPos = pDesc->vInitPos;
		m_vPos.z = 0.f;
		m_vSize = pDesc->vScale;
		m_vSize.z = 1.f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vPos);
		m_pTransformCom->Scaling(m_vSize);
	}
	return S_OK;
}

void CUI::Priority_Update(_float fTimeDelta)
{
}

EVENT CUI::Update(_float fTimeDelta)
{
	return EVN_NONE;
}

void CUI::Late_Update(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this)))
		return;
}

HRESULT CUI::Render()
{
	if(FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Resource(static_cast<_uint>(m_fTextureNum))))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI::Ready_Components(void* pArg)
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(m_eLevelID, _wstring(TEXT("Prototype_Component_Texture_")) + m_szTextureID,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, _wstring(TEXT("Prototype_Component_VIBuffer_")) + m_szBufferType,
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), pArg)))
		return E_FAIL;

	return S_OK;
}

void CUI::Update_Rect()
{
	// 이게 정말 맞는 걸까요?
	m_tRect.left	= LONG(m_vPos.x - (m_vSize.x * 0.5f));
	m_tRect.top		= LONG(m_vPos.y - (m_vSize.y * 0.5f));
	m_tRect.right	= LONG(m_vPos.x + (m_vSize.x * 0.5f));
	m_tRect.bottom	= LONG(m_vPos.y + (m_vSize.y * 0.5f));
}

void CUI::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
}