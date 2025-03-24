#include "Loader.h"
#include "LevelData.h"

#include "BackGround.h"
#include "Terrain.h"
#include "RaceTerrain.h"
#include "RaceLandscape.h"
#include "Trapezoid.h"
#include "Stall.h"
#include "Cabinet.h"
#include "Signboard.h"
#include "MyComputer.h"
#include "Canopy.h"
#include "BuildingH.h"
#include "Explosion.h"


//테스트용
#include "MyCube.h"
#include "TestBullet.h"
/* 테스트 터레인*/
#include "TestTerrain.h"

//플레이어와 무기
#include "Player.h"
#include "PlayerOnBoat.h"
#include "Weapon_LoverBoy.h"
#include "Weapon_Chaingun.h"


//파티클 인클루드
#include "Explosion.h"
#include "Firework.h"
#include "EmptyBullet.h"
#include "Rain.h"
#include "Sprite.h"
#include "Tornado.h"
#include "Smoke.h"
#include "Dust.h"
#include "Sphere.h"
#include "CameraSprite.h"
#include "Blood.h"
#include "BulletImpactSpark.h"

//UI 인클루드
#include "Aim.h"
#include "Logo.h"
#include "Ammo.h"
#include "Armor.h"
#include "Portrait.h"
#include "MainMenu.h"
#include "ScreenDust.h"
#include "Button.h"
#include "Button_Main.h"

//맵 인클루드
#include "Block.h"

//전시용 플랫폼
#include "Flatform.h"

//몬스터
#include "Ttakkeun_i.h"
#include "Wenteko.h"
#include "Shotgunner.h"
#include "Nukemutant.h"
#include "Mechsect.h"
#include "Greater.h"
#include "Deacon.h"
#include "Cultist.h"
#include "Archangel.h"


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

	case LEVEL_RACE:
		hr = Loading_For_Race();
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

void CLoader::Clear_MapData()
{
	//m_pData->
}

HRESULT CLoader::Loading_For_Logo()
{
#pragma region 파티클 준비물(스테틱)

	///////////////////////////////////////////스프라이트//////////////////////////////////////////////////////

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Sprite"),
		CSprite::Create(m_pGraphic_Device))))
		return E_FAIL;
	m_fLoadPercent += 0.03f;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Flatform"),
		CFlatform::Create(m_pGraphic_Device))))
		return E_FAIL;
	m_fLoadPercent += 0.03f;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_CameraSprite"),
		CCameraSprite::Create(m_pGraphic_Device))))
		return E_FAIL;
	m_fLoadPercent += 0.03f;

	////////////////////////////////////////////파티클//////////////////////////////////////////////////////

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_BulletImpactSpark"),
		CBulletImpactSpark::Create(m_pGraphic_Device, L"PARTICLE_BulletImpactSpark"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Blood"),
		CBlood::Create(m_pGraphic_Device, L"PARTICLE_Blood"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Sphere"),
		CSphere::Create(m_pGraphic_Device, L"PARTICLE_Sphere"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Tornado"),
		CTornado::Create(m_pGraphic_Device, L"PARTICLE_Tornado"))))
		return E_FAIL;
	m_fLoadPercent += 0.03f;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Rain"),
		CRain::Create(m_pGraphic_Device, L"PARTICLE_SNOW", 200, _float3(550.f, 0.f, -350.f), _float3(1050.f, 500.f, -50.f)))))
		return E_FAIL;
	m_fLoadPercent += 0.03f;

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

	////////////////////////////////////////////텍스처//////////////////////////////////////////////////////

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_FireMachineGun"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Particle/FireMachineGun%d.png"), 7))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GunFireMachineGun"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Particle/GunFireMachineGun%d.png"), 7))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_PS_Blood"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Particle/PS_Blood%d.png"), 5))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Explorer"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Particle/Effect_Explorer%d.png"), 24))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Revolver"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Particle/Effect_Revolver%d.png"), 3))))
		return E_FAIL;
	m_fLoadPercent += 0.03f;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_RevolverTacer"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Particle/Effect_RevolverTacer%d.png"), 3))))
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

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_PC_Fire"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Particle/PC_Fire%d.png"), 20))))
		return E_FAIL;
	m_fLoadPercent += 0.03f;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MonsterFlatform"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Monster/Flatform/Flatform.PNG"), 1))))
		return E_FAIL;
	m_fLoadPercent += 0.03f;

#pragma endregion

#pragma region TEXTURE
	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));
	ADD_TEXTURE(BackGround, "../Bin/Resources/Textures/Default%d.jpg", 2);
	m_fLoadPercent += 0.03f;
	ADD_TEXTURE(MainMenu, "../Bin/Resources/Textures/UI/Background/MainMenu_Background.PNG", 1);
	m_fLoadPercent += 0.03f;
	ADD_TEXTURE(Screen_Dust, "../Bin/Resources/Textures/Particle/ScreenDust/Screen_Dust%d.PNG", 50);
	ADD_TEXTURE(Save_Background, "../Bin/Resources/Textures/UI/Background/Save_Background.PNG", 1);
	m_fLoadPercent += 0.03f;
	for (size_t i = 0; i < 100000; i++)
	{
		for (size_t j = 0; j < 10000; j++)
		{
			int a = 0;
		}
		if (KEY_PRESSING(DIK_SPACE))
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
		if (KEY_PRESSING(DIK_SPACE))
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
		if (KEY_PRESSING(DIK_SPACE))
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
		if (KEY_PRESSING(DIK_SPACE))
			break;
	}
#pragma endregion
	m_fLoadPercent += 0.03f;

#pragma region PRTOBJ
	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩중입니다."));

	ADD_PRTOBJ(BackGround);
	m_fLoadPercent += 0.03f;
	ADD_PRTOBJ(MainMenu);
	ADD_PRTOBJ(ScreenDust);
	ADD_PRTOBJ(Button);
	m_fLoadPercent = 4.f / 5.f;
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
		if (KEY_PRESSING(DIK_SPACE))
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
		if (KEY_PRESSING(DIK_SPACE))
			break;
	}
	m_fLoadPercent += 0.03f;

	for (size_t i = 0; i < 100000; i++)
	{
		for (size_t j = 0; j < 10000; j++)
		{
			int a = 0;
			
		}
		if (KEY_PRESSING(DIK_SPACE))
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
	
	/* 터레인 텍스쳐 잠깐 바꾸겠습니다 */
	ADD_TEXTURE(Terrain, "../Bin/Resources/Textures/Check_Tile.PNG", 1);
	// ADD_TEXTURE(Terrain, "../Bin/Resources/Textures/Map/tile6191.PNG", 1);

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
	ADD_TEXTURE(Canopy, "../Bin/Resources/Textures/Object/Canopy/Canopy%d.png", 2);

	ADD_TEXTURE(TestBullet, "../Bin/Resources/Textures/Bullet/Test/tile7859.png", 1);
	ADD_TEXTURE(Weapon_LoverBoy, "../Bin/Resources/Textures/Weapon/LoverBoy/LoverBoy%d.PNG", 15);
	ADD_TEXTURE(LeftHand, "../Bin/Resources/Textures/Weapon/LeftHand/LeftHand%d.PNG", 2);
	ADD_TEXTURE(Weapon_Chaingun, "../Bin/Resources/Textures/Weapon/ChainGun/ChainGun%d.PNG", 16);

	ADD_TEXTURE(TTest, "../Bin/Resources/Textures/Object/Cabinet/Cabinet%d.png", 3);
	ADD_TEXTURE(Test, "../Bin/Resources/Textures/TileTest/tile%d.PNG", 121);


#pragma region BOSS_TEXTURES
	ADD_TEXTURE(Boss_Walk_0, "../bin/Resources/Textures/Monster/boss/walk/0/boss_walk_0_%d.PNG", 12);
	ADD_TEXTURE(Boss_Walk_22, "../bin/Resources/Textures/Monster/boss/walk/22/boss_walk_22_%d.PNG", 12);
	ADD_TEXTURE(Boss_Walk_45, "../bin/Resources/Textures/Monster/boss/walk/45/boss_walk_45_%d.PNG", 12);
	ADD_TEXTURE(Boss_Walk_67, "../bin/Resources/Textures/Monster/boss/walk/67/boss_walk_67_%d.PNG", 12);
	ADD_TEXTURE(Boss_Walk_90, "../bin/Resources/Textures/Monster/boss/walk/90/boss_walk_90_%d.PNG", 12);
	ADD_TEXTURE(Boss_Walk_112, "../bin/Resources/Textures/Monster/boss/walk/112/boss_walk_112_%d.PNG", 12);
	ADD_TEXTURE(Boss_Walk_135, "../bin/Resources/Textures/Monster/boss/walk/135/boss_walk_135_%d.PNG", 12);
	ADD_TEXTURE(Boss_Walk_157, "../bin/Resources/Textures/Monster/boss/walk/157/boss_walk_157_%d.PNG", 12);
	ADD_TEXTURE(Boss_Walk_180, "../bin/Resources/Textures/Monster/boss/walk/180/boss_walk_180_%d.PNG", 12); 

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	ADD_TEXTURE(Boss_Jump_0, "../bin/Resources/Textures/Monster/boss/jump/0/boss_jump_0_%d.PNG", 5);
	ADD_TEXTURE(Boss_Jump_22, "../bin/Resources/Textures/Monster/boss/jump/22/boss_jump_22_%d.PNG", 5);
	ADD_TEXTURE(Boss_Jump_45, "../bin/Resources/Textures/Monster/boss/jump/45/boss_jump_45_%d.PNG", 5);
	ADD_TEXTURE(Boss_Jump_67, "../bin/Resources/Textures/Monster/boss/jump/67/boss_jump_67_%d.PNG", 5);
	ADD_TEXTURE(Boss_Jump_90, "../bin/Resources/Textures/Monster/boss/jump/90/boss_jump_90_%d.PNG", 5);
	ADD_TEXTURE(Boss_Jump_112, "../bin/Resources/Textures/Monster/boss/jump/112/boss_jump_112_%d.PNG", 5);
	ADD_TEXTURE(Boss_Jump_135, "../bin/Resources/Textures/Monster/boss/jump/135/boss_jump_135_%d.PNG", 5);
	ADD_TEXTURE(Boss_Jump_157, "../bin/Resources/Textures/Monster/boss/jump/157/boss_jump_157_%d.PNG", 5);
	ADD_TEXTURE(Boss_Jump_180, "../bin/Resources/Textures/Monster/boss/jump/180/boss_jump_180_%d.PNG", 5);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	/* Fly는 상하좌우 구분이 필요해서 일단 제외 */
	//ADD_TEXTURE(Boss_Fly_0, "../bin/Resources/Textures/Monster/boss/fly/0/boss_fly_0_%d.PNG", 5);
	//ADD_TEXTURE(Boss_Fly_22, "../bin/Resources/Textures/Monster/boss/fly/22/boss_fly_22_%d.PNG", 5);
	//ADD_TEXTURE(Boss_Fly_45, "../bin/Resources/Textures/Monster/boss/fly/45/boss_fly_45_%d.PNG", 5);
	//ADD_TEXTURE(Boss_Fly_67, "../bin/Resources/Textures/Monster/boss/fly/67/boss_fly_67_%d.PNG", 5);
	//ADD_TEXTURE(Boss_Fly_90, "../bin/Resources/Textures/Monster/boss/fly/90/boss_fly_90_%d.PNG", 5);
	//ADD_TEXTURE(Boss_Fly_112, "../bin/Resources/Textures/Monster/boss/fly/112/boss_fly_112_%d.PNG", 5);
	//ADD_TEXTURE(Boss_Fly_135, "../bin/Resources/Textures/Monster/boss/fly/135/boss_fly_135_%d.PNG", 5);
	//ADD_TEXTURE(Boss_Fly_157, "../bin/Resources/Textures/Monster/boss/fly/157/boss_fly_157_%d.PNG", 5);
	//ADD_TEXTURE(Boss_Fly_180, "../bin/Resources/Textures/Monster/boss/fly/180/boss_fly_180_%d.PNG", 5);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	//ADD_TEXTURE(Boss_Fly_Attack_0,   "../bin/Resources/Textures/Monster/boss/fly_attack/0/boss_fly_0_%d.PNG", 5);
	//ADD_TEXTURE(Boss_Fly_Attack_22,  "../bin/Resources/Textures/Monster/boss/fly_attack/22/boss_fly_22_%d.PNG", 5);
	//ADD_TEXTURE(Boss_Fly_Attack_45,  "../bin/Resources/Textures/Monster/boss/fly_attack/45/boss_fly_45_%d.PNG", 5);
	//ADD_TEXTURE(Boss_Fly_Attack_67,  "../bin/Resources/Textures/Monster/boss/fly_attack/67/boss_fly_67_%d.PNG", 5);
	//ADD_TEXTURE(Boss_Fly_Attack_90,  "../bin/Resources/Textures/Monster/boss/fly_attack/90/boss_fly_90_%d.PNG", 5);
	//ADD_TEXTURE(Boss_Fly_Attack_112, "../bin/Resources/Textures/Monster/boss/fly_attack/112/boss_fly_112_%d.PNG", 5);
	//ADD_TEXTURE(Boss_Fly_Attack_135, "../bin/Resources/Textures/Monster/boss/fly_attack/135/boss_fly_135_%d.PNG", 5);
	//ADD_TEXTURE(Boss_Fly_Attack_157, "../bin/Resources/Textures/Monster/boss/fly_attack/157/boss_fly_157_%d.PNG", 5);
	//ADD_TEXTURE(Boss_Fly_Attack_180, "../bin/Resources/Textures/Monster/boss/fly_attack/180/boss_fly_180_%d.PNG", 5);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	ADD_TEXTURE(Boss_Lava_Attack_0, "../bin/Resources/Textures/Monster/boss/lava/attack/0/boss_lava_attack_0_%d.PNG", 2);
	ADD_TEXTURE(Boss_Lava_Attack_22, "../bin/Resources/Textures/Monster/boss/lava/attack/22/boss_lava_attack_22_%d.PNG", 2);
	ADD_TEXTURE(Boss_Lava_Attack_45, "../bin/Resources/Textures/Monster/boss/lava/attack/45/boss_lava_attack_45_%d.PNG", 2);
	ADD_TEXTURE(Boss_Lava_Attack_67, "../bin/Resources/Textures/Monster/boss/lava/attack/67/boss_lava_attack_67_%d.PNG", 2);
	ADD_TEXTURE(Boss_Lava_Attack_90, "../bin/Resources/Textures/Monster/boss/lava/attack/90/boss_lava_attack_90_%d.PNG", 2);
	ADD_TEXTURE(Boss_Lava_Attack_112, "../bin/Resources/Textures/Monster/boss/lava/attack/112/boss_lava_attack_112_%d.PNG", 2);
	ADD_TEXTURE(Boss_Lava_Attack_135, "../bin/Resources/Textures/Monster/boss/lava/attack/135/boss_lava_attack_135_%d.PNG", 2);
	ADD_TEXTURE(Boss_Lava_Attack_157, "../bin/Resources/Textures/Monster/boss/lava/attack/157/boss_lava_attack_157_%d.PNG", 2);
	ADD_TEXTURE(Boss_Lava_Attack_180, "../bin/Resources/Textures/Monster/boss/lava/attack/180/boss_lava_attack_180_%d.PNG", 2);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	ADD_TEXTURE(Boss_Lava_DiveIn, "../bin/Resources/Textures/Monster/boss/lava/divein/boss_divein%d.PNG", 4);

#pragma endregion

#pragma region SHOTGUNNER_TEXTURES

	ADD_TEXTURE(Shotgunner_Attack_0,   "../Bin/Resources/Textures/Monster/Shotgunner/attack/0/shotgunner_attack_front%d.PNG", 3);
	ADD_TEXTURE(Shotgunner_Attack_45,  "../Bin/Resources/Textures/Monster/Shotgunner/attack/45/shotgunner_attack_sidefront%d.PNG", 3);
	ADD_TEXTURE(Shotgunner_Attack_90,  "../Bin/Resources/Textures/Monster/Shotgunner/attack/90/shotgunner_attack_side%d.PNG", 3);
	ADD_TEXTURE(Shotgunner_Attack_135, "../Bin/Resources/Textures/Monster/Shotgunner/attack/135/shotgunner_attack_sideback%d.PNG", 3);
	ADD_TEXTURE(Shotgunner_Attack_180, "../Bin/Resources/Textures/Monster/Shotgunner/attack/180/shotgunner_attack_back%d.PNG", 3);

	ADD_TEXTURE(Shotgunner_Death_0,   "../Bin/Resources/Textures/Monster/Shotgunner/death/shotgunner_death%d.PNG", 6); 

	ADD_TEXTURE(Shotgunner_Move_0,   "../bin/Resources/Textures/Monster/Shotgunner/move/0/shotgunner_move_front%d.PNG", 4);
	ADD_TEXTURE(Shotgunner_Move_45,  "../Bin/Resources/Textures/Monster/Shotgunner/move/45/shotgunner_move_sidefront%d.PNG", 4);
	ADD_TEXTURE(Shotgunner_Move_90,  "../Bin/Resources/Textures/Monster/Shotgunner/move/90/shotgunner_move_side%d.PNG", 4);
	ADD_TEXTURE(Shotgunner_Move_135, "../Bin/Resources/Textures/Monster/Shotgunner/move/135/shotgunner_move_sideback%d.PNG", 4);
	ADD_TEXTURE(Shotgunner_Move_180, "../Bin/Resources/Textures/Monster/Shotgunner/move/180/shotgunner_move_back%d.PNG", 4);


	// shock는 전기봉 추가되면 넣기 
	//ADD_TEXTURE(Shotgunner_Shock_0, "../Bin/Resources/Textures/Monster/Shotgunner/shock/0/shotgunner_shock_front%d.PNG", 3);
	//ADD_TEXTURE(Shotgunner_Shock_45, "../Bin/Resources/Textures/Monster/Shotgunner/shock/45/shotgunner_shock_sidefront%d.PNG", 3);
	//ADD_TEXTURE(Shotgunner_Shock_90, "../Bin/Resources/Textures/Monster/Shotgunner/shock/90/shotgunner_shock_side%d.PNG", 3);
	//ADD_TEXTURE(Shotgunner_Shock_135, "../Bin/Resources/Textures/Monster/Shotgunner/shock/135/shotgunner_shock_sideback%d.PNG", 3);
	//ADD_TEXTURE(Shotgunner_Shock_180, "../Bin/Resources/Textures/Monster/Shotgunner/shock/180/shotgunner_shock_back%d.PNG", 3);


#pragma endregion

#pragma region NUKEMUTANT_TEXTURES

	ADD_TEXTURE(Nukemutant_Move_0, "../Bin/Resources/Textures/Monster/Nukemutant/move/0/%d.PNG", 4);
	ADD_TEXTURE(Nukemutant_Move_45, "../Bin/Resources/Textures/Monster/Nukemutant/move/45/%d.PNG", 4);
	ADD_TEXTURE(Nukemutant_Move_90, "../Bin/Resources/Textures/Monster/Nukemutant/move/90/%d.PNG", 4);
	ADD_TEXTURE(Nukemutant_Move_135, "../Bin/Resources/Textures/Monster/Nukemutant/move/135/%d.PNG", 4);
	ADD_TEXTURE(Nukemutant_Move_180, "../Bin/Resources/Textures/Monster/Nukemutant/move/180/%d.PNG", 4);

#pragma endregion


#pragma region MECHSECT_TEXTURES

	ADD_TEXTURE(Mechsect_Move_0  , "../Bin/Resources/Textures/Monster/Mechsect/move/0/%d.PNG", 4);
	ADD_TEXTURE(Mechsect_Move_45 , "../Bin/Resources/Textures/Monster/Mechsect/move/45/%d.PNG", 4);
	ADD_TEXTURE(Mechsect_Move_90 , "../Bin/Resources/Textures/Monster/Mechsect/move/90/%d.PNG", 4);
	ADD_TEXTURE(Mechsect_Move_135, "../Bin/Resources/Textures/Monster/Mechsect/move/135/%d.PNG", 4);
	ADD_TEXTURE(Mechsect_Move_180, "../Bin/Resources/Textures/Monster/Mechsect/move/180/%d.PNG", 4);

#pragma endregion


#pragma region GREATER_TEXTURES

	ADD_TEXTURE(Greater_Move_0, "../Bin/Resources/Textures/Monster/greater/move/0/%d.PNG", 4);
	ADD_TEXTURE(Greater_Move_45, "../Bin/Resources/Textures/Monster/greater/move/45/%d.PNG", 4);
	ADD_TEXTURE(Greater_Move_90, "../Bin/Resources/Textures/Monster/greater/move/90/%d.PNG", 4);
	ADD_TEXTURE(Greater_Move_135, "../Bin/Resources/Textures/Monster/greater/move/135/%d.PNG", 4);
	ADD_TEXTURE(Greater_Move_180, "../Bin/Resources/Textures/Monster/greater/move/180/%d.PNG", 4);
#pragma endregion


#pragma region DEACON_TEXTURES
	ADD_TEXTURE(Deacon_Move_0, "../Bin/Resources/Textures/Monster/deacon/move/0/%d.PNG", 1);
	ADD_TEXTURE(Deacon_Move_45, "../Bin/Resources/Textures/Monster/deacon/move/45/%d.PNG", 1);
	ADD_TEXTURE(Deacon_Move_90, "../Bin/Resources/Textures/Monster/deacon/move/90/%d.PNG", 1);
	ADD_TEXTURE(Deacon_Move_135, "../Bin/Resources/Textures/Monster/deacon/move/135/%d.PNG", 1);
	ADD_TEXTURE(Deacon_Move_180, "../Bin/Resources/Textures/Monster/deacon/move/180/%d.PNG", 1);
#pragma endregion


#pragma region CULTIST_TEXTURES
	ADD_TEXTURE(Cultist_Move_0, "../Bin/Resources/Textures/Monster/cultist/move/0/%d.PNG", 4);
	ADD_TEXTURE(Cultist_Move_45, "../Bin/Resources/Textures/Monster/cultist/move/45/%d.PNG", 4);
	ADD_TEXTURE(Cultist_Move_90, "../Bin/Resources/Textures/Monster/cultist/move/90/%d.PNG", 4);
	ADD_TEXTURE(Cultist_Move_135, "../Bin/Resources/Textures/Monster/cultist/move/135/%d.PNG", 4);
	ADD_TEXTURE(Cultist_Move_180, "../Bin/Resources/Textures/Monster/cultist/move/180/%d.PNG", 4);
#pragma endregion

#pragma region ARCHANGEL_TEXTURES
	ADD_TEXTURE(Archangel_Move_0, "../Bin/Resources/Textures/Monster/archangel/move/0/%d.PNG", 4);
	ADD_TEXTURE(Archangel_Move_45, "../Bin/Resources/Textures/Monster/archangel/move/45/%d.PNG", 4);
	ADD_TEXTURE(Archangel_Move_90, "../Bin/Resources/Textures/Monster/archangel/move/90/%d.PNG", 4);
	ADD_TEXTURE(Archangel_Move_135, "../Bin/Resources/Textures/Monster/archangel/move/135/%d.PNG", 4);
	ADD_TEXTURE(Archangel_Move_180, "../Bin/Resources/Textures/Monster/archangel/move/180/%d.PNG", 4);
#pragma endregion
 
 
 
 
//#pragma region WENTEKO_TEXTURES
//	ADD_TEXTURE(_Move_0, "../Bin/Resources/Textures/Monster/      /move/0/%d.PNG", 4);
//	ADD_TEXTURE(_Move_45, "../Bin/Resources/Textures/Monster/     /move/45/%d.PNG", 4);
//	ADD_TEXTURE(_Move_90, "../Bin/Resources/Textures/Monster/     /move/90/%d.PNG", 4);
//	ADD_TEXTURE(_Move_135, "../Bin/Resources/Textures/Monster/    /move/135/%d.PNG", 4);
//	ADD_TEXTURE(_Move_180, "../Bin/Resources/Textures/Monster/    /move/180/%d.PNG", 4);
//#pragma endregion



	//몬스터
	ADD_TEXTURE(Ttakkeun_i_Walk, "../Bin/Resources/Textures/Monster/Ttakkeun_i/Ttakkeun_i_Walk%d.PNG", 12);
	ADD_TEXTURE(Wenteko_Walk, "../Bin/Resources/Textures/Monster/Wenteko/Wenteko_Walk%d.PNG", 6);
	ADD_TEXTURE(Shotgunner_Walk, "../Bin/Resources/Textures/Monster/Shotgunner/Shotgunner_Walk%d.PNG", 4);
	ADD_TEXTURE(Nukemutant_Walk, "../Bin/Resources/Textures/Monster/Nukemutant/Nukemutant_Walk%d.PNG", 4);
	ADD_TEXTURE(Mechsect_Walk, "../Bin/Resources/Textures/Monster/Mechsect/Mechsect_Walk%d.PNG", 4);
	ADD_TEXTURE(Greater_Run, "../Bin/Resources/Textures/Monster/Greater/Greater_Run%d.PNG", 4);
	ADD_TEXTURE(Deacon_Walk, "../Bin/Resources/Textures/Monster/Deacon/Deacon_Walk%d.PNG", 1);
	ADD_TEXTURE(Cultist_Run, "../Bin/Resources/Textures/Monster/Cultist/Cultist_Run%d.PNG", 4);
	ADD_TEXTURE(Archangel_Walk, "../Bin/Resources/Textures/Monster/Archangel/Archangel_Walk%d.PNG", 4);

#pragma endregion

#pragma region MODEL
	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩중입니다."));
	Load_For_Terrain(TEXT("MapData.txt"), TEXT("../bin/Resources/Textures/Terrain/BossMap.bmp"));
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
	ADD_PRTOBJ(Canopy);
	ADD_PRTOBJ(Player);
	ADD_PRTOBJ(TestBullet);
	ADD_PRTOBJ(Weapon_LoverBoy);
	ADD_PRTOBJ(Weapon_Chaingun);
	ADD_PRTOBJ(Block);
	ADD_PRTOBJ(BackGround);

	//몬스터
	ADD_PRTOBJ(Ttakkeun_i);
	ADD_PRTOBJ(Wenteko);
	ADD_PRTOBJ(Shotgunner);
	ADD_PRTOBJ(Nukemutant);
	ADD_PRTOBJ(Mechsect);
	ADD_PRTOBJ(Greater);
	ADD_PRTOBJ(Deacon);
	ADD_PRTOBJ(Cultist);
	ADD_PRTOBJ(Archangel);


#pragma endregion

#pragma region DATA
	lstrcpy(m_szLoadingText, TEXT("데이터를 읽어들이는 중입니다."));
	Add_Data(TEXT("GamePlayLevelData.csv"));

#pragma region BULLET
	CTestBullet::DESC BulletDesc{};
	BulletDesc.fSpeedPerSec = 3000.f;
	BulletDesc.vScale = { 3.f,3.f,3.f };
	CREATE_OBJ_POOL(TestBullet, 30, &BulletDesc);
#pragma endregion

#pragma region GUNFIRE
	CCameraSprite::DESC SpriteDesc{};
	SpriteDesc.bActive = false;
	SpriteDesc.fMaxFrame = 3;
	SpriteDesc.fRotationPerSec = RADIAN(180.f);
	SpriteDesc.fSpeedPerSec = 100.f;
	SpriteDesc.szTextureTag = TEXT("Effect_Revolver");
	SpriteDesc.vInitPos = _float3{ 750.f, 450.f, 0.1f };
	SpriteDesc.vScale = _float3{ 200.f, 200.f, 1.f };
	SpriteDesc.fAniSpeed = 20.f;
	SpriteDesc.bRandom = false;
	SpriteDesc.eEffectType = CCameraSprite::eEffectType::GUNFIRE;

	if (FAILED(m_pGameInstance->Create_Object_Pool(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_CameraSprite"),
		TEXT("ObjectPool_Effect_Revolver"), 2, &SpriteDesc)))
		return E_FAIL;
#pragma endregion

#pragma region BULLET_TRACER
	SpriteDesc.bActive = false;
	SpriteDesc.fMaxFrame = 3;
	SpriteDesc.fRotationPerSec = RADIAN(180.f);
	SpriteDesc.fSpeedPerSec = 100.f;
	SpriteDesc.szTextureTag = TEXT("Effect_RevolverTacer");
	SpriteDesc.vInitPos = _float3{ 700.f, 400.f, 0.2f };
	SpriteDesc.vScale = _float3{ 200.f, 200.f, 1.f };
	SpriteDesc.fAniSpeed = 20.f;
	SpriteDesc.bRandom = false;
	SpriteDesc.eEffectType = CCameraSprite::eEffectType::BULLETTRACER;

	CGameObject* pObject = nullptr;
	CGameObject** ppOut = &pObject;
	if (FAILED(m_pGameInstance->Create_Object_Pool(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_CameraSprite"),
		TEXT("ObjectPool_Effect_RevolverTacer"), 2, &SpriteDesc)))
		return E_FAIL;
#pragma endregion

#pragma region BLOOD
	CPSystem::DESC BloodDesc{};
	BloodDesc.fMaxFrame = 5;
	BloodDesc.szTextureTag = TEXT("PS_Blood");
	BloodDesc.fSize = 3.f;
	if (FAILED(m_pGameInstance->Create_Object_Pool(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Blood"),
		TEXT("ObjectPool_Effect_PS_Blood"), 20, &BloodDesc)))
		return E_FAIL;
#pragma endregion

#pragma region BULLETIMPACTSPARK
	CPSystem::DESC BulletImpactSparkDesc{};
	BulletImpactSparkDesc.vPosition.y += -20.f;
	BulletImpactSparkDesc.fMaxFrame = 1;
	BulletImpactSparkDesc.szTextureTag = TEXT("PC_Generic");
	BulletImpactSparkDesc.fSize = 0.45f;
	if (FAILED(m_pGameInstance->Create_Object_Pool(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_BulletImpactSpark"),
		TEXT("ObjectPool_PC_BulletImpactSpark"), 10, &BulletImpactSparkDesc)))
		return E_FAIL;
#pragma endregion

#pragma region EmptyBullet
	CPSystem::DESC EmptyBulletDesc{};
	EmptyBulletDesc.fMaxFrame = 7;
	EmptyBulletDesc.szTextureTag = TEXT("PC_BulletShell");
	EmptyBulletDesc.iParticleNums = 1;
	if (FAILED(m_pGameInstance->Create_Object_Pool(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_EmptyBullet"),
		TEXT("ObjectPool_PC_EmptyBullet"), 10, &EmptyBulletDesc)))
		return E_FAIL;
#pragma endregion

#pragma endregion

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));
	m_isFinished = true;
	m_fLoadPercent = 1.f;
	return S_OK;
}

HRESULT CLoader::Loading_For_Race()/**/
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));
	ADD_TEXTURE(RaceTerrain, "../Bin/Resources/Textures/Map/RaceTerrain/RaceTerrain%d.PNG", 2);
	ADD_TEXTURE(RaceLandscape, "../Bin/Resources/Textures/Object/RaceLandscape/Tile15415.PNG", 1);
	ADD_TEXTURE(PlayerOnBoat, "../Bin/Resources/Textures/Player/PlayerOnBoat/Tile15947.PNG", 1);
	ADD_TEXTURE(BuildingH, "../Bin/Resources/Textures/Object/BuildingH/BuildingH%d.PNG", 4);

	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩중입니다."));
	ADD_MODEL_EX(RaceTerrain, 10, 1000);
	ADD_MODEL(Rect);

	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩중입니다."));
	ADD_PRTOBJ(RaceTerrain);

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_RACE, TEXT("Prototype_GameObject_RaceLandscapeLeft"),
		CRaceLandscape::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_RACE, TEXT("Prototype_GameObject_RaceLandscapeRight"),
		CRaceLandscape::Create(m_pGraphic_Device))))
		return E_FAIL;

	ADD_PRTOBJ(PlayerOnBoat);
	ADD_PRTOBJ(BuildingH);

	lstrcpy(m_szLoadingText, TEXT("데이터를 읽어들이는 중입니다."));
	Add_Data(TEXT("GamePlayLevelData.csv"));

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Test()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));

	ADD_TEXTURE(TTest, "../Bin/Resources/Textures/Object/Cabinet/Cabinet%d.png", 3);
	ADD_TEXTURE(Test, "../Bin/Resources/Textures/TileTest/tile%d.PNG", 121);
	ADD_TEXTURE(BackGround, "../Bin/Resources/Textures/Default%d.jpg", 2);
	ADD_TEXTURE(Terrain, "../Bin/Resources/Textures/Check_Tile.PNG", 1);
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

HRESULT CLoader::Load_For_Terrain(const _wstring& strFileTag, const _wstring& strHeightMapPath)
{
	_bool bResult = { true };
	_wstring FilePath;
	FilePath = TEXT("../bin/Resources/MapData/") + strFileTag;
	_ulong dwByte = {};

	HANDLE hFile = CreateFile(FilePath.c_str(), GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX("파일 개방 실패");
		return E_FAIL;
	}

	_int iNumVertexX = {}, iNumVertexZ = {};

	_float3 vPosition = {}, vScale = {}, vAngle = {};
	while (true)
	{
		bResult = ReadFile(hFile, &iNumVertexX, sizeof(_int), &dwByte, NULL);
		bResult = ReadFile(hFile, &iNumVertexZ, sizeof(_int), &dwByte, NULL);

		switch (m_eNextLevelID)
		{
		case LEVEL_LOGO:
			break;

		case LEVEL_GAMEPLAY:
			ADD_MODEL_EX(Terrain, iNumVertexX, iNumVertexZ, strHeightMapPath.c_str());
			break;

		case LEVEL_TEST:
			break;

		default:
			break;
		}

		break;
	}

	CloseHandle(hFile);

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
