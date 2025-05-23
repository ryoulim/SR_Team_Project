#include "FXMgr.h"
#include "ObjectPool.h"

#include "Particle_Define.h"
#include "CameraSprite.h"
#include "Sprite.h"
#include "PSystem.h"
#include "Smoke.h"
#include "CameraSprite.h"
#include "Explosion.h"
#include "Firework.h"
#include "EmptyBullet.h"
#include "BulletMark.h"
#include "ScreenSprite.h"
#include "RaceSprite.h"
#include "Rain.h"
#include "CameraManager.h"


namespace Client
{
	bool g_FogTrigger = true;
	float g_FogCustom = 2000.f;
}


CFXMgr::CFXMgr()
	: m_pGameInstance(CGameInstance::Get_Instance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CFXMgr::Initialize()
{
	CObjectPool* pPool{};

#pragma region BLOOD
	CPSystem::DESC BloodDesc{};
	BloodDesc.fMaxFrame = 5;
	BloodDesc.szTextureTag = TEXT("PS_Blood");
	BloodDesc.fSize = 3.f;
	pPool = CObjectPool::Create(40, LEVEL_STATIC,
		TEXT("Prototype_GameObject_PC_Blood"), &BloodDesc);
	if (!pPool)
		return E_FAIL;
	m_ObjectPools.emplace(TEXT("PS_Blood"), pPool);
#pragma endregion

#pragma region BULLETIMPACTSPARK
	CPSystem::DESC BulletImpactSparkDesc{};
	BulletImpactSparkDesc.vPosition.y += -20.f;
	BulletImpactSparkDesc.fMaxFrame = 1;
	BulletImpactSparkDesc.szTextureTag = TEXT("PC_Generic");
	BulletImpactSparkDesc.fSize = 0.3f;
	BulletImpactSparkDesc.iParticleNums = 2;
	BulletImpactSparkDesc.fNum = 100.f;
	pPool = CObjectPool::Create(20, LEVEL_STATIC,
		TEXT("Prototype_GameObject_PC_BulletImpactSpark"), &BulletImpactSparkDesc);
	if (!pPool)
		return E_FAIL;
	m_ObjectPools.emplace(TEXT("PC_BulletImpactSpark"), pPool);
#pragma endregion

#pragma region BULLETIMPACTSPARK2
	CPSystem::DESC BulletImpactSparkDesc2{};
	BulletImpactSparkDesc2.vPosition.y += -15.f;
	BulletImpactSparkDesc2.fMaxFrame = 1;
	BulletImpactSparkDesc2.szTextureTag = TEXT("PC_Generic");
	BulletImpactSparkDesc2.fSize = 0.2f;
	BulletImpactSparkDesc2.iParticleNums = 10;
	pPool = CObjectPool::Create(40, LEVEL_STATIC,
		TEXT("Prototype_GameObject_PC_BulletImpactSpark"), &BulletImpactSparkDesc2);
	if (!pPool)
		return E_FAIL;
	m_ObjectPools.emplace(TEXT("PC_BulletImpactSpark2"), pPool);
#pragma endregion

#pragma region BULLETTRASH
	CPSystem::DESC BulletTrash{};
	BulletTrash.vPosition.y += -15.f;
	BulletTrash.fMaxFrame = 15;
	BulletTrash.szTextureTag = TEXT("PS_Trash");
	BulletTrash.fSize = 0.5f;
	BulletTrash.iParticleNums = 2;
	BulletTrash.fNum = 100.f;
	pPool = CObjectPool::Create(20, LEVEL_STATIC,
		TEXT("Prototype_GameObject_PC_BulletImpactSpark"), &BulletTrash);
	if (!pPool)
		return E_FAIL;
	m_ObjectPools.emplace(TEXT("PC_BulletTrash"), pPool);
#pragma endregion

#pragma region BULLETSMOKE
	CPSystem::DESC BulletSmoke{};
	BulletSmoke.vPosition.y += -10.f;
	BulletSmoke.fMaxFrame = 6;
	BulletSmoke.szTextureTag = TEXT("PS_BulletSmoke");
	BulletSmoke.fSize = 3.f;
	BulletSmoke.iParticleNums = 5;
	BulletSmoke.fNum = 100.f;
	pPool = CObjectPool::Create(40, LEVEL_STATIC,
		TEXT("Prototype_GameObject_PC_BulletSmoke"), &BulletSmoke);
	if (!pPool)
		return E_FAIL;
	m_ObjectPools.emplace(TEXT("PC_BulletSmoke"), pPool);
#pragma endregion

#pragma region EmptyBullet
	CPSystem::DESC EmptyBulletDesc{};
	EmptyBulletDesc.fMaxFrame = 7;
	EmptyBulletDesc.szTextureTag = TEXT("PC_BulletShell");
	EmptyBulletDesc.fSize = 0.13f;
	pPool = CObjectPool::Create(20, LEVEL_STATIC,
		TEXT("Prototype_GameObject_PC_EmptyBullet"), &EmptyBulletDesc);
	if (!pPool)
		return E_FAIL;
	m_ObjectPools.emplace(TEXT("PC_BulletShell"), pPool);
#pragma endregion

	m_pSoundCom = dynamic_cast<CSoundController*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_COMPONENT, LEVEL_STATIC, TEXT("Prototype_Component_Sound_Thunder")));
	m_pSoundCom->SetVolume(0.5f);

	return S_OK;
}

HRESULT CFXMgr::Update(_float timeDelta)
{
	if (m_fFlashTimer > 0.f)
		m_fFlashTimer -= timeDelta;

	if (m_fThunderTimer > 0.f)
		m_fThunderTimer -= timeDelta;

	return S_OK;
}

bool CFXMgr::IsFlashing() const
{
	return m_fFlashTimer > 0.f;
}
bool CFXMgr::IsThunder() const
{
	return m_fThunderTimer > 0.f;
}

void CFXMgr::TriggerFlash()
{
	m_fFlashTimer = m_fMaxFlashTime; // 타이머 시작
}
void CFXMgr::TriggerThunder()
{
	m_fThunderTimer = m_fMaxThunderTime; // 타이머 시작
}

void CFXMgr::SpawnCustomExplosion(_float3 _vPosition, LEVEL eLevel, _float3 Size, const TCHAR* szTextureTag, _float Maxframe)
{
	CSprite::DESC ExplosionDesc{};
	ExplosionDesc.vInitPos = _vPosition;
	ExplosionDesc.vScale = Size;
	ExplosionDesc.bLoop = false;
	ExplosionDesc.fMaxFrame = Maxframe;
	ExplosionDesc.fRotationPerSec = RADIAN(180.f);
	ExplosionDesc.fSpeedPerSec = 100.f;
	ExplosionDesc.szTextureTag = szTextureTag;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Sprite"),
		eLevel, L"Layer_Effect", &ExplosionDesc)))
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
	SmokeDesc.fLifeTime = m_pGameInstance->RandomFloat(0.5, 1.5f);
	SmokeDesc.fSize = 3.f;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Smoke"),
		LEVEL_GAMEPLAY, L"Layer_Particle", &SmokeDesc)))
		return;
}

void CFXMgr::SpawnExplosion(_float3 _vPosition, LEVEL eLevel)
{
	CSprite::DESC ExplosionDesc{};
	ExplosionDesc.vInitPos = _vPosition;
	// ExplosionDesc.vScale = { 120.f,160.f,1.f };
	ExplosionDesc.vScale = { 60.f,80.f,1.f };
	ExplosionDesc.bLoop = false;
	ExplosionDesc.fMaxFrame = 32;
	ExplosionDesc.fRotationPerSec = RADIAN(180.f);
	ExplosionDesc.fSpeedPerSec = 100.f;
	ExplosionDesc.szTextureTag = TEXT("Effect_Explor");

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Sprite"),
		eLevel, L"Layer_Effect", &ExplosionDesc)))
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
	SmokeDesc.fLifeTime = m_pGameInstance->RandomFloat(0.5, 1.5f);
	SmokeDesc.fSize = 3.f;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Smoke"),
		LEVEL_GAMEPLAY, L"Layer_Particle", &SmokeDesc)))
		return;
}
void CFXMgr::SpawnExplosion2(_float3 _vPosition, LEVEL eLevel)
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
		eLevel, L"Layer_Effect", &ExplosionDesc)))
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
	SmokeDesc.fLifeTime = m_pGameInstance->RandomFloat(0.5, 1.5f);
	SmokeDesc.fSize = 3.f;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Smoke"),
		LEVEL_GAMEPLAY, L"Layer_Particle", &SmokeDesc)))
		return;


}
void CFXMgr::SpawnExplosion3(_float3 _vPosition, LEVEL eLevel)
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
		eLevel, L"Layer_Effect", &ExplosionDesc)))
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
	SmokeDesc.fLifeTime = m_pGameInstance->RandomFloat(0.5, 1.5f);
	SmokeDesc.fSize = 3.f;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Smoke"),
		LEVEL_GAMEPLAY, L"Layer_Particle", &SmokeDesc)))
		return;
}

void CFXMgr::SpawnRava(_float3 _vPosition, LEVEL eLevel)
{
	const int count = 300;
	const float baseRadius = 1000.f;

	for (int i = 0; i < count; i++)
	{
		// 1. 각도 계산 (기본 각도에 살짝 오차 추가)
		float baseAngle = (D3DX_PI * 2.0f / count) * i;
		float angleOffset = m_pGameInstance->RandomFloat(-150.f, 150.f); // 최대 약 ±11도 오차
		float angle = baseAngle + angleOffset;

		// 2. 반지름 오차도 약간 주기
		float radiusOffset = m_pGameInstance->RandomFloat(-350.f, 350.f);
		float radius = baseRadius + radiusOffset;

		// 3. 위치 계산 (중심 기준 원 + 약간 랜덤 퍼짐)
		_float3 vPosition;
		vPosition.x = _vPosition.x + cosf(angle) * radius;
		vPosition.y = _vPosition.y;
		vPosition.z = _vPosition.z + sinf(angle) * radius;

		if (1450.f < _vPosition.z && _vPosition.z > 1600.f)
			continue;

		// 4. 이펙트 생성
		CSprite::DESC SpriteDesc{};
		SpriteDesc.bLoop = true;
		SpriteDesc.fMaxFrame = 20;
		SpriteDesc.fRotationPerSec = RADIAN(180.f);
		SpriteDesc.fSpeedPerSec = 100.f;
		SpriteDesc.vInitPos = vPosition;
		SpriteDesc.vScale = _float3{ 30.f, 30.f, 1.f };

		_int iRandom = rand() % 2;
		if (iRandom == 1)
			SpriteDesc.szTextureTag = TEXT("Rava");
		else if (iRandom == 0)
			SpriteDesc.szTextureTag = TEXT("Lava");

		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Sprite"),
			eLevel, L"Layer_Effect", &SpriteDesc)))
			return;
	}
}
void CFXMgr::SpawnGunFire(_float3 _ScreenPos, LEVEL eLevel)
{
	CCameraSprite::DESC SpriteDesc{};
	SpriteDesc.bActive = false;
	SpriteDesc.fMaxFrame = 3;
	SpriteDesc.fRotationPerSec = RADIAN(180.f);
	SpriteDesc.fSpeedPerSec = 100.f;
	SpriteDesc.szTextureTag = TEXT("Effect_Revolver");
	//SpriteDesc.szTextureTag = TEXT("Check_Tile");
	SpriteDesc.vInitPos = _ScreenPos;
	SpriteDesc.vScale = _float3{ 200.f, 200.f, 1.f };
	SpriteDesc.fAniSpeed = 20.f;
	SpriteDesc.bRandom = false;           
	SpriteDesc.eEffectType = CCameraSprite::eEffectType::GUNFIRE;

	CGameObject* pObject = nullptr;
	CGameObject** ppOut = &pObject;
	if (FAILED(m_pGameInstance->Add_GameObjectReturn(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_CameraSprite"),
		eLevel, L"Layer_Effect", ppOut, &SpriteDesc)))
		return;

	TriggerFlash();
}

void CFXMgr::SpawnShotGunFire(_float3 _ScreenPos, LEVEL eLevel)
{
	CCameraSprite::DESC SpriteDesc{};
	SpriteDesc.bActive = false;
	SpriteDesc.fMaxFrame = 4;
	SpriteDesc.fRotationPerSec = RADIAN(180.f);
	SpriteDesc.fSpeedPerSec = 100.f;
	SpriteDesc.szTextureTag = TEXT("ShotGunFire");
	SpriteDesc.vInitPos = _ScreenPos;
	SpriteDesc.vScale = _float3{ 200.f, 200.f, 1.f };
	SpriteDesc.fAniSpeed = 20.f;
	SpriteDesc.bRandom = false;
	SpriteDesc.eEffectType = CCameraSprite::eEffectType::SHOTGUNFIRE;

	CGameObject* pObject = nullptr;
	CGameObject** ppOut = &pObject;
	if (FAILED(m_pGameInstance->Add_GameObjectReturn(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_CameraSprite"),
		eLevel, L"Layer_Effect", ppOut, &SpriteDesc)))
		return;

	TriggerFlash();
}
void CFXMgr::SpawnBulletTracer(_float3 _ScreenPos, LEVEL eLevel)
{
	CCameraSprite::DESC SpriteDesc{};
	SpriteDesc.bActive = false;
	SpriteDesc.fMaxFrame = 3;
	SpriteDesc.fRotationPerSec = RADIAN(180.f);
	SpriteDesc.fSpeedPerSec = 100.f;
	SpriteDesc.szTextureTag = TEXT("Effect_RevolverTacer");
	//SpriteDesc.szTextureTag = TEXT("Check_Tile");
	SpriteDesc.vInitPos = _ScreenPos;
	SpriteDesc.vScale = _float3{ 200.f, 200.f, 1.f };
	SpriteDesc.fAniSpeed = 20.f;
	SpriteDesc.bRandom = false;
	SpriteDesc.eEffectType = CCameraSprite::eEffectType::BULLETTRACER;

	CGameObject* pObject = nullptr;
	CGameObject** ppOut = &pObject;
	if (FAILED(m_pGameInstance->Add_GameObjectReturn(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_CameraSprite"),
		eLevel, L"Layer_Effect", ppOut, &SpriteDesc)))
		return;
}
void CFXMgr::SpawnBulletTracerMachineGun(_float3 _ScreenPos, LEVEL eLevel)
{
	CCameraSprite::DESC SpriteDesc{};
	SpriteDesc.bActive = false;
	SpriteDesc.fMaxFrame = 4;
	SpriteDesc.fRotationPerSec = RADIAN(180.f);
	SpriteDesc.fSpeedPerSec = 100.f;
	SpriteDesc.szTextureTag = TEXT("ChainGunTracer");
	SpriteDesc.vInitPos = _ScreenPos;
	SpriteDesc.vScale = _float3{ 200.f, 200.f, 1.f };
	SpriteDesc.fAniSpeed = 20.f;
	SpriteDesc.bRandom = false;
	SpriteDesc.eEffectType = CCameraSprite::eEffectType::CHAINGUN;

	CGameObject* pObject = nullptr;
	CGameObject** ppOut = &pObject;
	if (FAILED(m_pGameInstance->Add_GameObjectReturn(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_CameraSprite"),
		eLevel, L"Layer_Effect", ppOut, &SpriteDesc)))
		return;

	TriggerFlash();
}

void CFXMgr::SpawnShotGunTracer(_float3 _ScreenPos, LEVEL eLevel)
{
	CCameraSprite::DESC SpriteDesc{};
	SpriteDesc.bActive = false;
	SpriteDesc.fMaxFrame = 3;
	SpriteDesc.fRotationPerSec = RADIAN(180.f);
	SpriteDesc.fSpeedPerSec = 100.f;
	SpriteDesc.szTextureTag = TEXT("ShotGunTracer");
	SpriteDesc.vInitPos = _ScreenPos;
	SpriteDesc.vScale = _float3{ 200.f, 200.f, 1.f };
	SpriteDesc.fAniSpeed = 20.f;
	SpriteDesc.bRandom = false;
	SpriteDesc.eEffectType = CCameraSprite::eEffectType::CHAINGUN;

	CGameObject* pObject = nullptr;
	CGameObject** ppOut = &pObject;
	if (FAILED(m_pGameInstance->Add_GameObjectReturn(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_CameraSprite"),
		eLevel, L"Layer_Effect", ppOut, &SpriteDesc)))
		return;
}


void CFXMgr::SpawnGunFireMachineGun(_float3 _ScreenPos, LEVEL eLevel)
{
	CCameraSprite::DESC SpriteDesc{};
	SpriteDesc.bActive = false;
	SpriteDesc.fMaxFrame = 5;
	SpriteDesc.fRotationPerSec = RADIAN(180.f);
	SpriteDesc.fSpeedPerSec = 100.f;
	//SpriteDesc.szTextureTag = TEXT("Effect_RevolverTacer");
	SpriteDesc.szTextureTag = TEXT("GunFireMachineGun");
	SpriteDesc.vInitPos = _ScreenPos;
	SpriteDesc.vScale = _float3{ 250.f, 250.f, 1.f };
	SpriteDesc.fAniSpeed = 30.f;
	SpriteDesc.bRandom = false;
	SpriteDesc.eEffectType = CCameraSprite::eEffectType::GUNFIRE;

	CGameObject* pObject = nullptr;
	CGameObject** ppOut = &pObject;
	if (FAILED(m_pGameInstance->Add_GameObjectReturn(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_CameraSprite"),
		eLevel, L"Layer_Effect", ppOut, &SpriteDesc)))
		return;

	//m_vecSceenEffect.push_back(dynamic_cast<CCameraSprite*>(*ppOut));
}
void CFXMgr::SpawnFireMachineGun(_float3 _ScreenPos, LEVEL eLevel)
{
	CCameraSprite::DESC SpriteDesc{};
	SpriteDesc.bActive = false;
	SpriteDesc.fMaxFrame = 5;
	SpriteDesc.fRotationPerSec = RADIAN(180.f);
	SpriteDesc.fSpeedPerSec = 100.f;
	//SpriteDesc.szTextureTag = TEXT("Effect_RevolverTacer");
	SpriteDesc.szTextureTag = TEXT("FireMachineGun");
	SpriteDesc.vInitPos = _ScreenPos;
	SpriteDesc.vScale = _float3{ 200.f, 200.f, 1.f };
	SpriteDesc.fAniSpeed = 30.f;
	SpriteDesc.bRandom = false;
	SpriteDesc.eEffectType = CCameraSprite::eEffectType::FIRE;

	CGameObject* pObject = nullptr;
	CGameObject** ppOut = &pObject;
	if (FAILED(m_pGameInstance->Add_GameObjectReturn(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_CameraSprite"),
		eLevel, L"Layer_Effect", ppOut, &SpriteDesc)))
		return;

	//m_vecSceenEffect.push_back(dynamic_cast<CCameraSprite*>(*ppOut));
}
void CFXMgr::SpawnTornado(_float3 _vPosition, LEVEL eLevel)
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
		eLevel, L"Layer_Effect", &SpriteDesc)))
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
void CFXMgr::SpawnSpher(_float3 _vPosition, LEVEL eLevel)
{
	CPSystem::DESC SphereDesc{};
	SphereDesc.vPosition = _vPosition;
	SphereDesc.vPosition.y += 0.f;
	SphereDesc.fMaxFrame = 1;
	SphereDesc.szTextureTag = TEXT("PC_Generic");
	SphereDesc.iParticleNums = 1500;
	SphereDesc.fSize = 0.15f;
	SphereDesc.fLifeTime = GetRandomFloat(0.5f, 1.f);
	SphereDesc.fLifeTime = GetRandomFloat(0.5f, 1.f);
	SphereDesc.fMin = 800.f;
	SphereDesc.fMax = 1100.f;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Sphere"),
		eLevel, L"Layer_Particle", &SphereDesc)))
		return;

	CPSystem::DESC SphereDesc2{};
	SphereDesc2.vPosition = _vPosition;
	SphereDesc2.vPosition.y += 0.f;
	SphereDesc2.fMaxFrame = 1;
	SphereDesc2.szTextureTag = TEXT("PC_Generic");
	SphereDesc2.iParticleNums = 1500;
	SphereDesc2.fSize = 0.3f;
	SphereDesc2.fLifeTime = GetRandomFloat(0.5f, 1.f);
	SphereDesc2.fMin = 800.f;
	SphereDesc2.fMax = 1200.f;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Sphere"),
		LEVEL_GAMEPLAY, L"Layer_Particle", &SphereDesc2)))
		return;

}
void CFXMgr::SpawnSpherSmall(_float3 _vPosition, LEVEL eLevel)
{
	CPSystem::DESC SphereDesc{};
	SphereDesc.vPosition = _vPosition;
	SphereDesc.vPosition.y += 0.f;
	SphereDesc.fMaxFrame = 1;
	SphereDesc.szTextureTag = TEXT("PC_Generic");
	SphereDesc.iParticleNums = 1500;
	SphereDesc.fSize = 0.15f;
	SphereDesc.fLifeTime = GetRandomFloat(0.5f, 1.f);
	SphereDesc.fLifeTime = GetRandomFloat(0.5f, 1.f);
	SphereDesc.fMin = 50.f;
	SphereDesc.fMax = 70.f;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Sphere"),
		eLevel, L"Layer_Particle", &SphereDesc)))
		return;

	CPSystem::DESC SphereDesc2{};
	SphereDesc2.vPosition = _vPosition;
	SphereDesc2.vPosition.y += 0.f;
	SphereDesc2.fMaxFrame = 1;
	SphereDesc2.szTextureTag = TEXT("PC_Generic");
	SphereDesc2.iParticleNums = 1500;
	SphereDesc2.fSize = 0.3f;
	SphereDesc2.fLifeTime = GetRandomFloat(0.5f, 1.f);
	SphereDesc2.fMin = 40.f;
	SphereDesc2.fMax = 60.f;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Sphere"),
		LEVEL_GAMEPLAY, L"Layer_Particle", &SphereDesc2)))
		return;

}

void CFXMgr::SpawnField(_float3 _vPosition, LEVEL eLevel)
{
	CPSystem::DESC SphereDesc{};
	SphereDesc.vPosition = _vPosition;
	SphereDesc.vPosition.y = 44.f;
	SphereDesc.fMaxFrame = 1;
	SphereDesc.szTextureTag = TEXT("PC_Generic");
	SphereDesc.iParticleNums = 2500;
	SphereDesc.fSize = 0.01f;
	SphereDesc.fLifeTime = GetRandomFloat(0.5f, 1.f);
	SphereDesc.fMin = 50.f;
	SphereDesc.fMax = 70.f;
	SphereDesc.isLoop = true;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Sphere"),
		eLevel, L"Layer_Particle", &SphereDesc)))
		return;

	CPSystem::DESC SphereDesc2{};
	SphereDesc2.vPosition = _vPosition;
	SphereDesc2.vPosition.y = 44.f;
	SphereDesc2.fMaxFrame = 1;
	SphereDesc2.szTextureTag = TEXT("PC_Generic");
	SphereDesc2.iParticleNums = 1500;
	SphereDesc2.fSize = 0.05f;
	SphereDesc2.fLifeTime = GetRandomFloat(0.5f, 1.f);
	SphereDesc.fMin = 50.f;
	SphereDesc.fMax = 70.f;
	SphereDesc2.isLoop = true;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Sphere"),
		LEVEL_GAMEPLAY, L"Layer_Particle", &SphereDesc2)))
		return;
}

void CFXMgr::SpawnMultipleExplosions(_float fTimeDelta, LEVEL eLevel)
{
	static float fTimer = 0.0f;
	const float fInterval = 2.f; //반복주기

	fTimer += fTimeDelta;
	if (fTimer >= fInterval)
	{
		_float3 vPosition = { m_pGameInstance->RandomFloat(550.f, 1050.f) ,
			m_pGameInstance->RandomFloat(50.f, 200.f) ,
			m_pGameInstance->RandomFloat(-50.f, -350.f) };
		SpawnExplosion(vPosition, eLevel);

		fTimer = 0.0f;
	}
}
void CFXMgr::SpawnMultipleExplosions2(_float fTimeDelta, LEVEL eLevel)
{
	static float fTimer = 0.0f;
	const float fInterval = 1.5f; //반복주기

	fTimer += fTimeDelta;
	if (fTimer >= fInterval)
	{
		_float3 vPosition = { m_pGameInstance->RandomFloat(550.f, 1050.f) , m_pGameInstance->RandomFloat(50.f, 200.f) , m_pGameInstance->RandomFloat(-50.f, -350.f) };
		SpawnExplosion2(vPosition, eLevel);

		fTimer = 0.0f;
	}
}
void CFXMgr::SpawnMultipleExplosions3(_float fTimeDelta, LEVEL eLevel)
{
	static float fTimer = 0.0f;
	const float fInterval = 1.5f; //반복주기

	fTimer += fTimeDelta;
	if (fTimer >= fInterval)
	{
		_float3 vPosition = { m_pGameInstance->RandomFloat(550.f, 1050.f) , m_pGameInstance->RandomFloat(50.f, 200.f) , m_pGameInstance->RandomFloat(-50.f, -350.f) };
		SpawnExplosion3(vPosition, eLevel);

		fTimer = 0.0f;
	}
}

void CFXMgr::SpawnRaceExplosion(_float3 _vPosition, _float3 _vPosOffset , LEVEL eLevel, _float3 Size, const TCHAR* szTextureTag, _float Maxframe)
{
	CRaceSprite::DESC ExplosionDesc{};
	ExplosionDesc.vInitPos = {0.f, 0.f, 0.f};
	ExplosionDesc.vPosOffset = _vPosOffset;
	ExplosionDesc.vScale = Size;
	ExplosionDesc.bLoop = false;
	ExplosionDesc.fMaxFrame = Maxframe;
	ExplosionDesc.fRotationPerSec = RADIAN(180.f);
	ExplosionDesc.fSpeedPerSec = 100.f;
	ExplosionDesc.szTextureTag = szTextureTag;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_RaceSprite"),
		eLevel, L"Layer_Effect", &ExplosionDesc)))
		return;

	CPSystem::DESC FireworkDesc{};
	FireworkDesc.vPosition = _vPosition;
	FireworkDesc.fMaxFrame = 14;
	FireworkDesc.szTextureTag = TEXT("PC_Small_Fire");
	FireworkDesc.iParticleNums = 5;
	FireworkDesc.fSize = 1.f;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Firework"),
		eLevel, L"Layer_Particle", &FireworkDesc)))
		return;

	CPSystem::DESC FireworkDesc2{};
	FireworkDesc2.vPosition = _vPosition;
	FireworkDesc2.fMaxFrame = 1;
	FireworkDesc2.szTextureTag = TEXT("PC_Generic");
	FireworkDesc2.iParticleNums = 2;
	FireworkDesc2.fSize = 0.5f;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Firework"),
		eLevel, L"Layer_Particle", &FireworkDesc)))
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
	SmokeDesc.fLifeTime = m_pGameInstance->RandomFloat(0.5, 1.5f);
	SmokeDesc.fSize = 3.f;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Smoke"),
		eLevel, L"Layer_Particle", &SmokeDesc)))
		return;
}

void CFXMgr::SpawnMultipleExplosionRaceBoss(_float3 _Position , LEVEL eLevel)
{
	// 각 포지션 세팅
	_float3 vExplorerPos[5] = {
		{ _Position.x + 200.f, _Position.y + 10.f, _Position.z - 220.f },
		{ _Position.x + 100.f, _Position.y + 10.f, _Position.z - 222.f },
		{ _Position.x + 0.f, _Position.y + 10.f, _Position.z - 224.f },
		{ _Position.x - 100.f, _Position.y + 10.f, _Position.z - 226.f },
		{ _Position.x - 200.f, _Position.y + 10.f, _Position.z - 228.f }
	};

	// 폭발 인덱스 0~4
	std::vector<int> indices = { 0, 1, 2, 3, 4 };

	// 랜덤 셔플
	std::random_device rd;
	std::mt19937 gen(rd());
	std::shuffle(indices.begin(), indices.end(), gen);

	// 랜덤 개수 정하기 (예: 2~5개 사이)
	int iSpawnCount = 1 + rand() % 2;

	for (int i = 0; i < iSpawnCount; ++i)
	{
		int idx = indices[i];
		SpawnRaceExplosion(vExplorerPos[idx],{ vExplorerPos[idx].x - _Position.x, 50.f, vExplorerPos[idx].z - _Position.z},
			eLevel,
			{ 130.f, 140.f, 1.f },
			TEXT("Effect_Explor"),
			32.f
		);
	}

	CAMERA_MANAGER->Shake_Camera(0.5f, 0.5f);
}

void CFXMgr::SpawnMultipleExplosionRacePoint(_float fTimeDelta, _float3 _vPosition, _float3 _vPosOffset, LEVEL eLevel, _float3 Size, const TCHAR* szTextureTag, _float Maxframe)
{
	static float fTimer = 0.0f;
	const float fInterval = 0.1f; //반복주기

	fTimer += fTimeDelta;
	if (fTimer >= fInterval)
	{
		_float3 vParticlePos;
		vParticlePos.x = _vPosition.x + _vPosOffset.x;
		vParticlePos.y = _vPosition.y + _vPosOffset.y;
		vParticlePos.z = _vPosition.z + _vPosOffset.z;

		_float3 vParticlePosion = {
			m_pGameInstance->RandomFloat(vParticlePos.x - 40.f, vParticlePos.x + 40.f) ,
			m_pGameInstance->RandomFloat(vParticlePos.y - 40.f, vParticlePos.y + 40.f) ,
			m_pGameInstance->RandomFloat(vParticlePos.z - 100.f, vParticlePos.z + 150.f) };

		_float3 vPosOffset = {
			m_pGameInstance->RandomFloat(_vPosOffset.x - 40.f, _vPosOffset.x + 40.f) ,
			m_pGameInstance->RandomFloat(_vPosOffset.y - 40.f, _vPosOffset.y + 40.f) ,
			m_pGameInstance->RandomFloat(_vPosOffset.z - 100.f, _vPosOffset.z + 150.f) };

		//월드에서의 거리 , 중점에서의 거리

		const TCHAR* szEffectNames[] = {TEXT("Effect_Explor"),TEXT("PC_Explosion"),	TEXT("Effect_Explorer")	};
		int iRandom = rand() % 3;

		SpawnRaceExplosion(vParticlePosion, vPosOffset, eLevel, {30.f, 50.f, 1.f}, szEffectNames[iRandom], 13.f);

		fTimer = 0.0f;
	}
}

void CFXMgr::SpawnEmptyBullet(_float3 _vPosition, LEVEL eLevel)
{
	CPSystem::DESC EmptyBulletDesc{};
	EmptyBulletDesc.vPosition = _vPosition;
	EmptyBulletDesc.iParticleNums = 1;

	auto Iter = m_ObjectPools.find(TEXT("PC_BulletShell"));
	if (Iter == m_ObjectPools.end())
		return;

	Iter->second->Active_Object(eLevel, TEXT("Layer_Particle"), &EmptyBulletDesc);
}

void CFXMgr::SpawnBlood(_float3 _vPosition, LEVEL eLevel)
{
	CPSystem::DESC BloodDesc{};
	BloodDesc.vPosition = _vPosition;
	BloodDesc.iParticleNums = 1;
	
	auto Iter = m_ObjectPools.find(TEXT("PS_Blood"));
	if (Iter == m_ObjectPools.end())
		return;

	Iter->second->Active_Object(eLevel, TEXT("Layer_Particle"), &BloodDesc);

	BloodDesc.vPosition.x += 3;
	Iter->second->Active_Object(eLevel, TEXT("Layer_Particle"), &BloodDesc);


	CPSystem::DESC BulletImpactSparkDesc{};
	BulletImpactSparkDesc.vPosition = _vPosition;
	BulletImpactSparkDesc.vPosition.y += -20.f;
	BulletImpactSparkDesc.iParticleNums = 5;

	Iter = m_ObjectPools.find(TEXT("PC_BulletImpactSpark"));
	if (Iter == m_ObjectPools.end())
		return;

	Iter->second->Active_Object(eLevel, TEXT("Layer_Particle"), &BulletImpactSparkDesc);
}

void CFXMgr::SpawnBulletMark(_float3 _vPosition, LEVEL eLevel, _float3 _vLook , _int _iNum)
{
	/* [ 총알자국 스프라이트 ] */
	CBulletMark::DESC BulletMarkDesc{};
	BulletMarkDesc.bLoop = false;
	BulletMarkDesc.fMaxFrame = 5.f;
	BulletMarkDesc.fRotationPerSec = RADIAN(180.f);
	BulletMarkDesc.fSpeedPerSec = 100.f;
	BulletMarkDesc.szTextureTag = TEXT("BulletMark");
	//BulletMarkDesc.szTextureTag = TEXT("Check_Tile");
	BulletMarkDesc.vInitPos = _vPosition;
	BulletMarkDesc.iType = _iNum;
	BulletMarkDesc.vLook = _vLook;

	BulletMarkDesc.vScale = _float3{ 5.f, 5.f, 1.f };

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_BulletMark"),
		eLevel, L"Layer_Effect", &BulletMarkDesc)))
		return;

	/* [ 연기 튀는 파티클 ] */
	CPSystem::DESC BulletSmoke{};
	BulletSmoke.vPosition = _vPosition;
	BulletSmoke.iParticleNums = 5;

	auto Iter = m_ObjectPools.find(TEXT("PC_BulletSmoke"));
	if (Iter == m_ObjectPools.end())
		return;

	Iter->second->Active_Object(eLevel, TEXT("Layer_Particle"), &BulletSmoke);

	/* [ 스파크 튀는 파티클 ] */
	CPSystem::DESC BulletImpactSparkDesc{};
	BulletImpactSparkDesc.vPosition = _vPosition;
	BulletImpactSparkDesc.vPosition.y += -10.f;
	BulletImpactSparkDesc.iParticleNums = 2;

	Iter = m_ObjectPools.find(TEXT("PC_BulletTrash"));
	if (Iter == m_ObjectPools.end())
		return;

	Iter->second->Active_Object(eLevel, TEXT("Layer_Particle"), &BulletImpactSparkDesc);

	CPSystem::DESC BulletImpactSparkDesc2{};
	BulletImpactSparkDesc2.vPosition = _vPosition;
	BulletImpactSparkDesc2.vPosition.y += -5.f;
	BulletImpactSparkDesc2.iParticleNums = 10;

	Iter = m_ObjectPools.find(TEXT("PC_BulletImpactSpark2"));
	if (Iter == m_ObjectPools.end())
		return;

	Iter->second->Active_Object(eLevel, TEXT("Layer_Particle"), &BulletImpactSparkDesc2);
}
void CFXMgr::SpawnBulletDust(_float3 _vPosition, LEVEL eLevel)
{
	/* [ 연기 튀는 파티클 ] */
	CPSystem::DESC BulletSmoke{};
	BulletSmoke.vPosition = _vPosition;
	BulletSmoke.iParticleNums = 5;

	auto Iter = m_ObjectPools.find(TEXT("PC_BulletSmoke"));
	if (Iter == m_ObjectPools.end())
		return;

	Iter->second->Active_Object(eLevel, TEXT("Layer_Particle"), &BulletSmoke);

	/* [ 스파크 튀는 파티클 ] */
	CPSystem::DESC BulletImpactSparkDesc{};
	BulletImpactSparkDesc.vPosition = _vPosition;
	BulletImpactSparkDesc.vPosition.y += -10.f;
	BulletImpactSparkDesc.iParticleNums = 2;

	Iter = m_ObjectPools.find(TEXT("PC_BulletTrash"));
	if (Iter == m_ObjectPools.end())
		return;

	Iter->second->Active_Object(eLevel, TEXT("Layer_Particle"), &BulletImpactSparkDesc);

	CPSystem::DESC BulletImpactSparkDesc2{};
	BulletImpactSparkDesc2.vPosition = _vPosition;
	BulletImpactSparkDesc2.vPosition.y += -5.f;
	BulletImpactSparkDesc2.iParticleNums = 10;

	Iter = m_ObjectPools.find(TEXT("PC_BulletImpactSpark2"));
	if (Iter == m_ObjectPools.end())
		return;

	Iter->second->Active_Object(eLevel, TEXT("Layer_Particle"), &BulletImpactSparkDesc2);
}

void CFXMgr::SpawnHitEffect(LEVEL eLevel)
{
	/* [ 히트 스크린 스프라이트 ] */
	CScreenSprite::DESC ScreenHitDesc{};
	ScreenHitDesc.fMaxFrame = 25.f;
	ScreenHitDesc.fAniSpeed = 10.f;
	ScreenHitDesc.fRotationPerSec = RADIAN(180.f);
	ScreenHitDesc.fSpeedPerSec = 100.f;
	ScreenHitDesc.eEffectType = CScreenSprite::eEffectType::HIT;
	ScreenHitDesc.szTextureTag = TEXT("ScreenHit");
	//ScreenHitDesc.szTextureTag = TEXT("Check_Tile");


	ScreenHitDesc.vScale = _float3{ FWINCX, FWINCY, 1.f };
	ScreenHitDesc.vInitPos = { g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f,1.f };

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_ScreenSprite"),
		eLevel, L"Layer_Effect", &ScreenHitDesc)))
		return;
}

void CFXMgr::SpawnThanderEffect(LEVEL eLevel)
{
	if (CAMERA_MANAGER->Get_CurCameraID() == CCameraManager::FPS)
	{
		/* [ 히트 스크린 스프라이트 ] */
		CScreenSprite::DESC ScreenHitDesc{};
		ScreenHitDesc.fMaxFrame = 25.f;
		ScreenHitDesc.fAniSpeed = 10.f;
		ScreenHitDesc.fRotationPerSec = RADIAN(180.f);
		ScreenHitDesc.fSpeedPerSec = 100.f;
		ScreenHitDesc.eEffectType = CScreenSprite::eEffectType::THUNDER;
		ScreenHitDesc.szTextureTag = TEXT("White");
		//ScreenHitDesc.szTextureTag = TEXT("Check_Tile");


		ScreenHitDesc.vScale = _float3{ FWINCX, FWINCY, 1.f };
		ScreenHitDesc.vInitPos = { g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f,1.f };

		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_ScreenSprite"),
			eLevel, L"Layer_Effect", &ScreenHitDesc)))
			return;
	}
}

void CFXMgr::SpawnHealEffect(LEVEL eLevel)
{
	/* [ 히트 스크린 스프라이트 ] */
	CScreenSprite::DESC ScreenHealDesc{};
	ScreenHealDesc.fMaxFrame = 30.f;
	ScreenHealDesc.fAniSpeed = 10.f;
	ScreenHealDesc.fRotationPerSec = RADIAN(180.f);
	ScreenHealDesc.fSpeedPerSec = 100.f;
	ScreenHealDesc.eEffectType = CScreenSprite::eEffectType::HEAL;
	ScreenHealDesc.szTextureTag = TEXT("ScreenHeal");
	//ScreenHitDesc.szTextureTag = TEXT("Check_Tile");


	ScreenHealDesc.vScale = _float3{ FWINCX, FWINCY, 1.f };
	ScreenHealDesc.vInitPos = { g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f,1.f };

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_ScreenSprite"),
		eLevel, L"Layer_Effect", &ScreenHealDesc)))
		return;
}

void CFXMgr::FireAttack(_float3 _vPosition, LEVEL eLevel, _int _iNum)
{
	CPSystem::DESC FireAttackDesc{};
	FireAttackDesc.vPosition = _vPosition;
	FireAttackDesc.fMaxFrame = 20;
	FireAttackDesc.szTextureTag = TEXT("FireAttack");
	FireAttackDesc.iParticleNums = 8;
	FireAttackDesc.fSize = 3.f;
	FireAttackDesc.iNum = _iNum;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_FireAttack"),
		LEVEL_GAMEPLAY, L"Layer_Particle", &FireAttackDesc)))
		return;


	CPSystem::DESC FireAttackDesc2{};
	FireAttackDesc2.vPosition = _vPosition;
	FireAttackDesc2.fMaxFrame = 1;
	FireAttackDesc2.szTextureTag = TEXT("PC_Generic");
	FireAttackDesc2.iParticleNums = 5;
	FireAttackDesc2.fSize = 0.2f;
	FireAttackDesc2.fMin = -0.05f;
	FireAttackDesc2.fMax = 0.05f;
	FireAttackDesc2.iNum = _iNum;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_FireAttack"),
		LEVEL_GAMEPLAY, L"Layer_Particle", &FireAttackDesc2)))
		return;
}

void CFXMgr::JumpAttack(_float3 _vPosition, LEVEL eLevel)
{
	CPSystem::DESC JumpAttackDesc{};
	JumpAttackDesc.vPosition = _vPosition;
	JumpAttackDesc.fMaxFrame = 1;
	JumpAttackDesc.szTextureTag = TEXT("PS_Trash");
	JumpAttackDesc.iParticleNums = 60;
	JumpAttackDesc.fSize = 0.8f;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_JumpAttack"),
		LEVEL_GAMEPLAY, L"Layer_Particle", &JumpAttackDesc)))
		return;

	CPSystem::DESC JumpAttackDesc2{};
	JumpAttackDesc2.vPosition = _vPosition;
	JumpAttackDesc2.fMaxFrame = 15;
	JumpAttackDesc2.szTextureTag = TEXT("PS_Trash");
	JumpAttackDesc2.iParticleNums = 10;
	JumpAttackDesc2.fSize = 5.f;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_JumpAttack"),
		LEVEL_GAMEPLAY, L"Layer_Particle", &JumpAttackDesc2)))
		return;
}

void CFXMgr::CutSceneSmoke(_float3 _vPosition, LEVEL eLevel)
{
	CPSystem::DESC CutSceneSmokeDesc{};
	CutSceneSmokeDesc.vPosition = _vPosition;
	CutSceneSmokeDesc.szTextureTag = TEXT("PC_Small_Smoke");
	CutSceneSmokeDesc.iParticleNums = 100;
	CutSceneSmokeDesc.fSize = 12.f;
	CutSceneSmokeDesc.fMaxFrame = 20.f;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_CutSceneSmoke"),
		LEVEL_GAMEPLAY, L"Layer_Particle", &CutSceneSmokeDesc)))
		return;
}

void CFXMgr::PlayerDash(LEVEL eLevel)
{
	CCameraSprite::DESC SpriteDesc{};
	SpriteDesc.fMaxFrame = 10;
	SpriteDesc.szTextureTag = TEXT("Effect_Dash");
	SpriteDesc.vScale = _float3{ FWINCX, FWINCY, 1.f };
	SpriteDesc.vInitPos = { g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f,1.f }; 
	SpriteDesc.fAniSpeed = 20.f;
	SpriteDesc.bRandom = false;
	SpriteDesc.eEffectType = CCameraSprite::eEffectType::DASH;
;
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_CameraSprite"),
		eLevel, L"Layer_Particle", &SpriteDesc)))
		return;

}

void CFXMgr::Damage_Indicator(LEVEL eLevel, _float RotZRadian)
{
	CCameraSprite::DESC SpriteDesc{};
	SpriteDesc.fMaxFrame = 1;
	SpriteDesc.szTextureTag = TEXT("Damage_Indicator");
	SpriteDesc.vScale = _float3{ FWINCY, FWINCY, 1.f };
	SpriteDesc.vInitPos = { g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f,1.f };
	SpriteDesc.fAniSpeed = 1.4f;
	SpriteDesc.bRandom = false;
	SpriteDesc.eEffectType = CCameraSprite::eEffectType::DAMAGE_INDICATOR;

	CGameObject* pObj{};

	if (FAILED(m_pGameInstance->Add_GameObjectReturn(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_CameraSprite"),
		eLevel, L"Layer_Particle", &pObj, &SpriteDesc)))
		return;

	static_cast<CCameraSprite*>(pObj)->RotZ(RotZRadian);
}

void CFXMgr::SpawnRain(LEVEL eLevel)
{
	CRain::DESC RainDesc{};
	RainDesc.vMin = _float3(-1000.f, -1000.f, 0.f);
	RainDesc.vMax = _float3(3000.f, 1500.f, 3000.f);

	//빗방울
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Rain"),
		eLevel, L"Layer_Particle", &RainDesc)))
		return;
}

void CFXMgr::SpawnThunder(_float3 _vPosition, LEVEL eLevel)
{
	CSprite::DESC SpriteDesc{};
	SpriteDesc.bLoop = false;
	SpriteDesc.fStartFrame = GetRandomFloat(0.f, 9.f);
	SpriteDesc.fMaxFrame = SpriteDesc.fStartFrame + 2.f;
	SpriteDesc.fRotationPerSec = RADIAN(180.f);
	SpriteDesc.fSpeedPerSec = 100.f;
	SpriteDesc.vInitPos = _vPosition;
	SpriteDesc.vScale = _float3{ 64.f * 2.f, 128.f * 2.f, 1.f };
	SpriteDesc.vScale *= GetRandomFloat(3.5f, 4.5f);
	SpriteDesc.szTextureTag = TEXT("Thunderbolt");

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Sprite"),
		eLevel, L"Layer_Effect", &SpriteDesc)))
		return;
	
	TriggerThunder();

	int iRandom = GetRandomInt(0, 10);
	if (iRandom <= 1)
	{
		SpawnThanderEffect(eLevel);
	}
}

void CFXMgr::SpawnMultipleThunder(_float fTimeDelta, LEVEL eLevel)
{
	static float fTimer = 0.0f;
	const float fInterval = 1.f; //반복주기

	fTimer += fTimeDelta;
	if (fTimer >= fInterval)
	{
		_float fRandomX = m_pGameInstance->RandomFloat(101.0f, 1684.0f);
		_float fRandomY = m_pGameInstance->RandomFloat(1000.f, 1050.f);
		_float fRandomZ = m_pGameInstance->RandomFloat(95.0f, 949.0f);

		m_vThunderPos = { fRandomX, fRandomY, fRandomZ };

		_float3 vPosition = { fRandomX, fRandomY, fRandomZ };
		SpawnThunder(vPosition, eLevel);
		
		if (PERCENT(50))
		{
			switch (rand() % 4)
			{
			case 0:
				m_pSoundCom->Play("thunder_close");
				break;
			case 1:
				m_pSoundCom->Play("big_thunder_clap");
				break;
			case 2:
				m_pSoundCom->Play("loud_thunder");
				break;
			case 3:
				m_pSoundCom->Play("thunder");
				break;
			default:
				m_pSoundCom->Play("thunder_close");
				break;
			}
		}

		fTimer = 0.0f;
	}
}


CFXMgr* CFXMgr::Create()
{
	CFXMgr* pInstance = new CFXMgr();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CFXMgr");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFXMgr::Free()
{
	__super::Free();

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pSoundCom);

	for (auto Pair : m_ObjectPools)
		Safe_Release(Pair.second);
	m_ObjectPools.clear();
}

