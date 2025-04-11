#include "Level_RaceFirst.h"
#include "Level_Loading.h"

#include "GameInstance.h"
#include "Statue.h"
#include "Map.h"
#include "PlayerOnBoat.h"
#include "CameraManager.h"
#include "RaceBoss.h"
#include "UI_Manager.h"


#define CurLevel LEVEL_RACEFIRST

CLevel_RaceFirst::CLevel_RaceFirst(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel { pGraphic_Device }
{
}

HRESULT CLevel_RaceFirst::Initialize(CLevelData* pLevelData)
{
	if (FAILED(__super::Initialize(pLevelData)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Statue(TEXT("Layer_RaceLandscape"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Terrain(TEXT("Layer_RaceTerrain"))))
 		return E_FAIL;

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Pawn(TEXT("Layer_Pawn"))))
		return E_FAIL;


	if (FAILED(Ready_Layer_RaceBoss(TEXT("Layer_RaceBoss"))))
		return E_FAIL;


	return S_OK;
}

void CLevel_RaceFirst::Update(_float fTimeDelta)
{
	Check_Collision();

	if (m_iNextLevel)
	{
		m_pGameInstance->Change_Level(LEVEL_LOADING,
			CLevel_Loading::Create(m_pGraphic_Device, (LEVEL)m_iNextLevel));
	}
}

HRESULT CLevel_RaceFirst::Render()
{
	SetWindowText(g_hWnd, TEXT("레이싱 first레벨입니다."));

	return S_OK;
}

HRESULT CLevel_RaceFirst::Ready_Layer_Camera()
{
	auto CameraManager = CAMERA_MANAGER;
	CameraManager->Switch(CCameraManager::TPS);
	CameraManager->Set_Mouse_Fix(TRUE);

	return S_OK;
}

HRESULT CLevel_RaceFirst::Ready_Layer_Terrain(const _wstring& strLayerTag)
{
	CMap::DESC MapDesc{};
	MapDesc.vScale = _float3(1.f, 1.f, 1.f);
	MapDesc.eLevelID = LEVEL_RACEFIRST;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_RaceTerrain"),
		LEVEL_RACEFIRST, strLayerTag, &MapDesc)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Sky"),
		LEVEL_RACEFIRST, strLayerTag)))
		return E_FAIL;
	return S_OK;
}

HRESULT CLevel_RaceFirst::Ready_Layer_Statue(const _wstring& strLayerTag)
{
	CStatue::DESC desc = {};

	desc.vAngle		= _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	desc.vInitPos	= _float3(250.f, 36.f, -2000.f);
	desc.vScale		= _float3(72.f, 120.f, 72.f);
	desc.eLevelID = LEVEL_RACEFIRST;
	
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_RaceLandscape"),
		LEVEL_RACEFIRST, strLayerTag, &desc)))
		return E_FAIL;

	desc.vAngle		= _float3(D3DXToRadian(0.f), D3DXToRadian(180.f), D3DXToRadian(0.f));
	desc.vInitPos	= _float3(650.f, 36.f, 10000.f);
	desc.vScale		= _float3(72.f, 120.f, 72.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_RaceLandscape"),
		LEVEL_RACEFIRST, strLayerTag, &desc)))
		return E_FAIL;

	/*desc.vAngle		= _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	desc.vInitPos	= _float3(450.f, 250.f, 700.f);
	desc.vScale		= _float3(950.f, 500.f, 720.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_BuildingH"),
		LEVEL_RACEFIRST, strLayerTag, &desc)))
		return E_FAIL;*/

	desc.vAngle		= _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	desc.vInitPos	= _float3(1100.f, 250.f, 2200.f);
	desc.vScale		= _float3(800.f, 500.f, 720.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_BuildingU"),
		LEVEL_RACEFIRST, strLayerTag, &desc)))
		return E_FAIL;

	desc.vAngle		= _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	desc.vInitPos	= _float3(-200.f, 250.f, 2500.f);
	desc.vScale		= _float3(800.f, 500.f, 720.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_BuildingW"),
		LEVEL_RACEFIRST, strLayerTag, &desc)))
		return E_FAIL;

	desc.vAngle		= _float3(D3DXToRadian(0.f), D3DXToRadian(-90.f), D3DXToRadian(0.f));
	desc.vInitPos	= _float3(1100.f, 250.f, 3000.f);
	desc.vScale		= _float3(800.f, 500.f, 720.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_BuildingV"),
		LEVEL_RACEFIRST, strLayerTag, &desc)))
		return E_FAIL;

	desc.vAngle		= _float3(D3DXToRadian(0.f), D3DXToRadian(30.f), D3DXToRadian(0.f));
	desc.vInitPos	= _float3(-350.f, 250.f, 4000.f);
	desc.vScale		= _float3(800.f, 500.f, 720.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_BuildingU"),
		LEVEL_RACEFIRST, strLayerTag, &desc)))
		return E_FAIL;

	desc.vAngle		= _float3(D3DXToRadian(0.f), D3DXToRadian(90.f), D3DXToRadian(0.f));
	desc.vInitPos	= _float3(950.f, 250.f, 4500.f);
	desc.vScale		= _float3(800.f, 500.f, 720.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_BuildingV"),
		LEVEL_RACEFIRST, strLayerTag, &desc)))
		return E_FAIL;

	desc.vAngle		= _float3(D3DXToRadian(0.f), D3DXToRadian(-180.f), D3DXToRadian(0.f));
	desc.vInitPos	= _float3(-250.f, 250.f, 5500.f);
	desc.vScale		= _float3(800.f, 500.f, 720.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_BuildingU"),
		LEVEL_RACEFIRST, strLayerTag, &desc)))
		return E_FAIL;
	
	desc.vAngle		= _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	desc.vInitPos	= _float3(1150.f, 250.f, 6500.f);
	desc.vScale		= _float3(800.f, 500.f, 720.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_BuildingW"),
		LEVEL_RACEFIRST, strLayerTag, &desc)))
		return E_FAIL;

	desc.vAngle		= _float3(D3DXToRadian(0.f), D3DXToRadian(-90.f), D3DXToRadian(0.f));
	desc.vInitPos	= _float3(-120.f, 250.f, 7000.f);
	desc.vScale		= _float3(800.f, 500.f, 720.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_BuildingV"),
		LEVEL_RACEFIRST, strLayerTag, &desc)))
		return E_FAIL;

	desc.vAngle		= _float3(D3DXToRadian(0.f), D3DXToRadian(-90.f), D3DXToRadian(0.f));
	desc.vInitPos	= _float3(1100.f, 250.f, 8500.f);
	desc.vScale		= _float3(800.f, 500.f, 720.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_BuildingW"),
		LEVEL_RACEFIRST, strLayerTag, &desc)))
		return E_FAIL;

	desc.vAngle		= _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	desc.vInitPos	= _float3(-200.f, 250.f, 8200.f);
	desc.vScale		= _float3(800.f, 500.f, 720.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_BuildingU"),
		LEVEL_RACEFIRST, strLayerTag, &desc)))
		return E_FAIL;

	desc.vAngle		= _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	desc.vInitPos	= _float3(450.f, 100.f, 10000.f);
	desc.vScale		= _float3(350.f, 200.f, 200.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_RaceGate"),
		LEVEL_RACEFIRST, strLayerTag, &desc)))
		return E_FAIL;

	desc.vAngle = _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	desc.vInitPos = _float3(450.f, 30.f, -8000.f);
	desc.vScale = _float3(30.f, 30.f, 10000.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_RaceCylinder"),
		LEVEL_RACEFIRST, strLayerTag, &desc)))
		return E_FAIL;

	desc.vAngle = _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	desc.vInitPos = _float3(450.f, 30.f, 12480.f);//12480
	desc.vScale = _float3(30.f, 30.f, 5000.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_RaceCylinder"),
		LEVEL_RACEFIRST, strLayerTag, &desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_RaceFirst::Ready_Layer_Statue2(const _wstring& strLayerTag)
{
	CStatue::DESC desc = {};

	desc.vAngle = _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	desc.vInitPos = _float3(450.f, 10.f, 500.f);
	desc.vScale = _float3(10.f, 30.f, 0.f);
	desc.eLevelID = LEVEL_RACEFIRST;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_StreetLampHead"),
		LEVEL_RACEFIRST, strLayerTag, &desc)))
		return E_FAIL;

	return S_OK;
}

#include "PlayerOnBoat.h"
HRESULT CLevel_RaceFirst::Ready_Layer_Pawn(const _wstring& strLayerTag)
{
	// 이 레벨 플레이어 생성위치 
	_float3 vInitPosition = { 450.f, 17.f, -1000.f };

	// 만약 플레이어가 있다면? 플레이어를 리스트에서 빼서
	auto pPlayer = static_cast<CPawn*>(GET_PLAYER);
	if (pPlayer)
	{
		// 만약 플레이어가 컴먼이라면 뒤로 뺸다.
		if (pPlayer->Get_Type() == CPawn::COMMON)
		{
			m_pGameInstance->Release_Layer(LEVEL_STATIC, strLayerTag);
			pPlayer->Set_Active(FALSE);
		}
		// 만약 플레이어가 보트라면 위치만 바꾸어준다.
		else
		{
			vInitPosition = { 450.f, 17.f, -10000.f };
			static_cast<CTransform*>(pPlayer->Find_Component(TEXT("Com_Transform")))
				->Set_State(CTransform::STATE_POSITION, vInitPosition);
			static_cast<CPawn*>(pPlayer)->Set_Level(CurLevel);
			static_cast<CPlayerOnBoat*>(pPlayer)->Set_StartState(CPlayerOnBoat::ACCEL);
			return S_OK;
		}
	}

	/// 이 밑은 일반 플레이어 -> 보트 플레이어 전환시의 초기화 옵션
	CPlayerOnBoat::DESC PlayerOnBoatDesc = {};
	PlayerOnBoatDesc.vInitPos = vInitPosition;
	PlayerOnBoatDesc.vScale = _float3{ 223.f, 137.f, 223.f } * 0.15f;
	PlayerOnBoatDesc.fRotationPerSec = RADIAN(180.f);
	PlayerOnBoatDesc.fSpeedPerSec = RACE_SPEED_PER_SEC;
	PlayerOnBoatDesc.eLevelID = CurLevel;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PlayerOnBoat"),
		LEVEL_STATIC, strLayerTag, &PlayerOnBoatDesc)))
		return E_FAIL;

	auto pBoatPlayer = static_cast<CPawn*>(GET_PLAYER);

	if (pPlayer)
	{
		pBoatPlayer->Link_Player_Data(*pPlayer);
		Safe_AddRef(pPlayer);
		m_pGameInstance->Push_GameObject(pPlayer, LEVEL_STATIC, strLayerTag);
	}

	return S_OK;
}

#include "RaceBoss.h"
HRESULT CLevel_RaceFirst::Ready_Layer_RaceBoss(const _wstring& strLayerTag)
{
	_float3 vInitPosition = { 0.f, 1000.f, -500.f };

	auto pBoss = static_cast<CRaceBoss*>(m_pGameInstance->Find_Object(LEVEL_STATIC, TEXT("Layer_RaceBoss")));
	if (pBoss)
	{
		static_cast<CTransform*>(pBoss->Find_Component(TEXT("Com_Transform")))
			->Set_State(CTransform::STATE_POSITION, vInitPosition);
		pBoss->Set_StartState(CRaceBoss::ENTRANCE);
		return S_OK;
	}

	CRaceBoss::DESC RaceBossDesc = {};
	RaceBossDesc.fRotationPerSec = RADIAN(180.f);
	RaceBossDesc.fSpeedPerSec = RACE_SPEED_PER_SEC;
	RaceBossDesc.eLevelID = CurLevel;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_RaceBoss"),
		LEVEL_STATIC, strLayerTag, &RaceBossDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_RaceFirst::Ready_Layer_UI(const _wstring& strLayerTag)
{
	CUI::DESC Desc{};
	Desc.eLevelID = CurLevel;
	Desc.fDepth = _float(UI_HUD);
	Desc.vScale = _float3(1.f, 1.f, 1.f);
	Desc.vInitPos = _float3(0.f, 0.f, 0.1f);
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Aim"),
		Desc.eLevelID, strLayerTag, &Desc)))
		return E_FAIL;

	Desc.vScale = _float3(80.f, 80.f, 1.f);
	Desc.vInitPos = _float3(-(g_iWinSizeX / 2.f) + Desc.vScale.x / 2.f - 10.f, -(g_iWinSizeY / 2.f) + Desc.vScale.y / 2.f, 0.1f);
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Portrait"),
		Desc.eLevelID, strLayerTag, &Desc)))
		return E_FAIL;

	Desc.vScale = _float3(75.f, 75.f, 1.f);
	Desc.vInitPos = _float3(-(g_iWinSizeX / 2.f) + 208.f, -(g_iWinSizeY / 2.f) + Desc.vScale.y / 2.f, 0.1f);
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Armor"),
		Desc.eLevelID, strLayerTag, &Desc)))
		return E_FAIL;

	Desc.vScale = _float3(48.f, 54.f, 1.f);
	Desc.vInitPos = _float3((g_iWinSizeX / 2.f) - 40.f, -(g_iWinSizeY / 2.f) + Desc.vScale.y / 2.f + 7.f, 0.1f);
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Ammo"),
		Desc.eLevelID, strLayerTag, &Desc)))
		return E_FAIL;

	CUI_Manager::Get_Instance()->Initialize_GamePlayUI(CurLevel);

	Desc.vScale = _float3(240.f, 42.f, 1.f);
	Desc.vInitPos = _float3(0.f, g_iWinSizeY * 0.5f - 44.f, 0.1f);
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_BossHPBar"),
		Desc.eLevelID, strLayerTag, &Desc)))
		return E_FAIL;
	CUI_Manager::Get_Instance()->Initialize_BossHPUI(CurLevel);

#define MULTIPLIER 1.5f
	// 105 76
	Desc.vScale = _float3(105.f * MULTIPLIER, 76.f * MULTIPLIER, 1.f); // 1.25배됨
	Desc.vInitPos = _float3(0.f, g_iWinSizeY * -0.5f + 60.f, 0.1f);
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_RacingPanel"),
		Desc.eLevelID, strLayerTag, &Desc)))
		return E_FAIL;
	CUI_Manager::Get_Instance()->Initialize_RacingUI(CurLevel);


	/* ui생성 순서 중요 */
	// 과거의 나야 미안해 
	// 안미안해꺼져
	return S_OK;
}
void CLevel_RaceFirst::Check_Collision()
{
	m_pGameInstance->Intersect(CG_PAWN, CG_MBULLET);
	m_pGameInstance->Intersect(CG_MONSTER, CG_PBULLET);
}

CLevel_RaceFirst* CLevel_RaceFirst::Create(LPDIRECT3DDEVICE9 pGraphic_Device, class CLevelData* pLevelData)
{
	CLevel_RaceFirst* pInstance = new CLevel_RaceFirst(pGraphic_Device);

	if (FAILED(pInstance->Initialize(pLevelData)))
	{
		MSG_BOX("Failed to Created : CLevel_RaceFirst");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_RaceFirst::Free()
{
	__super::Free();
}
