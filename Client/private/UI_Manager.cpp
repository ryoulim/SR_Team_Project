// 내 클래스 이름 : UI_Manager
// 부모 클래스 이름 : Base

#include "UI_Manager.h"
#include "Client_Defines.h"
#include "Font_MediumBlue.h"
#include "FadeUI.h"
#include "Player.h"
#include "Portrait.h"
#include "Armor.h"

CUI_Manager* CUI_Manager::m_pInstance = nullptr;

CUI_Manager* CUI_Manager::Get_Instance()
{
	if (nullptr == m_pInstance)
	{
		return nullptr;
	}
	return m_pInstance;
}

CUI_Manager* CUI_Manager::Get_Instance(CGameInstance* pGameInstance)
{	
	if (nullptr == pGameInstance)
		return nullptr;
	if (nullptr == m_pInstance)						
	{
		m_pInstance = new CUI_Manager;
		m_pInstance->Set_GameInstance(pGameInstance);
		m_pInstance->Initialize();
	}
	return m_pInstance;
}

unsigned int CUI_Manager::Destroy_Instance()	
{						
	unsigned int iRefCnt = {};						
	if (nullptr != m_pInstance)						
	{												
		m_pInstance->Free();
		iRefCnt = m_pInstance->Release();				
	if (0 == iRefCnt)								
		m_pInstance = nullptr;						
	}												
	return iRefCnt;									
}

CUI_Manager::CUI_Manager()
{
}

HRESULT CUI_Manager::Initialize()
{
	Initialize_Font();

	Initialize_FadeUI();

	return S_OK;
}

void CUI_Manager::Priority_Update(_float fTimeDelta)
{
}

void CUI_Manager::Update(_float fTimeDelta)
{
}

void CUI_Manager::Late_Update(_float fTimeDelta)
{
}

HRESULT CUI_Manager::Render()
{
	return S_OK;
}

void CUI_Manager::Fade_In()
{
	CFadeUI::DESC desc;
	desc.fRotationPerSec = 0.f;
	desc.fSpeedPerSec = 0.f;
	desc.vInitPos = { 0.f,0.f,0.f };
	desc.vScale = { 1280.f,720.f,1.f };
	desc.m_isFadeIn = true;
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_FadeUI"),
		LEVEL_STATIC, L"Layer_FadeUI",&desc )))
		return;
}

void CUI_Manager::Fade_Out()
{
	CFadeUI::DESC desc;
	desc.fRotationPerSec = 0.f;
	desc.fSpeedPerSec = 0.f;
	desc.vInitPos = { 0.f,0.f,0.f };
	desc.vScale = { 1280.f,720.f,1.f };
	desc.m_isFadeIn = false;
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_FadeUI"),
		LEVEL_STATIC, L"Layer_FadeUI", &desc)))
		return;
}

HRESULT CUI_Manager::Initialize_GamePlayUI()
{
	m_GameUIs[GUI_AIM]		= m_pGameInstance->Find_Object(LEVEL_GAMEPLAY, L"Layer_UI", GUI_AIM);
	m_GameUIs[GUI_PORTRAIT] = m_pGameInstance->Find_Object(LEVEL_GAMEPLAY, L"Layer_UI", GUI_PORTRAIT);
	m_GameUIs[GUI_ARMOR]	= m_pGameInstance->Find_Object(LEVEL_GAMEPLAY, L"Layer_UI", GUI_ARMOR);
	m_GameUIs[GUI_AMMO]		= m_pGameInstance->Find_Object(LEVEL_GAMEPLAY, L"Layer_UI", GUI_AMMO);

	m_pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Find_Object(LEVEL_GAMEPLAY, L"Layer_Pawn"));
	if (nullptr == m_pPlayer)
		return E_FAIL;
	Safe_AddRef(m_pPlayer);

	for (size_t i = 0; i < GUI_END; i++)
	{
		if (nullptr == m_GameUIs[i])
			return E_FAIL;
		Safe_AddRef(m_GameUIs[i]);
	}

	return S_OK;
}

HRESULT CUI_Manager::Clear_GamePlayUI()
{
	for (size_t i = 0; i < GUI_END; i++)
	{
		Safe_Release(m_GameUIs[i]);
		m_GameUIs[i] = nullptr;
	}
	Safe_Release(m_pPlayer);
	m_pPlayer = nullptr;
	return S_OK;
}

HRESULT CUI_Manager::Update_GameUI(GAMEUI eUIType, CAmmo::AMMOTYPE eAmmoType)
{
	switch (eUIType)
	{
	case Client::CUI_Manager::GUI_AIM:
		break;
	case Client::CUI_Manager::GUI_PORTRAIT:
		static_cast<CPortrait*>(m_GameUIs[GUI_PORTRAIT])->Set_HP(/*m_pPlayer->Get_HP()*/rand()%100);
		break;
	case Client::CUI_Manager::GUI_ARMOR:
		static_cast<CArmor*>(m_GameUIs[GUI_ARMOR])->Set_Armor(/*m_pPlayer->Get_Armor()*/rand()%100);
		break;
	case Client::CUI_Manager::GUI_AMMO:
		static_cast<CAmmo*>(m_GameUIs[GUI_AMMO])->Set_Ammo(eAmmoType, /*m_pPlayer->Get_Ammo()*/rand() % 80);
		break;
	case Client::CUI_Manager::GUI_END:
		break;
	default:
		break;
	}
	return S_OK;
}

HRESULT CUI_Manager::Initialize_Font()
{
	m_Fonts[CFont::MEDIUMBLUE] = dynamic_cast<CFont*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_GAMEOBJECT, LEVEL_STATIC, TEXT("Prototype_GameObject_Font_MediumBlue")));
	if (nullptr == m_Fonts[CFont::MEDIUMBLUE])
		return E_FAIL;
	m_Fonts[CFont::BIGORANGE] = dynamic_cast<CFont*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_GAMEOBJECT, LEVEL_STATIC, TEXT("Prototype_GameObject_Font_BigOrange")));
	if (nullptr == m_Fonts[CFont::BIGORANGE])
		return E_FAIL;
	return S_OK;
}

HRESULT CUI_Manager::Initialize_FadeUI()
{
	m_FadeUI = dynamic_cast<CFadeUI*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_GAMEOBJECT, LEVEL_STATIC, TEXT("Prototype_GameObject_FadeUI")));
	if (nullptr == m_FadeUI)
		return E_FAIL;

	return S_OK;
}


void CUI_Manager::Free()
{
	__super::Free();

 	for (size_t i = 0; i < CFont::FONT_END; i++)
	{
		if (m_Fonts[i] != nullptr)
			Safe_Release(m_Fonts[i]);
	} 
	Safe_Release(m_FadeUI);
	Clear_GamePlayUI();
	Safe_Release(m_pGameInstance);

} 