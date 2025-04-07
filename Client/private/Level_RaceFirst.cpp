#include "Level_RaceFirst.h"
#include "Level_Loading.h"

#include "GameInstance.h"
#include "Statue.h"
#include "Map.h"
#include "PlayerOnBoat.h"
#include "CameraManager.h"
#include "RaceBoss.h"

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
	desc.vInitPos	= _float3(250.f, 36.f, 0.f);
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

	desc.vAngle		= _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	desc.vInitPos	= _float3(450.f, 30.f, 12480.f);//12480
	desc.vScale		= _float3(30.f, 30.f, 5000.f);

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

HRESULT CLevel_RaceFirst::Ready_Layer_Pawn(const _wstring& strLayerTag)
{
	// 만약 플레이어가 있다면? 플레이어를 죽여라
	auto pPlayer = static_cast<CPawn*>(GET_PLAYER);
	if (pPlayer)
		m_pGameInstance->Release_Layer(LEVEL_STATIC,strLayerTag);

	CPlayerOnBoat::DESC PlayerOnBoatDesc = {};
	PlayerOnBoatDesc.vInitPos = { 450.f, 17.f, 0.f };
	PlayerOnBoatDesc.vScale = { 35.f, 30.f, 20.f };
	PlayerOnBoatDesc.fRotationPerSec = RADIAN(180.f);
	PlayerOnBoatDesc.fSpeedPerSec = 400.f;
	PlayerOnBoatDesc.fMouseSensor = 0.1f;
	PlayerOnBoatDesc.eLevelID = CurLevel;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PlayerOnBoat"),
		LEVEL_STATIC, strLayerTag, &PlayerOnBoatDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_RaceFirst::Ready_Layer_RaceBoss(const _wstring& strLayerTag)
{
	CRaceBoss::DESC RaceBossDesc = {};
	RaceBossDesc.fRotationPerSec = RADIAN(180.f);
	RaceBossDesc.fSpeedPerSec = 400.f;
	RaceBossDesc.eLevelID = CurLevel;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_RaceBoss"),
		LEVEL_STATIC, strLayerTag, &RaceBossDesc)))
		return E_FAIL;

	return S_OK;
}

void CLevel_RaceFirst::Check_Collision()
{
	m_pGameInstance->Intersect(CG_PAWN, CG_MBULLET);
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
