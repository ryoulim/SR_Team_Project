#include "Level_GamePlay.h"
#include "Explosion.h"

#include "MyCube.h"


#define CurLevel LEVEL_GAMEPLAY

CLevel_GamePlay::CLevel_GamePlay(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel { pGraphic_Device }
{

}

HRESULT CLevel_GamePlay::Initialize(class CLevelData* pLevelData)
{
	if (FAILED(__super::Initialize(pLevelData)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Terrain(TEXT("Layer_Terrain"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Object(TEXT("Layer_Object"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;

	return S_OK;
}

void CLevel_GamePlay::Update(_float fTimeDelta)
{
	int a = 10;
}

HRESULT CLevel_GamePlay::Render()
{
	SetWindowText(g_hWnd, TEXT("게임플레이 레벨입니다."));

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Terrain(const _wstring& strLayerTag)
{
	//CTerrain::DESC TerrainDesc{};
	//TerrainDesc.vScale = { 128.f,128.f,128.f };
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Terrain"),
		LEVEL_GAMEPLAY, strLayerTag, m_pData->Find_Data(TEXT("Terrain")))))
		return E_FAIL;
	

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Dynamic_Camera"),
		LEVEL_GAMEPLAY, strLayerTag, m_pData->Find_Data(TEXT("Dynamic_Camera")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Camera"),
		LEVEL_GAMEPLAY, strLayerTag, m_pData->Find_Data(TEXT("UI_Camera")))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Object(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_MyCube"),
		LEVEL_GAMEPLAY, strLayerTag, m_pData->Find_Data(TEXT("MyCube1")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_MyCube"),
		LEVEL_GAMEPLAY, strLayerTag, m_pData->Find_Data(TEXT("MyCube2")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Trapezoid"),
		LEVEL_GAMEPLAY, strLayerTag, m_pData->Find_Data(TEXT("Trapezoid")))))
		return E_FAIL;

	CExplosion::DESC ExplosionDesc{};

	ExplosionDesc.vInitPos = { 160.f,60.f,200.f };
	ExplosionDesc.vScale = { 120.f,160.f,1.f };
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Explosion"),
		LEVEL_GAMEPLAY, strLayerTag, &ExplosionDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_UI(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Aim"),
		LEVEL_GAMEPLAY, strLayerTag, m_pData->Find_Data(TEXT("Aim")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Portrait"),
		LEVEL_GAMEPLAY, strLayerTag, m_pData->Find_Data(TEXT("Portrait")))))
		return E_FAIL;

	return S_OK;
}

CLevel_GamePlay* CLevel_GamePlay::Create(LPDIRECT3DDEVICE9 pGraphic_Device, class CLevelData* pLevelData)
{
	CLevel_GamePlay* pInstance = new CLevel_GamePlay(pGraphic_Device);

	if (FAILED(pInstance->Initialize(pLevelData)))
	{
		MSG_BOX("Failed to Created : CLevel_GamePlay");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLevel_GamePlay::Free()
{
	__super::Free();

}
