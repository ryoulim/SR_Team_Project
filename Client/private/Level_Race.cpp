#include "Level_Race.h"

#include "GameInstance.h"
#include "RaceLandscape.h"

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
	CRaceLandscape::DIRECTION dir = {};
	dir = CRaceLandscape::LEFT;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACE, TEXT("Prototype_GameObject_RaceLandscapeLeft"),
		LEVEL_RACE, strLayerTag, &dir)))
		return E_FAIL;
	
	dir = CRaceLandscape::RIGHT;
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACE, TEXT("Prototype_GameObject_RaceLandscapeRight"),
		LEVEL_RACE, strLayerTag, &dir)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Race::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Dynamic_Camera"),
		LEVEL_RACE, strLayerTag, m_pData->Find_Data(TEXT("Dynamic_Camera")))))
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
