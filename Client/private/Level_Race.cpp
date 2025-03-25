#include "Level_Race.h"

#include "GameInstance.h"
#include "RaceLandscape.h"
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

	return S_OK;
}

HRESULT CLevel_Race::Ready_Layer_Statue(const _wstring& strLayerTag)
{
	CRaceLandscape::DESC desc = {};
	desc.vLook = _float3(0.f, 0.f, 1.f);
	desc.vPosition = _float3(250.f, 36.f, 0.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACE, TEXT("Prototype_GameObject_RaceLandscapeLeft"),
		LEVEL_RACE, strLayerTag, &desc)))
		return E_FAIL;

	desc.vLook = _float3(0.f, 0.f, -1.f);
	desc.vPosition = _float3(650.f, 36.f, 7150.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACE, TEXT("Prototype_GameObject_RaceLandscapeRight"),
		LEVEL_RACE, strLayerTag, &desc)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACE, TEXT("Prototype_GameObject_BuildingH"),
		LEVEL_RACE, strLayerTag)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACE, TEXT("Prototype_GameObject_BuildingW"),
		LEVEL_RACE, strLayerTag)))
		return E_FAIL;

	/*if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACE, TEXT("Prototype_GameObject_BuildingV"),
		LEVEL_RACE, strLayerTag)))
		return E_FAIL;*/

	/*if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACE, TEXT("Prototype_GameObject_BuildingU"),
		LEVEL_RACE, strLayerTag)))
		return E_FAIL;*/

	return S_OK;
}

HRESULT CLevel_Race::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	CCamera::DESC desc = {};
	desc.vEye = _float3(0.f, 0.f, -20.f);		
	desc.vAt = _float3();						
	desc.fFov = 60.f;
	desc.fNear = 0.1f;
	desc.fFar = 1000.f;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_TPS_Camera"),
		LEVEL_RACE, strLayerTag, &desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Race::Ready_Layer_Pawn(const _wstring& strLayerTag)
{
	CPlayerOnBoat::DESC PlayerOnBoatDesc = {};
	PlayerOnBoatDesc.vInitPos = { 0.f, 10.f, 0.f };
	PlayerOnBoatDesc.vScale = { 20.f, 30.f, 20.f };
	PlayerOnBoatDesc.fRotationPerSec = RADIAN(180.f);
	PlayerOnBoatDesc.fSpeedPerSec = 450.f;
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
