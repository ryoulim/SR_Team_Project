#include "Level_Race.h"

#include "GameInstance.h"
#include "RaceLandscape.h"
#include "PlayerOnBoat.h"

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

	//if (FAILED(Ready_Layer_Pawn(TEXT("Layer_Pawn"))))
	//	return E_FAIL;

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

	return S_OK;
}

HRESULT CLevel_Race::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	//LEVEL_STATIC : 프로토타입에 있는 레벨
	//TEXT("Prototype_GameObject_Dynamic_Camera" : 프로토타입에 있는 CBase* 객체를 검색하기 위한 키값
	//LEVEL_RACE : 레이어에 추가할때의 레벨
	//strLayerTag : 레이어에 추가할때 설정해줄 키값, TEXT("Layer_Camera")
	//레이어는 CGameObject* 객체들이 들어있는 벡터
	//TEXT("Dynamic_Camera")를 통해 찾은 데이터는 pArg
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Dynamic_Camera"),
		LEVEL_RACE, strLayerTag, m_pData->Find_Data(TEXT("Dynamic_Camera")))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Race::Ready_Layer_Pawn(const _wstring& strLayerTag)
{
	CPlayerOnBoat::DESC PlayerOnBoatDesc = {};
	PlayerOnBoatDesc.vInitPos = { 10.f, 10.f, 10.f };
	PlayerOnBoatDesc.vScale = { 200.f, 300.f, 200.f };
	PlayerOnBoatDesc.fRotationPerSec = RADIAN(180.f);
	PlayerOnBoatDesc.fSpeedPerSec = 150.f;
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
