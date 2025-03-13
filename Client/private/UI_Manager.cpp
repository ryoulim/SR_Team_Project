// 내 클래스 이름 : UI_Manager
// 부모 클래스 이름 : Base

#include "UI_Manager.h"
#include "GameInstance.h"
#include "Client_Defines.h"
#include "Font_ItemDialog.h"

IMPLEMENT_SINGLETON(CUI_Manager);

CUI_Manager::CUI_Manager()
{
}

HRESULT CUI_Manager::Initialize(void* pArg)
{
	//m_pGameInstance = 




	//임시 추가
	//m_Fonts[0] = dynamic_cast<CFont*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_GAMEOBJECT, LEVEL_STATIC, TEXT("Prototype_GameObject_Font_ItemDialog")));
	//if (nullptr == m_Fonts[0])
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
}