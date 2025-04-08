#include "Level_OutDoor.h"

#include "GameInstance.h"
#include "Map.h"
#include "Statue.h"
#include "Dynamic_Camera.h"
#include "Player.h"
#include "Trigger.h"
#include "UI_Manager.h"
#include "FXMgr.h"
#include "Monster.h"

#define CurLevel LEVEL_OUTDOOR

CLevel_OutDoor::CLevel_OutDoor(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel{pGraphic_Device}
{
}

HRESULT CLevel_OutDoor::Initialize(CLevelData* pLevelData)
{
	if (FAILED(Load_Map(LEVEL_OUTDOOR, TEXT("OutDoorMapData.txt"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Sky"),
		CurLevel, TEXT("Layer_Sky"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;

	CUI_Manager::Get_Instance()->Initialize_GamePlayUI(CurLevel);

	if (FAILED(Ready_Layer_Pawn(TEXT("Layer_Pawn"))))
		return E_FAIL;
	
	if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
		return E_FAIL;

	CUI_Manager::Get_Instance()->Initialize_Player();

	FX_MGR->SpawnRain(LEVEL_OUTDOOR);

	return S_OK;
}
#include "Level_Loading.h"
void CLevel_OutDoor::Update(_float fTimeDelta)
{
	if (g_FogCustom < 151.f)
		FX_MGR->SpawnMultipleThunder(fTimeDelta, LEVEL_OUTDOOR);

	Check_Collision();
	if (m_iNextLevel)
	{
		m_pGameInstance->Change_Level(LEVEL_LOADING,
			CLevel_Loading::Create(m_pGraphic_Device, (LEVEL)m_iNextLevel));
	}
}

HRESULT CLevel_OutDoor::Render()
{
	return S_OK;
}

HRESULT CLevel_OutDoor::Load_Map(_uint iLevelIdx, const _wstring& FileName)
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
		iNumInviBlock{}, iNumAlphaRect{}, iNumAlphaBlock{}, iNumWater{}, iNumLadder{},
		iNumTelephonePole{}, iNumPicture{}, iNumTrashCan{}, iNumGarbageBag{}, iNumFirePlug{},
		iNumDoor{};
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
			tDesc.vInitPos = vPosition * OUTDOORSCALE;
			tDesc.vScale = vScale * OUTDOORSCALE;
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
			else if (Prototype == TEXT("Prototype_GameObject_Ladder"))
			{
				CGameObject* pGameObject = m_pGameInstance->Find_Object(iLevelIdx, Layertag, iNumLadder++);
				if (nullptr != pGameObject)
				{
					if (FAILED(__super::Load_VertexBuffer(pGameObject, hFile, &dwByte)))
					{
						MSG_BOX("버텍스 버퍼 로딩실패");
						return E_FAIL;
					}
				}
			}
			else if (Prototype == TEXT("Prototype_GameObject_TelephonePole"))
			{
				CGameObject* pGameObject = m_pGameInstance->Find_Object(iLevelIdx, Layertag, iNumTelephonePole++);
				if (nullptr != pGameObject)
				{
					if (FAILED(__super::Load_VertexBuffer(pGameObject, hFile, &dwByte)))
					{
						MSG_BOX("버텍스 버퍼 로딩실패");
						return E_FAIL;
					}
				}
			}
			else if (Prototype == TEXT("Prototype_GameObject_Picture"))
			{
				CGameObject* pGameObject = m_pGameInstance->Find_Object(iLevelIdx, Layertag, iNumPicture++);
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
			else if (Prototype == TEXT("Prototype_GameObject_FirePlug"))
			{
				CGameObject* pGameObject = m_pGameInstance->Find_Object(iLevelIdx, Layertag, iNumFirePlug++);
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
			tDesc.vInitPos = vPosition * OUTDOORSCALE;
			tDesc.vScale = vScale * OUTDOORSCALE;
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

HRESULT CLevel_OutDoor::Ready_Layer_UI(const _wstring& strLayerTag)
{
	CUI::DESC Desc{};
	Desc.eLevelID = LEVEL_OUTDOOR;
	Desc.fDepth = _float(UI_HUD);
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

HRESULT CLevel_OutDoor::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	CAMERA_MANAGER->Switch(CCameraManager::FPS);
	CAMERA_MANAGER->Set_Mouse_Fix(TRUE);

	return S_OK;
}

HRESULT CLevel_OutDoor::Ready_Layer_Pawn(const _wstring& strLayerTag)
{
	//이 레벨의 플레이어 생성위치
	_float3 vInitPosition = { 1938.f, 771.f, - 127.f};

	//// 플레이어가 있는지 체크하고 있으면 위치만 변경해줌.
	//auto pPlayer = GET_PLAYER;
	//if (pPlayer)
	//{
	//	static_cast<CTransform*>(pPlayer->Find_Component(TEXT("Com_Transform")))
	//		->Set_State(CTransform::STATE_POSITION, vInitPosition);
	//	static_cast<CPawn*>(pPlayer)->Set_LevelID(CurLevel);
	//	return S_OK;
	//}

	auto pPlayer = static_cast<CPawn*>(GET_PLAYER);
	if (pPlayer)
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

	return S_OK;
}

HRESULT CLevel_OutDoor::Ready_Layer_Monster(const _wstring& strLayerTag)
{
	_float3 Pos = {};
	CMonster::DESC Wenteko_iDesc{};
	Wenteko_iDesc.fSpeedPerSec = 60.f;
	Wenteko_iDesc.fRotationPerSec = RADIAN(180.f);
	Wenteko_iDesc.vActive = true;
	Wenteko_iDesc.eLevel = LEVEL_OUTDOOR;
	Wenteko_iDesc.fAttackDistance = 400.f;
	Wenteko_iDesc.fDetectiveDistance = 500.f;
	/******************************************/
	CMonster::DESC Nukemutant_iDesc{};
	Nukemutant_iDesc.fSpeedPerSec = 60.f;
	Nukemutant_iDesc.fRotationPerSec = RADIAN(180.f);
	Nukemutant_iDesc.vActive = true;
	Nukemutant_iDesc.eLevel = LEVEL_OUTDOOR;
	Nukemutant_iDesc.fAttackDistance = 400.f;
	Nukemutant_iDesc.fDetectiveDistance = 500.f;
	/******************************************/
	CMonster::DESC Mechsect_iDesc{};
	Mechsect_iDesc.fSpeedPerSec = 60.f;
	Mechsect_iDesc.fRotationPerSec = RADIAN(180.f);
	Mechsect_iDesc.vActive = true;
	Mechsect_iDesc.eLevel = LEVEL_OUTDOOR;
	Mechsect_iDesc.fAttackDistance = 300.f;
	Mechsect_iDesc.fDetectiveDistance = 600.f;
	/******************************************/
	CMonster::DESC Archangel_iDesc{};
	Archangel_iDesc.fSpeedPerSec = 60.f;
	Archangel_iDesc.fRotationPerSec = RADIAN(180.f);
	Archangel_iDesc.vActive = true;
	Archangel_iDesc.eLevel = LEVEL_OUTDOOR;
	Archangel_iDesc.fAttackDistance = 600.f;
	Archangel_iDesc.fDetectiveDistance = 800.f;



#define SPAWN_WENTEKO(x,y,z, level)							\
Pos = _float3{ x, y, z };									\
Wenteko_iDesc.vPosition = Pos;								\
Wenteko_iDesc.vReturnPos = Pos;								\
if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC,	\
TEXT("Prototype_GameObject_Wenteko"),						\
level, L"Layer_Monster", &Wenteko_iDesc)))					\
return E_FAIL;												\

#define SPAWN_NUKEMUTANT(x,y,z, level)						\
Pos = _float3{ x, y, z };									\
Nukemutant_iDesc.vPosition = Pos;							\
Nukemutant_iDesc.vReturnPos = Pos;							\
if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC,	\
TEXT("Prototype_GameObject_Nukemutant"),					\
level, L"Layer_Monster", &Nukemutant_iDesc)))				\
return E_FAIL;												\

#define SPAWN_MECHSECT(x,y,z, level)						\
Pos = _float3{ x, y, z };									\
Mechsect_iDesc.vPosition = Pos;								\
Mechsect_iDesc.vReturnPos = Pos;							\
if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC,	\
TEXT("Prototype_GameObject_Mechsect"),						\
level, L"Layer_Monster", &Mechsect_iDesc)))					\
return E_FAIL;												\

#define SPAWN_ARCHANGEL(x,y,z, level)						\
Pos = _float3{ x, y, z };									\
Archangel_iDesc.vPosition = Pos;							\
Archangel_iDesc.vReturnPos = Pos;							\
if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC,	\
TEXT("Prototype_GameObject_Archangel"),						\
level, L"Layer_Monster", &Archangel_iDesc)))				\
return E_FAIL;												\

	//SPAWN_ARCHANGEL(513.f, 150.f, 325.f, LEVEL_OUTDOOR);
	//SPAWN_NUKEMUTANT(1186.f, 150.f, 333.f, LEVEL_OUTDOOR);
	//SPAWN_MECHSECT(1917.f, 150.f, 329.f, LEVEL_OUTDOOR);
	//SPAWN_WENTEKO(1773.f, 150.f, 1544.f, LEVEL_OUTDOOR);

	return S_OK;

}

void CLevel_OutDoor::Check_Collision()
{
	/*PAWN*/
	m_pGameInstance->Intersect(CG_PAWN, CG_BLOCK);
	m_pGameInstance->Intersect(CG_PAWN, CG_INTERACTIVE);
	m_pGameInstance->Intersect(CG_PAWN, CG_TRIGGER);
	m_pGameInstance->Intersect(CG_PAWN, CG_ITEM);

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

CLevel_OutDoor* CLevel_OutDoor::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CLevelData* pLevelData)
{
	CLevel_OutDoor* pInstance = new CLevel_OutDoor(pGraphic_Device);

	if (FAILED(pInstance->Initialize(pLevelData)))
	{
		MSG_BOX("Failed to Created : CLevel_OutDoor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_OutDoor::Free()
{
	__super::Free();
}
