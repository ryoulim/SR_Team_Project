// 내 클래스 이름 : UI_Manager
// 부모 클래스 이름 : Base

#include "UI_Manager.h"
#include "Client_Defines.h"
#include "Font_MediumBlue.h"

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
	//임시 추가
	m_Fonts[CFont::MEDIUMBLUE] = dynamic_cast<CFont*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_GAMEOBJECT, LEVEL_STATIC, TEXT("Prototype_GameObject_Font_MediumBlue")));
	if (nullptr == m_Fonts[CFont::MEDIUMBLUE])
		return E_FAIL;
	m_Fonts[CFont::BIGORANGE] = dynamic_cast<CFont*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_GAMEOBJECT, LEVEL_STATIC, TEXT("Prototype_GameObject_Font_BigOrange")));
	if (nullptr == m_Fonts[CFont::BIGORANGE])
		return E_FAIL;
	//m_Fonts[CFont::BIGSILVER] = dynamic_cast<CFont*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_GAMEOBJECT, LEVEL_STATIC, TEXT("Prototype_GameObject_Font_BigSilver")));
	//if (nullptr == m_Fonts[CFont::BIGSILVER])
	//	return E_FAIL;
	//m_Fonts[CFont::TINYBLUE] = dynamic_cast<CFont*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_GAMEOBJECT, LEVEL_STATIC, TEXT("Prototype_GameObject_Font_TinyBlue")));
	//if (nullptr == m_Fonts[CFont::TINYBLUE])
	//	return E_FAIL;




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

void CUI_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pGameInstance);
 	for (size_t i = 0; i < CFont::FONT_END; i++)
	{
		if (m_Fonts[i] != nullptr)
			Safe_Release(m_Fonts[i]);
	}
} 