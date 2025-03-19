#include "Loader.h"
#include "LevelData.h"

#include "BackGround.h"
#include "Terrain.h"
#include "Trapezoid.h"
#include "Stall.h"
#include "Cabinet.h"
#include "Signboard.h"
#include "MyComputer.h"
#include "Explosion.h"


//테스트용
#include "MyCube.h"
#include "TestMonster.h"
#include "TestBullet.h"
/* 테스트 터레인*/
#include "TestTerrain.h"

//플레이어와 무기
#include "Player.h"
#include "Weapon_LoverBoy.h"


//파티클 인클루드
#include "Explosion.h"
#include "Firework.h"
#include "EmptyBullet.h"
#include "Rain.h"
#include "Sprite.h"
#include "Tornado.h"
#include "Smoke.h"
#include "Sphere.h"
#include "CameraSprite.h"

//UI 인클루드
#include "Aim.h"
#include "Logo.h"
#include "Ammo.h"
#include "Armor.h"
#include "Portrait.h"
#include "MainMenu.h"
#include "Button_Main.h"

//맵 인클루드
#include "Block.h"


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

	case LEVEL_TEST:
		hr = Loading_For_Test();
		break;
	}

	LeaveCriticalSection(&m_CriticalSection);

	if (FAILED(hr))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading_For_Logo()
{
#pragma region 파티클 준비물(스테틱)

	///////////////////////////////////////////스프라이트//////////////////////////////////////////////////////

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Sprite"),
		CSprite::Create(m_pGraphic_Device))))
		return E_FAIL;
	m_fLoadPercent += 0.03f;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Explosion"),
		CExplosion::Create(m_pGraphic_Device))))
		return E_FAIL;
	m_fLoadPercent += 0.03f;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_CameraSprite"),
		CCameraSprite::Create(m_pGraphic_Device))))
		return E_FAIL;
	m_fLoadPercent += 0.03f;
	for (size_t i = 0; i < 100000; i++)
	{
		for (size_t j = 0; j < 10000; j++)
		{
			int a = 0;
		}
		if (GetAsyncKeyState(VK_SPACE))
			break;
	}
	////////////////////////////////////////////파티클//////////////////////////////////////////////////////

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Sphere"),
		CSphere::Create(m_pGraphic_Device, L"PARTICLE_Sphere"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Tornado"),
		CTornado::Create(m_pGraphic_Device, L"PARTICLE_Tornado"))))
		return E_FAIL;
	m_fLoadPercent += 0.03f;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Rain"),
		CRain::Create(m_pGraphic_Device, L"PARTICLE_SNOW", 200, _float3(0.f, 0.f, 0.f), _float3(500.f, 500.f, 500.f)))))
		return E_FAIL;
	m_fLoadPercent += 0.03f;
	for (size_t i = 0; i < 100000; i++)
	{
		for (size_t j = 0; j < 10000; j++)
		{
			int a = 0;
		}
		if (GetAsyncKeyState(VK_SPACE))
			break;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Firework"),
		CFirework::Create(m_pGraphic_Device, L"PARTICLE_FIREWORK"))))
		return E_FAIL;
	m_fLoadPercent += 0.03f;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Smoke"),
		CSmoke::Create(m_pGraphic_Device, L"PARTICLE_SMOKE"))))
		return E_FAIL;
	m_fLoadPercent += 0.03f;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_EmptyBullet"),
		CEmptyBullet::Create(m_pGraphic_Device, L"PARTICLE_EMPTYBULLET"))))
		return E_FAIL;
	m_fLoadPercent += 0.03f;
	for (size_t i = 0; i < 100000; i++)
	{
		for (size_t j = 0; j < 10000; j++)
		{
			int a = 0;
		}
		if (GetAsyncKeyState(VK_SPACE))
			break;
	}
	////////////////////////////////////////////텍스처//////////////////////////////////////////////////////

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Explorer"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Particle/Effect_Explorer%d.png"), 24))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_BulletTacer"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Particle/Effect_BulletTacer%d.png"), 3))))
		return E_FAIL;
	m_fLoadPercent += 0.03f;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Explor"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Particle/Effect_Explor%d.png"), 32))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_PC_Generic"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Particle/PC_Generic.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_GunFire"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Particle/Effect_GunFire2%d.png"), 3))))
		return E_FAIL;
	m_fLoadPercent += 0.03f;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Check_Tile"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Particle/Check_Tile.png"), 1))))
		return E_FAIL;
	m_fLoadPercent += 0.03f;
	for (size_t i = 0; i < 100000; i++)
	{
		for (size_t j = 0; j < 10000; j++)
		{
			int a = 0;
		}
		if (GetAsyncKeyState(VK_SPACE))
			break;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_PC_Small_Fire"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Particle/PC_Small_Fire%d.png"), 5))))
		return E_FAIL;
	m_fLoadPercent += 0.03f;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_PC_Tornado"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Particle/PC_Tornado%d.png"), 25))))
		return E_FAIL;
	m_fLoadPercent += 0.03f;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_PC_Small_Smoke"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Particle/PC_Small_Smoke%d.png"), 20))))
		return E_FAIL;
	m_fLoadPercent += 0.03f;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_PC_Explosion"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Particle/PS_EXPLOSION%d.png"), 14))))
		return E_FAIL;
	m_fLoadPercent += 0.03f;
	for (size_t i = 0; i < 100000; i++)
	{
		for (size_t j = 0; j < 10000; j++)
		{
			int a = 0;
		}
		if (GetAsyncKeyState(VK_SPACE))
			break;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_PC_BulletShell"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Particle/PC_BulletShell%d.png"), 7))))
		return E_FAIL;
	m_fLoadPercent += 0.03f;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_PS_Firework"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Particle/PS_Firework%d.png"), 4))))
		return E_FAIL;
	m_fLoadPercent += 0.03f;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_PC_Rain"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Particle/PC_Rain.png"), 1))))
		return E_FAIL;
	m_fLoadPercent += 0.03f;
	for (size_t i = 0; i < 100000; i++)
	{
		for (size_t j = 0; j < 10000; j++)
		{
			int a = 0;
		}
		if (GetAsyncKeyState(VK_SPACE))
			break;
	}
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_PC_Fire"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Particle/PC_Fire%d.png"), 20))))
		return E_FAIL;
	m_fLoadPercent += 0.03f;

#pragma endregion

#pragma region TEXTURE
	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));
	ADD_TEXTURE(BackGround, "../Bin/Resources/Textures/Default%d.jpg", 2);
	m_fLoadPercent += 0.03f;
	ADD_TEXTURE(MainMenu, "../Bin/Resources/Textures/UI/Background/MainMenu_Background.PNG", 1);
	m_fLoadPercent += 0.03f;
	ADD_TEXTURE(Save_Background, "../Bin/Resources/Textures/UI/Background/Save_Background.PNG", 1);
	m_fLoadPercent += 0.03f;
	for (size_t i = 0; i < 100000; i++)
	{
		for (size_t j = 0; j < 10000; j++)
		{
			int a = 0;
		}
		if (GetAsyncKeyState(VK_SPACE))
			break;
	}
	ADD_TEXTURE(Logo, "../Bin/Resources/Textures/UI/Logo/logo0.PNG", 1);
	m_fLoadPercent += 0.03f;
	ADD_TEXTURE(Logo_Shadow, "../Bin/Resources/Textures/UI/Logo/logo1.PNG", 1);
	m_fLoadPercent += 0.03f;


	for (size_t i = 0; i < 100000; i++)
	{
		for (size_t j = 0; j < 10000; j++)
		{
			int a = 0;
		}
		if (GetAsyncKeyState(VK_SPACE))
			break;
	}
#pragma endregion

#pragma region MODEL
	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩중입니다."));
	for (size_t i = 0; i < 100000; i++)
	{
		for (size_t j = 0; j < 10000; j++)
		{
			int a = 0;
		}
		if (GetAsyncKeyState(VK_SPACE))
			break;
	}
	m_fLoadPercent += 0.03f;
#pragma endregion

#pragma region SOUND
	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩중입니다."));
	for (size_t i = 0; i < 100000; i++)
	{
		for (size_t j = 0; j < 10000; j++)
		{
			int a = 0;
		}
		if (GetAsyncKeyState(VK_SPACE))
			break;
	}
#pragma endregion
	m_fLoadPercent += 0.03f;

#pragma region PRTOBJ
	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩중입니다."));

	ADD_PRTOBJ(BackGround);
	m_fLoadPercent += 0.03f;
	ADD_PRTOBJ(MainMenu);
	m_fLoadPercent += 0.03f;
	ADD_PRTOBJ(Button_Main);
	m_fLoadPercent += 0.03f;
	ADD_PRTOBJ(Logo);
	for (size_t i = 0; i < 100000; i++)
	{
		for (size_t j = 0; j < 10000; j++)
		{
			int a = 0;
		}
		if (GetAsyncKeyState(VK_SPACE))
			break;
	}
	m_fLoadPercent += 0.03f;

#pragma endregion

#pragma region DATA
	lstrcpy(m_szLoadingText, TEXT("데이터를 읽어들이는 중입니다."));
	Add_Data(TEXT("GamePlayLevelData.csv"));
	for (size_t i = 0; i < 100000; i++)
	{
		for (size_t j = 0; j < 10000; j++)
		{
			int a = 0;
		}
		if (GetAsyncKeyState(VK_SPACE))
			break;
	}
	m_fLoadPercent += 0.03f;

	for (size_t i = 0; i < 100000; i++)
	{
		for (size_t j = 0; j < 10000; j++)
		{
			int a = 0;
			
		}
		if (GetAsyncKeyState(VK_SPACE))
			break;
	}
#pragma endregion

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));
	m_isFinished = true;
	m_fLoadPercent = 1.f;
	return S_OK;
}

HRESULT CLoader::Loading_For_GamePlay()
{
#pragma region TEXTURE
	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));
	ADD_TEXTURE(Terrain, "../Bin/Resources/Textures/Map/tile6191.PNG", 1);
	ADD_TEXTURE(MyCube, "../Bin/Resources/Textures/Snow/Snow.png", 1);
	ADD_TEXTURE(Aim, "../Bin/Resources/Textures/Aim/aim0.PNG", 1);
	ADD_TEXTURE(Armor, "../Bin/Resources/Textures/UI/Armor/armor%d.PNG", 3);
	ADD_TEXTURE(Ammo, "../Bin/Resources/Textures/UI/Ammo/ammo%d.PNG", 8);
  	ADD_TEXTURE(Portrait, "../Bin/Resources/Textures/UI/Portrait/portrait%d.PNG", 25);

	ADD_TEXTURE(Box, "../Bin/Resources/Textures/Object/Box/tile6628.png", 1);
	ADD_TEXTURE(Cabinet, "../Bin/Resources/Textures/Object/Cabinet/Cabinet%d.png", 3);
	ADD_TEXTURE(Trapezoid, "../Bin/Resources/Textures/Object/Trapezoid/Trapezoid%d.png", 2);
	ADD_TEXTURE(Stall, "../Bin/Resources/Textures/Object/Stall/Stall%d.png", 2);
	ADD_TEXTURE(Signboard, "../Bin/Resources/Textures/Object/Signboard/Signboard%d.png", 5);
	ADD_TEXTURE(MyComputer, "../Bin/Resources/Textures/Object/Computer/Computer%d.png", 3);

	ADD_TEXTURE(TestBullet, "../Bin/Resources/Textures/Bullet/Test/tile7859.png", 1);
	ADD_TEXTURE(TestMonster, "../Bin/Resources/Textures/Monster/Test/TestMonster%d.PNG", 2);
	ADD_TEXTURE(Weapon_LoverBoy, "../Bin/Resources/Textures/Weapon/LoverBoy/LoverBoy%d.PNG", 15);
	ADD_TEXTURE(LeftHand, "../Bin/Resources/Textures/Weapon/LeftHand/LeftHand%d.PNG", 2);

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
	ADD_PRTOBJ(Trapezoid);
	ADD_PRTOBJ(Stall);
	ADD_PRTOBJ(Cabinet);
	ADD_PRTOBJ(Signboard);
	ADD_PRTOBJ(MyComputer);
	ADD_PRTOBJ(Player);
	ADD_PRTOBJ(TestBullet);
	ADD_PRTOBJ(TestMonster);
	ADD_PRTOBJ(Weapon_LoverBoy);



#pragma endregion

#pragma region DATA
	lstrcpy(m_szLoadingText, TEXT("데이터를 읽어들이는 중입니다."));
	Add_Data(TEXT("GamePlayLevelData.csv"));
	//CRETAE_OBJ_POOL(MyCube, 50);
	
#pragma endregion

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));
	m_isFinished = true;
	m_fLoadPercent = 1.f;
	return S_OK;
}

HRESULT CLoader::Loading_For_Test()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));

	ADD_TEXTURE(TTest, "../Bin/Resources/Textures/Object/Cabinet/Cabinet%d.png", 3);
	ADD_TEXTURE(Test, "../Bin/Resources/Textures/TileTest/tile%d.PNG", 121);
	ADD_TEXTURE(BackGround, "../Bin/Resources/Textures/Default%d.jpg", 2);
	ADD_TEXTURE(Terrain, "../Bin/Resources/Textures/Map/Tile22/tile6498.PNG", 1);
	ADD_TEXTURE(MyCube, "../Bin/Resources/Textures/Snow/Snow.png", 1);
	ADD_TEXTURE(Box, "../Bin/Resources/Textures/Object/Box/tile6628.png", 1);
	ADD_TEXTURE(Cabinet, "../Bin/Resources/Textures/Object/Cabinet/Cabinet%d.png", 3);
	ADD_TEXTURE(Trapezoid, "../Bin/Resources/Textures/Object/Trapezoid/Trapezoid%d.PNG", 2);


	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩중입니다."));
	ADD_PRTOBJ(Cabinet);
	ADD_PRTOBJ(Block);
	ADD_PRTOBJ(BackGround);
	ADD_PRTOBJ(Trapezoid);
	ADD_PRTOBJ(Terrain);
	ADD_PRTOBJ(TestTerrain);

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
