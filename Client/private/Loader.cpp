#include "Loader.h"
#include "LevelData.h"

#include "BackGround.h"
#include "Terrain.h"
#include "RaceTerrain.h"
#include "RaceLandscape.h"
#include "RaceGate.h"
#include "RaceCylinder.h"
#include "Trapezoid.h"
#include "Stall.h"
#include "Cabinet.h"
#include "Signboard.h"
#include "MyComputer.h"
#include "Canopy.h"
#include "StreetLampHead.h"
#include "BuildingH.h"
#include "BuildingW.h"
#include "BuildingV.h"
#include "BuildingU.h"
#include "Forest.h"
#include "Rock.h"
#include "Explosion.h"

//테스트용
#include "MyCube.h"
#include "TestBullet.h"

//플레이어와 무기
#include "Player.h"
#include "PlayerOnBoat.h"
#include "Weapon_LoverBoy.h"
#include "Weapon_Chaingun.h"
#include "Weapon_Dispenser.h"
#include "GrenadeBullet.h"

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
#include "FireAttack.h"
#include "FlyEffect.h"
#include "JumpAttack.h"
#include "MonsterGuidBullet.h"
#include "MonsterNormalBullet.h"

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
#include "TriangularPillar.h"
#include "AnimeRect.h"
#include "AnimeBlock.h"
#include "InvisibleBlock.h"
#include "Lava.h"
#include "AlphaBlock.h"
#include "AlphaRect.h"
#include "BossBridge.h"

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
#include "MonsterBullet.h"
#include "MonsterMissile.h"
#include "LevelLoadingMenu.h"

#include "Sky.h"

#include "Trigger.h"

/* 맵툴에서 넘어오는 텍스쳐 갯수, 건들지 말아주세요 감사합니다 */
#define NUMMAPTEX 136


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
		hr = Loding_For_Static();
		hr = Loading_For_Logo();
		break;

	case LEVEL_GAMEPLAY:
		hr = Loading_For_GamePlay();
		break;

	case LEVEL_RACEFIRST:
		hr = Loading_For_RaceFirst();
		break;

	case LEVEL_RACESECOND:
		hr = Loading_For_RaceSecond();
		break;

	case LEVEL_RACETHIRD:
		hr = Loading_For_RaceThird();
		break;

	case LEVEL_BOSS:
		hr = Loading_For_Boss();
		break;

	case LEVEL_INDOOR:
		hr = Loading_For_Indoor();
		break;
	}

	LeaveCriticalSection(&m_CriticalSection);

	if (FAILED(hr))
		return E_FAIL;

	return S_OK;
}

void CLoader::Output_LoadingText()
{
	SetWindowText(g_hWnd, m_szLoadingText);

	if (m_eNextLevelID == LEVEL_LOGO ||
		m_eNextLevelID == LEVEL_STATIC)
	{
		auto LoadingMenu = m_pGameInstance->Find_Object(LEVEL_LOADING, TEXT("Layer_UI"));
		dynamic_cast<CLoadingMenu*>(LoadingMenu)->Set_LoadingGauge(m_fLoadPercent);
	}
	else
	{
		auto LevelLoadingMenu = m_pGameInstance->Find_Object(LEVEL_LOADING, TEXT("Layer_UI"));
		dynamic_cast<CLevelLoadingMenu*>(LevelLoadingMenu)->Set_LoadingGauge(m_fLoadPercent);
	}
}

void CLoader::Clear_MapData()
{
	//m_pData->
}

HRESULT CLoader::Loding_For_Static()
{
	LEVEL Tmp = m_eNextLevelID;
	m_eNextLevelID = LEVEL_STATIC;

	// 모델
	ADD_MODEL(Cube);
	ADD_MODEL(CubeEx);
	ADD_MODEL(TriangularPillar);
	ADD_MODEL(Trapezoid);
	ADD_MODEL(Stall);
	ADD_MODEL(Cabinet);
	ADD_MODEL(Signboard);
	ADD_MODEL(Computer);
	ADD_MODEL(Canopy);
	ADD_MODEL(RaceLandscape);
	ADD_MODEL(BuildingH);
	ADD_MODEL(BuildingW);
	ADD_MODEL(BuildingV);
	ADD_MODEL(BuildingU);
	ADD_MODEL(RaceGate);
	ADD_MODEL(RaceCylinder);

	// 기능 컴포넌트
	ADD_PRTCOM(Gravity);
	ADD_PRTCOM(Collider_AABB_Cube);
	ADD_PRTCOM(Collider_OBB_Cube);
	ADD_PRTCOM(Collider_Sphere);
	ADD_PRTCOM(Collider_Capsule);
	ADD_PRTCOM(Collider_Line);
	ADD_PRTCOM(Collider_Rect);

	ADD_TEXTURE_EX(Sky, "../Bin/Resources/Textures/SkyBox/Sky_%d.dds", 1, CTexture::TYPE_CUBE); // 치워도?돼
	ADD_TEXTURE(Aim, "../Bin/Resources/Textures/Aim/aim0.PNG", 1);						// 치워도돼
	ADD_TEXTURE(Armor, "../Bin/Resources/Textures/UI/Armor/armor%d.PNG", 3);			// 치워도돼
	ADD_TEXTURE(Ammo, "../Bin/Resources/Textures/UI/Ammo/ammo%d.PNG", 8);				// 치워도돼
	ADD_TEXTURE(Portrait, "../Bin/Resources/Textures/UI/Portrait/portrait%d.PNG", 25);	// 치워도돼

#pragma region PLAYER
	ADD_TEXTURE(MyCube, "../Bin/Resources/Textures/Snow/Snow.png", 1);
	ADD_TEXTURE(GrenadeBullet, "../Bin/Resources/Textures/Bullet/Grenade/GrenadeBullet.png", 1);
	ADD_TEXTURE(Weapon_LoverBoy, "../Bin/Resources/Textures/Weapon/LoverBoy/LoverBoy%d.PNG", 15);
	ADD_TEXTURE(LeftHand, "../Bin/Resources/Textures/Weapon/LeftHand/LeftHand%d.PNG", 2);
	ADD_TEXTURE(Weapon_Chaingun, "../Bin/Resources/Textures/Weapon/ChainGun/ChainGun%d.PNG", 16);
	ADD_TEXTURE(Weapon_Dispenser, "../Bin/Resources/Textures/Weapon/Dispenser/Dispenser%d.PNG", 60);
	ADD_PRTOBJ(PlayerOnBoat);

#pragma endregion

	ADD_PRTOBJ(Trigger);
	ADD_PRTOBJ(Sky);
	ADD_PRTOBJ(Aim);
	ADD_PRTOBJ(Ammo);
	ADD_PRTOBJ(Portrait);
	ADD_PRTOBJ(Armor);

#pragma region PLAYER
	ADD_PRTOBJ(Player);
	ADD_PRTOBJ(Weapon_LoverBoy);
	ADD_PRTOBJ(Weapon_Chaingun);
	ADD_PRTOBJ(Weapon_Dispenser);
	ADD_PRTOBJ(GrenadeBullet);
#pragma endregion

#pragma region 몬스터 텍스처
#pragma region SHOTGUNNER_TEXTURES
	ADD_TEXTURE(Shotgunner_Attack_0, "../Bin/Resources/Textures/Monster/Shotgunner/attack/0/shotgunner_attack_front%d.PNG", 3);  
	ADD_TEXTURE(Shotgunner_Attack_45, "../Bin/Resources/Textures/Monster/Shotgunner/attack/45/shotgunner_attack_sidefront%d.PNG", 3);  
	ADD_TEXTURE(Shotgunner_Attack_90, "../Bin/Resources/Textures/Monster/Shotgunner/attack/90/shotgunner_attack_side%d.PNG", 3);  
	ADD_TEXTURE(Shotgunner_Attack_135, "../Bin/Resources/Textures/Monster/Shotgunner/attack/135/shotgunner_attack_sideback%d.PNG", 3);  
	ADD_TEXTURE(Shotgunner_Attack_180, "../Bin/Resources/Textures/Monster/Shotgunner/attack/180/shotgunner_attack_back%d.PNG", 3);  
	ADD_TEXTURE(Shotgunner_Death, "../Bin/Resources/Textures/Monster/Shotgunner/death/shotgunner_death%d.PNG", 6);  
	ADD_TEXTURE(Shotgunner_Move_0, "../bin/Resources/Textures/Monster/Shotgunner/move/0/shotgunner_move_front%d.PNG", 4);  
	ADD_TEXTURE(Shotgunner_Move_45, "../Bin/Resources/Textures/Monster/Shotgunner/move/45/shotgunner_move_sidefront%d.PNG", 4);  
	ADD_TEXTURE(Shotgunner_Move_90, "../Bin/Resources/Textures/Monster/Shotgunner/move/90/shotgunner_move_side%d.PNG", 4);  
	ADD_TEXTURE(Shotgunner_Move_135, "../Bin/Resources/Textures/Monster/Shotgunner/move/135/shotgunner_move_sideback%d.PNG", 4);  
	ADD_TEXTURE(Shotgunner_Move_180, "../Bin/Resources/Textures/Monster/Shotgunner/move/180/shotgunner_move_back%d.PNG", 4);  
#pragma endregion
#pragma region NUKEMUTANT_TEXTURES
	ADD_TEXTURE(Nukemutant_Move_0, "../Bin/Resources/Textures/Monster/Nukemutant/move/0/%d.PNG", 4);  
	ADD_TEXTURE(Nukemutant_Move_45, "../Bin/Resources/Textures/Monster/Nukemutant/move/45/%d.PNG", 4);  
	ADD_TEXTURE(Nukemutant_Move_90, "../Bin/Resources/Textures/Monster/Nukemutant/move/90/%d.PNG", 4);  
	ADD_TEXTURE(Nukemutant_Move_135, "../Bin/Resources/Textures/Monster/Nukemutant/move/135/%d.PNG", 4);  
	ADD_TEXTURE(Nukemutant_Move_180, "../Bin/Resources/Textures/Monster/Nukemutant/move/180/%d.PNG", 4);  
	ADD_TEXTURE(Nukemutant_Attack_0, "../Bin/Resources/Textures/Monster/Nukemutant/attack/0/%d.PNG", 5);  
	ADD_TEXTURE(Nukemutant_Attack_45, "../Bin/Resources/Textures/Monster/Nukemutant/attack/45/%d.PNG", 5);  
	ADD_TEXTURE(Nukemutant_Attack_90, "../Bin/Resources/Textures/Monster/Nukemutant/attack/90/%d.PNG", 5);  
	ADD_TEXTURE(Nukemutant_Attack_135, "../Bin/Resources/Textures/Monster/Nukemutant/attack/135/%d.PNG", 5);  
	ADD_TEXTURE(Nukemutant_Attack_180, "../Bin/Resources/Textures/Monster/Nukemutant/attack/180/%d.PNG", 5);  
	ADD_TEXTURE(Nukemutant_Dead, "../Bin/Resources/Textures/Monster/Nukemutant/death/%d.PNG", 6);  
	ADD_TEXTURE(Nukemutant_Revive, "../Bin/Resources/Textures/Monster/Nukemutant/revive/%d.PNG", 7);  
#pragma endregion
#pragma region MECHSECT_TEXTURES
	ADD_TEXTURE(Mechsect_Move_0, "../Bin/Resources/Textures/Monster/Mechsect/move/0/%d.PNG", 4);  
	ADD_TEXTURE(Mechsect_Move_45, "../Bin/Resources/Textures/Monster/Mechsect/move/45/%d.PNG", 4);  
	ADD_TEXTURE(Mechsect_Move_90, "../Bin/Resources/Textures/Monster/Mechsect/move/90/%d.PNG", 4);  
	ADD_TEXTURE(Mechsect_Move_135, "../Bin/Resources/Textures/Monster/Mechsect/move/135/%d.PNG", 4);  
	ADD_TEXTURE(Mechsect_Move_180, "../Bin/Resources/Textures/Monster/Mechsect/move/180/%d.PNG", 4);  
	ADD_TEXTURE(Mechsect_Jump_0, "../Bin/Resources/Textures/Monster/Mechsect/jump/0/%d.PNG", 4);  
	ADD_TEXTURE(Mechsect_Jump_45, "../Bin/Resources/Textures/Monster/Mechsect/jump/45/%d.PNG", 4);  
	ADD_TEXTURE(Mechsect_Jump_90, "../Bin/Resources/Textures/Monster/Mechsect/jump/90/%d.PNG", 4);  
	ADD_TEXTURE(Mechsect_Jump_135, "../Bin/Resources/Textures/Monster/Mechsect/jump/135/%d.PNG", 4);  
	ADD_TEXTURE(Mechsect_Jump_180, "../Bin/Resources/Textures/Monster/Mechsect/jump/180/%d.PNG", 4);  
	ADD_TEXTURE(Mechsect_Dead, "../Bin/Resources/Textures/Monster/Mechsect/death/%d.PNG", 7);  
#pragma endregion
#pragma region GREATER_TEXTURES
	ADD_TEXTURE(Greater_Move_0, "../Bin/Resources/Textures/Monster/greater/move/0/%d.PNG", 4);  
	ADD_TEXTURE(Greater_Move_45, "../Bin/Resources/Textures/Monster/greater/move/45/%d.PNG", 4);  
	ADD_TEXTURE(Greater_Move_90, "../Bin/Resources/Textures/Monster/greater/move/90/%d.PNG", 4);  
	ADD_TEXTURE(Greater_Move_135, "../Bin/Resources/Textures/Monster/greater/move/135/%d.PNG", 4);  
	ADD_TEXTURE(Greater_Move_180, "../Bin/Resources/Textures/Monster/greater/move/180/%d.PNG", 4);  
	ADD_TEXTURE(Greater_Run_0, "../Bin/Resources/Textures/Monster/greater/run/0/%d.PNG", 4);  
	ADD_TEXTURE(Greater_Run_45, "../Bin/Resources/Textures/Monster/greater/run/45/%d.PNG", 4);  
	ADD_TEXTURE(Greater_Run_90, "../Bin/Resources/Textures/Monster/greater/run/90/%d.PNG", 4);  
	ADD_TEXTURE(Greater_Run_135, "../Bin/Resources/Textures/Monster/greater/run/135/%d.PNG", 4);  
	ADD_TEXTURE(Greater_Run_180, "../Bin/Resources/Textures/Monster/greater/run/180/%d.PNG", 4);  
	ADD_TEXTURE(Greater_Attack_0, "../Bin/Resources/Textures/Monster/greater/Attack/0/%d.PNG", 3);  
	ADD_TEXTURE(Greater_Attack_45, "../Bin/Resources/Textures/Monster/greater/Attack/45/%d.PNG", 3);  
	ADD_TEXTURE(Greater_Attack_90, "../Bin/Resources/Textures/Monster/greater/Attack/90/%d.PNG", 3);  
	ADD_TEXTURE(Greater_Attack_135, "../Bin/Resources/Textures/Monster/greater/Attack/135/%d.PNG", 3);  
	ADD_TEXTURE(Greater_Attack_180, "../Bin/Resources/Textures/Monster/greater/Attack/180/%d.PNG", 3);  
	ADD_TEXTURE(Greater_Dead, "../Bin/Resources/Textures/Monster/greater/death/%d.PNG", 8);  
#pragma endregion
#pragma region DEACON_TEXTURES
	ADD_TEXTURE(Deacon_Fly_0, "../Bin/Resources/Textures/Monster/deacon/move/0/%d.PNG", 4);  
	ADD_TEXTURE(Deacon_Fly_45, "../Bin/Resources/Textures/Monster/deacon/move/45/%d.PNG", 4);  
	ADD_TEXTURE(Deacon_Fly_90, "../Bin/Resources/Textures/Monster/deacon/move/90/%d.PNG", 4);  
	ADD_TEXTURE(Deacon_Fly_135, "../Bin/Resources/Textures/Monster/deacon/move/135/%d.PNG", 4);  
	ADD_TEXTURE(Deacon_Fly_180, "../Bin/Resources/Textures/Monster/deacon/move/180/%d.PNG", 4);  
	ADD_TEXTURE(Deacon_Attack_0, "../Bin/Resources/Textures/Monster/deacon/attack/0/%d.PNG", 4);  
	ADD_TEXTURE(Deacon_Attack_45, "../Bin/Resources/Textures/Monster/deacon/attack/45/%d.PNG", 4);  
	ADD_TEXTURE(Deacon_Attack_90, "../Bin/Resources/Textures/Monster/deacon/attack/90/%d.PNG", 4);  
	ADD_TEXTURE(Deacon_Attack_135, "../Bin/Resources/Textures/Monster/deacon/attack/135/%d.PNG", 4);  
	ADD_TEXTURE(Deacon_Attack_180, "../Bin/Resources/Textures/Monster/deacon/attack/180/%d.PNG", 4); 
	ADD_TEXTURE(Deacon_Dead, "../Bin/Resources/Textures/Monster/deacon/death/%d.PNG", 8);  
#pragma endregion
#pragma region CULTIST_TEXTURES
	ADD_TEXTURE(Cultist_Move_0, "../Bin/Resources/Textures/Monster/cultist/move/0/%d.PNG", 4);  
	ADD_TEXTURE(Cultist_Move_45, "../Bin/Resources/Textures/Monster/cultist/move/45/%d.PNG", 4);  
	ADD_TEXTURE(Cultist_Move_90, "../Bin/Resources/Textures/Monster/cultist/move/90/%d.PNG", 4);  
	ADD_TEXTURE(Cultist_Move_135, "../Bin/Resources/Textures/Monster/cultist/move/135/%d.PNG", 4);  
	ADD_TEXTURE(Cultist_Move_180, "../Bin/Resources/Textures/Monster/cultist/move/180/%d.PNG", 4);  
	ADD_TEXTURE(Cultist_Run_0, "../Bin/Resources/Textures/Monster/cultist/run/0/%d.PNG", 4);  
	ADD_TEXTURE(Cultist_Run_45, "../Bin/Resources/Textures/Monster/cultist/run/45/%d.PNG", 4);  
	ADD_TEXTURE(Cultist_Run_90, "../Bin/Resources/Textures/Monster/cultist/run/90/%d.PNG", 4);  
	ADD_TEXTURE(Cultist_Run_135, "../Bin/Resources/Textures/Monster/cultist/run/135/%d.PNG", 4);  
	ADD_TEXTURE(Cultist_Run_180, "../Bin/Resources/Textures/Monster/cultist/run/180/%d.PNG", 4);  
	ADD_TEXTURE(Cultist_Attack_0, "../Bin/Resources/Textures/Monster/cultist/Attack/0/%d.PNG", 3);  
	ADD_TEXTURE(Cultist_Attack_45, "../Bin/Resources/Textures/Monster/cultist/Attack/45/%d.PNG", 3);  
	ADD_TEXTURE(Cultist_Attack_90, "../Bin/Resources/Textures/Monster/cultist/Attack/90/%d.PNG", 3);  
	ADD_TEXTURE(Cultist_Attack_135, "../Bin/Resources/Textures/Monster/cultist/Attack/135/%d.PNG", 3);  
	ADD_TEXTURE(Cultist_Attack_180, "../Bin/Resources/Textures/Monster/cultist/Attack/180/%d.PNG", 3);  
	ADD_TEXTURE(Cultist_Dead, "../Bin/Resources/Textures/Monster/cultist/death/%d.PNG", 6);  
#pragma endregion
#pragma region ARCHANGEL_TEXTURES
	ADD_TEXTURE(Archangel_Move_0, "../Bin/Resources/Textures/Monster/archangel/move/0/%d.PNG", 4);  
	ADD_TEXTURE(Archangel_Move_45, "../Bin/Resources/Textures/Monster/archangel/move/45/%d.PNG", 4);  
	ADD_TEXTURE(Archangel_Move_90, "../Bin/Resources/Textures/Monster/archangel/move/90/%d.PNG", 4);  
	ADD_TEXTURE(Archangel_Move_135, "../Bin/Resources/Textures/Monster/archangel/move/135/%d.PNG", 4);  
	ADD_TEXTURE(Archangel_Move_180, "../Bin/Resources/Textures/Monster/archangel/move/180/%d.PNG", 4);  
	ADD_TEXTURE(Archangel_Attack_0, "../Bin/Resources/Textures/Monster/archangel/attack/0/%d.PNG", 4);  
	ADD_TEXTURE(Archangel_Attack_45, "../Bin/Resources/Textures/Monster/archangel/attack/45/%d.PNG", 4);  
	ADD_TEXTURE(Archangel_Attack_90, "../Bin/Resources/Textures/Monster/archangel/attack/90/%d.PNG", 4);  
	ADD_TEXTURE(Archangel_Attack_135, "../Bin/Resources/Textures/Monster/archangel/attack/135/%d.PNG", 4);  
	ADD_TEXTURE(Archangel_Attack_180, "../Bin/Resources/Textures/Monster/archangel/attack/180/%d.PNG", 4);  
	ADD_TEXTURE(Archangel_Stay_0, "../Bin/Resources/Textures/Monster/archangel/idle/0/%d.PNG", 4);  
	ADD_TEXTURE(Archangel_Stay_45, "../Bin/Resources/Textures/Monster/archangel/idle/45/%d.PNG", 4);  
	ADD_TEXTURE(Archangel_Stay_90, "../Bin/Resources/Textures/Monster/archangel/idle/90/%d.PNG", 4);  
	ADD_TEXTURE(Archangel_Stay_135, "../Bin/Resources/Textures/Monster/archangel/idle/135/%d.PNG", 4);  
	ADD_TEXTURE(Archangel_Stay_180, "../Bin/Resources/Textures/Monster/archangel/idle/180/%d.PNG", 4);  
	ADD_TEXTURE(Archangel_Fly_0, "../Bin/Resources/Textures/Monster/archangel/fly/0/%d.PNG", 4);  
	ADD_TEXTURE(Archangel_Fly_45, "../Bin/Resources/Textures/Monster/archangel/fly/45/%d.PNG", 4);  
	ADD_TEXTURE(Archangel_Fly_90, "../Bin/Resources/Textures/Monster/archangel/fly/90/%d.PNG", 4);  
	ADD_TEXTURE(Archangel_Fly_135, "../Bin/Resources/Textures/Monster/archangel/fly/135/%d.PNG", 4);  
	ADD_TEXTURE(Archangel_Fly_180, "../Bin/Resources/Textures/Monster/archangel/fly/180/%d.PNG", 4);  
	ADD_TEXTURE(Archangel_Dead, "../Bin/Resources/Textures/Monster/archangel/death/%d.PNG", 12);  
#pragma endregion
#pragma endregion

#pragma region 몬스터 프로토타입
	ADD_PRTOBJ(Wenteko);  
	ADD_PRTOBJ(Shotgunner);  
	ADD_PRTOBJ(Nukemutant);  
	ADD_PRTOBJ(Mechsect);  
	ADD_PRTOBJ(Greater);  
	ADD_PRTOBJ(Deacon);  
	ADD_PRTOBJ(Cultist);  
	ADD_PRTOBJ(Archangel);  
#pragma endregion


#pragma region 파티클 준비물(스테틱)
	
	/* [ 스프라이트 ] */

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_MonsterGuidBullet"),
		CMonsterGuidBullet::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_MonsterBullet"),
		CMonsterBullet::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_MonsterNormalBullet"),
		CMonsterNormalBullet::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Sprite"),
		CSprite::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Flatform"),
		CFlatform::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_CameraSprite"),
		CCameraSprite::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* [ 파티클 ] */

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_JumpAttack"),
		CJumpAttack::Create(m_pGraphic_Device, L"PARTICLE_JumpAttack"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_FlyEffect"),
		CFlyEffect::Create(m_pGraphic_Device, L"PARTICLE_FlyEffect"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_MonsterMissile"),
		CMonsterMissile::Create(m_pGraphic_Device, L"PARTICLE_MonsterMissile"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_FireAttack"),
		CFireAttack::Create(m_pGraphic_Device, L"PARTICLE_FireAttack"))))
		return E_FAIL;
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
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Rain"),
		CRain::Create(m_pGraphic_Device, L"PARTICLE_SNOW", 200, _float3(550.f, 0.f, -350.f), _float3(1050.f, 500.f, -50.f)))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Firework"),
		CFirework::Create(m_pGraphic_Device, L"PARTICLE_FIREWORK"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Smoke"),
		CSmoke::Create(m_pGraphic_Device, L"PARTICLE_SMOKE"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_EmptyBullet"),
		CEmptyBullet::Create(m_pGraphic_Device, L"PARTICLE_EMPTYBULLET"))))
		return E_FAIL;

	/* [ 텍스쳐 ] */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MonsterBounce"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Bullet/MonsterBounce/MonsterBounce%d.PNG"), 4))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Dash"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Player/Dash/DashEffect%d.png"), 10))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_DeaconSpawn"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Particle/DeaconSpawn%d.png"), 8))))
		return E_FAIL; 
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_FireAttack"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Particle/FireAttack%d.png"), 20))))
		return E_FAIL; 
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
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_RevolverTacer"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Particle/Effect_RevolverTacer%d.png"), 3))))
		return E_FAIL; 
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Explor"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Particle/Effect_Explor%d.png"), 32))))
		return E_FAIL; 
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_PC_Generic"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Particle/PC_Generic.png"), 1))))
		return E_FAIL; 
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_GunFire"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Particle/Effect_GunFire2%d.png"), 3))))
		return E_FAIL; 
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Check_Tile"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Particle/Check_Tile.png"), 1))))
		return E_FAIL; 
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_PC_Small_Fire"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Particle/PC_Small_Fire%d.png"), 5))))
		return E_FAIL; 
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_PC_Tornado"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Particle/PC_Tornado%d.png"), 25))))
		return E_FAIL; 
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_PC_Small_Smoke"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Particle/PC_Small_Smoke%d.png"), 20))))
		return E_FAIL; 
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_PC_Explosion"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Particle/PS_EXPLOSION%d.png"), 14))))
		return E_FAIL; 
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_PC_BulletShell"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Particle/PC_BulletShell%d.png"), 7))))
		return E_FAIL; 
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_PS_Firework"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Particle/PS_Firework%d.png"), 4))))
		return E_FAIL; 
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_PC_Rain"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Particle/PC_Rain.png"), 1))))
		return E_FAIL; 
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_PC_Fire"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Particle/PC_Fire%d.png"), 20))))
		return E_FAIL; 
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MonsterFlatform"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Monster/Flatform/Flatform.PNG"), 1))))
		return E_FAIL; 
#pragma endregion

#pragma region 매니저 생성

	if (FAILED(m_pGameInstance->Add_Manager(
		TEXT("FX_Manager"), CFXMgr::Create())))
		return E_FAIL;

#pragma endregion

	m_eNextLevelID = Tmp;
	return S_OK;
}

HRESULT CLoader::Loading_For_Logo()
{
	float fDataNum = {49.f};
	float fDataCurNum = {};

#pragma region TEXTURE
	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));
	ADD_TEXTURE(BackGround, "../Bin/Resources/Textures/Default%d.jpg", 2);fDataCurNum++;m_fLoadPercent = fDataCurNum / fDataNum;
	
	ADD_TEXTURE(MainMenu, "../Bin/Resources/Textures/UI/Background/MainMenu_Background.PNG", 1);fDataCurNum++;m_fLoadPercent = fDataCurNum / fDataNum;
	
	ADD_TEXTURE(Screen_Dust, "../Bin/Resources/Textures/Particle/ScreenDust/Screen_Dust%d.PNG", 50);fDataCurNum++;m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Save_Background, "../Bin/Resources/Textures/UI/Background/Save_Background.PNG", 1);fDataCurNum++;m_fLoadPercent = fDataCurNum / fDataNum;

	ADD_TEXTURE(Logo, "../Bin/Resources/Textures/UI/Logo/logo0.PNG", 1);fDataCurNum++;m_fLoadPercent = fDataCurNum / fDataNum;
	
	ADD_TEXTURE(Logo_Shadow, "../Bin/Resources/Textures/UI/Logo/logo1.PNG", 1);fDataCurNum++;m_fLoadPercent = fDataCurNum / fDataNum;

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
	
	ADD_PRTOBJ(MainMenu);fDataCurNum++;m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(ScreenDust);fDataCurNum++;m_fLoadPercent = fDataCurNum / fDataNum;
	//ADD_PRTOBJ(Button);fDataCurNum++;m_fLoadPercent = fDataCurNum / fDataNum;
	m_fLoadPercent = 4.f / 5.f;
	
	ADD_PRTOBJ(Button_Main);fDataCurNum++;m_fLoadPercent = fDataCurNum / fDataNum;
	
	ADD_PRTOBJ(Logo);fDataCurNum++;m_fLoadPercent = fDataCurNum / fDataNum;
	

#pragma endregion

#pragma region DATA
	lstrcpy(m_szLoadingText, TEXT("데이터를 읽어들이는 중입니다."));
	Add_Data(TEXT("GamePlayLevelData.csv"));fDataCurNum++;m_fLoadPercent = fDataCurNum / fDataNum;
	m_fLoadPercent = 1.01f;

#pragma endregion

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));
	m_isFinished = true;
	m_fLoadPercent = 1.f;
	return S_OK;
}

HRESULT CLoader::Loading_For_GamePlay()
{
	float fDataNum = {188.f};
	float fDataCurNum = {};

#pragma region TEXTURE
	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));

	ADD_TEXTURE(Terrain, "../Bin/Resources/Textures/Check_Tile.PNG", 1);fDataCurNum++;m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Lava, "../Bin/Resources/Textures/Map/Lava/Tile%d.PNG", 16); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;

	ADD_TEXTURE(Box, "../Bin/Resources/Textures/Object/Box/tile6628.png", 1);fDataCurNum++;m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Cabinet, "../Bin/Resources/Textures/Object/Cabinet/Cabinet%d.png", 3);fDataCurNum++;m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Trapezoid, "../Bin/Resources/Textures/Object/Trapezoid/Trapezoid%d.png", 2);fDataCurNum++;m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Stall, "../Bin/Resources/Textures/Object/Stall/Stall%d.png", 2);fDataCurNum++;m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Signboard, "../Bin/Resources/Textures/Object/Signboard/Signboard%d.png", 5);fDataCurNum++;m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(MyComputer, "../Bin/Resources/Textures/Object/Computer/Computer%d.png", 3);fDataCurNum++;m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Canopy, "../Bin/Resources/Textures/Object/Canopy/Canopy%d.png", 2);fDataCurNum++;m_fLoadPercent = fDataCurNum / fDataNum;

	ADD_TEXTURE(TestBullet, "../Bin/Resources/Textures/Bullet/Test/tile7859.png", 1);fDataCurNum++;m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Test, "../Bin/Resources/Textures/TileTest/tile%d.PNG", NUMMAPTEX); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;

#pragma region BOSS_TEXTURES

	ADD_TEXTURE(Boss_Walk_0, "../bin/Resources/Textures/Monster/boss/walk/0/boss_walk_0_%d.PNG", 12); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Walk_22, "../bin/Resources/Textures/Monster/boss/walk/22/boss_walk_22_%d.PNG", 12); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Walk_45, "../bin/Resources/Textures/Monster/boss/walk/45/boss_walk_45_%d.PNG", 12); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Walk_67, "../bin/Resources/Textures/Monster/boss/walk/67/boss_walk_67_%d.PNG", 12); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Walk_90, "../bin/Resources/Textures/Monster/boss/walk/90/boss_walk_90_%d.PNG", 12); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Walk_112, "../bin/Resources/Textures/Monster/boss/walk/112/boss_walk_112_%d.PNG", 12); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Walk_135, "../bin/Resources/Textures/Monster/boss/walk/135/boss_walk_135_%d.PNG", 12); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Walk_157, "../bin/Resources/Textures/Monster/boss/walk/157/boss_walk_157_%d.PNG", 12); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Walk_180, "../bin/Resources/Textures/Monster/boss/walk/180/boss_walk_180_%d.PNG", 12); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;

	ADD_TEXTURE(Boss_Jump_0, "../bin/Resources/Textures/Monster/boss/jump/0/boss_jump_0_%d.PNG", 5); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Jump_22, "../bin/Resources/Textures/Monster/boss/jump/22/boss_jump_22_%d.PNG", 5); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Jump_45, "../bin/Resources/Textures/Monster/boss/jump/45/boss_jump_45_%d.PNG", 5); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Jump_67, "../bin/Resources/Textures/Monster/boss/jump/67/boss_jump_67_%d.PNG", 5); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Jump_90, "../bin/Resources/Textures/Monster/boss/jump/90/boss_jump_90_%d.PNG", 5); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Jump_112, "../bin/Resources/Textures/Monster/boss/jump/112/boss_jump_112_%d.PNG", 5); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Jump_135, "../bin/Resources/Textures/Monster/boss/jump/135/boss_jump_135_%d.PNG", 5); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Jump_157, "../bin/Resources/Textures/Monster/boss/jump/157/boss_jump_157_%d.PNG", 5); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Jump_180, "../bin/Resources/Textures/Monster/boss/jump/180/boss_jump_180_%d.PNG", 5); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;

	ADD_TEXTURE(Boss_Fly_0, "../bin/Resources/Textures/Monster/boss/fly/0/%d.PNG", 4); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Fly_22, "../bin/Resources/Textures/Monster/boss/fly/22/%d.PNG", 4); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Fly_45, "../bin/Resources/Textures/Monster/boss/fly/45/%d.PNG", 4); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Fly_67, "../bin/Resources/Textures/Monster/boss/fly/67/%d.PNG", 4); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Fly_90, "../bin/Resources/Textures/Monster/boss/fly/90/%d.PNG", 4); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Fly_112, "../bin/Resources/Textures/Monster/boss/fly/112/%d.PNG", 4); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Fly_135, "../bin/Resources/Textures/Monster/boss/fly/135/%d.PNG", 4); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Fly_157, "../bin/Resources/Textures/Monster/boss/fly/157/%d.PNG", 4); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Fly_180, "../bin/Resources/Textures/Monster/boss/fly/180/%d.PNG", 4); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;

	ADD_TEXTURE(Boss_Fly_Attack_0, "../bin/Resources/Textures/Monster/boss/fly_attack/0/%d.PNG", 8); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Fly_Attack_22, "../bin/Resources/Textures/Monster/boss/fly_attack/22/%d.PNG", 8); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Fly_Attack_45, "../bin/Resources/Textures/Monster/boss/fly_attack/45/%d.PNG", 8); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Fly_Attack_67, "../bin/Resources/Textures/Monster/boss/fly_attack/67/%d.PNG", 8); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Fly_Attack_90, "../bin/Resources/Textures/Monster/boss/fly_attack/90/%d.PNG", 8); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Fly_Attack_112, "../bin/Resources/Textures/Monster/boss/fly_attack/112/%d.PNG", 8); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Fly_Attack_135, "../bin/Resources/Textures/Monster/boss/fly_attack/135/%d.PNG", 8); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Fly_Attack_157, "../bin/Resources/Textures/Monster/boss/fly_attack/157/%d.PNG", 8); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Fly_Attack_180, "../bin/Resources/Textures/Monster/boss/fly_attack/180/%d.PNG", 8); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;

	ADD_TEXTURE(Boss_Bomb_0, "../bin/Resources/Textures/Monster/boss/bomb/0/%d.PNG", 2);
	ADD_TEXTURE(Boss_Bomb_22, "../bin/Resources/Textures/Monster/boss/bomb/22/%d.PNG", 2);
	ADD_TEXTURE(Boss_Bomb_45, "../bin/Resources/Textures/Monster/boss/bomb/45/%d.PNG", 2);
	ADD_TEXTURE(Boss_Bomb_67, "../bin/Resources/Textures/Monster/boss/bomb/67/%d.PNG", 2);
	ADD_TEXTURE(Boss_Bomb_90, "../bin/Resources/Textures/Monster/boss/bomb/90/%d.PNG", 2);
	ADD_TEXTURE(Boss_Bomb_112, "../bin/Resources/Textures/Monster/boss/bomb/112/%d.PNG", 2);
	ADD_TEXTURE(Boss_Bomb_135, "../bin/Resources/Textures/Monster/boss/bomb/135/%d.PNG", 2);
	ADD_TEXTURE(Boss_Bomb_157, "../bin/Resources/Textures/Monster/boss/bomb/157/%d.PNG", 2);
	ADD_TEXTURE(Boss_Bomb_180, "../bin/Resources/Textures/Monster/boss/bomb/180/%d.PNG", 2);

	ADD_TEXTURE(Boss_Missile_0, "../bin/Resources/Textures/Monster/boss/missile/0/%d.PNG", 2);
	ADD_TEXTURE(Boss_Missile_22, "../bin/Resources/Textures/Monster/boss/missile/22/%d.PNG", 2);
	ADD_TEXTURE(Boss_Missile_45, "../bin/Resources/Textures/Monster/boss/missile/45/%d.PNG", 2);
	ADD_TEXTURE(Boss_Missile_67, "../bin/Resources/Textures/Monster/boss/missile/67/%d.PNG", 2);
	ADD_TEXTURE(Boss_Missile_90, "../bin/Resources/Textures/Monster/boss/missile/90/%d.PNG", 2);
	ADD_TEXTURE(Boss_Missile_112, "../bin/Resources/Textures/Monster/boss/missile/112/%d.PNG", 2);
	ADD_TEXTURE(Boss_Missile_135, "../bin/Resources/Textures/Monster/boss/missile/135/%d.PNG", 2);
	ADD_TEXTURE(Boss_Missile_157, "../bin/Resources/Textures/Monster/boss/missile/157/%d.PNG", 2);
	ADD_TEXTURE(Boss_Missile_180, "../bin/Resources/Textures/Monster/boss/missile/180/%d.PNG", 2);
#pragma endregion

#pragma region MODEL
	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩중입니다."));
	Load_For_Terrain(TEXT("MapData.txt"));
#pragma endregion

#pragma region SOUND
	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩중입니다."));fDataCurNum++;m_fLoadPercent = fDataCurNum / fDataNum;
#pragma endregion

#pragma region PRTOBJ
	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩중입니다."));fDataCurNum++;m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(Terrain);fDataCurNum++;m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(MyCube);fDataCurNum++;m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(Aim);fDataCurNum++;m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(Armor);fDataCurNum++;m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(Ammo);fDataCurNum++;m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(Portrait);fDataCurNum++;m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(Trapezoid);fDataCurNum++;m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(Stall);fDataCurNum++;m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(Cabinet);fDataCurNum++;m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(Signboard);fDataCurNum++;m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(MyComputer);fDataCurNum++;m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(Canopy);fDataCurNum++;m_fLoadPercent = fDataCurNum / fDataNum;

	ADD_PRTOBJ(TriangularPillar);
	ADD_PRTOBJ(Block);fDataCurNum++;m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(BackGround);fDataCurNum++;m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(AnimeRect);fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(AnimeBlock);fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(InvisibleBlock);fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(Lava); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(AlphaRect); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(AlphaBlock); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(BossBridge);

	//몬스터
	ADD_PRTOBJ(Ttakkeun_i);fDataCurNum++;m_fLoadPercent = fDataCurNum / fDataNum;

#pragma endregion

#pragma region DATA
	lstrcpy(m_szLoadingText, TEXT("데이터를 읽어들이는 중입니다."));
	Add_Data(TEXT("GamePlayLevelData.csv"));

#pragma endregion

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));
	m_isFinished = true;
	m_fLoadPercent = 1.f;
	return S_OK;
}

HRESULT CLoader::Loading_For_RaceFirst()/**/
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));
	ADD_TEXTURE(PlayerOnBoat, "../Bin/Resources/Textures/Player/PlayerOnBoat/Tile15947.PNG", 1);
	ADD_TEXTURE(RaceTerrain, "../Bin/Resources/Textures/Map/RaceTerrain/RaceTerrain%d.PNG", 2);
	ADD_TEXTURE(RaceLandscape, "../Bin/Resources/Textures/Object/RaceLandscape/Tile15415.PNG", 1);
	ADD_TEXTURE(BuildingH, "../Bin/Resources/Textures/Object/BuildingH/BuildingH%d.PNG", 4);
	ADD_TEXTURE(BuildingW, "../Bin/Resources/Textures/Object/BuildingW/BuildingW.PNG", 1);
	ADD_TEXTURE(BuildingV, "../Bin/Resources/Textures/Object/BuildingV/BuildingV.PNG", 1);
	ADD_TEXTURE(BuildingU, "../Bin/Resources/Textures/Object/BuildingU/BuildingU.PNG", 1);
	ADD_TEXTURE(RaceGate, "../Bin/Resources/Textures/Object/RaceGate/RaceGate%d.PNG", 3);
	ADD_TEXTURE(RaceCylinder, "../Bin/Resources/Textures/Object/RaceCylinder/RaceCylinder%d.PNG", 3);

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_RACEFIRST, TEXT("Prototype_Component_Texture_StreetLampHead"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Object/StreetLamp/StreetLampHead.PNG"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_RACEFIRST, TEXT("Prototype_Component_Texture_StreetLampPillar"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Object/StreetLamp/StreetLampPillar.PNG"), 1))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩중입니다."));
	ADD_MODEL_EX(RaceTerrain, 10, 1000);
	ADD_MODEL(Rect);

	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩중입니다."));
	ADD_PRTOBJ(RaceTerrain);
	ADD_PRTOBJ(RaceGate);
	ADD_PRTOBJ(RaceLandscape);
	ADD_PRTOBJ(BuildingH);
	ADD_PRTOBJ(BuildingU);
	ADD_PRTOBJ(BuildingV);
	ADD_PRTOBJ(BuildingW);
	ADD_PRTOBJ(RaceCylinder);
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_StreetLampHead"),
		CStreetLampHead::Create(m_pGraphic_Device))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("데이터를 읽어들이는 중입니다."));
	Add_Data(TEXT("GamePlayLevelData.csv"));

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_RaceSecond()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));
	ADD_TEXTURE(RaceTerrain, "../Bin/Resources/Textures/Map/RaceTerrain/RaceTerrain%d.PNG", 2);
	ADD_TEXTURE(RaceLandscape, "../Bin/Resources/Textures/Object/RaceLandscape/Tile15415.PNG", 1);
	ADD_TEXTURE(RaceGate, "../Bin/Resources/Textures/Object/RaceGate/RaceGate%d.PNG", 3);
	ADD_TEXTURE(RaceCylinder, "../Bin/Resources/Textures/Object/RaceCylinder/RaceCylinder%d.PNG", 3);
	ADD_TEXTURE(Forest, "../Bin/Resources/Textures/Object/Forest/Forest.PNG", 1);

	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩중입니다."));
	ADD_MODEL_EX(RaceTerrain, 10, 1000);

	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩중입니다."));
	ADD_PRTOBJ(RaceTerrain);
	ADD_PRTOBJ(RaceGate);
	ADD_PRTOBJ(RaceLandscape);
	ADD_PRTOBJ(RaceCylinder);
	ADD_PRTOBJ(Forest);
	
	lstrcpy(m_szLoadingText, TEXT("데이터를 읽어들이는 중입니다."));
	Add_Data(TEXT("GamePlayLevelData.csv"));


	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_RaceThird()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));
	ADD_TEXTURE(RaceTerrain, "../Bin/Resources/Textures/Map/RaceTerrain/RaceTerrain%d.PNG", 2);
	ADD_TEXTURE(RaceGate, "../Bin/Resources/Textures/Object/RaceGate/RaceGate%d.PNG", 3);
	ADD_TEXTURE(RaceCylinder, "../Bin/Resources/Textures/Object/RaceCylinder/RaceCylinder%d.PNG", 3);
	ADD_TEXTURE(Rock, "../Bin/Resources/Textures/Object/Rock/tile6374.PNG", 1);
	ADD_TEXTURE(Forest, "../Bin/Resources/Textures/Object/Forest/Forest1.PNG", 1);

	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩중입니다."));
	ADD_MODEL_EX(RaceTerrain, 10, 1000);
	
	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩중입니다."));
	ADD_PRTOBJ(RaceTerrain);
	ADD_PRTOBJ(RaceGate);
	ADD_PRTOBJ(RaceCylinder);
	ADD_PRTOBJ(Rock);
	ADD_PRTOBJ(Forest);

	lstrcpy(m_szLoadingText, TEXT("데이터를 읽어들이는 중입니다."));
	Add_Data(TEXT("GamePlayLevelData.csv"));

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Boss()
{
	float fDataNum = { 188.f };
	float fDataCurNum = {};

#pragma region TEXTURE
	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));

	/* 터레인 텍스쳐 잠깐 바꾸겠습니다 */
	ADD_TEXTURE(Terrain, "../Bin/Resources/Textures/Check_Tile.PNG", 1); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Lava, "../Bin/Resources/Textures/Map/Lava/Tile%d.PNG", 16); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	// ADD_TEXTURE(Terrain, "../Bin/Resources/Textures/Map/tile6191.PNG", 1);fDataCurNum++;m_fLoadPercent = fDataCurNum / fDataNum;

	ADD_TEXTURE(MyCube, "../Bin/Resources/Textures/Snow/Snow.png", 1); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Aim, "../Bin/Resources/Textures/Aim/aim0.PNG", 1); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Armor, "../Bin/Resources/Textures/UI/Armor/armor%d.PNG", 3); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Ammo, "../Bin/Resources/Textures/UI/Ammo/ammo%d.PNG", 8); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Portrait, "../Bin/Resources/Textures/UI/Portrait/portrait%d.PNG", 25); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;

	ADD_TEXTURE(Box, "../Bin/Resources/Textures/Object/Box/tile6628.png", 1); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Cabinet, "../Bin/Resources/Textures/Object/Cabinet/Cabinet%d.png", 3); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Trapezoid, "../Bin/Resources/Textures/Object/Trapezoid/Trapezoid%d.png", 2); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Stall, "../Bin/Resources/Textures/Object/Stall/Stall%d.png", 2); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Signboard, "../Bin/Resources/Textures/Object/Signboard/Signboard%d.png", 5); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(MyComputer, "../Bin/Resources/Textures/Object/Computer/Computer%d.png", 3); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Canopy, "../Bin/Resources/Textures/Object/Canopy/Canopy%d.png", 2); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;

	ADD_TEXTURE(TestBullet, "../Bin/Resources/Textures/Bullet/Test/tile7859.png", 1); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Weapon_LoverBoy, "../Bin/Resources/Textures/Weapon/LoverBoy/LoverBoy%d.PNG", 15); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(LeftHand, "../Bin/Resources/Textures/Weapon/LeftHand/LeftHand%d.PNG", 2); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Weapon_Chaingun, "../Bin/Resources/Textures/Weapon/ChainGun/ChainGun%d.PNG", 16); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Weapon_Dispenser, "../Bin/Resources/Textures/Weapon/Dispenser/Dispenser%d.PNG", 60);
	ADD_TEXTURE(MonsterBounce, "../Bin/Resources/Textures/Bullet/MonsterBounce/MonsterBounce%d.PNG", 4);

	ADD_TEXTURE(Test, "../Bin/Resources/Textures/TileTest/tile%d.PNG", NUMMAPTEX); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	//ADD_TEXTURE(Test, "../Bin/Resources/Textures/TileTest/tile%d.PNG", 121);
	for (size_t i = 0; i < 100000; i++) { for (size_t j = 0; j < 10000; j++)int a = 0; if (KEY_PRESSING(DIK_SPACE))break; }

	/*******************************************************************************************************************************************/
	/********************************************************     여기 접기 !!!!     ************************************************************/
	/*******************************************************************************************************************************************/
#pragma region MONSTER_LOADING 

#pragma region BOSS_TEXTURES
	ADD_TEXTURE(Boss_Walk_0, "../bin/Resources/Textures/Monster/boss/walk/0/boss_walk_0_%d.PNG", 12); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Walk_22, "../bin/Resources/Textures/Monster/boss/walk/22/boss_walk_22_%d.PNG", 12); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Walk_45, "../bin/Resources/Textures/Monster/boss/walk/45/boss_walk_45_%d.PNG", 12); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Walk_67, "../bin/Resources/Textures/Monster/boss/walk/67/boss_walk_67_%d.PNG", 12); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Walk_90, "../bin/Resources/Textures/Monster/boss/walk/90/boss_walk_90_%d.PNG", 12); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Walk_112, "../bin/Resources/Textures/Monster/boss/walk/112/boss_walk_112_%d.PNG", 12); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Walk_135, "../bin/Resources/Textures/Monster/boss/walk/135/boss_walk_135_%d.PNG", 12); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Walk_157, "../bin/Resources/Textures/Monster/boss/walk/157/boss_walk_157_%d.PNG", 12); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Walk_180, "../bin/Resources/Textures/Monster/boss/walk/180/boss_walk_180_%d.PNG", 12); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;

	ADD_TEXTURE(Boss_Jump_0, "../bin/Resources/Textures/Monster/boss/jump/0/boss_jump_0_%d.PNG", 5); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Jump_22, "../bin/Resources/Textures/Monster/boss/jump/22/boss_jump_22_%d.PNG", 5); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Jump_45, "../bin/Resources/Textures/Monster/boss/jump/45/boss_jump_45_%d.PNG", 5); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Jump_67, "../bin/Resources/Textures/Monster/boss/jump/67/boss_jump_67_%d.PNG", 5); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Jump_90, "../bin/Resources/Textures/Monster/boss/jump/90/boss_jump_90_%d.PNG", 5); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Jump_112, "../bin/Resources/Textures/Monster/boss/jump/112/boss_jump_112_%d.PNG", 5); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Jump_135, "../bin/Resources/Textures/Monster/boss/jump/135/boss_jump_135_%d.PNG", 5); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Jump_157, "../bin/Resources/Textures/Monster/boss/jump/157/boss_jump_157_%d.PNG", 5); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Jump_180, "../bin/Resources/Textures/Monster/boss/jump/180/boss_jump_180_%d.PNG", 5); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;

	ADD_TEXTURE(Boss_Fly_0, "../bin/Resources/Textures/Monster/boss/fly/0/%d.PNG", 4); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Fly_22, "../bin/Resources/Textures/Monster/boss/fly/22/%d.PNG", 4); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Fly_45, "../bin/Resources/Textures/Monster/boss/fly/45/%d.PNG", 4); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Fly_67, "../bin/Resources/Textures/Monster/boss/fly/67/%d.PNG", 4); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Fly_90, "../bin/Resources/Textures/Monster/boss/fly/90/%d.PNG", 4); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Fly_112, "../bin/Resources/Textures/Monster/boss/fly/112/%d.PNG", 4); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Fly_135, "../bin/Resources/Textures/Monster/boss/fly/135/%d.PNG", 4); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Fly_157, "../bin/Resources/Textures/Monster/boss/fly/157/%d.PNG", 4); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Fly_180, "../bin/Resources/Textures/Monster/boss/fly/180/%d.PNG", 4); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;

	ADD_TEXTURE(Boss_Fly_Attack_0, "../bin/Resources/Textures/Monster/boss/fly_attack/0/%d.PNG", 8); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Fly_Attack_22, "../bin/Resources/Textures/Monster/boss/fly_attack/22/%d.PNG", 8); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Fly_Attack_45, "../bin/Resources/Textures/Monster/boss/fly_attack/45/%d.PNG", 8); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Fly_Attack_67, "../bin/Resources/Textures/Monster/boss/fly_attack/67/%d.PNG", 8); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Fly_Attack_90, "../bin/Resources/Textures/Monster/boss/fly_attack/90/%d.PNG", 8); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Fly_Attack_112, "../bin/Resources/Textures/Monster/boss/fly_attack/112/%d.PNG", 8); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Fly_Attack_135, "../bin/Resources/Textures/Monster/boss/fly_attack/135/%d.PNG", 8); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Fly_Attack_157, "../bin/Resources/Textures/Monster/boss/fly_attack/157/%d.PNG", 8); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_TEXTURE(Boss_Fly_Attack_180, "../bin/Resources/Textures/Monster/boss/fly_attack/180/%d.PNG", 8); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;

	ADD_TEXTURE(Boss_Bomb_0, "../bin/Resources/Textures/Monster/boss/bomb/0/%d.PNG", 2);
	ADD_TEXTURE(Boss_Bomb_22, "../bin/Resources/Textures/Monster/boss/bomb/22/%d.PNG", 2);
	ADD_TEXTURE(Boss_Bomb_45, "../bin/Resources/Textures/Monster/boss/bomb/45/%d.PNG", 2);
	ADD_TEXTURE(Boss_Bomb_67, "../bin/Resources/Textures/Monster/boss/bomb/67/%d.PNG", 2);
	ADD_TEXTURE(Boss_Bomb_90, "../bin/Resources/Textures/Monster/boss/bomb/90/%d.PNG", 2);
	ADD_TEXTURE(Boss_Bomb_112, "../bin/Resources/Textures/Monster/boss/bomb/112/%d.PNG", 2);
	ADD_TEXTURE(Boss_Bomb_135, "../bin/Resources/Textures/Monster/boss/bomb/135/%d.PNG", 2);
	ADD_TEXTURE(Boss_Bomb_157, "../bin/Resources/Textures/Monster/boss/bomb/157/%d.PNG", 2);
	ADD_TEXTURE(Boss_Bomb_180, "../bin/Resources/Textures/Monster/boss/bomb/180/%d.PNG", 2);

	ADD_TEXTURE(Boss_Missile_0, "../bin/Resources/Textures/Monster/boss/missile/0/%d.PNG", 2);
	ADD_TEXTURE(Boss_Missile_22, "../bin/Resources/Textures/Monster/boss/missile/22/%d.PNG", 2);
	ADD_TEXTURE(Boss_Missile_45, "../bin/Resources/Textures/Monster/boss/missile/45/%d.PNG", 2);
	ADD_TEXTURE(Boss_Missile_67, "../bin/Resources/Textures/Monster/boss/missile/67/%d.PNG", 2);
	ADD_TEXTURE(Boss_Missile_90, "../bin/Resources/Textures/Monster/boss/missile/90/%d.PNG", 2);
	ADD_TEXTURE(Boss_Missile_112, "../bin/Resources/Textures/Monster/boss/missile/112/%d.PNG", 2);
	ADD_TEXTURE(Boss_Missile_135, "../bin/Resources/Textures/Monster/boss/missile/135/%d.PNG", 2);
	ADD_TEXTURE(Boss_Missile_157, "../bin/Resources/Textures/Monster/boss/missile/157/%d.PNG", 2);
	ADD_TEXTURE(Boss_Missile_180, "../bin/Resources/Textures/Monster/boss/missile/180/%d.PNG", 2);

#pragma endregion

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));
	m_isFinished = true;

#pragma endregion

#pragma region MODEL
	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩중입니다."));
	Load_For_Terrain(TEXT("MapData.txt"));
#pragma endregion

#pragma region SOUND
	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩중입니다.")); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
#pragma endregion

#pragma region PRTOBJ
	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩중입니다.")); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(Terrain); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(MyCube); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(Aim); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(Armor); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(Ammo); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(Portrait); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(Trapezoid); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(Stall); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(Cabinet); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(Signboard); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(MyComputer); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(Canopy); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(Player); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(TestBullet); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(Weapon_LoverBoy); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(Weapon_Chaingun); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(Weapon_Dispenser);
	ADD_PRTOBJ(Block); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(BackGround); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(AnimeRect); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(AnimeBlock); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(InvisibleBlock); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(Lava); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(AlphaRect); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(AlphaBlock); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(BossBridge);

	//for (size_t i = 0; i < 100000; i++) { for (size_t j = 0; j < 10000; j++)int a = 0; if (KEY_PRESSING(DIK_SPACE))break; }
	//몬스터
	ADD_PRTOBJ(Ttakkeun_i); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(Wenteko); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(Shotgunner); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(Nukemutant); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(Mechsect); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(Greater); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(Deacon); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(Cultist); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;
	ADD_PRTOBJ(Archangel); fDataCurNum++; m_fLoadPercent = fDataCurNum / fDataNum;

	ADD_PRTOBJ(TriangularPillar);
#pragma endregion

#pragma region DATA
	lstrcpy(m_szLoadingText, TEXT("데이터를 읽어들이는 중입니다."));
	Add_Data(TEXT("GamePlayLevelData.csv"));

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));
	m_isFinished = true;
	m_fLoadPercent = 1.f;

	return S_OK;
}

HRESULT CLoader::Loading_For_Indoor()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));

	ADD_TEXTURE(Test, "../Bin/Resources/Textures/TileTest/tile%d.PNG", NUMMAPTEX);
	ADD_TEXTURE(BackGround, "../Bin/Resources/Textures/Default%d.jpg", 2);
	ADD_TEXTURE(Terrain, "../Bin/Resources/Textures/Check_Tile.PNG", 1);

	ADD_TEXTURE(MyCube, "../Bin/Resources/Textures/Snow/Snow.png", 1);
	ADD_TEXTURE(Portrait, "../Bin/Resources/Textures/UI/Portrait/portrait%d.PNG", 25); 
	ADD_TEXTURE(Box, "../Bin/Resources/Textures/Object/Box/tile6628.png", 1);				 
	ADD_TEXTURE(Cabinet, "../Bin/Resources/Textures/Object/Cabinet/Cabinet%d.png", 3);	     
	ADD_TEXTURE(Trapezoid, "../Bin/Resources/Textures/Object/Trapezoid/Trapezoid%d.png", 2); 
	ADD_TEXTURE(Stall, "../Bin/Resources/Textures/Object/Stall/Stall%d.png", 2);			 
	ADD_TEXTURE(Signboard, "../Bin/Resources/Textures/Object/Signboard/Signboard%d.png", 5); 
	ADD_TEXTURE(MyComputer, "../Bin/Resources/Textures/Object/Computer/Computer%d.png", 3);  
	ADD_TEXTURE(Canopy, "../Bin/Resources/Textures/Object/Canopy/Canopy%d.png", 2);

	ADD_TEXTURE(BuildingH, "../Bin/Resources/Textures/Object/BuildingH/BuildingH%d.PNG", 4);
	ADD_TEXTURE(BuildingW, "../Bin/Resources/Textures/Object/BuildingW/BuildingW.PNG", 1);
	ADD_TEXTURE(BuildingV, "../Bin/Resources/Textures/Object/BuildingV/BuildingV.PNG", 1);
	ADD_TEXTURE(BuildingU, "../Bin/Resources/Textures/Object/BuildingU/BuildingU.PNG", 1);
	ADD_TEXTURE(RaceGate, "../Bin/Resources/Textures/Object/RaceGate/RaceGate%d.PNG", 3);

	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩중입니다."));
	Load_For_Terrain(TEXT("NormalMapData.txt"));

	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩중입니다."));
	ADD_PRTOBJ(Terrain);

	ADD_PRTOBJ(Portrait);
	ADD_PRTOBJ(Trapezoid);
	ADD_PRTOBJ(Stall);
	ADD_PRTOBJ(Cabinet);
	ADD_PRTOBJ(Signboard);
	ADD_PRTOBJ(MyComputer);
	ADD_PRTOBJ(Canopy);

	ADD_PRTOBJ(Block);
	ADD_PRTOBJ(BackGround);
	ADD_PRTOBJ(AnimeRect);
	ADD_PRTOBJ(AnimeBlock);
	ADD_PRTOBJ(AlphaRect);
	ADD_PRTOBJ(AlphaBlock);
	ADD_PRTOBJ(InvisibleBlock);
	ADD_PRTOBJ(TriangularPillar);

	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("데이터를 읽어들이는 중입니다."));

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

HRESULT CLoader::Load_For_Terrain(const _wstring& strFileTag)
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

	bResult = ReadFile(hFile, &iNumVertexX, sizeof(_int), &dwByte, NULL);
	bResult = ReadFile(hFile, &iNumVertexZ, sizeof(_int), &dwByte, NULL);

	ADD_MODEL_EX(Terrain, iNumVertexX, iNumVertexZ);

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
