#include "Level_Loading.h"

#include "Level_GamePlay.h"
#include "Level_Race.h"
#include "Level_Test.h"
#include "Level_Logo.h"
#include "Loader.h"

#include "GameInstance.h"
#include <LoadingMenu.h>
#include "UI_Manager.h"

CLevel_Loading::CLevel_Loading(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel { pGraphic_Device }
{

}

HRESULT CLevel_Loading::Initialize(LEVEL eNextLevelID)
{
	m_eNextLevelID = eNextLevelID;

	/* 로딩레벨 자체에 필요한 객체를 생성한다. */
	/* 배경, 로딩바, 버튼, font */

	/* 로딩의 역할(다음레벨에 필요한 자원(Resource)(텍스쳐, 모델, 사운드 등등등 )을 생성하는)을 
	수행할 로더객체를 생성한다. */

	m_pLoader = CLoader::Create(m_pGraphic_Device, m_eNextLevelID);
	if (nullptr == m_pLoader)
		return E_FAIL;

	
	if (eNextLevelID == LEVEL_LOGO)
	{
		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_LoadingMenu"),
			LEVEL_LOADING, TEXT("Layer_UI"))))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_LevelLoadingMenu"),
			LEVEL_LOADING, TEXT("Layer_UI"))))
			return E_FAIL;
	}



	
	return S_OK;
}

void CLevel_Loading::Update(_float fTimeDelta)
{

	//if (KEY_DOWN(VK_SPACE))
	{
		if (true == m_pLoader->isFinished())
		{
			CLevel* pLevel = { nullptr };

			switch (m_eNextLevelID)
			{
			case LEVEL_LOGO:
				pLevel = CLevel_Logo::Create(m_pGraphic_Device, m_pLoader->Get_LevelData());
				break;

			case LEVEL_GAMEPLAY:
				pLevel = CLevel_GamePlay::Create(m_pGraphic_Device, m_pLoader->Get_LevelData());
				break;

			case LEVEL_RACE:
				pLevel = CLevel_Race::Create(m_pGraphic_Device, m_pLoader->Get_LevelData());
				break;

			case LEVEL_TEST:
				pLevel = CLevel_Test::Create(m_pGraphic_Device, m_pLoader->Get_LevelData());
				break;
			}

			if (nullptr == pLevel)
				return;

			if (FAILED(m_pGameInstance->Change_Level(m_eNextLevelID, pLevel)))
				return;
							
		}
	}	
}

HRESULT CLevel_Loading::Render()
{
	m_pLoader->Output_LoadingText();
	

	return S_OK;
}

CLevel_Loading* CLevel_Loading::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevelID)
{
	CLevel_Loading* pInstance = new CLevel_Loading(pGraphic_Device);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed to Created : CLevel_Loading");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);
}