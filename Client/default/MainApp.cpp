#include "MainApp.h"
#include "GameInstance.h"

#include "Level_Logo.h"
#include "Level_Loading.h"

#include "Dynamic_Camera.h"
#include "UI_Camera.h"
#include "Font_ItemDialog.h"

#include "DebugMode.h"

// 매크로를 위한 매크로(건드리지 마시오)
#define m_eNextLevelID LEVEL_STATIC

CMainApp::CMainApp()
	: m_pGameInstance{ CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Initialize()
{
	ENGINE_DESC		Desc{};
	Desc.hInst = g_hInst;
	Desc.hWnd = g_hWnd;
	Desc.isWindowed = true;
	Desc.iWinSizeX = g_iWinSizeX;
	Desc.iWinSizeY = g_iWinSizeY;
	Desc.iNumLevels = LEVEL_END;

	if (FAILED(m_pGameInstance->Initialize_Engine(Desc, &m_pGraphic_Device)))
		return E_FAIL;

	if (FAILED(Ready_Default_Setting()))
		return E_FAIL;

	if (FAILED(Ready_Component_For_Static()))
		return E_FAIL;

	if(FAILED(Ready_Object_For_Static()))
		return E_FAIL;

	/* 최초 보여줄 레벨을 할당하자. */
	if (FAILED(Open_Level(LEVEL_LOGO)))
		return E_FAIL;

	if (FAILED(Ready_Debug_Mode()))
		return E_FAIL;

	return S_OK;
}

void CMainApp::Update(_float fTimeDelta)
{
	m_pGameInstance->Update_Engine(fTimeDelta);

	if(KEY_DOWN(VK_ESCAPE))
		PostQuitMessage(0);
}

HRESULT CMainApp::Render()
{
	m_pGameInstance->Draw();

	return S_OK;
}

HRESULT CMainApp::Ready_Component_For_Static()
{
	ADD_MODEL(Cube);
	ADD_MODEL(Rect);
	ADD_MODEL(Trapezoid);
	ADD_MODEL(Cabinet);
	ADD_MODEL_EX(Terrain, 129, 129, TEXT("../Bin/Resources/Textures/Terrain/Height__.bmp"));

	ADD_PRTCOM(Transform);
	ADD_PRTCOM(Gravity);

	ADD_TEXTURE(Font_ItemDialog, "../Bin/Resources/Textures/UI/Font/font%d.PNG", 94);

	return S_OK;
}

HRESULT CMainApp::Ready_Object_For_Static()
{
	ADD_PRTOBJ(Dynamic_Camera);
	ADD_PRTOBJ(UI_Camera);
	ADD_PRTOBJ(Font_ItemDialog);

	return S_OK;
}

HRESULT CMainApp::Ready_Debug_Mode()
{
	ADD_PRTOBJ(DebugMode);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_DebugMode"),
		LEVEL_STATIC, TEXT("Layer_DebugMode"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Open_Level(LEVEL eLevelID)
{
	if (FAILED(m_pGameInstance->Change_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, eLevelID))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Default_Setting()
{
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, false);

	return S_OK;
}

CMainApp* CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{
	__super::Free();

	Safe_Release(m_pGraphic_Device);

	m_pGameInstance->Release_Engine();

	/* 내멤버를 정리한다.*/	
	Safe_Release(m_pGameInstance);
}
