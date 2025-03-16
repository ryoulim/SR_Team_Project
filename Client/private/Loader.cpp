#include "Loader.h"
#include "LevelData.h"

#include "BackGround.h"
#include "MainMenu.h"
#include "Terrain.h"
#include "Dynamic_Camera.h"
#include "MyCube.h"
#include "UI_Camera.h"
#include "Ammo.h"
#include "Armor.h"
#include "Portrait.h"
#include "Aim.h"
#include "Explosion.h"
#include "Trapezoid.h"
#include "Cabinet.h"
#include "Signboard.h"
#include "Player.h"
#include "TestMonster.h"
#include "TestBullet.h"

//파티클 인클루드
#include "Firework.h"
#include "Gun.h"
#include "Rain.h"


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
	ADD_TEXTURE(MainMenu, "../Bin/Resources/Textures/UI/Background/MainMenu_Background.PNG", 1);
	ADD_TEXTURE(Save_Background, "../Bin/Resources/Textures/UI/Background/Save_Background.PNG", 1);
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
	ADD_PRTOBJ(MainMenu);
#pragma endregion

#pragma region DATA
	lstrcpy(m_szLoadingText, TEXT("데이터를 읽어들이는 중입니다."));
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
	ADD_TEXTURE(Armor, "../Bin/Resources/Textures/UI/Armor/armor%d.PNG", 3);
	ADD_TEXTURE(Ammo, "../Bin/Resources/Textures/UI/Ammo/ammo%d.PNG", 8);
  	ADD_TEXTURE(Portrait, "../Bin/Resources/Textures/UI/Portrait/portrait%d.PNG", 25);
	ADD_TEXTURE(Explosion, "../Bin/Resources/Textures/Explosion/Explosion%d.png", 90);

	ADD_TEXTURE(Box, "../Bin/Resources/Textures/Object/Box/tile6628.png", 1);
	ADD_TEXTURE(Cabinet, "../Bin/Resources/Textures/Object/Cabinet/Cabinet%d.png", 3);
	ADD_TEXTURE(Trapezoid, "../Bin/Resources/Textures/Object/Trapezoid/Trapezoid%d.png", 2);
	ADD_TEXTURE(Signboard, "../Bin/Resources/Textures/Object/Signboard/Signboard%d.png", 5);

	ADD_TEXTURE(TestBullet, "../Bin/Resources/Textures/Bullet/Test/tile7859.png", 1);
	ADD_TEXTURE(TestMonster, "../Bin/Resources/Textures/Monster/Test/TestMonster%d.PNG", 2);

	//파티클
	ADD_TEXTURE(PC_Bullet, "../Bin/Resources/Textures/Particle/PC_Bullet.png", 1);
	ADD_TEXTURE(PS_Firework, "../Bin/Resources/Textures/Particle/PS_Firework%d.png", 4);
	ADD_TEXTURE(PC_Rain, "../Bin/Resources/Textures/Particle/PC_Rain.png", 1);


	
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
	ADD_PRTOBJ(Armor);
	ADD_PRTOBJ(Ammo);
	ADD_PRTOBJ(Portrait);
	ADD_PRTOBJ(Explosion);
	ADD_PRTOBJ(Trapezoid);
	ADD_PRTOBJ(Cabinet);
	ADD_PRTOBJ(Signboard);
	ADD_PRTOBJ(Player);
	ADD_PRTOBJ(TestBullet);
	ADD_PRTOBJ(TestMonster);

#pragma region 파티클 원형객체
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Snow"),
		CRain::Create(m_pGraphic_Device, L"PARTICLE_SNOW", 5000, _float3(0.f, 0.f, 0.f), _float3(500.f, 500.f, 500.f)))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Firework"),
		CFirework::Create(m_pGraphic_Device, L"PARTICLE_FIREWORK"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Gun"),
		CGun::Create(m_pGraphic_Device, L"PARTICLE_GUN"))))
		return E_FAIL;
#pragma endregion


#pragma endregion

#pragma region DATA
	lstrcpy(m_szLoadingText, TEXT("데이터를 읽어들이는 중입니다."));
	Add_Data(TEXT("GamePlayLevelData.csv"));
	//CRETAE_OBJ_POOL(MyCube, 50);
	
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
