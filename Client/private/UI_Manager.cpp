// 내 클래스 이름 : UI_Manager
// 부모 클래스 이름 : Base

#include "UI_Manager.h"
#include "Client_Defines.h"
#include "Font_MediumBlue.h"
#include "FadeUI.h"

CUI_Manager* CUI_Manager::m_pInstance = nullptr;
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
		//m_pInstance->Free();
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

	Safe_Release(m_pGameInstance);
 	for (size_t i = 0; i < CFont::FONT_END; i++)
	{
		if (m_Fonts[i] != nullptr)
			Safe_Release(m_Fonts[i]);
	} 
	Safe_Release(m_FadeUI);
} 