#include "WeaponUI.h"
#include "GameInstance.h"

CWeaponUI::CWeaponUI(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI{ pGraphic_Device }
{
}

CWeaponUI::CWeaponUI(const CWeaponUI& Prototype)
	: CUI(Prototype)
{
}

HRESULT CWeaponUI::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWeaponUI::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_STATIC;
	m_szTextureID = TEXT("WeaponUI");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CWeaponUI::Ready_Components(void* pArg)
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, _wstring(TEXT("Prototype_Component_Texture_")) + m_szTextureID,
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

	Ready_ShaderComponent();

	return S_OK;
}

void CWeaponUI::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CWeaponUI::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CWeaponUI::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CWeaponUI::Render()
{

	return __super::Render();
}

CWeaponUI* CWeaponUI::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CWeaponUI* pInstance = new CWeaponUI(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CWeaponUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWeaponUI::Clone(void* pArg)
{
	CWeaponUI* pInstance = new CWeaponUI(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CWeaponUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWeaponUI::Free()
{
	__super::Free();
}