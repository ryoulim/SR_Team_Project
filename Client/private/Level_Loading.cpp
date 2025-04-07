#include "Level_Loading.h"

#include "Level_GamePlay.h"
#include "Level_RaceFirst.h"
#include "Level_RaceSecond.h"
#include "Level_RaceThird.h"
#include "Level_Indoor.h"
#include "Level_OutDoor.h"
#include "Level_Logo.h"
#include "Level_Boss.h"
#include "Loader.h"

#include "GameInstance.h"
#include "LoadingMenu.h"
#include "LevelLoadingMenu.h"
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
		CUI::DESC BackGroundDesc{};
		BackGroundDesc.eLevelID = LEVEL_LOADING;
		BackGroundDesc.vInitPos = { 0.f,0.f,0.9f };
		BackGroundDesc.vScale = { FWINCX, FWINCY, 1.f };
		BackGroundDesc.fDepth = 10.f;
		//if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_LoadingMenu"),
		//	LEVEL_LOADING, TEXT("Layer_UI"), &BackGroundDesc)))
		//	return E_FAIL;
		if (FAILED(m_pGameInstance->Add_GameObjectReturn(LEVEL_STATIC, TEXT("Prototype_GameObject_LoadingMenu"),
			LEVEL_LOADING, TEXT("Layer_UI"), &m_pLoadingMenu, &BackGroundDesc)))
			return E_FAIL;
	}
	else
	{
		CUI::DESC BackGroundDesc{};
		BackGroundDesc.eLevelID = LEVEL_LOADING;
		BackGroundDesc.vInitPos = { 0.f,0.f,0.9f };
		BackGroundDesc.vScale = { FWINCX, FWINCY, 1.f };
		BackGroundDesc.fDepth = 10.f;
		//if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_LevelLoadingMenu"),
		//	LEVEL_LOADING, TEXT("Layer_UI"), &BackGroundDesc)))
		//	return E_FAIL;
		if (FAILED(m_pGameInstance->Add_GameObjectReturn(LEVEL_STATIC, TEXT("Prototype_GameObject_LevelLoadingMenu"),
			LEVEL_LOADING, TEXT("Layer_UI"), &m_pLoadingMenu, &BackGroundDesc)))
			return E_FAIL;
	}



	
	return S_OK;
}

void CLevel_Loading::Update(_float fTimeDelta)
{

	//if (KEY_DOWN(VK_SPACE))
	{
		if (m_pLoadingMenu != nullptr)
		{
			_bool isLoadingComplete = false;
			switch (m_eNextLevelID)
			{
			case Client::LEVEL_LOGO:
				isLoadingComplete = (static_cast<CLoadingMenu*>(m_pLoadingMenu))->IsLoadingComplete();
				break;
			default:
				isLoadingComplete = m_pLoader->isFinished(); /*(static_cast<CLevelLoadingMenu*>(m_pLoadingMenu))->IsLoadingComplete();*/
				break;
			}
			if (isLoadingComplete)
			{
				CLevel* pLevel = { nullptr };
				/* 클리어 함수에 콜라이더의 클리어를 넣으니까 이 멍청한 콜라이더 매니저가
				pLevel에서 만든 콜라이더까지 싹다 지워버리는 불상사가 생겨서 따로 뺴둠*/
				m_pGameInstance->Clear_Collider();

				switch (m_eNextLevelID)
				{
				case LEVEL_LOGO:
					pLevel = CLevel_Logo::Create(m_pGraphic_Device, m_pLoader->Get_LevelData());
					break;

				case LEVEL_GAMEPLAY:
					pLevel = CLevel_GamePlay::Create(m_pGraphic_Device, m_pLoader->Get_LevelData());
					break;

				case LEVEL_RACEFIRST:
					pLevel = CLevel_RaceFirst::Create(m_pGraphic_Device, m_pLoader->Get_LevelData());
					break;

				case LEVEL_RACESECOND:
					pLevel = CLevel_RaceSecond::Create(m_pGraphic_Device, m_pLoader->Get_LevelData());
					break;

				case LEVEL_RACETHIRD:
					pLevel = CLevel_RaceThird::Create(m_pGraphic_Device, m_pLoader->Get_LevelData());
					break;

				case LEVEL_INDOOR:
					pLevel = CLevel_Indoor::Create(m_pGraphic_Device, m_pLoader->Get_LevelData());
					break;

				case LEVEL_BOSS:
					pLevel = CLevel_Boss::Create(m_pGraphic_Device, m_pLoader->Get_LevelData());
					break;

				case LEVEL_OUTDOOR:
					pLevel = CLevel_OutDoor::Create(m_pGraphic_Device, m_pLoader->Get_LevelData());
					break;
				}

				if (nullptr == pLevel)
					return;

				if (FAILED(m_pGameInstance->Change_Level(m_eNextLevelID, pLevel)))
					return;

			}
		}
		//if (true == m_pLoader->isFinished())
		//{
		//	
		//	CLevel* pLevel = { nullptr };
		//	/* 클리어 함수에 콜라이더의 클리어를 넣으니까 이 멍청한 콜라이더 매니저가 
		//	pLevel에서 만든 콜라이더까지 싹다 지워버리는 불상사가 생겨서 따로 뺴둠*/
		//	m_pGameInstance->Clear_Collider();

		//	switch (m_eNextLevelID)
		//	{
		//	case LEVEL_LOGO:
		//		pLevel = CLevel_Logo::Create(m_pGraphic_Device, m_pLoader->Get_LevelData());
		//		break;

		//	case LEVEL_GAMEPLAY:
		//		pLevel = CLevel_GamePlay::Create(m_pGraphic_Device, m_pLoader->Get_LevelData());
		//		break;

		//	case LEVEL_RACEFIRST:
		//		pLevel = CLevel_RaceFirst::Create(m_pGraphic_Device, m_pLoader->Get_LevelData());
		//		break;

		//	case LEVEL_RACESECOND:
		//		pLevel = CLevel_RaceSecond::Create(m_pGraphic_Device, m_pLoader->Get_LevelData());
		//		break;

		//	case LEVEL_RACETHIRD:
		//		pLevel = CLevel_RaceThird::Create(m_pGraphic_Device, m_pLoader->Get_LevelData());
		//		break;

		//	case LEVEL_INDOOR:
		//		pLevel = CLevel_Indoor::Create(m_pGraphic_Device, m_pLoader->Get_LevelData());
		//		break;

		//	case LEVEL_BOSS:
		//		pLevel = CLevel_Boss::Create(m_pGraphic_Device, m_pLoader->Get_LevelData());
		//		break;

		//	case LEVEL_OUTDOOR:
		//		pLevel = CLevel_OutDoor::Create(m_pGraphic_Device, m_pLoader->Get_LevelData());
		//		break;
		//	}

		//	if (nullptr == pLevel)
		//		return;

 	//		if (FAILED(m_pGameInstance->Change_Level(m_eNextLevelID, pLevel)))
		//		return;
		//					
		//}
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