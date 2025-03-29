#include "Level_GamePlay.h"
#include "Level_Loading.h"
#include "Explosion.h"
#include "Firework.h"
#include "GameInstance.h"
#include "Flatform.h"
#include "Monster.h"
#include "FXMgr.h"
#include "UI_Manager.h"

#include "Trigger.h"
#include "Map.h"

#define CurLevel LEVEL_GAMEPLAY

CLevel_GamePlay::CLevel_GamePlay(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel { pGraphic_Device }
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

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;

	CUI_Manager::Get_Instance(m_pGameInstance)->Initialize_GamePlayUI();

	if (FAILED(Ready_Layer_Pawn(TEXT("Layer_Pawn"))))
		return E_FAIL;

	CUI_Manager::Get_Instance()->Initialize_Player();

	if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Statue(TEXT("Layer_Statue"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Particle(TEXT("Layer_Particle"))))
		return E_FAIL;

	//if (FAILED(Ready_Layer_Item(TEXT("Layer_Item"))))
	//	return E_FAIL;

	//if(FAILED(Ready_Light()))
	//	return E_FAIL;

	if (FAILED(Load_Map(LEVEL_GAMEPLAY, TEXT("NormalMapData.txt"))))
		return E_FAIL;

	//if (FAILED(Ready_Layer_Trigger(TEXT("Layer_Trigger"))))
	//	return E_FAIL;

	ShowCursor(FALSE);


	return S_OK;
}

void CLevel_GamePlay::Update(_float fTimeDelta)
{
	CFXMgr::Get_Instance()->SpawnMultipleExplosions(fTimeDelta, LEVEL_GAMEPLAY);
	CFXMgr::Get_Instance()->SpawnMultipleExplosions2(fTimeDelta, LEVEL_GAMEPLAY);
	CFXMgr::Get_Instance()->SpawnMultipleExplosions3(fTimeDelta, LEVEL_GAMEPLAY);

	//방목용
	if (KEY_DOWN(DIK_M))
	{
		SpawnTtakkeun_i(_float3{ 1400.f, 100.f, 1500.f }, true); 
		//SpawnWenteko(_float3{ 900.f, 40.f, 600.f }, true);
		//SpawnShotgunner(_float3{ 900.f, 20.f, 600.f }, true);
		//SpawnNukemutant(_float3{ 900.f, 30.f, 600.f }, true);
		//SpawnMechsect(_float3{ 900.f, 10.f, 600.f }, true);
		//SpawnGreater(_float3{ 900.f, 20.f, 600.f }, true);
		//SpawnDeacon(_float3{ 1400.f, 80.f, 1500.f }, true);
		//SpawnArchangel(_float3{ 900.f, 80.f, 600.f }, true);
	}
	if (KEY_DOWN(DIK_Z))
	{
		SpawnCultist(_float3{ 1200.f, 50.f, 1500.f }, false);
		SpawnShotgunner(_float3{ 1250.f, 50.f, 1500.f }, false);
		SpawnNukemutant(_float3{ 1300.f, 50.f, 1500.f }, false);
		SpawnGreater(_float3{ 1350.f, 50.f, 1500.f }, false);
		SpawnArchangel(_float3{ 1400.f, 50.f, 1500.f }, false);
		SpawnDeacon(_float3{ 1450.f, 50.f, 1500.f }, false);
		SpawnMechsect(_float3{ 1500.f, 80.f, 1500.f }, false);
	}
		//SpawnWenteko(_float3{ 1400.f, 80.f, 1500.f }, true);

	Check_Collision();

	if (KEY_DOWN(DIK_F11))
	{
		if (FAILED(m_pGameInstance->Change_Level(LEVEL_LOADING,
			CLevel_Loading::Create(m_pGraphic_Device, LEVEL_RACE))))
			return;
	}
	//디버깅 모드
	if (KEY_DOWN(DIK_F4))
	{
		auto MosterList = m_pGameInstance->Find_Objects(LEVEL_GAMEPLAY , L"Layer_Monster");
		for (auto Monster : MosterList)
		{
			static_cast<CMonster*>(Monster)->ToggleDebugMode();
		}
		auto BossList = m_pGameInstance->Find_Objects(LEVEL_GAMEPLAY, L"Layer_Boss");
		for (auto Boss : BossList)
		{
			static_cast<CMonster*>(Boss)->ToggleDebugMode();
		}
	}
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


  	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Sky"),
		LEVEL_GAMEPLAY, strLayerTag)))
		return E_FAIL;


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
	//플렛폼 생성
	CFlatform::DESC FlatformDESC;
	FlatformDESC.vInitPos = _float3{ 800.f, 0.f, -200.f };
	FlatformDESC.vScale = _float3{ 500.f, 300.f, 1.f };
	FlatformDESC.bLoop = true;
	FlatformDESC.fMaxFrame = 1.f;
	FlatformDESC.fRotationPerSec = RADIAN(180.f);
	FlatformDESC.fSpeedPerSec = 100.f;
	FlatformDESC.szTextureTag = TEXT("MonsterFlatform");
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Flatform"),
		LEVEL_GAMEPLAY, strLayerTag, &FlatformDESC)))
		return E_FAIL;

	//빗방울
	CFXMgr::Get_Instance()->SpawnRain(LEVEL_GAMEPLAY);
	
	//소용돌이
	CFXMgr::Get_Instance()->SpawnSpher(_float3{ 800.f, 0.f, -200.f }, LEVEL_GAMEPLAY);
	
	//토네이도(불기둥)
	CFXMgr::Get_Instance()->SpawnTornado(_float3{ 630.f, 25.f, -320.f }, LEVEL_GAMEPLAY);
	CFXMgr::Get_Instance()->SpawnTornado(_float3{ 760.f, 25.f, -100.f }, LEVEL_GAMEPLAY);
	CFXMgr::Get_Instance()->SpawnTornado(_float3{ 850.f, 25.f, -230.f }, LEVEL_GAMEPLAY);
	
	//불지르기
	CFXMgr::Get_Instance()->SpawnFire(_float3{ 800.f, 0.f, -200.f }, LEVEL_GAMEPLAY);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Effect(const _wstring& strLayerTag)
{
	return S_OK;
}

#include "Player.h"

HRESULT CLevel_GamePlay::Ready_Layer_Pawn(const _wstring& strLayerTag)
{
	CPlayer::DESC PlayerDesc{};
	PlayerDesc.vInitPos = _float3{ 1400.f, 150.f, 200.f };
	PlayerDesc.vScale = { 20.f, 30.f, 20.f };
	PlayerDesc.fRotationPerSec = RADIAN(180.f);
	PlayerDesc.fSpeedPerSec = 150.f;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Player"),
		LEVEL_GAMEPLAY, strLayerTag, &PlayerDesc)))
		return E_FAIL;

	return S_OK;
}

#include "Ttakkeun_i.h"
#include <UI_Manager.h>

HRESULT CLevel_GamePlay::Ready_Layer_Monster(const _wstring& strLayerTag)
{
	//플렛폼 생성
	CFlatform::DESC FlatformDESC;
	FlatformDESC.vInitPos = _float3{ 250.f, 0.f, -200.f };
	FlatformDESC.vScale = _float3{ 500.f, 300.f, 1.f };
	FlatformDESC.bLoop = true;
	FlatformDESC.fMaxFrame = 1.f;
	FlatformDESC.fRotationPerSec = RADIAN(180.f);
	FlatformDESC.fSpeedPerSec = 100.f;
	FlatformDESC.szTextureTag = TEXT("MonsterFlatform");
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Flatform"),
		LEVEL_GAMEPLAY, TEXT("Layer_Flatform"), &FlatformDESC)))
		return E_FAIL;	

	//전시용
	//SpawnTtakkeun_i(_float3{ 100.f, 66.f, -250.f }, false);
	//SpawnWenteko(_float3{ 100.f, 40.f, -100.f }, false);
	SpawnShotgunner(_float3{ 180.f, 20.f, -120.f }, false);
	SpawnNukemutant(_float3{ 250.f, 30.f, -120.f }, false);
	SpawnMechsect(_float3{ 330.f, 10.f, -120.f }, false);
	SpawnGreater(_float3{ 410.f, 20.f, -120.f }, false);
	SpawnDeacon(_float3{ 220.f, 50.f, -250.f }, false);
	SpawnCultist(_float3{ 320.f, 30.f, -250.f }, false);
	SpawnArchangel(_float3{ 420.f, 30.f, -250.f }, false);

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

HRESULT CLevel_GamePlay::Ready_Layer_Trigger(const _wstring& strLayerTag)
{
	CTrigger::DESC tDesc = {};

	tDesc.fRotationPerSec = 0;
	tDesc.fSpeedPerSec = 0;
	tDesc.LayerTag = TEXT("Layer_BossBridge");
	tDesc.vAngle = { 0.f, 0.f, 0.f };
	tDesc.vInitPos = {1400.f, 70.f, 950.f};
	tDesc.vScale = {200.f, 100.f, 100.f};
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Trigger"),
		LEVEL_GAMEPLAY, strLayerTag, &tDesc)))
		return E_FAIL;

	return S_OK;
}

void CLevel_GamePlay::Check_Collision()
{
	m_pGameInstance->Intersect(CG_PAWN, CG_BLOCK);
	//m_pGameInstance->Intersect(CG_PBULLET, CG_MONSTER);
	//m_pGameInstance->Intersect(CG_PBULLET, CG_BLOCK);
	//m_pGameInstance->Intersect(CG_MBULLET, CG_BLOCK);
	//m_pGameInstance->Intersect(CG_PAWN, CG_TRIGGER);
}

void CLevel_GamePlay::SpawnTtakkeun_i(_float3 _Position, bool m_bActive)
{
	CMonster::DESC Ttakkeun_iDesc2{};
	Ttakkeun_iDesc2.vPosition = _Position;
	Ttakkeun_iDesc2.fSpeedPerSec = 60.f;
	Ttakkeun_iDesc2.fRotationPerSec = RADIAN(180.f);
	Ttakkeun_iDesc2.vActive = m_bActive;
	Ttakkeun_iDesc2.vReturnPos = _Position;
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Ttakkeun_i"),
		LEVEL_GAMEPLAY, L"Layer_Boss", &Ttakkeun_iDesc2)))
		return;
}
void CLevel_GamePlay::SpawnWenteko(_float3 _Position, bool m_bActive)
{
	//웬테코
	CMonster::DESC Wenteko_iDesc{};
	Wenteko_iDesc.vPosition = _Position;
	Wenteko_iDesc.fSpeedPerSec = 60.f;
	Wenteko_iDesc.fRotationPerSec = RADIAN(180.f);
	Wenteko_iDesc.vActive = m_bActive;
	Wenteko_iDesc.vReturnPos = _Position;
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Wenteko"),
		LEVEL_GAMEPLAY, L"Layer_Monster", &Wenteko_iDesc)))
		return;
}
void CLevel_GamePlay::SpawnShotgunner(_float3 _Position, bool m_bActive)
{
	//샷거너
	CMonster::DESC Shotgunner_iDesc{};
	Shotgunner_iDesc.vPosition = _Position;
	Shotgunner_iDesc.fSpeedPerSec = 60.f;
	Shotgunner_iDesc.fRotationPerSec = RADIAN(180.f);
	Shotgunner_iDesc.vActive = m_bActive;
	Shotgunner_iDesc.vReturnPos = _Position;
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Shotgunner"),
		LEVEL_GAMEPLAY, L"Layer_Monster", &Shotgunner_iDesc)))
		return;
}
void CLevel_GamePlay::SpawnNukemutant(_float3 _Position, bool m_bActive)
{
	CMonster::DESC Nukemutant_iDesc{};
	Nukemutant_iDesc.vPosition = _Position;
	Nukemutant_iDesc.fSpeedPerSec = 60.f;
	Nukemutant_iDesc.fRotationPerSec = RADIAN(180.f);
	Nukemutant_iDesc.vActive = m_bActive;
	Nukemutant_iDesc.vReturnPos = _Position;
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Nukemutant"),
		LEVEL_GAMEPLAY, L"Layer_Monster", &Nukemutant_iDesc)))
		return;
}
void CLevel_GamePlay::SpawnMechsect(_float3 _Position, bool m_bActive)
{
	CMonster::DESC Mechsect_iDesc{};
	Mechsect_iDesc.vPosition = _Position;
	Mechsect_iDesc.fSpeedPerSec = 60.f;
	Mechsect_iDesc.fRotationPerSec = RADIAN(180.f);
	Mechsect_iDesc.vActive = m_bActive;
	Mechsect_iDesc.vReturnPos = _Position;
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Mechsect"),
		LEVEL_GAMEPLAY, L"Layer_Monster", &Mechsect_iDesc)))
		return;
}
void CLevel_GamePlay::SpawnGreater(_float3 _Position, bool m_bActive)
{
	CMonster::DESC Greater_iDesc{};
	Greater_iDesc.vPosition = _Position;
	Greater_iDesc.fSpeedPerSec = 60.f;
	Greater_iDesc.fRotationPerSec = RADIAN(180.f);
	Greater_iDesc.vActive = m_bActive;
	Greater_iDesc.vReturnPos = _Position;
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Greater"),
		LEVEL_GAMEPLAY, L"Layer_Monster", &Greater_iDesc)))
		return;
}
void CLevel_GamePlay::SpawnDeacon(_float3 _Position, bool m_bActive)
{
	CMonster::DESC Deacon_iDesc{};
	Deacon_iDesc.vPosition = _Position;
	Deacon_iDesc.fSpeedPerSec = 60.f;
	Deacon_iDesc.fRotationPerSec = RADIAN(180.f);
	Deacon_iDesc.vActive = m_bActive;
	Deacon_iDesc.vReturnPos = _Position;
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Deacon"),
		LEVEL_GAMEPLAY, L"Layer_Monster", &Deacon_iDesc)))
		return ;
}
void CLevel_GamePlay::SpawnCultist(_float3 _Position, bool m_bActive)
{
	CMonster::DESC Cultist_iDesc{};
	Cultist_iDesc.vPosition = _Position;
	Cultist_iDesc.fSpeedPerSec = 60.f;
	Cultist_iDesc.fRotationPerSec = RADIAN(180.f);
	Cultist_iDesc.vActive = m_bActive;
	Cultist_iDesc.vReturnPos = _Position;
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Cultist"),
		LEVEL_GAMEPLAY, L"Layer_Monster", &Cultist_iDesc)))
		return;
}
void CLevel_GamePlay::SpawnArchangel(_float3 _Position, bool m_bActive)
{
	CMonster::DESC Archangel_iDesc{};
	Archangel_iDesc.vPosition = _Position;
	Archangel_iDesc.fSpeedPerSec = 60.f;
	Archangel_iDesc.fRotationPerSec = RADIAN(180.f);
	Archangel_iDesc.vActive = m_bActive;
	Archangel_iDesc.vReturnPos = _Position;
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Archangel"),
		LEVEL_GAMEPLAY, L"Layer_Monster", &Archangel_iDesc)))
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
	/* 텍스쿠드 변경해서 적용시켜 줄 때 각 오브젝트를 갖고오는 변수 ( 점점추가될 예정 )*/
	/* 배열로 선언할까 싶기도 했는데, 레벨마다 쓸 녀석과 안 쓸 녀석이 나뉘어질 거같기때문에,, */
	_int iNumTile{}, iNumBlock{}, iNumTriPil{}, iNumAniRect{}, iNumAniBlock{},
		iNumInviBlock{}, iNumLava{}, iNumAlphaRect{}, iNumAlphaBlock{}, iNumBossBridge{};
	/* 불러오기용 변수 */
	_int iNumVertexX = {}, iNumVertexZ = {}, iLoadLength = {};
	_uint iNumBackGround = {}, iNumModel = {};
	_float fSpeedPerSec = {}, fRotationPerSec = {}, fTextureIdx = {};
	_tchar szPrototypeTag[MAX_PATH] = {};;
	_bool  bCollision = {};
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
			bResult = ReadFile(hFile, &bCollision, sizeof(_bool), &dwByte, NULL);

			CMap::DESC tDesc = {};
			tDesc.fSpeedPerSec = fSpeedPerSec;
			tDesc.fRotationPerSec = fRotationPerSec;
			tDesc.vInitPos = vPosition;
			tDesc.vScale = vScale;
			tDesc.vAngle = vAngle;
			tDesc.fTextureIdx = fTextureIdx;
			tDesc.bCollision = bCollision;

			_wstring strKey = szPrototypeTag;

			_wstring Prototype = strKey;

			strKey = Compute_PrototypeName(strKey);

			_wstring Layertag = TEXT("Layer_") + strKey;
			

   			CGravity::Add_StandableObjLayerTag(CG_BLOCK);

			if (FAILED(m_pGameInstance->Add_GameObject(iLevelIdx, Prototype, iLevelIdx, Layertag, &tDesc)))
			{
				MSG_BOX("객체 생성 실패");
				return E_FAIL;
			}

			// 큐브인지 렉트인지 분기 필요함
			if (Prototype == TEXT("Prototype_GameObject_BackGround"))
			{
				CGameObject* pGameObject = m_pGameInstance->Find_Object(iLevelIdx, Layertag, iNumTile++);
				if (nullptr != pGameObject)
				{
					if (FAILED(__super::Load_VertexBuffer(pGameObject, hFile, &dwByte)))
					{
						MSG_BOX("버텍스 버퍼 로딩실패");
						return E_FAIL;
					}
				}

			}
			else if (Prototype == TEXT("Prototype_GameObject_Block"))
			{
				CGameObject* pGameObject = m_pGameInstance->Find_Object(iLevelIdx, Layertag, iNumBlock++);
				if (nullptr != pGameObject)
				{
					if (FAILED(__super::Load_VertexBuffer(pGameObject, hFile, &dwByte)))
					{
						MSG_BOX("버텍스 버퍼 로딩실패");
						return E_FAIL;
					}
				}
			}
			else if (Prototype == TEXT("Prototype_GameObject_TriangularPillar"))
			{
				CGameObject* pGameObject = m_pGameInstance->Find_Object(iLevelIdx, Layertag, iNumTriPil++);
				if (nullptr != pGameObject)
				{
					if (FAILED(__super::Load_VertexBuffer(pGameObject, hFile, &dwByte)))
					{
						MSG_BOX("버텍스 버퍼 로딩실패");
						return E_FAIL;
					}
				}
			}
			else if (Prototype == TEXT("Prototype_GameObject_AnimeRect"))
			{
				CGameObject* pGameObject = m_pGameInstance->Find_Object(iLevelIdx, Layertag, iNumAniRect++);
				if (nullptr != pGameObject)
				{
					if (FAILED(__super::Load_VertexBuffer(pGameObject, hFile, &dwByte)))
					{
						MSG_BOX("버텍스 버퍼 로딩실패");
						return E_FAIL;
					}
				}
			}
			else if (Prototype == TEXT("Prototpye_GameObject_AnimeBlock"))
			{
				CGameObject* pGameObject = m_pGameInstance->Find_Object(iLevelIdx, Layertag, iNumAniBlock++);
				if (nullptr != pGameObject)
				{
					if (FAILED(__super::Load_VertexBuffer(pGameObject, hFile, &dwByte)))
					{
						MSG_BOX("버텍스 버퍼 로딩실패");
						return E_FAIL;
					}
				}
			}
			else if (Prototype == TEXT("Prototype_GameObject_InvisibleBlock"))
			{
				CGameObject* pGameObject = m_pGameInstance->Find_Object(iLevelIdx, Layertag, iNumInviBlock++);
				if (nullptr != pGameObject)
				{
					if (FAILED(__super::Load_VertexBuffer(pGameObject, hFile, &dwByte)))
					{
						MSG_BOX("버텍스 버퍼 로딩실패");
						return E_FAIL;
					}
				}
			}
			else if (Prototype == TEXT("Prototype_GameObject_Lava"))
			{
				CGameObject* pGameObject = m_pGameInstance->Find_Object(iLevelIdx, Layertag, iNumLava++);
				if (nullptr != pGameObject)
				{
					if (FAILED(__super::Load_VertexBuffer(pGameObject, hFile, &dwByte)))
					{
						MSG_BOX("버텍스 버퍼 로딩실패");
						return E_FAIL;
					}
				}
			}
			else if (Prototype == TEXT("Prototype_GameObject_AlphaRect"))
			{
				CGameObject* pGameObject = m_pGameInstance->Find_Object(iLevelIdx, Layertag, iNumAlphaRect++);
				if (nullptr != pGameObject)
				{
					if (FAILED(__super::Load_VertexBuffer(pGameObject, hFile, &dwByte)))
					{
						MSG_BOX("버텍스 버퍼 로딩실패");
						return E_FAIL;
					}
				}
			}
			else if (Prototype == TEXT("Prototype_GameObject_AlphaBlock"))
			{
				CGameObject* pGameObject = m_pGameInstance->Find_Object(iLevelIdx, Layertag, iNumAlphaBlock++);
				if (nullptr != pGameObject)
				{
					if (FAILED(__super::Load_VertexBuffer(pGameObject, hFile, &dwByte)))
					{
						MSG_BOX("버텍스 버퍼 로딩실패");
						return E_FAIL;
					}
				}
			}
			else if (Prototype == TEXT("Prototype_GameObject_BossBridge"))
			{
				CGameObject* pGameObject = m_pGameInstance->Find_Object(iLevelIdx, Layertag, iNumBossBridge++);
				if (nullptr != pGameObject)
				{
					if (FAILED(__super::Load_VertexBuffer(pGameObject, hFile, &dwByte)))
					{
						MSG_BOX("버텍스 버퍼 로딩실패");
						return E_FAIL;
					}
				}
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
			bResult = ReadFile(hFile, &bCollision, sizeof(_bool), &dwByte, NULL);

			CStatue::DESC tDesc = {};
			tDesc.fSpeedPerSec = fSpeedPerSec;
			tDesc.fRotationPerSec = fRotationPerSec;
			tDesc.vInitPos = vPosition;
			tDesc.vScale = vScale;
			tDesc.vAngle = vAngle;
			tDesc.bCollision = bCollision;

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