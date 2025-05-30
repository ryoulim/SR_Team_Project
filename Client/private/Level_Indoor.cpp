﻿#include "Level_Indoor.h"

#include "GameInstance.h"
#include "Map.h"
#include "Statue.h"
#include "Dynamic_Camera.h"
#include "Player.h"
#include "Trigger.h"
#include "Level_Loading.h"
#include "Monster.h"
#include "UI_Manager.h"
#include "Item.h"

#define CurLevel LEVEL_INDOOR

CLevel_Indoor::CLevel_Indoor(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CLevel{pGraphic_Device}
{
}

HRESULT CLevel_Indoor::Initialize(CLevelData* pLevelData)
{	
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Sky"),
		CurLevel, TEXT("Layer_Sky"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Pawn(TEXT("Layer_Pawn"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Trigger(TEXT("Layer_Trigger"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
		return E_FAIL;

	if (FAILED(Load_Map(LEVEL_INDOOR, TEXT("InDoorMapData.txt"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Item(TEXT("Layer_Item"))))
		return E_FAIL;

	m_pBGM = m_pGameInstance->Get_Single_Sound("lemansion");
	m_pBGM->Set_Volume(0.5f);
	m_pBGM->Play();

	return S_OK;
}

void CLevel_Indoor::Update(_float fTimeDelta)
{
	FX_MGR->Update(fTimeDelta);
	Check_Collision();

	if (m_iNextLevel)
	{
		m_pGameInstance->Change_Level(LEVEL_LOADING,
			CLevel_Loading::Create(m_pGraphic_Device, (LEVEL)m_iNextLevel));
	}
}

HRESULT CLevel_Indoor::Render()
{
	SetWindowText(g_hWnd, TEXT("실내 레벨입니다."));
	return S_OK;
}

HRESULT CLevel_Indoor::Load_Map(_uint iLevelIdx, const _wstring& FileName)
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
		iNumInviBlock{}, iNumAlphaRect{}, iNumAlphaBlock{}, iNumDoor{}, iNumDoorSecurity{};
	/* 불러오기용 변수 */
	_int iNumVertexX = {}, iNumVertexZ = {}, iLoadLength = {};
	_uint iNumBackGround = {}, iNumModel = {}, iNumItem{}, iItemID{};
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
			tDesc.vInitPos = vPosition * INDOORSCALE;
			tDesc.vScale = vScale * INDOORSCALE;
			tDesc.vAngle = vAngle;
			tDesc.fTextureIdx = fTextureIdx;
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
			else if (Prototype == TEXT("Prototype_GameObject_Door"))
			{
				CGameObject* pGameObject = m_pGameInstance->Find_Object(iLevelIdx, Layertag, iNumDoor++);
				if (nullptr != pGameObject)
				{
					if (FAILED(__super::Load_VertexBuffer(pGameObject, hFile, &dwByte)))
					{
						MSG_BOX("버텍스 버퍼 로딩실패");
						return E_FAIL;
					}
				}
			}
			else if (Prototype == TEXT("Prototype_GameObject_DoorSecurity"))
			{
				CGameObject* pGameObject = m_pGameInstance->Find_Object(iLevelIdx, Layertag, iNumDoorSecurity++);
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
			tDesc.vInitPos = vPosition * INDOORSCALE;
			tDesc.vScale = vScale * INDOORSCALE;
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

		bResult = ReadFile(hFile, &iNumItem, sizeof(_uint), &dwByte, NULL);

		for (_uint i = 0; i < iNumItem; i++)
		{
			bResult = ReadFile(hFile, &fSpeedPerSec, sizeof(_float), &dwByte, NULL);
			bResult = ReadFile(hFile, &fRotationPerSec, sizeof(_float), &dwByte, NULL);
			bResult = ReadFile(hFile, &vPosition, sizeof(_float3), &dwByte, NULL);
			bResult = ReadFile(hFile, &vScale, sizeof(_float3), &dwByte, NULL);
			bResult = ReadFile(hFile, &vAngle, sizeof(_float3), &dwByte, NULL);
			bResult = ReadFile(hFile, &iLoadLength, sizeof(_int), &dwByte, NULL);
			bResult = ReadFile(hFile, &szPrototypeTag, iLoadLength * sizeof(_tchar), &dwByte, NULL);
			bResult = ReadFile(hFile, &iItemID, sizeof(_uint), &dwByte, NULL);
			bResult = ReadFile(hFile, &fTextureIdx, sizeof(_float), &dwByte, NULL);

			CItem::DESC tDesc = {};
			tDesc.vInitPos = vPosition * INDOORSCALE;
			tDesc.vScale = vScale * INDOORITEMSCALE;
			tDesc.fRotationPerSec = fRotationPerSec;
			tDesc.fSpeedPerSec = fSpeedPerSec;
			tDesc.fTextureNum = fTextureIdx;
			tDesc.eLevelID = static_cast<LEVEL>(iLevelIdx);

			switch (iItemID)
			{
			case 0:
				tDesc.eColID = COLLIDER_ID::CI_ITEM_AMMO_CHAINGUN;
				tDesc.szTextureID = TEXT("Item_Ammo");
				break;
			case 1:
				tDesc.eColID = COLLIDER_ID::CI_ITEM_AMMO_DISPENSER_SCATTER;
				tDesc.szTextureID = TEXT("Item_Ammo");
				break;
			case 2:
				tDesc.eColID = COLLIDER_ID::CI_ITEM_AMMO_DISPENSER_CANNON;
				tDesc.szTextureID = TEXT("Item_Ammo");
				break;
			case 3:
				tDesc.eColID = COLLIDER_ID::CI_ITEM_AMMO_LOVERBOY;
				tDesc.szTextureID = TEXT("Item_Ammo");
				break;
			case 4:
				tDesc.eColID = COLLIDER_ID::CI_ITEM_ARMOR_PIECE;
				tDesc.szTextureID = TEXT("Item_Armor");
				break;
			case 5:
				tDesc.eColID = COLLIDER_ID::CI_ITEM_ARMOR_FULL;
				tDesc.szTextureID = TEXT("Item_Armor");
				break;
			case 6:
				tDesc.eColID = COLLIDER_ID::CI_ITEM_HEALKIT;
				tDesc.szTextureID = TEXT("Item_Healkit");
				break;
			case 7:
				tDesc.eColID = COLLIDER_ID::CI_ITEM_CARDKEY;
				tDesc.szTextureID = TEXT("Item_Cardkey");
				break;
			}

			_wstring strKey = szPrototypeTag;

			if (FAILED(m_pGameInstance->Add_GameObject(iLevelIdx, strKey, iLevelIdx, TEXT("Layer_Item"), &tDesc)))
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

HRESULT CLevel_Indoor::Ready_Layer_Trigger(const _wstring& strLayerTag)
{
	CTrigger::DESC tDesc = {};
	tDesc.fRotationPerSec = 0;
	tDesc.fSpeedPerSec = 0;
	tDesc.vAngle = { 0.f, 0.f, 0.f };
	tDesc.vInitPos = { 1760.f, 30.f, 1730.f };
	tDesc.vScale = { 200.f, 100.f, 50.f };
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Trigger"),
		LEVEL_INDOOR, strLayerTag, &tDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Indoor::Ready_Layer_UI(const _wstring& strLayerTag)
{
	CUI::DESC Desc{};
	Desc.eLevelID = LEVEL_INDOOR;
	Desc.fDepth = 3.f;
	//Desc.vScale = _float3(1.f, 1.f, 1.f);
	//Desc.vInitPos = _float3(0.f, 0.f, 0.1f);
	//if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Aim"),
	//	Desc.eLevelID, strLayerTag, &Desc)))
	//	return E_FAIL;

	//Desc.vScale = _float3(80.f, 80.f, 1.f);
	//Desc.vInitPos = _float3(-(g_iWinSizeX / 2.f) + Desc.vScale.x / 2.f - 10.f, -(g_iWinSizeY / 2.f) + Desc.vScale.y / 2.f, 0.1f);
	//if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Portrait"),
	//	Desc.eLevelID, strLayerTag, &Desc)))
	//	return E_FAIL;

	//Desc.vScale = _float3(75.f, 75.f, 1.f);
	//Desc.vInitPos = _float3(-(g_iWinSizeX / 2.f) + 208.f, -(g_iWinSizeY / 2.f) + Desc.vScale.y / 2.f, 0.1f);
	//if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Armor"),
	//	Desc.eLevelID, strLayerTag, &Desc)))
	//	return E_FAIL;

	//Desc.vScale = _float3(48.f, 54.f, 1.f);
	//Desc.vInitPos = _float3((g_iWinSizeX / 2.f) - 40.f, -(g_iWinSizeY / 2.f) + Desc.vScale.y / 2.f + 7.f, 0.1f);
	//if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Ammo"),
	//	Desc.eLevelID, strLayerTag, &Desc)))
	//	return E_FAIL;

	//CUI_Manager::Get_Instance()->Initialize_GamePlayUI(CurLevel);
	
	if (FAILED(m_pGameInstance->Add_GameObject(Desc.eLevelID, TEXT("Prototype_GameObject_InteractPromptUI"),
		Desc.eLevelID, strLayerTag, &Desc)))
		return E_FAIL;


	/* ui생성 순서 중요 */
	return S_OK;
}

HRESULT CLevel_Indoor::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	CAMERA_MANAGER->Switch(CCameraManager::FPS);
	CAMERA_MANAGER->Set_Mouse_Fix(TRUE);

	return S_OK;
}

HRESULT CLevel_Indoor::Ready_Layer_Pawn(const _wstring& strLayerTag)
{
	//이 레벨의 플레이어 생성위치
	_float3 vInitPosition = { 311.f, 50.f, 284.f };

	// 플레이어가 있는지 체크하고 있으면 위치만 변경해줌.
	auto pPlayer = GET_PLAYER;
	if (pPlayer)
	{
		static_cast<CTransform*>(pPlayer->Find_Component(TEXT("Com_Transform")))
			->Set_State(CTransform::STATE_POSITION, vInitPosition);
		static_cast<CPawn*>(pPlayer)->Set_Level(CurLevel);
		return S_OK;
	}

	//없으면 새로 생성해서 넣어줌
	CPlayer::DESC PlayerDesc{};
	PlayerDesc.vInitPos = vInitPosition;
	PlayerDesc.vScale = { 30.f, 50.f, 30.f };
	PlayerDesc.fRotationPerSec = RADIAN(180.f);
	PlayerDesc.fSpeedPerSec = 150.f;
	PlayerDesc.eLevelID = CurLevel;

	// 최초 게임 입장할때 어디에서 입장하던 스태틱에 생성해준다.
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Player"),
		LEVEL_STATIC, strLayerTag, &PlayerDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Indoor::Ready_Layer_Monster(const _wstring& strLayerTag)
{
	/* 첫 광장 통로쪽 상자 뒤 */
	SPAWN_SHOTGUNNER(900.f, 50.f, 830.f, LEVEL_INDOOR, true);
	SPAWN_SHOTGUNNER(900.f, 50.f, 865.f, LEVEL_INDOOR, true);

	/* 첫 광장쪽 좀비 */
	SPAWN_NUKEMUTANT(717.f, 50.f, 511.f, LEVEL_INDOOR, false);
	SPAWN_NUKEMUTANT(600.f, 50.f, 591.f, LEVEL_INDOOR, false);
	SPAWN_NUKEMUTANT(700.f, 50.f, 671.f, LEVEL_INDOOR, false);
	SPAWN_NUKEMUTANT(500.f, 50.f, 511.f, LEVEL_INDOOR, false);
	SPAWN_NUKEMUTANT(500.f, 50.f, 591.f, LEVEL_INDOOR, false);

	/* 통로쪽 벽 왼쪽 */
	SPAWN_SHOTGUNNER(1150.f, 50.f, 700.f, LEVEL_INDOOR, true);
	SPAWN_SHOTGUNNER(1200.f, 50.f, 700.f, LEVEL_INDOOR, true);
	SPAWN_SHOTGUNNER(1250.f, 50.f, 700.f, LEVEL_INDOOR, true);

	/* 통로쪽 벽 오른쪽 */
	SPAWN_SHOTGUNNER(1250.f, 50.f, 330.f, LEVEL_INDOOR, true);

	/* 통로지난 광장 왼쪽 구석 */
	SPAWN_SHOTGUNNER(1550.f, 50.f, 825.f, LEVEL_INDOOR, true);
	SPAWN_SHOTGUNNER(1580.f, 50.f, 800.f, LEVEL_INDOOR, true);
	SPAWN_SHOTGUNNER(1610.f, 50.f, 825.f, LEVEL_INDOOR, true);

	/* 문 열면 깝툭튀 */
	SPAWN_SHOTGUNNER(2150.f, 50.f, 930.f, LEVEL_INDOOR, true);
	SPAWN_SHOTGUNNER(2126.f, 50.f, 885.f, LEVEL_INDOOR, true);
	SPAWN_SHOTGUNNER(2126.f, 50.f, 930.f, LEVEL_INDOOR, true);

	/* 두번째 광장 쪽 좀비 */
	SPAWN_NUKEMUTANT(1434.f, 50.f, 550.f, LEVEL_INDOOR, false);
	SPAWN_NUKEMUTANT(1565.f, 50.f, 800.f, LEVEL_INDOOR, false);	 
	SPAWN_NUKEMUTANT(1667.f, 50.f, 320.f, LEVEL_INDOOR, false);
	SPAWN_NUKEMUTANT(2100.f, 50.f, 545.f, LEVEL_INDOOR, false);
	SPAWN_NUKEMUTANT(1663.f, 50.f, 993.f, LEVEL_INDOOR, false);
	SPAWN_NUKEMUTANT(2180.f, 50.f, 846.f, LEVEL_INDOOR, false);
	SPAWN_NUKEMUTANT(1980.f, 50.f, 800.f, LEVEL_INDOOR, false);

	return S_OK;
}

HRESULT CLevel_Indoor::Ready_Layer_Item(const _wstring& strLayerTag)
{
	CItem::DESC tDesc = {};
	tDesc.vInitPos = { 500.f, 55.f, 391.f };
	tDesc.vScale = _float3( 135.f, 50.f, 1.f ) * 0.3f;
	tDesc.eLevelID = CurLevel;
	tDesc.fSpeedPerSec = 300.f;
	tDesc.fTextureNum = 0.f;

	tDesc.szTextureID = TEXT("Item_Weapon_Dispenser");
	tDesc.eColID = CI_ITEM_DISPENSER;

	if (FAILED(m_pGameInstance->Add_GameObject(tDesc.eLevelID, TEXT("Prototype_GameObject_Item_Dispenser"),
		tDesc.eLevelID, strLayerTag, &tDesc)))
		return E_FAIL;
		
	return S_OK;
}

void CLevel_Indoor::Check_Collision()
{
	/*PAWN*/
	m_pGameInstance->Intersect(CG_PAWN, CG_BLOCK);
	m_pGameInstance->Intersect(CG_PAWN, CG_INTERACTIVE);
	m_pGameInstance->Intersect(CG_PAWN, CG_TRIGGER);
	m_pGameInstance->Intersect(CG_PAWN, CG_ITEM);
	m_pGameInstance->Intersect(CG_PAWN, CG_ALPHARECT);
	m_pGameInstance->Intersect(CG_PAWN, CG_MONSTER_BODY);

	/*PBULLET*/
	m_pGameInstance->Intersect(CG_PBULLET, CG_MONSTER);
	m_pGameInstance->Intersect(CG_PBULLET, CG_BLOCK);
	m_pGameInstance->Intersect(CG_PBULLET, CG_INTERACTIVE);
	m_pGameInstance->Intersect(CG_PBULLET, CG_ALPHARECT);

	/*MBULLET*/
	m_pGameInstance->Intersect(CG_MBULLET, CG_PAWN);
	m_pGameInstance->Intersect(CG_MBULLET, CG_BLOCK);
	m_pGameInstance->Intersect(CG_MBULLET, CG_INTERACTIVE);

	/*MONSTER*/
	m_pGameInstance->Intersect(CG_MONSTER, CG_BLOCK);
	m_pGameInstance->Intersect(CG_MONSTER, CG_ALPHARECT);
	m_pGameInstance->Intersect(CG_MONSTER, CG_INTERACTIVE);
}


CLevel_Indoor* CLevel_Indoor::Create(LPDIRECT3DDEVICE9 pGraphic_Device,CLevelData* pLevelData)
{
	CLevel_Indoor* pInstance = new CLevel_Indoor(pGraphic_Device);

	if (FAILED(pInstance->Initialize(pLevelData)))
	{
		MSG_BOX("Failed to Created : CLevel_Indoor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Indoor::Free()
{
	CCameraManager* Cameramanager = CAMERA_MANAGER;
	if (Cameramanager)
		Cameramanager->Switch(CCameraManager::DYNAMIC);
	__super::Free();
	m_pBGM->Stop();
	Safe_Release(m_pBGM);
}
