#include "Level_UnderGround.h"

#include "GameInstance.h"
#include "Map.h"
#include "Statue.h"
#include "Dynamic_Camera.h"
#include "Player.h"
#include "Trigger.h"
#include "UI_Manager.h"
#include "FXMgr.h"
#include "Monster.h"
#include "Level_Loading.h"
#include "Item.h"

#define CurLevel LEVEL_UNDERGROUND

CLevel_UnderGround::CLevel_UnderGround(LPDIRECT3DDEVICE9 pGraphic_Device) 
	: CLevel{pGraphic_Device}
{
}

HRESULT CLevel_UnderGround::Initialize(CLevelData* pLevelData)
{
	m_pGameInstance->Release_Layer(LEVEL_STATIC, TEXT("Layer_RaceBoss"));
	m_pGameInstance->Release_Layer(LEVEL_STATIC, TEXT("Layer_RaceBossBullet"));

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Sky"),
		CurLevel, TEXT("Layer_Sky"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;

	CUI_Manager::Get_Instance()->Initialize_GamePlayUI(CurLevel);

	if (FAILED(Load_Map(CurLevel, TEXT("UnderGroundMapData.txt"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Pawn(TEXT("Layer_Pawn"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Trigger(TEXT("Layer_Trigger"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Item(TEXT("Layer_Item"))))
		return E_FAIL;

	return S_OK;
}

void CLevel_UnderGround::Update(_float fTimeDelta)
{
	FX_MGR->Update(fTimeDelta);
	Check_Collision();

	if (m_iNextLevel)
	{
		m_pGameInstance->Change_Level(LEVEL_LOADING,
			CLevel_Loading::Create(m_pGraphic_Device, (LEVEL)m_iNextLevel));
	}
}

HRESULT CLevel_UnderGround::Render()
{
	return S_OK;
}

HRESULT CLevel_UnderGround::Load_Map(_uint iLevelIdx, const _wstring& FileName)
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
	/* 객체별로 레이어 나누지말걸 객체별로 레이어 나누지말걸 객체별로 레이어 나누지말걸 객체별로 레이어 나누지말걸*/
	_int iNumTile{}, iNumBlock{}, iNumTriPil{}, iNumAniRect{}, iNumAniBlock{},
		iNumInviBlock{}, iNumAlphaRect{}, iNumAlphaBlock{}, iNumWater{},
		iNumTrashCan{}, iNumGarbageBag{}, iNumDoor{};
	/* 불러오기용 변수 */
	_int iNumVertexX = {}, iNumVertexZ = {}, iLoadLength = {};
	_uint iNumBackGround = {}, iNumItem = {}, iItemID = {};
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
			tDesc.vInitPos = vPosition * UNDERGROUNDSCALE;
			tDesc.vScale = vScale * UNDERGROUNDSCALE;
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
			else if (Prototype == TEXT("Prototype_GameObject_Water"))
			{
				CGameObject* pGameObject = m_pGameInstance->Find_Object(iLevelIdx, Layertag, iNumWater++);
				if (nullptr != pGameObject)
				{
					if (FAILED(__super::Load_VertexBuffer(pGameObject, hFile, &dwByte)))
					{
						MSG_BOX("버텍스 버퍼 로딩실패");
						return E_FAIL;
					}
				}
			}
			else if (Prototype == TEXT("Prototype_GameObject_TrashCan"))
			{
				CGameObject* pGameObject = m_pGameInstance->Find_Object(iLevelIdx, Layertag, iNumTrashCan++);
				if (nullptr != pGameObject)
				{
					if (FAILED(__super::Load_VertexBuffer(pGameObject, hFile, &dwByte)))
					{
						MSG_BOX("버텍스 버퍼 로딩실패");
						return E_FAIL;
					}
				}
			}
			else if (Prototype == TEXT("Prototype_GameObject_GarbageBag"))
			{
				CGameObject* pGameObject = m_pGameInstance->Find_Object(iLevelIdx, Layertag, iNumGarbageBag++);
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
			tDesc.vInitPos = vPosition * UNDERGROUNDSCALE;
			tDesc.vScale = vScale * UNDERGROUNDITEMSCALE;
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

HRESULT CLevel_UnderGround::Ready_Layer_UI(const _wstring& strLayerTag)
{
	CUI::DESC Desc{};
	Desc.eLevelID = LEVEL_UNDERGROUND;
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

	if (FAILED(m_pGameInstance->Add_GameObject(Desc.eLevelID, TEXT("Prototype_GameObject_InteractPromptUI"),
		Desc.eLevelID, strLayerTag, &Desc)))
		return E_FAIL;

	/* ui생성 순서 중요, player 생성 이후 호출 중요  */
	// 과거의 나야 미안해 
	return S_OK;
}

HRESULT CLevel_UnderGround::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	CAMERA_MANAGER->Switch(CCameraManager::FPS);
	CAMERA_MANAGER->Set_Mouse_Fix(TRUE);

	return S_OK;
}

HRESULT CLevel_UnderGround::Ready_Layer_Pawn(const _wstring& strLayerTag)
{
	//이 레벨의 플레이어 생성위치
	_float3 vInitPosition = { 430.f, 200.f, 16.f };

	// 만약 플레이어가 있다면? 플레이어를 리스트에서 빼서
	auto pPlayer1 = static_cast<CPawn*>(m_pGameInstance->Find_Object(LEVEL_STATIC, L"Layer_Pawn",0));
	auto pPlayer2 = static_cast<CPawn*>(m_pGameInstance->Find_Object(LEVEL_STATIC, L"Layer_Pawn",1));

	if (pPlayer1)
	{
		// 만약 플레이어가 보트라면 죽여라
		if (pPlayer1->Get_Type() == CPawn::BOAT)
		{
			if (pPlayer2)
			{
				pPlayer2->Link_Player_Data(*pPlayer1);
				pPlayer2->AddRef();
				m_pGameInstance->Release_Layer(LEVEL_STATIC, strLayerTag);
				m_pGameInstance->Push_GameObject(pPlayer2, LEVEL_STATIC, strLayerTag);
			}
			else
			{
				m_pGameInstance->Release_Layer(LEVEL_STATIC, strLayerTag);

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
			}
		}
		else // 왜인진 모르겠지만 1번이 컴먼일경우
		{
			static_cast<CTransform*>(pPlayer1->Find_Component(TEXT("Com_Transform")))
				->Set_State(CTransform::STATE_POSITION, vInitPosition);
			static_cast<CPawn*>(pPlayer1)->Set_Level(CurLevel);
		}

		if (pPlayer2)
		{
			// 만약 플레이어가 컴먼이라면 위치만 바꾸어준다
			static_cast<CTransform*>(pPlayer2->Find_Component(TEXT("Com_Transform")))
				->Set_State(CTransform::STATE_POSITION, vInitPosition);
			static_cast<CGravity*>(pPlayer2->Find_Component(TEXT("Com_Gravity")))
				->Stop_Jump();
			static_cast<CPawn*>(pPlayer2)->Set_Level(CurLevel);
			pPlayer2->Set_Active(TRUE);
		}

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

#include "Particle_Define.h"
HRESULT CLevel_UnderGround::Ready_Layer_Monster(const _wstring& strLayerTag)
{
	// 랜덤 범위
	float minX = 170.f;
	float maxX = 645.f;
	float minZ = 330.f;
	float maxZ = 5000.f;


	/* [ 스마트하게 배치 ] */
	for (int i = 0; i < 80; ++i)
	{
		float x = GetRandomFloat(minX, maxX);
		float z = GetRandomFloat(minZ, maxZ);
		float y = 75.f;
	
		// 몬스터 종류 랜덤 선택 (예시: 0~2)
		int monsterType = rand() % 3;
	
		switch (monsterType)
		{
		case 0: SPAWN_MECHSECT(x, y, z, LEVEL_UNDERGROUND); break;
		case 1: SPAWN_WENTEKO(x, y, z, LEVEL_UNDERGROUND); break;
		case 2: SPAWN_NUKEMUTANT(x, y, z, LEVEL_UNDERGROUND); break;
		}
	}

	return S_OK;
}

HRESULT CLevel_UnderGround::Ready_Layer_Trigger(const _wstring& strLayerTag)
{
	CTrigger::DESC tDesc = {};
	tDesc.fRotationPerSec = 0;
	tDesc.fSpeedPerSec = 0;
	tDesc.vAngle = { 0.f, 0.f, 0.f };
	tDesc.vInitPos = { 420.f, 79.f, 5525.f };
	tDesc.vScale = { 200.f, 100.f, 50.f };
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Trigger"),
		CurLevel, strLayerTag, &tDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_UnderGround::Ready_Layer_Item(const _wstring& strLayerTag)
{
	CItem::DESC tDesc = {};
	tDesc.vInitPos = { 400.f, 125.f, 110.f };
	tDesc.vScale = _float3(145.f, 54.f, 1.f) * 0.3f;
	tDesc.eLevelID = CurLevel;
	tDesc.fSpeedPerSec = 300.f;
	tDesc.fTextureNum = 0.f;

	tDesc.szTextureID = TEXT("Item_Weapon_ChainGun");
	tDesc.eColID = CI_ITEM_CHAINGUN;

	if (FAILED(m_pGameInstance->Add_GameObject(tDesc.eLevelID, TEXT("Prototype_GameObject_Item_ChainGun"),
		tDesc.eLevelID, strLayerTag, &tDesc)))
		return E_FAIL;

	return S_OK;
}

void CLevel_UnderGround::Check_Collision()
{
	/*PAWN*/
	m_pGameInstance->Intersect(CG_PAWN, CG_BLOCK);
	m_pGameInstance->Intersect(CG_PAWN, CG_INTERACTIVE);
	m_pGameInstance->Intersect(CG_PAWN, CG_TRIGGER);
	m_pGameInstance->Intersect(CG_PAWN, CG_ITEM);
	m_pGameInstance->Intersect(CG_PAWN, CG_MONSTER_BODY);

	/*PBULLET*/
	m_pGameInstance->Intersect(CG_PBULLET, CG_MONSTER);
	m_pGameInstance->Intersect(CG_PBULLET, CG_BLOCK);
	m_pGameInstance->Intersect(CG_PBULLET, CG_INTERACTIVE);

	/*MBULLET*/
	m_pGameInstance->Intersect(CG_MBULLET, CG_PAWN);
	m_pGameInstance->Intersect(CG_MBULLET, CG_BLOCK);

	/*MONSTER*/
	m_pGameInstance->Intersect(CG_MONSTER, CG_BLOCK);
}

CLevel_UnderGround* CLevel_UnderGround::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CLevelData* pLevelData)
{
	CLevel_UnderGround* pInstance = new CLevel_UnderGround(pGraphic_Device);

	if (FAILED(pInstance->Initialize(pLevelData)))
	{
		MSG_BOX("Failed to Created : CLevel_UnderGround");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_UnderGround::Free()
{
	CCameraManager* Cameramanager = CAMERA_MANAGER;
	if(Cameramanager)
		Cameramanager->Switch(CCameraManager::DYNAMIC);
	__super::Free();
}