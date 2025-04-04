#include "MainApp.h"
#include "GameInstance.h"

#include "Level_Logo.h"
#include "Level_Loading.h"

#include "CameraManager.h"
#include "FPS_Camera.h"
#include "TPS_Camera.h"
#include "Dynamic_Camera.h"
#include "CutScene_Camera.h"
#include "UI_Camera.h"

#include "Font_MediumBlue.h"
#include "Font_BigOrange.h"
#include "Shader.h"
#include "LoadingMenu.h"
#include "LevelLoadingMenu.h"
#include "UI_Manager.h"
#include "FadeUI.h"
#include "FXMgr.h"

#include "DebugMode.h"

#include "Sky.h"
#include "PlayerOnBoat.h"

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
#ifdef _CONSOL

	if (::AllocConsole() == TRUE)
	{
		FILE* nfp[3];
		freopen_s(nfp + 0, "CONOUT$", "rb", stdin);
		freopen_s(nfp + 1, "CONOUT$", "wb", stdout);
		freopen_s(nfp + 2, "CONOUT$", "wb", stderr);
		std::ios::sync_with_stdio();
	}

#endif

	ENGINE_DESC		Desc{};
	Desc.hInst = g_hInst;
	Desc.hWnd = g_hWnd;
	Desc.isWindowed = true;
	Desc.iWinSizeX = g_iWinSizeX;
	Desc.iWinSizeY = g_iWinSizeY;
	Desc.iNumLevels = LEVEL_END;
	Desc.INumColliderGroups = CG_END;
	//Desc.strBankFilePath = "../bin/BankFiles/";

	if (FAILED(m_pGameInstance->Initialize_Engine(Desc, &m_pGraphic_Device)))
		return E_FAIL;

#ifdef _IMGUI
	if (FAILED(Ready_Imgui()))
		return E_FAIL;
#endif

	/*디폴트 셋팅을 설정하는 함수입니다.*/
	if (FAILED(Ready_Default_Setting()))
		return E_FAIL;

	/*로딩레벨에서 쓰이는 컴포넌트의 프로토타입을 생성하는 함수입니다.*/
	if (FAILED(Ready_Component_For_Static()))
		return E_FAIL;

	/*로딩레벨에서 쓰이는 오브젝트의 프로토타입을 생성하는 함수입니다.*/
	if(FAILED(Ready_Protype_Object_For_Static()))
		return E_FAIL;

	/*로딩레벨에서 쓰이는 오브젝트의 사본을 생성하는 함수입니다.*/
	if (FAILED(Ready_Object_For_Static()))
		return E_FAIL;

	/* 최초 보여줄 레벨을 할당하자. */
 	if(FAILED(Open_Level(LEVEL_LOGO)))
		return E_FAIL;

	/*FPS 출력용*/
	if (FAILED(Ready_Debug_Mode()))
		return E_FAIL;

	//if (FAILED(m_pGameInstance->LoadBank("Master")))
	//	return E_FAIL;

	//if (FAILED(m_pGameInstance->LoadBank("Master.strings")))
	//	return E_FAIL;

	ShowCursor(FALSE);

	return S_OK;
}

void CMainApp::Update(_float fTimeDelta)
{
	m_pGameInstance->Update_Engine(fTimeDelta);

	if(KEY_DOWN(DIK_ESCAPE))
		PostQuitMessage(0);

#pragma region 카메라 전환
	if (KEY_DOWN(DIK_F1))
		CAMERA_MANAGER->Switch(CCameraManager::FPS);
	if (KEY_DOWN(DIK_F2))
		CAMERA_MANAGER->Switch(CCameraManager::TPS);
	if (KEY_DOWN(DIK_F3))
		CAMERA_MANAGER->Switch(CCameraManager::DYNAMIC);
	if (KEY_DOWN(DIK_TAB))
	{
		CAMERA_MANAGER->Set_Mouse_Fix(m_bMouseToggle);
		m_bMouseToggle = !m_bMouseToggle;
	}

#ifdef _COLLIDERRENDER
	//CCollider::m_b
#endif
#pragma endregion

}

HRESULT CMainApp::Render()
{
	m_pGameInstance->Draw();

	return S_OK;
}

HRESULT CMainApp::Ready_Component_For_Static()
{
	ADD_MODEL(Rect);
	ADD_PRTCOM(Transform);
	ADD_TEXTURE(LevelLoadingMenu, "../Bin/Resources/Textures/UI/Loading/lvlloading%d.png", 4);

	ADD_TEXTURE(Font_MediumBlue, "../Bin/Resources/Textures/UI/Font/Font_MediumBlue/font%d.PNG", 94);
	ADD_TEXTURE(Font_BigOrange, "../Bin/Resources/Textures/UI/Font/Font_BigOrange/font%d.PNG", 46);

	ADD_TEXTURE(LoadingMenu, "../Bin/Resources/Textures/UI/Background/MainMenu_Background_Orig.png", 1);
	ADD_TEXTURE(Loading_Anim, "../Bin/Resources/Textures/UI/Loading/loadinganim%d.PNG", 8);
	ADD_TEXTURE(Loading_BarBack, "../Bin/Resources/Textures/UI/Loading/loadingbar0.PNG", 1);
	ADD_TEXTURE(Loading_Bar, "../Bin/Resources/Textures/UI/Loading/loadingbar1.PNG", 1);


	ADD_TEXTURE(FadeUI, "../Bin/Resources/Textures/UI/black.PNG", 1);

	ADD_PRTCOM_EX(Shader, L"../bin/ShaderFiles/Shader_TextureEffect.hlsl");
	
	if (((FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, _wstring(L"Prototype_Component_Shader_Particle"),
		CShader::Create(m_pGraphic_Device, L"../bin/ShaderFiles/Shader_Particle.hlsl"))))))
		return E_FAIL;

	return S_OK;
}

#include "Player.h"
#include "Weapon_LoverBoy.h"
#include "Weapon_Chaingun.h"
#include "Weapon_Dispenser.h"
#include "GrenadeBullet.h"

HRESULT CMainApp::Ready_Protype_Object_For_Static()
{
	ADD_PRTOBJ(Dynamic_Camera);
	ADD_PRTOBJ(FPS_Camera);
	ADD_PRTOBJ(TPS_Camera);
	ADD_PRTOBJ(CutScene_Camera);
	ADD_PRTOBJ(UI_Camera);

	ADD_PRTOBJ(LevelLoadingMenu); // 치워도 안돼

	// UI Ready
	ADD_PRTOBJ(Font_MediumBlue);
	ADD_PRTOBJ(Font_BigOrange);
	ADD_PRTOBJ(LoadingMenu);
	ADD_PRTOBJ(FadeUI);

	return S_OK;
}

HRESULT CMainApp::Ready_Object_For_Static()
{
	if (FAILED(m_pGameInstance->Add_Manager(
		TEXT("Camera_Manager"), CCameraManager::Create())))
		return E_FAIL;

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

	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);

	CGravity::Add_StandableObjLayerTag(CG_BLOCK);
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
#ifdef _CONSOL

	FreeConsole();

#endif

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
