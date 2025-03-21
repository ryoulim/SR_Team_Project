#include "Level_GamePlay.h"
#include "Explosion.h"
#include "Firework.h"
#include "GameInstance.h"
#include "EmptyBullet.h"
#include "Sprite.h"
#include "Smoke.h"
#include "Monster.h"

#include "Map.h"

#define CurLevel LEVEL_GAMEPLAY

CLevel_GamePlay::CLevel_GamePlay(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel { pGraphic_Device }
	, m_Gen(m_Rd())
{
}

HRESULT CLevel_GamePlay::Initialize(class CLevelData* pLevelData)
{
	if (FAILED(__super::Initialize(pLevelData)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Terrain(TEXT("Layer_Terrain"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Pawn(TEXT("Layer_Pawn"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Statue(TEXT("Layer_Statue"))))
		return E_FAIL;

	/*if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Particle(TEXT("Layer_Particle"))))
		return E_FAIL;*/

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;

	//if (FAILED(Ready_Layer_Item(TEXT("Layer_Item"))))
	//	return E_FAIL;

	//if(FAILED(Ready_Light()))
	//	return E_FAIL;

	ShowCursor(FALSE);
	if (FAILED(Load_Map(LEVEL_GAMEPLAY, TEXT("MapData.txt"))))
		return E_FAIL;

	return S_OK;
}

void CLevel_GamePlay::Update(_float fTimeDelta)
{
	SpawnMultipleExplosions(fTimeDelta);
	SpawnMultipleExplosions2(fTimeDelta);
	SpawnMultipleExplosions3(fTimeDelta);

	////탄피 생성
	//if (KEY_PRESSING(VK_LBUTTON))
	//{
	//	SpawnEmptyBullet(_float3(0.f, 0.f, 0.f));
	//}

	////카메라 스프라이트 테스트
	//if (KEY_PRESSING(VK_LBUTTON))
	//{
	//	for (auto SceenEffect : m_vecSceenEffect)
	//	{
	//		SceenEffect->isActive(true);
	//	}
	//}
	//else
	//{
	//	for (auto SceenEffect : m_vecSceenEffect)
	//	{
	//		SceenEffect->isActive(false);
	//	}
	//}

	m_pGameInstance->Intersect(LEVEL_GAMEPLAY, TEXT("Layer_Pawn"), TEXT("Layer_Statue"));
	m_pGameInstance->Intersect(LEVEL_GAMEPLAY, TEXT("Layer_Pawn"), TEXT("Layer_Monster"));
	m_pGameInstance->Intersect(LEVEL_GAMEPLAY, TEXT("Layer_Pawn"), TEXT("Layer_Block"));
	m_pGameInstance->Intersect(LEVEL_GAMEPLAY, TEXT("Layer_PBullet"), TEXT("Layer_Monster"));
	//m_pGameInstance->Intersect(LEVEL_GAMEPLAY, TEXT("Layer_PBullet"), TEXT("Layer_Statue"));
}

HRESULT CLevel_GamePlay::Render()
{
	SetWindowText(g_hWnd, TEXT("게임플레이 레벨입니다."));

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Terrain(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Terrain"),
		LEVEL_GAMEPLAY, strLayerTag)))
		return E_FAIL;

	auto pTerrain = m_pGameInstance-> 
		Find_Object(LEVEL_GAMEPLAY, strLayerTag);
	auto pTerrainBuffer = static_cast<CVIBuffer_Terrain*>(pTerrain->
		Find_Component(TEXT("Com_VIBuffer")));
	auto pTerrainTransform = static_cast<CTransform*>(pTerrain->
		Find_Component(TEXT("Com_Transform")));

	CGravity::DESC GravityDesc{};
	GravityDesc.vTerrainScale = pTerrainTransform->Compute_Scaled();
	GravityDesc.pTerrainVtxPos = pTerrainBuffer->Get_VertexPos();
	GravityDesc.iTerrainVtxNumX = 129;
  	GravityDesc.iTerrainVtxNumZ = 129;
	CGravity::Set_TerrainInfo(GravityDesc);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_CameraManager"),
		LEVEL_GAMEPLAY, strLayerTag, m_pData->Find_Data(TEXT("Dynamic_Camera")))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Camera"),
		LEVEL_GAMEPLAY, strLayerTag, m_pData->Find_Data(TEXT("UI_Camera")))))
		return E_FAIL;

	return S_OK;
}

#include "MyCube.h"

HRESULT CLevel_GamePlay::Ready_Layer_Statue(const _wstring& strLayerTag)
{
	/*CMyCube::DESC CubeDesc{};

	CubeDesc.vInitPos = { 150.f,50.f,150.f };
	CubeDesc.vScale = { 50.f,100.f,50.f };
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_MyCube"),
		LEVEL_GAMEPLAY, strLayerTag, &CubeDesc)))
		return E_FAIL;*/

	/*if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_MyComputer"),
		LEVEL_GAMEPLAY, strLayerTag, m_pData->Find_Data(TEXT("MyComputer")))))
		return E_FAIL;*/

	//if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Signboard"),
	//	LEVEL_GAMEPLAY, strLayerTag, m_pData->Find_Data(TEXT("Signboard")))))
	//	return E_FAIL;

	/*if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Cabinet"),
		LEVEL_GAMEPLAY, strLayerTag, m_pData->Find_Data(TEXT("Cabinet")))))
		return E_FAIL;*/

	/*if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Trapezoid"),

		LEVEL_GAMEPLAY, strLayerTag, m_pData->Find_Data(TEXT("Trapezoid")))))
		return E_FAIL;*/

	/*if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Stall"),
		LEVEL_GAMEPLAY, strLayerTag, m_pData->Find_Data(TEXT("Stall")))))
		return E_FAIL;*/

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Canopy"),
		LEVEL_GAMEPLAY, strLayerTag, m_pData->Find_Data(TEXT("Canopy")))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Item(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Item_Ammo"),
		LEVEL_GAMEPLAY, strLayerTag, m_pData->Find_Data(TEXT("Item_Ammo")))))
		return E_FAIL;

	return E_NOTIMPL;
}

HRESULT CLevel_GamePlay::Ready_Layer_Particle(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Rain"),
		LEVEL_GAMEPLAY, strLayerTag)))
		return E_FAIL;

	SpawnSpher(_float3{ 400.f, 0.f, 200.f });
	
	//토네이도
	for (int i = 0; i < 5; ++i)
	{
		_float3 vPosition = { GetRandomValue(100, 700) , 25.f , GetRandomValue(100, 700) };
		SpawnTornado(vPosition);
	}
	//불지르기
	for (int i = 0; i < 10; ++i)
	{
		_float3 vPosition = { GetRandomValue(100, 700) , 25.f , GetRandomValue(100, 700) };
		SpawnFire(vPosition);
	}

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Effect(const _wstring& strLayerTag)
{
	SpawnGunFire(_float3(5.f,-5.f,20.f));
	SpawnBulletTracer(_float3(5.f, -5.f, 50.f));

	return S_OK;
}

#include "Player.h"

HRESULT CLevel_GamePlay::Ready_Layer_Pawn(const _wstring& strLayerTag)
{
	CPlayer::DESC PlayerDesc{};
	PlayerDesc.vInitPos = { 10.f,20.f,10.f };
	PlayerDesc.vScale = { 5.f, 30.f, 5.f };
	PlayerDesc.fRotationPerSec = RADIAN(180.f);
	PlayerDesc.fSpeedPerSec = 150.f;
	PlayerDesc.fMouseSensor = 0.1f;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Player"),
		LEVEL_GAMEPLAY, strLayerTag, &PlayerDesc)))
		return E_FAIL;

	return S_OK;
}

#include "Ttakkeun_i.h"
#include <UI_Manager.h>
HRESULT CLevel_GamePlay::Ready_Layer_Monster(const _wstring& strLayerTag)
{

	CMonster::DESC MonsterDesc{};
	MonsterDesc.vPosition = { 400.f, 70.f, 200.f };

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Ttakkeun_i"),
<<<<<<< Updated upstream
		LEVEL_GAMEPLAY, strLayerTag, &MonsterDesc)))
		return E_FAIL;
=======
		LEVEL_GAMEPLAY, strLayerTag, &Ttakkeun_iDesc)))
		return E_FAIL;

	////웬테코
	//CMonster::DESC Wenteko_iDesc{};
	//Wenteko_iDesc.vPosition = { 100.f, 40.f, -100.f };
	//if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Wenteko"),
	//	LEVEL_GAMEPLAY, strLayerTag, &Wenteko_iDesc)))
	//	return E_FAIL;

	//샷거너
	CMonster::DESC Shotgunner_iDesc{};
	Shotgunner_iDesc.vPosition = { 180.f, 20.f, -120.f };
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Shotgunner"),
		LEVEL_GAMEPLAY, strLayerTag, &Shotgunner_iDesc)))
		return E_FAIL;

	////누크뮤턴트
	//CMonster::DESC Nukemutant_iDesc{};
	//Nukemutant_iDesc.vPosition = { 250.f, 30.f, -120.f };
	//if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Nukemutant"),
	//	LEVEL_GAMEPLAY, strLayerTag, &Nukemutant_iDesc)))
	//	return E_FAIL;

	////거미쉐끼
	//CMonster::DESC Mechsect_iDesc{};
	//Mechsect_iDesc.vPosition = { 330.f, 10.f, -120.f };
	//if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Mechsect"),
	//	LEVEL_GAMEPLAY, strLayerTag, &Mechsect_iDesc)))
	//	return E_FAIL;

	////그리터
	//CMonster::DESC Greater_iDesc{};
	//Greater_iDesc.vPosition = { 410.f, 20.f, -120.f };
	//if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Greater"),
	//	LEVEL_GAMEPLAY, strLayerTag, &Greater_iDesc)))
	//	return E_FAIL;

	////다콘
	//CMonster::DESC Deacon_iDesc{};
	//Deacon_iDesc.vPosition = { 220.f, 50.f, -250.f };
	//if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Deacon"),
	//	LEVEL_GAMEPLAY, strLayerTag, &Deacon_iDesc)))
	//	return E_FAIL;

	////컬티스트
	//CMonster::DESC Cultist_iDesc{};
	//Cultist_iDesc.vPosition = { 320.f, 30.f, -250.f };
	//if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Cultist"),
	//	LEVEL_GAMEPLAY, strLayerTag, &Cultist_iDesc)))
	//	return E_FAIL;

	////알찬겔
	//CMonster::DESC Archangel_iDesc{};
	//Archangel_iDesc.vPosition = { 420.f, 80.f, -250.f };
	//if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Archangel"),
	//	LEVEL_GAMEPLAY, strLayerTag, &Archangel_iDesc)))
	//	return E_FAIL;
>>>>>>> Stashed changes

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_UI(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Aim"),
		LEVEL_GAMEPLAY, strLayerTag, m_pData->Find_Data(TEXT("Aim")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Portrait"),
		LEVEL_GAMEPLAY, strLayerTag, m_pData->Find_Data(TEXT("Portrait")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Armor"),
		LEVEL_GAMEPLAY, strLayerTag, m_pData->Find_Data(TEXT("Armor")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Ammo"),
		LEVEL_GAMEPLAY, strLayerTag, m_pData->Find_Data(TEXT("Ammo")))))
		return E_FAIL;
	/* ui생성 순서 중요, player 생성 이후 호출 중요  */
	CUI_Manager::Get_Instance(m_pGameInstance)->Initialize_GamePlayUI();
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Light()
{
	//D3DLIGHT9		tLightInfo;
	//ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	//tLightInfo.Type = D3DLIGHT_DIRECTIONAL;

	//tLightInfo.Diffuse = { 1.f, 1.f, 1.f, 1.f };
	//tLightInfo.Specular = { 1.f, 1.f, 1.f, 1.f };
	//tLightInfo.Ambient = { 1.f, 1.f, 1.f, 1.f };
	//tLightInfo.Direction = { 1.f, -1.f, 1.f };
	//tLightInfo.Position = { 50.f,50.f,50.f };

	//++m_iIndex;

	//m_pGraphic_Device->SetLight(m_iIndex, &tLightInfo);

	//m_pGraphic_Device->LightEnable(m_iIndex, TRUE);
	//
	//m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, TRUE);

	//m_pGraphic_Device->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(50, 50, 50));

	return S_OK;
}

void CLevel_GamePlay::SpawnExplosion(_float3 _vPosition)
{
	CSprite::DESC ExplosionDesc{};
	ExplosionDesc.vInitPos = _vPosition;
	ExplosionDesc.vScale = { 120.f,160.f,1.f };
	ExplosionDesc.bLoop = false;
	ExplosionDesc.fMaxFrame = 32;
	ExplosionDesc.fRotationPerSec = RADIAN(180.f);
	ExplosionDesc.fSpeedPerSec = 100.f;
	ExplosionDesc.szTextureTag= TEXT("Effect_Explor");

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Sprite"),
		LEVEL_GAMEPLAY, L"Layer_Effect", &ExplosionDesc)))
		return;

	
	CPSystem::DESC FireworkDesc{};
	FireworkDesc.vPosition = _vPosition;
	FireworkDesc.fMaxFrame = 14;
	FireworkDesc.szTextureTag = TEXT("PC_Small_Fire");
	FireworkDesc.iParticleNums = 5;
	FireworkDesc.fSize = 1.f;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Firework"),
		LEVEL_GAMEPLAY, L"Layer_Particle", &FireworkDesc)))
		return;

	CPSystem::DESC FireworkDesc2{};
	FireworkDesc2.vPosition = _vPosition;
	FireworkDesc2.fMaxFrame = 1;
	FireworkDesc2.szTextureTag = TEXT("PC_Generic");
	FireworkDesc2.iParticleNums = 2;
	FireworkDesc2.fSize = 0.5f;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Firework"),
		LEVEL_GAMEPLAY, L"Layer_Particle", &FireworkDesc)))
		return;

	CSmoke::DESC SmokeDesc{};
	SmokeDesc.vPosition = _vPosition;
	SmokeDesc.vPosition.y += -10.f;
	SmokeDesc.fMaxFrame = 20;
	SmokeDesc.szTextureTag = TEXT("PC_Small_Smoke");
	SmokeDesc.iParticleNums = 5;
	SmokeDesc.fVelocity = 100.f;
	SmokeDesc.vecMinDirection = _float3(-2.f, 0.f, -2.f);
	SmokeDesc.vecMaxDirection = _float3(3.f, 1.5f, 3.f);
	SmokeDesc.fLifeTime = GetRandomValue(0.5, 1.5f);
	SmokeDesc.fSize = 3.f;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Smoke"),
		LEVEL_GAMEPLAY, L"Layer_Particle", &SmokeDesc)))
		return;


}
void CLevel_GamePlay::SpawnExplosion2(_float3 _vPosition)
{
	CSprite::DESC ExplosionDesc{};
	ExplosionDesc.vInitPos = _vPosition;
	ExplosionDesc.vScale = { 120.f,160.f,1.f };
	ExplosionDesc.bLoop = false;
	ExplosionDesc.fMaxFrame = 14;
	ExplosionDesc.fRotationPerSec = RADIAN(180.f);
	ExplosionDesc.fSpeedPerSec = 100.f;
	ExplosionDesc.szTextureTag = TEXT("PC_Explosion");

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Sprite"),
		LEVEL_GAMEPLAY, L"Layer_Effect", &ExplosionDesc)))
		return;


	CPSystem::DESC FireworkDesc{};
	FireworkDesc.vPosition = _vPosition;
	FireworkDesc.fMaxFrame = 14;
	FireworkDesc.szTextureTag = TEXT("PC_Small_Fire");
	FireworkDesc.iParticleNums = 10;
	FireworkDesc.fSize = 1.f;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Firework"),
		LEVEL_GAMEPLAY, L"Layer_Particle", &FireworkDesc)))
		return;

	CPSystem::DESC FireworkDesc2{};
	FireworkDesc2.vPosition = _vPosition;
	FireworkDesc2.fMaxFrame = 1;
	FireworkDesc2.szTextureTag = TEXT("PC_Generic");
	FireworkDesc2.iParticleNums = 5;
	FireworkDesc2.fSize = 0.5f;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Firework"),
		LEVEL_GAMEPLAY, L"Layer_Particle", &FireworkDesc)))
		return;

	CSmoke::DESC SmokeDesc{};
	SmokeDesc.vPosition = _vPosition;
	SmokeDesc.vPosition.y += -10.f;
	SmokeDesc.fMaxFrame = 20;
	SmokeDesc.szTextureTag = TEXT("PC_Small_Smoke");
	SmokeDesc.iParticleNums = 5;
	SmokeDesc.fVelocity = 100.f;
	SmokeDesc.vecMinDirection = _float3(-2.f, 0.f, -2.f);
	SmokeDesc.vecMaxDirection = _float3(3.f, 1.5f, 3.f);
	SmokeDesc.fLifeTime = GetRandomValue(0.5, 1.5f);
	SmokeDesc.fSize = 3.f;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Smoke"),
		LEVEL_GAMEPLAY, L"Layer_Particle", &SmokeDesc)))
		return;


}
void CLevel_GamePlay::SpawnExplosion3(_float3 _vPosition)
{
	CSprite::DESC ExplosionDesc{};
	ExplosionDesc.vInitPos = _vPosition;
	ExplosionDesc.vScale = { 120.f,160.f,1.f };
	ExplosionDesc.bLoop = false;
	ExplosionDesc.fMaxFrame = 24;
	ExplosionDesc.fRotationPerSec = RADIAN(180.f);
	ExplosionDesc.fSpeedPerSec = 100.f;
	ExplosionDesc.szTextureTag = TEXT("Effect_Explorer");

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Sprite"),
		LEVEL_GAMEPLAY, L"Layer_Effect", &ExplosionDesc)))
		return;


	CPSystem::DESC FireworkDesc{};
	FireworkDesc.vPosition = _vPosition;
	FireworkDesc.fMaxFrame = 14;
	FireworkDesc.szTextureTag = TEXT("PC_Small_Fire");
	FireworkDesc.iParticleNums = 10;
	FireworkDesc.fSize = 1.f;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Firework"),
		LEVEL_GAMEPLAY, L"Layer_Particle", &FireworkDesc)))
		return;

	CPSystem::DESC FireworkDesc2{};
	FireworkDesc2.vPosition = _vPosition;
	FireworkDesc2.fMaxFrame = 1;
	FireworkDesc2.szTextureTag = TEXT("PC_Generic");
	FireworkDesc2.iParticleNums = 5;
	FireworkDesc2.fSize = 0.5f;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Firework"),
		LEVEL_GAMEPLAY, L"Layer_Particle", &FireworkDesc)))
		return;

	CSmoke::DESC SmokeDesc{};
	SmokeDesc.vPosition = _vPosition;
	SmokeDesc.vPosition.y += -10.f;
	SmokeDesc.fMaxFrame = 20;
	SmokeDesc.szTextureTag = TEXT("PC_Small_Smoke");
	SmokeDesc.iParticleNums = 5;
	SmokeDesc.fVelocity = 100.f;
	SmokeDesc.vecMinDirection = _float3(-2.f, 0.f, -2.f);
	SmokeDesc.vecMaxDirection = _float3(3.f, 1.5f, 3.f);
	SmokeDesc.fLifeTime = GetRandomValue(0.5, 1.5f);
	SmokeDesc.fSize = 3.f;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Smoke"),
		LEVEL_GAMEPLAY, L"Layer_Particle", &SmokeDesc)))
		return;
}

void CLevel_GamePlay::SpawnFire(_float3 _vPosition)
{
	CSprite::DESC SpriteDesc{};
	SpriteDesc.bLoop = true;
	SpriteDesc.fMaxFrame = 18;
	SpriteDesc.fRotationPerSec = RADIAN(180.f);
	SpriteDesc.fSpeedPerSec = 100.f;
	SpriteDesc.szTextureTag = TEXT("PC_Fire");
	SpriteDesc.vInitPos = _vPosition;
	
	_float fRandom = GetRandomFloat(10.f, 100.f);
	SpriteDesc.vScale = _float3{ fRandom, fRandom , 1.f};

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Sprite"),
		LEVEL_GAMEPLAY, L"Layer_Particle", &SpriteDesc)))
		return;
}
void CLevel_GamePlay::SpawnGunFire(_float3 _ScreenPos)
{
	CCameraSprite::DESC SpriteDesc{};
	SpriteDesc.bActive = false;
	SpriteDesc.fMaxFrame = 3;
	SpriteDesc.fRotationPerSec = RADIAN(180.f);
	SpriteDesc.fSpeedPerSec = 100.f;
	SpriteDesc.szTextureTag = TEXT("Effect_GunFire");
	SpriteDesc.vInitPos = _float3{0.f, 0.f, 0.f};
	SpriteDesc.vScale = _float3{ 10.f, 10.f, 1.f };
	SpriteDesc.fSceenX = _ScreenPos.x;
	SpriteDesc.fSceenY = _ScreenPos.y;
	SpriteDesc.fSceenZ = _ScreenPos.z;

	CGameObject* pObject = nullptr;
	CGameObject** ppOut = &pObject;
	if (FAILED(m_pGameInstance->Add_GameObjectReturn(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_CameraSprite"),
		LEVEL_GAMEPLAY, L"Layer_Particle", ppOut, &SpriteDesc)))
		return;

	m_vecSceenEffect.push_back(dynamic_cast<CCameraSprite*>(*ppOut));
}
void CLevel_GamePlay::SpawnBulletTracer(_float3 _ScreenPos)
{
	CCameraSprite::DESC SpriteDesc{};
	SpriteDesc.bActive = false;
	SpriteDesc.fMaxFrame = 3;
	SpriteDesc.fRotationPerSec = RADIAN(180.f);
	SpriteDesc.fSpeedPerSec = 100.f;
	SpriteDesc.szTextureTag = TEXT("Effect_BulletTacer");
	//SpriteDesc.szTextureTag = TEXT("Check_Tile");
	SpriteDesc.vInitPos = _float3{ 0.f, 0.f, 0.f };
	SpriteDesc.vScale = _float3{ 15.f, 15.f, 1.f };
	SpriteDesc.fSceenX = _ScreenPos.x;
	SpriteDesc.fSceenY = _ScreenPos.y;
	SpriteDesc.fSceenZ = _ScreenPos.z;

	CGameObject* pObject = nullptr;
	CGameObject** ppOut = &pObject;
	if (FAILED(m_pGameInstance->Add_GameObjectReturn(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_CameraSprite"),
		LEVEL_GAMEPLAY, L"Layer_Particle", ppOut, &SpriteDesc)))
		return;

	m_vecSceenEffect.push_back(dynamic_cast<CCameraSprite*>(*ppOut));
}
void CLevel_GamePlay::SpawnTornado(_float3 _vPosition)
{
	CSprite::DESC SpriteDesc{};
	SpriteDesc.bLoop = true;
	SpriteDesc.fMaxFrame = 24;
	SpriteDesc.fRotationPerSec = RADIAN(180.f);
	SpriteDesc.fSpeedPerSec = 100.f;
	SpriteDesc.szTextureTag = TEXT("PC_Tornado");
	SpriteDesc.vInitPos = _vPosition;
	SpriteDesc.vInitPos.y += 20.f;
	
	SpriteDesc.vScale = _float3{ 100.f, 100.f, 1.f };

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Sprite"),
		LEVEL_GAMEPLAY, L"Layer_Particle", &SpriteDesc)))
		return;

	CPSystem::DESC TornadoDesc{};
	TornadoDesc.vPosition = _vPosition;
	TornadoDesc.vPosition.y += -30.f;
	TornadoDesc.fMaxFrame = 5;
	TornadoDesc.szTextureTag = TEXT("PC_Small_Fire");
	TornadoDesc.iParticleNums = 20;
	TornadoDesc.fSize = 0.5f;
	TornadoDesc.fLifeTime = GetRandomFloat(0.5f, 1.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Tornado"),
		LEVEL_GAMEPLAY, L"Layer_Particle", &TornadoDesc)))
		return;


	CPSystem::DESC TornadoDescGeneric{};
	TornadoDescGeneric.vPosition = _vPosition;
	TornadoDescGeneric.vPosition.y += -30.f;
	TornadoDescGeneric.fMaxFrame = 1;
	TornadoDescGeneric.szTextureTag = TEXT("PC_Generic");
	TornadoDescGeneric.iParticleNums = 10;
	TornadoDescGeneric.fSize = 0.5f;
	TornadoDescGeneric.fLifeTime = GetRandomFloat(0.5f, 1.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Tornado"),
		LEVEL_GAMEPLAY, L"Layer_Particle", &TornadoDescGeneric)))
		return;

}
void CLevel_GamePlay::SpawnSpher(_float3 _vPosition)
{
	CPSystem::DESC SphereDesc{};
	SphereDesc.vPosition = _vPosition;
	SphereDesc.vPosition.y += 0.f;
	SphereDesc.fMaxFrame = 1;
	SphereDesc.szTextureTag = TEXT("PC_Generic");
	SphereDesc.iParticleNums = 2500;
	SphereDesc.fSize = 0.15f;
	SphereDesc.fLifeTime = GetRandomFloat(0.5f, 1.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Sphere"),
		LEVEL_GAMEPLAY, L"Layer_Particle", &SphereDesc)))
		return;

	CPSystem::DESC SphereDesc2{};
	SphereDesc2.vPosition = _vPosition;
	SphereDesc2.vPosition.y += 0.f;
	SphereDesc2.fMaxFrame = 1;
	SphereDesc2.szTextureTag = TEXT("PC_Generic");
	SphereDesc2.iParticleNums = 1500;
	SphereDesc2.fSize = 0.3f;
	SphereDesc2.fLifeTime = GetRandomFloat(0.5f, 1.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Sphere"),
		LEVEL_GAMEPLAY, L"Layer_Particle", &SphereDesc2)))
		return;

}

void CLevel_GamePlay::SpawnMultipleExplosions(_float fTimeDelta)
{
	static float fTimer = 0.0f;
	const float fInterval = 0.5f; //반복주기

	fTimer += fTimeDelta;
	if (fTimer >= fInterval)
	{
		_float3 vPosition = { GetRandomValue(200, 800) , GetRandomValue(100, 200) , GetRandomValue(100, 800) };
		SpawnExplosion(vPosition);

		fTimer = 0.0f;
	}
}
void CLevel_GamePlay::SpawnMultipleExplosions2(_float fTimeDelta)
{
	static float fTimer = 0.0f;
	const float fInterval = 0.5f; //반복주기

	fTimer += fTimeDelta;
	if (fTimer >= fInterval)
	{
		_float3 vPosition = { GetRandomValue(200, 800) , GetRandomValue(100, 200) , GetRandomValue(100, 800) };
		SpawnExplosion2(vPosition);

		fTimer = 0.0f;
	}
}
void CLevel_GamePlay::SpawnMultipleExplosions3(_float fTimeDelta)
{
	static float fTimer = 0.0f;
	const float fInterval = 0.5f; //반복주기

	fTimer += fTimeDelta;
	if (fTimer >= fInterval)
	{
		_float3 vPosition = { GetRandomValue(200, 800) , GetRandomValue(100, 200) , GetRandomValue(100, 800) };
		SpawnExplosion3(vPosition);

		fTimer = 0.0f;
	}
}
void CLevel_GamePlay::SpawnEmptyBullet(_float3 _vPosition)
{
	CPSystem::DESC EmptyBulletDesc{};
	EmptyBulletDesc.vPosition = _vPosition;
	EmptyBulletDesc.fMaxFrame = 7;
	EmptyBulletDesc.szTextureTag = TEXT("PC_BulletShell");
	EmptyBulletDesc.iParticleNums = 1;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_EmptyBullet"),
		LEVEL_GAMEPLAY, L"Layer_Particle", &EmptyBulletDesc)))
		return;
}

HRESULT CLevel_GamePlay::Load_Map(_uint iLevelIdx, const _wstring& FileName)
{
	_bool bResult = { true };
	_wstring FilePath;
	FilePath = TEXT("../bin/Resources/MapData/") + FileName;
	_ulong dwByte = {};

	HANDLE hFile = CreateFile(FilePath.c_str(), GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX("파일 개방 실패");
		return E_FAIL;
	}

	_int iNumVertexX = {}, iNumVertexZ = {}, iLoadLength = {};
	_uint iNumBackGround = {}, iNumModel = {};
	_float fSpeedPerSec = {}, fRotationPerSec = {}, fTextureIdx = {};
	_tchar szPrototypeTag[MAX_PATH] = {};;

	_float3 vPosition = {}, vScale = {}, vAngle = {};
	while (true)
	{
		bResult = ReadFile(hFile, &iNumVertexX, sizeof(_int), &dwByte, NULL);
		bResult = ReadFile(hFile, &iNumVertexZ, sizeof(_int), &dwByte, NULL);

		bResult = ReadFile(hFile, &iNumBackGround, sizeof(_uint), &dwByte, NULL);

		if (0 == dwByte)
			break;

		for (_uint i = 0; i < iNumBackGround; i++)
		{
			bResult = ReadFile(hFile, &fSpeedPerSec, sizeof(_float), &dwByte, NULL);
			bResult = ReadFile(hFile, &fRotationPerSec, sizeof(_float), &dwByte, NULL);
			bResult = ReadFile(hFile, &vPosition, sizeof(_float3), &dwByte, NULL);
			bResult = ReadFile(hFile, &vScale, sizeof(_float3), &dwByte, NULL);
			bResult = ReadFile(hFile, &vAngle, sizeof(_float3), &dwByte, NULL);
			bResult = ReadFile(hFile, &fTextureIdx, sizeof(_float), &dwByte, NULL);
			bResult = ReadFile(hFile, &iLoadLength, sizeof(_int), &dwByte, NULL);
			bResult = ReadFile(hFile, &szPrototypeTag, (iLoadLength * sizeof(_tchar)), &dwByte, NULL);

			CMap::DESC tDesc = {};
			tDesc.fSpeedPerSec = fSpeedPerSec;
			tDesc.fRotationPerSec = fRotationPerSec;
			tDesc.vInitPos = vPosition;
			tDesc.vScale = vScale;
			tDesc.vAngle = vAngle;
			tDesc.fTextureIdx = fTextureIdx;

			/* 클라에서 해 줄 작업 */
			//m_pData->Add_Data(strKey, {});
			_wstring strKey = szPrototypeTag;
			strKey = Compute_PrototypeName(strKey);

			_wstring Prototype = TEXT("Prototype_GameObject_") + strKey;

			_wstring Layertag;

			if (strKey == TEXT("BackGround"))
				Layertag = TEXT("Layer_BackGround");
			if (strKey == TEXT("Block"))
				Layertag = TEXT("Layer_Block");

			if (FAILED(m_pGameInstance->Add_GameObject(iLevelIdx, Prototype, iLevelIdx, Layertag, &tDesc)))
			{
				MSG_BOX("객체 생성 실패");
				return E_FAIL;
			}

			ZeroMemory(szPrototypeTag, sizeof(szPrototypeTag));
		}

		bResult = ReadFile(hFile, &iNumModel, sizeof(_uint), &dwByte, NULL);

		for (_uint i = 0; i < iNumModel; i++)
		{
			bResult = ReadFile(hFile, &fSpeedPerSec, sizeof(_float), &dwByte, NULL);
			bResult = ReadFile(hFile, &fRotationPerSec, sizeof(_float), &dwByte, NULL);
			bResult = ReadFile(hFile, &vPosition, sizeof(_float3), &dwByte, NULL);
			bResult = ReadFile(hFile, &vScale, sizeof(_float3), &dwByte, NULL);
			bResult = ReadFile(hFile, &vAngle, sizeof(_float3), &dwByte, NULL);
			bResult = ReadFile(hFile, &iLoadLength, sizeof(_int), &dwByte, NULL);
			bResult = ReadFile(hFile, &szPrototypeTag, (iLoadLength * sizeof(_tchar)), &dwByte, NULL);

			CStatue::DESC tDesc = {};
			tDesc.fSpeedPerSec = fSpeedPerSec;
			tDesc.fRotationPerSec = fRotationPerSec;
			tDesc.vInitPos = vPosition;
			tDesc.vScale = vScale;
			tDesc.vAngle = vAngle;

			/* 클라에서 해 줄 작업 */
			_wstring strKey = szPrototypeTag;
			strKey = Compute_PrototypeName(strKey);

			_wstring Prototype = TEXT("Prototype_GameObject_") + strKey;

			_wstring Layertag;

			if (strKey == TEXT("Cabinet") || strKey == TEXT("Trapezoid"))
				Layertag = TEXT("Layer_Staute");



			if (FAILED(m_pGameInstance->Add_GameObject(iLevelIdx, Prototype, iLevelIdx, Layertag, &tDesc)))
			{
				MSG_BOX("객체 생성 실패");
				return E_FAIL;
			}

			ZeroMemory(szPrototypeTag, sizeof(szPrototypeTag));
		}
	}

	CloseHandle(hFile);
	MSG_BOX("파일 로드 성공");

	return S_OK;
}

CLevel_GamePlay* CLevel_GamePlay::Create(LPDIRECT3DDEVICE9 pGraphic_Device, class CLevelData* pLevelData)
{
	CLevel_GamePlay* pInstance = new CLevel_GamePlay(pGraphic_Device);

  	if (FAILED(pInstance->Initialize(pLevelData)))
	{
		MSG_BOX("Failed to Created : CLevel_GamePlay");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLevel_GamePlay::Free()
{
	__super::Free();

	//CUI_Manager::Get_Instance(m_pGameInstance)->Clear_GamePlayUI();

	while (m_iIndex >= 0)
	{
		m_pGraphic_Device->LightEnable(m_iIndex, FALSE);
		--m_iIndex;
	}
}