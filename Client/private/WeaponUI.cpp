#include "WeaponUI.h"
#include "GameInstance.h"
#include <UI_Manager.h>

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
	Update_WeaponSettings(); // 플레이어 키 인풋이 지금 Priority에 있으므로 Update에 위치해둠

	return __super::Update(fTimeDelta);
}

void CWeaponUI::Late_Update(_float fTimeDelta)
{
	if (m_bRender)
	{
		__super::Late_Update(fTimeDelta);
	}
}

HRESULT CWeaponUI::Render()
{
	//for (auto& Weapon : CUI_Manager::Get_Instance()->Get_Weapon())
	//{
	//	if (FAILED(Weapon->Render()))
	//		return E_FAIL;
	//}

	// 이전 무기(위쪽)
	int prevIndex = (currentIndex - 1 + weaponList.size()) % weaponList.size();
	RenderTexture(weaponList[prevIndex].texture, centerX, centerY - offsetY, scale = 1.0f, opacity = 0.5f);

	// 선택된 무기(중앙)
	RenderTexture(weaponList[currentIndex].texture, centerX, centerY, scale = 1.1f, opacity = 1.0f);

	// 다음 무기(아래쪽)
	int nextIndex = (currentIndex + 1) % weaponList.size();
	RenderTexture(weaponList[nextIndex].texture, centerX, centerY + offsetY, scale = 1.0f, opacity = 0.5f);

	return S_OK;
}

HRESULT CWeaponUI::Update_WeaponSettings()
{

	return S_OK;
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