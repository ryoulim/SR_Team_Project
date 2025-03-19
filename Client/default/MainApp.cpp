#include "MainApp.h"
#include "GameInstance.h"

#include "Level_Logo.h"
#include "Level_Loading.h"

#include "Dynamic_Camera.h"
#include "FPS_Camera.h"
#include "UI_Camera.h"

#include "Font_MediumBlue.h"
#include "Font_BigOrange.h"
#include "LoadingMenu.h"
#include "UI_Manager.h"
#include "FadeUI.h"

#include "DebugMode.h"
#include "CameraManager.h"

#ifdef _IMGUI

void ImGui::Render_Begin()
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGui::Render_End()
{
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

HRESULT CMainApp::Ready_Imgui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	ImGui::StyleColorsDark();
	io.Fonts->AddFontFromFileTTF("C://Windows//Fonts//gulim.ttc", 14.0f, nullptr, io.Fonts->GetGlyphRangesKorean());

	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX9_Init(m_pGraphic_Device);

	return S_OK;
}

HRESULT CMainApp::Free_Imgui()
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	return S_OK;
}

#endif

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

#ifdef _IMGUI
	if (FAILED(Ready_Imgui()))
		return E_FAIL;
#endif

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

	ShowCursor(FALSE);

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
	ADD_MODEL(Stall);
	ADD_MODEL(Cabinet);
	ADD_MODEL(Signboard);
	ADD_MODEL(Computer);
	ADD_MODEL_EX(Terrain, 129, 129, TEXT("../Bin/Resources/Textures/Terrain/Height__.bmp"));

	ADD_PRTCOM(Transform);
	ADD_PRTCOM(Gravity);
	ADD_PRTCOM(Collider_AABB_Cube);
	ADD_PRTCOM(Collider_OBB_Cube);
	ADD_PRTCOM(Collider_Sphere);
	ADD_PRTCOM(Collider_Capsule);
	ADD_PRTCOM(Collider_Line);

	ADD_TEXTURE(Font_MediumBlue, "../Bin/Resources/Textures/UI/Font/Font_MediumBlue/font%d.PNG", 94);
	ADD_TEXTURE(Font_BigOrange, "../Bin/Resources/Textures/UI/Font/Font_BigOrange/font%d.PNG", 46);

	ADD_TEXTURE(LoadingMenu, "../Bin/Resources/Textures/UI/Background/MainMenu_Background_Orig.png", 1);
	ADD_TEXTURE(Loading_Anim, "../Bin/Resources/Textures/UI/Loading/loadinganim%d.PNG", 8);
	ADD_TEXTURE(Loading_BarBack, "../Bin/Resources/Textures/UI/Loading/loadingbar0.PNG", 1);
	ADD_TEXTURE(Loading_Bar, "../Bin/Resources/Textures/UI/Loading/loadingbar1.PNG", 1);

	ADD_TEXTURE(FadeUI, "../Bin/Resources/Textures/UI/black.PNG", 1);
	//ADD_TEXTURE(DipWhiteUI, "../Bin/Resources/Textures/UI/white.PNG", 1);

	return S_OK;
}

HRESULT CMainApp::Ready_Object_For_Static()
{
	ADD_PRTOBJ(CameraManager);
	ADD_PRTOBJ(Dynamic_Camera);
	ADD_PRTOBJ(FPS_Camera);
	ADD_PRTOBJ(UI_Camera);
	ADD_PRTOBJ(Font_MediumBlue);
	ADD_PRTOBJ(Font_BigOrange);
	ADD_PRTOBJ(LoadingMenu);
	ADD_PRTOBJ(FadeUI);

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
	/* 둘만한 곳이 여기밖에 안 보여서 임시로 둡니다 .. */
	CUI_Camera::DESC UIDesc{};
	UIDesc.fFar = 1000.f;
	UIDesc.fNear = 0.f;
	UIDesc.fFov = 0;
	UIDesc.vAt = {0.f,0.f,1.f};
	UIDesc.vEye = { 0.f,0.f,0.f };
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Camera"),
		LEVEL_STATIC, TEXT("Layer_Camera"), &UIDesc)))
		return E_FAIL;

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

#ifdef _IMGUI
	Free_Imgui();
#endif

	CUI_Manager::Destroy_Instance();
	Safe_Release(m_pGraphic_Device);

	m_pGameInstance->Release_Engine();

	/* 내멤버를 정리한다.*/	
	Safe_Release(m_pGameInstance);
}


