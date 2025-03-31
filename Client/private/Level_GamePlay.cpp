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

	//if (FAILED(m_pGameInstance->LoadBank("Test_Bank")))
	//	return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Terrain(TEXT("Layer_Terrain"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;

	CUI_Manager::Get_Instance()->Initialize_GamePlayUI(LEVEL_GAMEPLAY);

	if (FAILED(Ready_Layer_Pawn(TEXT("Layer_Pawn"))))
		return E_FAIL;

	CUI_Manager::Get_Instance()->Initialize_Player(/*CurLevel */ );

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

	if (FAILED(Load_Map(LEVEL_GAMEPLAY, TEXT("MapData.txt"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Trigger(TEXT("Layer_Trigger"))))
		return E_FAIL;

	ShowCursor(FALSE);


	return S_OK;
}

void CLevel_GamePlay::Update(_float fTimeDelta)
{
	auto FX_Manager = FX_MGR;
	FX_Manager->SpawnMultipleExplosions(fTimeDelta, LEVEL_GAMEPLAY);
	FX_Manager->SpawnMultipleExplosions2(fTimeDelta, LEVEL_GAMEPLAY);
	FX_Manager->SpawnMultipleExplosions3(fTimeDelta, LEVEL_GAMEPLAY);

	Check_Collision();

	if (KEY_DOWN(DIK_F11))
	{
		if (FAILED(m_pGameInstance->Change_Level(LEVEL_LOADING,
			CLevel_Loading::Create(m_pGraphic_Device, LEVEL_RACEFIRST))))
			return;
	}

	if (KEY_DOWN(DIK_F4))
	{
		auto MosterList = m_pGameInstance->Find_Objects(LEVEL_GAMEPLAY, L"Layer_Monster");
		if (MosterList)
		{
			for (auto Monster : *MosterList)
			{
				static_cast<CMonster*>(Monster)->ToggleDebugMode();
			}
		}
		auto BossList = m_pGameInstance->Find_Objects(LEVEL_GAMEPLAY, L"Layer_Boss");
		if (BossList)
		{
			for (auto Boss : *BossList)
			{
				static_cast<CMonster*>(Boss)->ToggleDebugMode();
			}
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
	CMap::DESC MapDesc{};
	MapDesc.eLevelID = LEVEL_GAMEPLAY;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Terrain"),
		LEVEL_GAMEPLAY, strLayerTag, &MapDesc)))
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
	auto Camera_Manager = CAMERA_MANAGER;
	Camera_Manager->Switch(CCameraManager::FPS);
	Camera_Manager->Mouse_Fix_Mode_Switch();

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

	/*if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Canopy"),
		LEVEL_GAMEPLAY, strLayerTag, m_pData->Find_Data(TEXT("Canopy")))))
		return E_FAIL;*/

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

	auto FX_Manager = FX_MGR;

	//빗방울
	CFXMgr::Get_Instance()->SpawnRain(LEVEL_GAMEPLAY);
	//불지르기
	FX_Manager->SpawnFire(_float3{ 800.f, 0.f, -200.f }, LEVEL_GAMEPLAY);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Effect(const _wstring& strLayerTag)
{
	return S_OK;
}

#include "Player.h"
HRESULT CLevel_GamePlay::Ready_Layer_Pawn(const _wstring& strLayerTag)
{
	//이 레벨의 플레이어 생성위치
	_float3 vInitPosition = { 1400.f, 150.f, 200.f };

	// 플레이어가 있는지 체크하고 있으면 위치만 변경해줌.
	auto pPlayer = GET_PLAYER;
	if (pPlayer)
	{
		static_cast<CTransform*>(pPlayer->Find_Component(TEXT("Com_Transform")))
			->Set_State(CTransform::STATE_POSITION, vInitPosition);
		static_cast<CPawn*>(pPlayer)->Set_LevelID(CurLevel);
		return S_OK;
	}

	//없으면 새로 생성해서 넣어줌
	CPlayer::DESC PlayerDesc{};
	PlayerDesc.vInitPos = vInitPosition;
	PlayerDesc.vScale = { 20.f, 30.f, 20.f };
	PlayerDesc.fRotationPerSec = RADIAN(180.f);
	PlayerDesc.fSpeedPerSec = 150.f;
	PlayerDesc.eLevelID = LEVEL_STATIC;

	// 최초 게임 입장할때 어디에서 입장하던 스태틱에 생성해준다.
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Player"),
		LEVEL_STATIC, strLayerTag, &PlayerDesc)))
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

	//따끈이
	SpawnTtakkeun_i(_float3{ 1200.f, 100.f, 1500.f }, true, 0, LEVEL_GAMEPLAY);
	SpawnTtakkeun_i(_float3{ 1600.f, 100.f, 1500.f }, true, 1, LEVEL_GAMEPLAY);

	//전시용 (게임플레이 이니셜)
	//SpawnWenteko(_float3{ 100.f, 40.f, -100.f }, false, LEVEL_GAMEPLAY);
	SpawnShotgunner(_float3{ 180.f, 20.f, -120.f }, false, LEVEL_GAMEPLAY);
	SpawnNukemutant(_float3{ 250.f, 30.f, -120.f }, false, LEVEL_GAMEPLAY);
	SpawnMechsect(_float3{ 330.f, 10.f, -120.f }, false, LEVEL_GAMEPLAY);
	SpawnGreater(_float3{ 410.f, 20.f, -120.f }, false, LEVEL_GAMEPLAY);
	SpawnDeacon(_float3{ 220.f, 50.f, -250.f }, false, LEVEL_GAMEPLAY);
	SpawnCultist(_float3{ 320.f, 30.f, -250.f }, false, LEVEL_GAMEPLAY);
	SpawnArchangel(_float3{ 420.f, 30.f, -250.f }, false, LEVEL_GAMEPLAY);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_UI(const _wstring& strLayerTag)
{
	CUI::DESC Desc{};
	Desc.eLevelID = LEVEL_GAMEPLAY;
	Desc.fDepth = 3.f;
	Desc.vScale = _float3(1.f, 1.f, 1.f);
	Desc.vInitPos = _float3(0.f, 0.f, 0.1f);
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Aim"),
		Desc.eLevelID, strLayerTag, &Desc)))
		return E_FAIL;

	Desc.vScale = _float3(80.f, 80.f, 1.f); 
	Desc.vInitPos = _float3(-(g_iWinSizeX / 2.f) + Desc.vScale.x / 2.f - 10.f, -(g_iWinSizeY / 2.f) + Desc.vScale.y / 2.f, 0.1f);
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Portrait"),
		Desc.eLevelID, strLayerTag, &Desc)))
		return E_FAIL;

	Desc.vScale = _float3(75.f, 75.f, 1.f);
	Desc.vInitPos = _float3(-(g_iWinSizeX / 2.f) + 208.f, -(g_iWinSizeY / 2.f) + Desc.vScale.y / 2.f, 0.1f);
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Armor"),
		Desc.eLevelID, strLayerTag, &Desc)))
		return E_FAIL;

	Desc.vScale = _float3(48.f, 54.f, 1.f);
	Desc.vInitPos = _float3((g_iWinSizeX / 2.f) - 40.f, -(g_iWinSizeY / 2.f) + Desc.vScale.y / 2.f + 7.f, 0.1f);
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Ammo"),
		Desc.eLevelID, strLayerTag, &Desc)))
		return E_FAIL;


	/* ui생성 순서 중요, player 생성 이후 호출 중요  */
	// 과거의 나야 미안해 
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
	m_pGameInstance->Intersect(CG_PBULLET, CG_MONSTER);
	m_pGameInstance->Intersect(CG_PBULLET, CG_BLOCK);
	m_pGameInstance->Intersect(CG_MBULLET, CG_BLOCK);
	m_pGameInstance->Intersect(CG_PAWN, CG_TRIGGER);
	m_pGameInstance->Intersect(CG_MONSTER, CG_BLOCK);
}

void CLevel_GamePlay::SpawnTtakkeun_i(const _float3& _Position, _bool m_bActive, _int _iNum, LEVEL _eLevel)
{
	CMonster::DESC Ttakkeun_iDesc2{};
	Ttakkeun_iDesc2.vPosition = _Position;
	Ttakkeun_iDesc2.fSpeedPerSec = 60.f;
	Ttakkeun_iDesc2.fRotationPerSec = RADIAN(180.f);
	Ttakkeun_iDesc2.vActive = m_bActive;
	Ttakkeun_iDesc2.vReturnPos = _Position;
	Ttakkeun_iDesc2.iNums = _iNum;
	Ttakkeun_iDesc2.eLevel = _eLevel;
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Ttakkeun_i"),
		_eLevel, L"Layer_Boss", &Ttakkeun_iDesc2)))
		return;
}

void CLevel_GamePlay::SpawnWenteko(_float3 _Position, bool m_bActive, LEVEL _eLevel)
{
	//웬테코
	CMonster::DESC Wenteko_iDesc{};
	Wenteko_iDesc.vPosition = _Position;
	Wenteko_iDesc.fSpeedPerSec = 60.f;
	Wenteko_iDesc.fRotationPerSec = RADIAN(180.f);
	Wenteko_iDesc.vActive = m_bActive;
	Wenteko_iDesc.vReturnPos = _Position;
	Wenteko_iDesc.eLevel = _eLevel;
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Wenteko"),
		_eLevel, L"Layer_Monster", &Wenteko_iDesc)))
		return;
}
void CLevel_GamePlay::SpawnShotgunner(_float3 _Position, bool m_bActive, LEVEL _eLevel)
{
	//샷거너
	CMonster::DESC Shotgunner_iDesc{};
	Shotgunner_iDesc.vPosition = _Position;
	Shotgunner_iDesc.fSpeedPerSec = 60.f;
	Shotgunner_iDesc.fRotationPerSec = RADIAN(180.f);
	Shotgunner_iDesc.vActive = m_bActive;
	Shotgunner_iDesc.vReturnPos = _Position;
	Shotgunner_iDesc.eLevel = _eLevel;
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Shotgunner"),
		_eLevel, L"Layer_Monster", &Shotgunner_iDesc)))
		return;
}
void CLevel_GamePlay::SpawnNukemutant(_float3 _Position, bool m_bActive, LEVEL _eLevel)
{
	CMonster::DESC Nukemutant_iDesc{};
	Nukemutant_iDesc.vPosition = _Position;
	Nukemutant_iDesc.fSpeedPerSec = 60.f;
	Nukemutant_iDesc.fRotationPerSec = RADIAN(180.f);
	Nukemutant_iDesc.vActive = m_bActive;
	Nukemutant_iDesc.vReturnPos = _Position;
	Nukemutant_iDesc.eLevel = _eLevel;
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Nukemutant"),
		_eLevel, L"Layer_Monster", &Nukemutant_iDesc)))
		return;
}
void CLevel_GamePlay::SpawnMechsect(_float3 _Position, bool m_bActive, LEVEL _eLevel)
{
	CMonster::DESC Mechsect_iDesc{};
	Mechsect_iDesc.vPosition = _Position;
	Mechsect_iDesc.fSpeedPerSec = 60.f;
	Mechsect_iDesc.fRotationPerSec = RADIAN(180.f);
	Mechsect_iDesc.vActive = m_bActive;
	Mechsect_iDesc.vReturnPos = _Position;
	Mechsect_iDesc.eLevel = _eLevel;
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Mechsect"),
		_eLevel, L"Layer_Monster", &Mechsect_iDesc)))
		return;
}
void CLevel_GamePlay::SpawnGreater(_float3 _Position, bool m_bActive, LEVEL _eLevel)
{
	CMonster::DESC Greater_iDesc{};
	Greater_iDesc.vPosition = _Position;
	Greater_iDesc.fSpeedPerSec = 60.f;
	Greater_iDesc.fRotationPerSec = RADIAN(180.f);
	Greater_iDesc.vActive = m_bActive;
	Greater_iDesc.vReturnPos = _Position;
	Greater_iDesc.eLevel = _eLevel;
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Greater"),
		_eLevel, L"Layer_Monster", &Greater_iDesc)))
		return;
}
void CLevel_GamePlay::SpawnDeacon(_float3 _Position, bool m_bActive, LEVEL _eLevel)
{
	CMonster::DESC Deacon_iDesc{};
	Deacon_iDesc.vPosition = _Position;
	Deacon_iDesc.fSpeedPerSec = 60.f;
	Deacon_iDesc.fRotationPerSec = RADIAN(180.f);
	Deacon_iDesc.vActive = m_bActive;
	Deacon_iDesc.vReturnPos = _Position;
	Deacon_iDesc.eLevel = _eLevel;
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Deacon"),
		_eLevel, L"Layer_Monster", &Deacon_iDesc)))
		return;
}
void CLevel_GamePlay::SpawnCultist(_float3 _Position, bool m_bActive, LEVEL _eLevel)
{
	CMonster::DESC Cultist_iDesc{};
	Cultist_iDesc.vPosition = _Position;
	Cultist_iDesc.fSpeedPerSec = 60.f;
	Cultist_iDesc.fRotationPerSec = RADIAN(180.f);
	Cultist_iDesc.vActive = m_bActive;
	Cultist_iDesc.vReturnPos = _Position;
	Cultist_iDesc.eLevel = _eLevel;
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Cultist"),
		_eLevel, L"Layer_Monster", &Cultist_iDesc)))
		return;
}
void CLevel_GamePlay::SpawnArchangel(_float3 _Position, bool m_bActive, LEVEL _eLevel)
{
	CMonster::DESC Archangel_iDesc{};
	Archangel_iDesc.vPosition = _Position;
	Archangel_iDesc.fSpeedPerSec = 60.f;
	Archangel_iDesc.fRotationPerSec = RADIAN(180.f);
	Archangel_iDesc.vActive = m_bActive;
	Archangel_iDesc.vReturnPos = _Position;
	Archangel_iDesc.eLevel = _eLevel;
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Archangel"),
		_eLevel, L"Layer_Monster", &Archangel_iDesc)))
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
			tDesc.vInitPos = vPosition * BOSSSCALE;
			tDesc.vScale = vScale * BOSSSCALE;
			tDesc.vAngle = vAngle;
			tDesc.fTextureIdx = fTextureIdx;
			tDesc.bCollision = bCollision;
			tDesc.eLevelID = static_cast<LEVEL>(iLevelIdx);

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
			tDesc.vInitPos = vPosition * BOSSSCALE;
			tDesc.vScale = vScale * BOSSSCALE;
			tDesc.vAngle = vAngle;
			tDesc.bCollision = bCollision;
			tDesc.eLevelID = static_cast<LEVEL>(iLevelIdx);

			_wstring strKey = szPrototypeTag;

			_wstring Prototype = strKey;

			strKey = Compute_PrototypeName(strKey);

			_wstring Layertag = TEXT("Layer_") + strKey;


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

	//CUI_Manager::Get_Instance()->Clear_GamePlayUI();

	while (m_iIndex >= 0)
	{
		m_pGraphic_Device->LightEnable(m_iIndex, FALSE);
		--m_iIndex;
	}
}