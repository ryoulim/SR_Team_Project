// 내 클래스 이름 : MainMenu
// 부모 클래스 이름 : UI

#include "MainMenu.h"
#include "GameInstance.h"

CMainMenu::CMainMenu(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI{ pGraphic_Device }
{
}

CMainMenu::CMainMenu(const CMainMenu& Prototype)
	: CUI(Prototype)
{
}

HRESULT CMainMenu::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMainMenu::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_STATIC;
	m_szTextureID = TEXT("MainMenu");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CMainMenu::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CMainMenu::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	return EVN_NONE;
}

void CMainMenu::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CMainMenu::Render()
{
	return __super::Render();
}

CMainMenu* CMainMenu::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMainMenu* pInstance = new CMainMenu(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMainMenu");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMainMenu::Clone(void* pArg)
{
	CMainMenu* pInstance = new CMainMenu(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CMainMenu");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainMenu::Free()
{
	__super::Free();

}