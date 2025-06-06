#include "Level_Logo.h"
#include "Level_Loading.h"
#include "BackGround.h"
#include "Button.h"
#include "UI_Manager.h"

CLevel_Logo::CLevel_Logo(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel { pGraphic_Device }
{

}

HRESULT CLevel_Logo::Initialize(class CLevelData* pLevelData)
{
	if (FAILED(__super::Initialize(pLevelData)))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_Background"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Gameplay_UI(TEXT("Layer_GUI"))))
		return E_FAIL;

	m_pBGM = m_pGameInstance->Get_Single_Sound("intro_af");
	m_pBGM->Set_Volume(0.3f);
	m_pBGM->Play();

	ShowCursor(TRUE);
	return S_OK;
}

void CLevel_Logo::Update(_float fTimeDelta)
{
	if (KEY_DOWN(DIK_RETURN))
	{
		if (FAILED(m_pGameInstance->Change_Level(LEVEL_LOADING,
			CLevel_Loading::Create(m_pGraphic_Device, START_LEVEL))))
			return;
	}
}

HRESULT CLevel_Logo::Render()
{
	SetWindowText(g_hWnd, TEXT("로고레벨입니다."));

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_BackGround(const _wstring& strLayerTag)
{

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_UI(const _wstring& strLayerTag)
{
	CUI::DESC BackGroundDesc{};
	BackGroundDesc.eLevelID = LEVEL_LOGO;
	BackGroundDesc.vInitPos = { 0.f,0.f,9.f };
	BackGroundDesc.vScale = { FWINCX, FWINCY, 1.f };
	BackGroundDesc.fDepth = _float(UI_BACKGROUND);
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_LOGO, TEXT("Prototype_GameObject_MainMenu"),
		LEVEL_LOGO, strLayerTag, &BackGroundDesc)))
		return E_FAIL;

	BackGroundDesc.vInitPos = { 0.f,0.f,1.f };
	BackGroundDesc.vScale = { FWINCX , FWINCY, 1.f };
	BackGroundDesc.fDepth = _float(UI_EFFECT);
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_LOGO, TEXT("Prototype_GameObject_ScreenDust"),
		LEVEL_LOGO, TEXT("Layer_UI"), &BackGroundDesc)))
		return E_FAIL;

	CUI::DESC TestButtonDesc{};

	TestButtonDesc.eLevelID = LEVEL_LOGO;
	TestButtonDesc.vInitPos = { 280.f - g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f - 460.f,1.f };
	TestButtonDesc.vScale = { 200.f, 38.f, 10.f };
	TestButtonDesc.fDepth = _float(UI_BUTTON);
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_LOGO, TEXT("Prototype_GameObject_Button_Main"),
		LEVEL_LOGO, strLayerTag, &TestButtonDesc)))
		return E_FAIL;



	CUI::DESC pDesc;
	pDesc.fRotationPerSec = 0.f;
	pDesc.fSpeedPerSec = 0.f;
	pDesc.vInitPos = { -250.f,-20.f,0.9f };
	pDesc.vScale = { 1.f,1.f,1.f };
	pDesc.eLevelID = LEVEL_LOGO;
	pDesc.fDepth = _float(UI_DECO);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_LOGO, TEXT("Prototype_GameObject_Logo"),
		LEVEL_LOGO, strLayerTag, &pDesc)))
		return E_FAIL;

	CGameObject* pDialog{ nullptr };
	pDesc.fDepth = _float(UI_FONT);
	if (FAILED(m_pGameInstance->Add_GameObjectReturn(LEVEL_STATIC, TEXT("Prototype_GameObject_ItemDialog"),
		LEVEL_STATIC, strLayerTag, &pDialog, &pDesc)))
		return E_FAIL;
	if (pDialog)
	{
		CUI_Manager::Get_Instance()->Initialize_Dialog(pDialog);
	}


	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_Gameplay_UI(const _wstring& strLayerTag)
{
	CUI::DESC Desc{};
	Desc.eLevelID = LEVEL_STATIC;
	Desc.fDepth = 3.f;
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

	Desc.vScale = _float3(48.f, 54.f, 1.f);
	Desc.vInitPos = _float3((g_iWinSizeX / 2.f) - 40.f, -(g_iWinSizeY / 2.f) + Desc.vScale.y / 2.f + 7.f, 0.1f);
 	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_WeaponUI"),
		Desc.eLevelID, strLayerTag, &Desc)))
		return E_FAIL;

	CUI_Manager::Get_Instance()->Initialize_GamePlayUI(LEVEL_STATIC);

	return S_OK;
}

CLevel_Logo* CLevel_Logo::Create(LPDIRECT3DDEVICE9 pGraphic_Device, class CLevelData* pLevelData)
{
	CLevel_Logo* pInstance = new CLevel_Logo(pGraphic_Device);

	if (FAILED(pInstance->Initialize(pLevelData)))
	{
		MSG_BOX("Failed to Created : CLevel_Logo");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLevel_Logo::Free()
{
	__super::Free();
	m_pBGM->Stop();
	Safe_Release(m_pBGM);
}
