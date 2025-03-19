#include "Level_GamePlay.h"
#include "Explosion.h"
#include "Firework.h"
#include "GameInstance.h"
#include "EmptyBullet.h"
#include "Sprite.h"

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

	if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Particle(TEXT("Layer_Particle"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;

	//if(FAILED(Ready_Light()))
	//	return E_FAIL;


	return S_OK;
}

void CLevel_GamePlay::Update(_float fTimeDelta)
{
	SpawnMultipleExplosions(fTimeDelta);
	SpawnMultipleExplosions(fTimeDelta);

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
	m_pGameInstance->Intersect(LEVEL_GAMEPLAY, TEXT("Layer_PBullet"), TEXT("Layer_Monster"));
	m_pGameInstance->Intersect(LEVEL_GAMEPLAY, TEXT("Layer_PBullet"), TEXT("Layer_Statue"));
}

HRESULT CLevel_GamePlay::Render()
{
	SetWindowText(g_hWnd, TEXT("게임플레이 레벨입니다."));

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Terrain(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Terrain"),
		LEVEL_GAMEPLAY, strLayerTag, m_pData->Find_Data(TEXT("Terrain")))))
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
	CMyCube::DESC CubeDesc{};

	CubeDesc.vInitPos = { 150.f,50.f,150.f };
	CubeDesc.vScale = { 50.f,100.f,50.f };
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_MyCube"),
		LEVEL_GAMEPLAY, strLayerTag, &CubeDesc)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Trapezoid"),
		LEVEL_GAMEPLAY, strLayerTag, m_pData->Find_Data(TEXT("Trapezoid")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Stall"),
		LEVEL_GAMEPLAY, strLayerTag, m_pData->Find_Data(TEXT("Stall")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Cabinet"),
		LEVEL_GAMEPLAY, strLayerTag, m_pData->Find_Data(TEXT("Cabinet")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Signboard"),
		LEVEL_GAMEPLAY, strLayerTag, m_pData->Find_Data(TEXT("Signboard")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_MyComputer"),
		LEVEL_GAMEPLAY, strLayerTag, m_pData->Find_Data(TEXT("MyComputer")))))
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
	
	//토네이도
	for (int i = 0; i < 3; ++i)
	{
		_float3 vPosition = { GetRandomValue(100, 700) , 25.f , GetRandomValue(100, 700) };
		SpawnTornado(vPosition);
	}
	//불지르기
	for (int i = 0; i < 20; ++i)
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

#include "TestMonster.h"
HRESULT CLevel_GamePlay::Ready_Layer_Monster(const _wstring& strLayerTag)
{
	CTestMonster::DESC MonsterDesc{};
	MonsterDesc.vInitPos = { 400.f,25.f,200.f };
	MonsterDesc.vScale = {20.f,50.f,20.f};

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_TestMonster"),
		LEVEL_GAMEPLAY, strLayerTag, &MonsterDesc)))
		return E_FAIL;

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
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Light()
{
	D3DLIGHT9		tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;

	tLightInfo.Diffuse = { 1.f, 1.f, 1.f, 1.f };
	tLightInfo.Specular = { 1.f, 1.f, 1.f, 1.f };
	tLightInfo.Ambient = { 1.f, 1.f, 1.f, 1.f };
	tLightInfo.Direction = { 1.f, -1.f, 1.f };
	tLightInfo.Position = { 50.f,50.f,50.f };

	++m_iIndex;

	m_pGraphic_Device->SetLight(m_iIndex, &tLightInfo);

	m_pGraphic_Device->LightEnable(m_iIndex, TRUE);
	
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pGraphic_Device->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(50, 50, 50));

	return S_OK;
}

void CLevel_GamePlay::SpawnExplosion(_float3 _vPosition)
{
	CExplosion::DESC ExplosionDesc{};
	ExplosionDesc.vInitPos = _vPosition;
	ExplosionDesc.vScale = { 120.f,160.f,1.f };

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Explosion"),
		LEVEL_GAMEPLAY, L"Layer_Effect", &ExplosionDesc)))
		return;

	
	CPSystem::DESC FireworkDesc{};
	FireworkDesc.vPosition = _vPosition;
	FireworkDesc.fMaxFrame = 14;
	FireworkDesc.szTextureTag = TEXT("PC_Explosion");
	FireworkDesc.iParticleNums = 10;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Firework"),
		LEVEL_GAMEPLAY, L"Layer_Particle", &FireworkDesc)))
		return;

	CPSystem::DESC SmokeDesc{};
	SmokeDesc.vPosition = _vPosition;
	SmokeDesc.fMaxFrame = 20;
	SmokeDesc.szTextureTag = TEXT("PC_Small_Smoke");
	SmokeDesc.iParticleNums = 4;

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
	SpriteDesc.vScale = _float3{ 60.f, 60.f, 1.f };

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
	SpriteDesc.vInitPos.y += 90.f;
	
	SpriteDesc.vScale = _float3{ 250.f, 250.f, 1.f };

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Sprite"),
		LEVEL_GAMEPLAY, L"Layer_Particle", &SpriteDesc)))
		return;

	CPSystem::DESC TornadoDesc{};
	TornadoDesc.vPosition = _vPosition;
	TornadoDesc.vPosition.y += -20;
	TornadoDesc.fMaxFrame = 5;
	TornadoDesc.szTextureTag = TEXT("PC_Small_Fire");
	TornadoDesc.iParticleNums = 50;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Tornado"),
		LEVEL_GAMEPLAY, L"Layer_Particle", &TornadoDesc)))
		return;

}

void CLevel_GamePlay::SpawnMultipleExplosions(_float fTimeDelta)
{
	static float fTimer = 0.0f;
	const float fInterval = 0.2f; //반복주기

	fTimer += fTimeDelta;
	if (fTimer >= fInterval)
	{
		_float3 vPosition = { GetRandomValue(200, 800) , GetRandomValue(100, 200) , GetRandomValue(100, 800) };
		SpawnExplosion(vPosition);

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

	while (m_iIndex >= 0)
	{
		m_pGraphic_Device->LightEnable(m_iIndex, FALSE);
		--m_iIndex;
	}
}
