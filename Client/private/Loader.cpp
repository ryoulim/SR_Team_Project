#include "Loader.h"
#include "LevelData.h"

#include "BackGround.h"
#include "Terrain.h"
#include "Dynamic_Camera.h"
#include "MyCube.h"
#include "UI_Camera.h"
#include "Portrait.h"
#include "Aim.h"
#include "Explosion.h"
#include "Trapezoid.h"

CLoader::CLoader(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device { pGraphic_Device }
	, m_pGameInstance { CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pGraphic_Device);
}

_uint APIENTRY LoadingMain(void* pArg)
{
	/* 자원로딩한다. */
	CLoader*		pLoader = static_cast<CLoader*>(pArg);

	if (FAILED(pLoader->Loading()))
		return 1;	

	return 0;

}

HRESULT CLoader::Initialize(LEVEL eNextLevelID)
{
	m_eNextLevelID = eNextLevelID;

		m_pData = CLevelData::Create();
	if (nullptr == m_pData)
		return E_FAIL;

	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingMain, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading()
{
	EnterCriticalSection(&m_CriticalSection);

	HRESULT		hr = {};

	switch (m_eNextLevelID)
	{
	case LEVEL_LOGO:
		hr = Loading_For_Logo();
		break;

	case LEVEL_GAMEPLAY:
		hr = Loading_For_GamePlay();
		break;
	}

	LeaveCriticalSection(&m_CriticalSection);

	if (FAILED(hr))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading_For_Logo()
{
#pragma region TEXTURE
	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));
	ADD_TEXTURE(BackGround, "../Bin/Resources/Textures/Default%d.jpg", 2);
#pragma endregion

#pragma region MODEL
	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩중입니다."));
#pragma endregion

#pragma region SOUND
	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩중입니다."));
#pragma endregion

#pragma region PRTOBJ
	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩중입니다."));
	ADD_PRTOBJ(BackGround);
	Add_Data(TEXT("GamePlayLevelData.csv"));
#pragma endregion

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));
	m_isFinished = true;
	return S_OK;
}

HRESULT CLoader::Loading_For_GamePlay()
{
#pragma region TEXTURE
	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));
	ADD_TEXTURE(Terrain, "../Bin/Resources/Textures/Map/tile6191.PNG", 1);
	ADD_TEXTURE(MyCube, "../Bin/Resources/Textures/Snow/Snow.png", 1);
	ADD_TEXTURE(Aim, "../Bin/Resources/Textures/Aim/tile11426.PNG", 1);
	ADD_TEXTURE(Portrait, "../Bin/Resources/Textures/UI/tile5535.PNG", 1);
	ADD_TEXTURE(Explosion, "../Bin/Resources/Textures/Explosion/Explosion%d.png", 90);

	ADD_TEXTURE(Box, "../Bin/Resources/Textures/Object/Box/tile6628.png", 1);
	ADD_TEXTURE(Trapezoid, "../Bin/Resources/Textures/Object/Trapezoid/tile6153.png", 1);
	//ADD_TEXTURE(Trapezoid, "../Bin/Resources/Textures/Object/Trapezoid/tile9896.png", 1);
	
#pragma endregion

#pragma region MODEL
	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩중입니다."));

#pragma endregion

#pragma region SOUND
	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩중입니다."));
#pragma endregion

#pragma region PRTOBJ
	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩중입니다."));
	ADD_PRTOBJ(Terrain);
	ADD_PRTOBJ(MyCube);
	ADD_PRTOBJ(Aim);
	ADD_PRTOBJ(Portrait);
	ADD_PRTOBJ(Explosion);
	ADD_PRTOBJ(Trapezoid);
	Add_Data(TEXT("GamePlayLevelData.csv"));

#pragma endregion

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));
	m_isFinished = true;
	return S_OK;
}

HRESULT CLoader::Add_Data(const _wstring& strFilePath)
{
	_wstring strFullPath = L"../bin/Resources/Data/" + strFilePath;
	if (FAILED(m_pGameInstance->Readcsv(strFullPath, m_pData)))
	{
		Safe_Release(m_pData);
		return E_FAIL;
	}

	return S_OK;
}

CLoader* CLoader::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevelID)
{
	CLoader* pInstance = new CLoader(pGraphic_Device);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed to Created : CLoader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	__super::Free();

	WaitForSingleObject(m_hThread, INFINITE);

	DeleteObject(m_hThread);

	CloseHandle(m_hThread);

	Safe_Release(m_pData);

	Safe_Release(m_pGameInstance);

	Safe_Release(m_pGraphic_Device);

	DeleteCriticalSection(&m_CriticalSection);
}
