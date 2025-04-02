#include "Level_RaceSecond.h"
#include "Level_Loading.h"

#include "GameInstance.h"
#include "Statue.h"
#include "Map.h"
#include "PlayerOnBoat.h"
#include "CameraManager.h"

#define CurLevel LEVEL_RACESECOND

CLevel_RaceSecond::CLevel_RaceSecond(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel { pGraphic_Device }
{
}

HRESULT CLevel_RaceSecond::Initialize(CLevelData* pLevelData)
{
	if (FAILED(__super::Initialize(pLevelData)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Statue(TEXT("Layer_RaceLandscape"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Terrain(TEXT("Layer_RaceTerrain"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Pawn(TEXT("Layer_Pawn"))))
		return E_FAIL;

	return S_OK;
}

void CLevel_RaceSecond::Update(_float fTimeDelta)
{
	if (m_iNextLevel)
	{
		m_pGameInstance->Change_Level(LEVEL_LOADING,
			CLevel_Loading::Create(m_pGraphic_Device, (LEVEL)m_iNextLevel));
	}
}

HRESULT CLevel_RaceSecond::Render()
{
	SetWindowText(g_hWnd, TEXT("레이싱 second레벨입니다."));

	return S_OK;
}

HRESULT CLevel_RaceSecond::Ready_Layer_Camera()
{
	auto CameraManager = CAMERA_MANAGER;
	CameraManager->Switch(CCameraManager::TPS);
	CameraManager->Set_Mouse_Fix(TRUE);

	return S_OK;
}

HRESULT CLevel_RaceSecond::Ready_Layer_Terrain(const _wstring& strLayerTag)
{
	CMap::DESC MapDesc{};
	MapDesc.vScale = _float3(1.f, 1.f, 1.f);
	MapDesc.eLevelID = LEVEL_RACESECOND;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACESECOND, TEXT("Prototype_GameObject_RaceTerrain"),
		LEVEL_RACESECOND, strLayerTag, &MapDesc)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Sky"),
		LEVEL_RACESECOND, strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_RaceSecond::Ready_Layer_Statue(const _wstring& strLayerTag)
{
	CStatue::DESC desc = {};

	desc.vAngle = _float3(D3DXToRadian(0.f), D3DXToRadian(80.f), D3DXToRadian(0.f));
	desc.vInitPos = _float3(800.f, 300.f, -3000.f);
	desc.vScale = _float3(600.f, 600.f, 600.f);
	desc.eLevelID = LEVEL_RACESECOND;

	for (_uint i = 0; i < 25; i++)
	{
		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACESECOND, TEXT("Prototype_GameObject_Forest"),
			LEVEL_RACESECOND, strLayerTag, &desc)))
			return E_FAIL;

		desc.vInitPos.z += 600.f;
	}

	desc.vAngle = _float3(D3DXToRadian(0.f), D3DXToRadian(-80.f), D3DXToRadian(0.f));
	desc.vInitPos = _float3(100.f, 300.f, -3000.f);
	desc.vScale = _float3(600.f, 600.f, 600.f);
	desc.eLevelID = LEVEL_RACESECOND;

	for (_uint i = 0; i < 25; i++)
	{
		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACESECOND, TEXT("Prototype_GameObject_Forest"),
			LEVEL_RACESECOND, strLayerTag, &desc)))
			return E_FAIL;

		desc.vInitPos.z += 600.f;
	}


	desc.vAngle		= _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	desc.vInitPos	= _float3(250.f, 36.f, -3000.f);
	desc.vScale		= _float3(72.f, 120.f, 72.f);
	desc.eLevelID = LEVEL_RACESECOND;
	
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACESECOND, TEXT("Prototype_GameObject_RaceLandscape"),
		LEVEL_RACESECOND, strLayerTag, &desc)))
		return E_FAIL;

	desc.vAngle		= _float3(D3DXToRadian(0.f), D3DXToRadian(180.f), D3DXToRadian(0.f));
	desc.vInitPos	= _float3(650.f, 36.f, 10000.f);
	desc.vScale		= _float3(72.f, 120.f, 72.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACESECOND, TEXT("Prototype_GameObject_RaceLandscape"),
		LEVEL_RACESECOND, strLayerTag, &desc)))
		return E_FAIL;

	desc.vAngle = _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	desc.vInitPos = _float3(450.f, 100.f, 10000.f);
	desc.vScale = _float3(350.f, 200.f, 200.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACESECOND, TEXT("Prototype_GameObject_RaceGate"),
		LEVEL_RACESECOND, strLayerTag, &desc)))
		return E_FAIL;

	desc.vAngle = _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	desc.vInitPos = _float3(450.f, 30.f, -8000.f);
	desc.vScale = _float3(30.f, 30.f, 10000.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACESECOND, TEXT("Prototype_GameObject_RaceCylinder"),
		LEVEL_RACESECOND, strLayerTag, &desc)))
		return E_FAIL;

	desc.vAngle = _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	desc.vInitPos = _float3(450.f, 30.f, 12480.f);//12480
	desc.vScale = _float3(30.f, 30.f, 5000.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACESECOND, TEXT("Prototype_GameObject_RaceCylinder"),
		LEVEL_RACESECOND, strLayerTag, &desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_RaceSecond::Ready_Layer_Pawn(const _wstring& strLayerTag)
{
	//이 레벨의 플레이어 생성위치
	_float3 vInitPosition = { 450.f, 17.f, -10000.f };

	// 플레이어가 있는지 체크하고 있으면 위치만 변경해줌.
	auto pPlayer = static_cast<CPawn*>(GET_PLAYER);
	if (pPlayer)
	{
		static_cast<CTransform*>(pPlayer->Find_Component(TEXT("Com_Transform")))
			->Set_State(CTransform::STATE_POSITION, vInitPosition);
		pPlayer->Set_LevelID(CurLevel);
		return S_OK;
	}

	return S_OK;
}

CLevel_RaceSecond* CLevel_RaceSecond::Create(LPDIRECT3DDEVICE9 pGraphic_Device, class CLevelData* pLevelData)
{
	CLevel_RaceSecond* pInstance = new CLevel_RaceSecond(pGraphic_Device);

	if (FAILED(pInstance->Initialize(pLevelData)))
	{
		MSG_BOX("Failed to Created : CLevel_RaceSecond");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_RaceSecond::Free()
{
	__super::Free();
}
