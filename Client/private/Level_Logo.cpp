#include "Level_Logo.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "BackGround.h"
#include "Button.h"

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


	return S_OK;
}

void CLevel_Logo::Update(_float fTimeDelta)
{
	if (KEY_DOWN(VK_RETURN))
	{
		if (FAILED(m_pGameInstance->Change_Level(LEVEL_LOADING,
			CLevel_Loading::Create(m_pGraphic_Device, LEVEL_GAMEPLAY))))
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

	BackGroundDesc.vInitPos = { 0.f,0.f,1.f };
	BackGroundDesc.vScale = { FWINCX , FWINCY, 1.f };
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_LOGO, TEXT("Prototype_GameObject_MainMenu"),
		LEVEL_LOGO, TEXT("Layer_UI"), &BackGroundDesc)))
		return E_FAIL;

	BackGroundDesc.vInitPos = { 0.f,0.f,1.f };
	BackGroundDesc.vScale = { FWINCX , FWINCY, 1.f };
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_LOGO, TEXT("Prototype_GameObject_ScreenDust"),
		LEVEL_LOGO, TEXT("Layer_UI"), &BackGroundDesc)))
		return E_FAIL;

	CUI::DESC TestButtonDesc{};

	TestButtonDesc.vInitPos = { 280.f - g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f - 460.f,1.f };
	TestButtonDesc.vScale = { 140.f, 20.f, 0.1f };

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_LOGO, TEXT("Prototype_GameObject_Button"),
		LEVEL_LOGO, strLayerTag, &TestButtonDesc)))
		return E_FAIL;
	
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
}
