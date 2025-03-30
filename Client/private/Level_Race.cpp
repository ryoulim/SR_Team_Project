#include "Level_Race.h"

#include "GameInstance.h"
#include "Statue.h"
#include "PlayerOnBoat.h"
#include "Camera.h"

CLevel_Race::CLevel_Race(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel { pGraphic_Device }
{
}

HRESULT CLevel_Race::Initialize(CLevelData* pLevelData)
{
	if (FAILED(__super::Initialize(pLevelData)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Statue(TEXT("Layer_RaceLandscape"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Terrain(TEXT("Layer_RaceTerrain"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Pawn(TEXT("Layer_Pawn"))))
		return E_FAIL;

	return S_OK;
}

void CLevel_Race::Update(_float fTimeDelta)
{
}

HRESULT CLevel_Race::Render()
{
	SetWindowText(g_hWnd, TEXT("레이싱 레벨입니다."));

	return S_OK;
}

HRESULT CLevel_Race::Ready_Layer_Terrain(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACE, TEXT("Prototype_GameObject_RaceTerrain"),
		LEVEL_RACE, strLayerTag)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Sky"),
		LEVEL_RACE, strLayerTag)))
		return E_FAIL;
	return S_OK;
}

HRESULT CLevel_Race::Ready_Layer_Statue(const _wstring& strLayerTag)
{
	CStatue::DESC desc = {};

	desc.vAngle		= _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	desc.vInitPos	= _float3(250.f, 36.f, 0.f);
	desc.vScale		= _float3(72.f, 120.f, 72.f);
	desc.eLevelID = LEVEL_RACE;
	
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACE, TEXT("Prototype_GameObject_RaceLandscapeLeft"),
		LEVEL_RACE, strLayerTag, &desc)))
		return E_FAIL;

	desc.vAngle		= _float3(D3DXToRadian(0.f), D3DXToRadian(180.f), D3DXToRadian(0.f));
	desc.vInitPos	= _float3(650.f, 36.f, 10000.f);
	desc.vScale		= _float3(72.f, 120.f, 72.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACE, TEXT("Prototype_GameObject_RaceLandscapeRight"),
		LEVEL_RACE, strLayerTag, &desc)))
		return E_FAIL;

	desc.vAngle		= _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	desc.vInitPos	= _float3(450.f, 250.f, 700.f);
	desc.vScale		= _float3(950.f, 500.f, 720.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACE, TEXT("Prototype_GameObject_BuildingH"),
		LEVEL_RACE, strLayerTag, &desc)))
		return E_FAIL;

	desc.vAngle		= _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	desc.vInitPos	= _float3(1100.f, 250.f, 2200.f);
	desc.vScale		= _float3(800.f, 500.f, 720.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACE, TEXT("Prototype_GameObject_BuildingU1"),
		LEVEL_RACE, strLayerTag, &desc)))
		return E_FAIL;

	desc.vAngle		= _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	desc.vInitPos	= _float3(-200.f, 250.f, 2500.f);
	desc.vScale		= _float3(800.f, 500.f, 720.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACE, TEXT("Prototype_GameObject_BuildingW1"),
		LEVEL_RACE, strLayerTag, &desc)))
		return E_FAIL;

	desc.vAngle		= _float3(D3DXToRadian(0.f), D3DXToRadian(-90.f), D3DXToRadian(0.f));
	desc.vInitPos	= _float3(1100.f, 250.f, 3000.f);
	desc.vScale		= _float3(800.f, 500.f, 720.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACE, TEXT("Prototype_GameObject_BuildingV1"),
		LEVEL_RACE, strLayerTag, &desc)))
		return E_FAIL;

	desc.vAngle		= _float3(D3DXToRadian(0.f), D3DXToRadian(30.f), D3DXToRadian(0.f));
	desc.vInitPos	= _float3(-350.f, 250.f, 4000.f);
	desc.vScale		= _float3(800.f, 500.f, 720.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACE, TEXT("Prototype_GameObject_BuildingU2"),
		LEVEL_RACE, strLayerTag, &desc)))
		return E_FAIL;

	desc.vAngle		= _float3(D3DXToRadian(0.f), D3DXToRadian(90.f), D3DXToRadian(0.f));
	desc.vInitPos	= _float3(950.f, 250.f, 4500.f);
	desc.vScale		= _float3(800.f, 500.f, 720.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACE, TEXT("Prototype_GameObject_BuildingV2"),
		LEVEL_RACE, strLayerTag, &desc)))
		return E_FAIL;

	desc.vAngle		= _float3(D3DXToRadian(0.f), D3DXToRadian(-180.f), D3DXToRadian(0.f));
	desc.vInitPos	= _float3(-250.f, 250.f, 5500.f);
	desc.vScale		= _float3(800.f, 500.f, 720.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACE, TEXT("Prototype_GameObject_BuildingU3"),
		LEVEL_RACE, strLayerTag, &desc)))
		return E_FAIL;
	
	desc.vAngle		= _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	desc.vInitPos	= _float3(1150.f, 250.f, 6500.f);
	desc.vScale		= _float3(800.f, 500.f, 720.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACE, TEXT("Prototype_GameObject_BuildingW2"),
		LEVEL_RACE, strLayerTag, &desc)))
		return E_FAIL;

	desc.vAngle		= _float3(D3DXToRadian(0.f), D3DXToRadian(-90.f), D3DXToRadian(0.f));
	desc.vInitPos	= _float3(-120.f, 250.f, 7000.f);
	desc.vScale		= _float3(800.f, 500.f, 720.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACE, TEXT("Prototype_GameObject_BuildingV3"),
		LEVEL_RACE, strLayerTag, &desc)))
		return E_FAIL;

	desc.vAngle		= _float3(D3DXToRadian(0.f), D3DXToRadian(-90.f), D3DXToRadian(0.f));
	desc.vInitPos	= _float3(1100.f, 250.f, 8500.f);
	desc.vScale		= _float3(800.f, 500.f, 720.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACE, TEXT("Prototype_GameObject_BuildingW3"),
		LEVEL_RACE, strLayerTag, &desc)))
		return E_FAIL;

	desc.vAngle		= _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	desc.vInitPos	= _float3(-200.f, 250.f, 8200.f);
	desc.vScale		= _float3(800.f, 500.f, 720.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACE, TEXT("Prototype_GameObject_BuildingU4"),
		LEVEL_RACE, strLayerTag, &desc)))
		return E_FAIL;

	desc.vAngle		= _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	desc.vInitPos	= _float3(450.f, 100.f, 10000.f);
	desc.vScale		= _float3(350.f, 200.f, 200.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACE, TEXT("Prototype_GameObject_RaceGate"),
		LEVEL_RACE, strLayerTag, &desc)))
		return E_FAIL;

	desc.vAngle		= _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	desc.vInitPos	= _float3(450.f, 30.f, 12480.f);//12480
	desc.vScale		= _float3(30.f, 30.f, 5000.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACE, TEXT("Prototype_GameObject_RaceCylinder"),
		LEVEL_RACE, strLayerTag, &desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Race::Ready_Layer_Statue2(const _wstring& strLayerTag)
{
	CStatue::DESC desc = {};

	desc.vAngle = _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	desc.vInitPos = _float3(450.f, 10.f, 500.f);
	desc.vScale = _float3(10.f, 30.f, 0.f);
	desc.eLevelID = LEVEL_RACE;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACE, TEXT("Prototype_GameObject_StreetLampHead"),
		LEVEL_RACE, strLayerTag, &desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Race::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	CCamera::DESC desc = {};
	desc.vEye = _float3(0.f, 0.f, -20.f);		
	desc.vAt = _float3();						
	desc.fFov = 60.f;
	desc.fNear = 0.1f;
	desc.fFar = 2000.f;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_TPS_Camera"),
		LEVEL_RACE, strLayerTag, &desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Race::Ready_Layer_Pawn(const _wstring& strLayerTag)
{
	CPlayerOnBoat::DESC PlayerOnBoatDesc = {};
	PlayerOnBoatDesc.vInitPos = { 450.f, 17.f, 0.f };
	PlayerOnBoatDesc.vScale = { 35.f, 30.f, 20.f };
	PlayerOnBoatDesc.fRotationPerSec = RADIAN(180.f);
	PlayerOnBoatDesc.fSpeedPerSec = 1000.f;
	PlayerOnBoatDesc.fMouseSensor = 0.1f;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACE, TEXT("Prototype_GameObject_PlayerOnBoat"),
		LEVEL_RACE, strLayerTag, &PlayerOnBoatDesc)))
		return E_FAIL;

	return S_OK;
}

CLevel_Race* CLevel_Race::Create(LPDIRECT3DDEVICE9 pGraphic_Device, class CLevelData* pLevelData)
{
	CLevel_Race* pInstance = new CLevel_Race(pGraphic_Device);

	if (FAILED(pInstance->Initialize(pLevelData)))
	{
		MSG_BOX("Failed to Created : CLevel_Race");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Race::Free()
{
	__super::Free();
}
