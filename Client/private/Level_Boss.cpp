#include "Level_Boss.h"

#include "GameInstance.h"
#include "Map.h"
#include "Statue.h"
#include "Dynamic_Camera.h"

#include "FXMgr.h"
#include "UI_Manager.h"


CLevel_Boss::CLevel_Boss(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel{pGraphic_Device}
{
}

HRESULT CLevel_Boss::Initialize(CLevelData* pLevelData)
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

	if (FAILED(Load_Map(LEVEL_GAMEPLAY, TEXT("MapData.txt"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Trigger(TEXT("Layer_Trigger"))))
		return E_FAIL;

	ShowCursor(FALSE);

	return S_OK;
}

void CLevel_Boss::Update(_float fTimeDelta)
{
}

HRESULT CLevel_Boss::Render()
{
	SetWindowText(g_hWnd, TEXT("보스 레벨입니다."));
	return S_OK;
}

HRESULT CLevel_Boss::Ready_Layer_Terrain(const _wstring& strLayerTag)
{

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_BOSS, TEXT("Prototype_GameObject_Terrain"),
		LEVEL_BOSS, strLayerTag)))
		return E_FAIL;

	auto pTerrain = m_pGameInstance->
		Find_Object(LEVEL_BOSS, strLayerTag);
	auto pTerrainBuffer = static_cast<CVIBuffer_Terrain*>(pTerrain->
		Find_Component(TEXT("Com_VIBuffer")));
	auto pTerrainTransform = static_cast<CTransform*>(pTerrain->
		Find_Component(TEXT("Com_Transform")));


	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Sky"),
		LEVEL_GAMEPLAY, strLayerTag)))
		return E_FAIL;


	return S_OK;
}

HRESULT CLevel_Boss::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	CAMERA_MANAGER->Mouse_Fix_Mode_Switch();

	//if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_CameraManager"),
	//	LEVEL_GAMEPLAY, strLayerTag, m_pData->Find_Data(TEXT("Dynamic_Camera")))))
	//	return E_FAIL;

	return S_OK;
}

#include "MyCube.h"

HRESULT CLevel_Boss::Ready_Layer_Statue(const _wstring& strLayerTag)
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

HRESULT CLevel_Boss::Ready_Layer_Item(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Item_Ammo"),
		LEVEL_GAMEPLAY, strLayerTag, m_pData->Find_Data(TEXT("Item_Ammo")))))
		return E_FAIL;

	return E_NOTIMPL;
}

#include "Flatform.h"

HRESULT CLevel_Boss::Ready_Layer_Particle(const _wstring& strLayerTag)
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

HRESULT CLevel_Boss::Ready_Layer_Effect(const _wstring& strLayerTag)
{
	return S_OK;
}

#include "Player.h"

HRESULT CLevel_Boss::Ready_Layer_Pawn(const _wstring& strLayerTag)
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
HRESULT CLevel_Boss::Ready_Layer_Monster(const _wstring& strLayerTag)
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

	SpawnTtakkeun_i(_float3{ 1200.f, 100.f, 1500.f }, true, 0);
	SpawnTtakkeun_i(_float3{ 1600.f, 100.f, 1500.f }, true, 1);

	//전시용
	//SpawnTtakkeun_i(_float3{ 100.f, 66.f, -250.f }, false);
	//SpawnWenteko(_float3{ 100.f, 40.f, -100.f }, false);
	//SpawnShotgunner(_float3{ 180.f, 20.f, -120.f }, false);
	//SpawnNukemutant(_float3{ 250.f, 30.f, -120.f }, false);
	//SpawnMechsect(_float3{ 330.f, 10.f, -120.f }, false);
	//SpawnGreater(_float3{ 410.f, 20.f, -120.f }, false);
	//SpawnDeacon(_float3{ 220.f, 50.f, -250.f }, false);
	//SpawnCultist(_float3{ 320.f, 30.f, -250.f }, false);
	//SpawnArchangel(_float3{ 420.f, 30.f, -250.f }, false);

	return S_OK;
}

HRESULT CLevel_Boss::Ready_Layer_UI(const _wstring& strLayerTag)
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

HRESULT CLevel_Boss::Ready_Light()
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

#include "Trigger.h"
HRESULT CLevel_Boss::Ready_Layer_Trigger(const _wstring& strLayerTag)
{
	CTrigger::DESC tDesc = {};

	tDesc.fRotationPerSec = 0;
	tDesc.fSpeedPerSec = 0;
	tDesc.LayerTag = TEXT("Layer_BossBridge");
	tDesc.vAngle = { 0.f, 0.f, 0.f };
	tDesc.vInitPos = { 1400.f, 70.f, 950.f };
	tDesc.vScale = { 200.f, 100.f, 100.f };
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Trigger"),
		LEVEL_GAMEPLAY, strLayerTag, &tDesc)))
		return E_FAIL;

	return S_OK;
}

void CLevel_Boss::Check_Collision()
{
	m_pGameInstance->Intersect(CG_PAWN, CG_BLOCK);
	m_pGameInstance->Intersect(CG_PBULLET, CG_MONSTER);
	m_pGameInstance->Intersect(CG_PBULLET, CG_BLOCK);
	m_pGameInstance->Intersect(CG_MBULLET, CG_BLOCK);
	m_pGameInstance->Intersect(CG_PAWN, CG_TRIGGER);
	m_pGameInstance->Intersect(CG_MONSTER, CG_BLOCK);
}


HRESULT CLevel_Boss::Load_Map(_uint iLevelIdx, const _wstring& FileName)
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

void CLevel_Boss::SpawnTtakkeun_i(const _float3& _Position, _bool m_bActive, _int _iNum)
{
	CMonster::DESC Ttakkeun_iDesc2{};
	Ttakkeun_iDesc2.vPosition = _Position;
	Ttakkeun_iDesc2.fSpeedPerSec = 60.f;
	Ttakkeun_iDesc2.fRotationPerSec = RADIAN(180.f);
	Ttakkeun_iDesc2.vActive = m_bActive;
	Ttakkeun_iDesc2.vReturnPos = _Position;
	Ttakkeun_iDesc2.iNums = _iNum;
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Ttakkeun_i"),
		LEVEL_GAMEPLAY, L"Layer_Boss", &Ttakkeun_iDesc2)))
		return;
}

CLevel_Boss* CLevel_Boss::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CLevelData* pLevelData)
{
	CLevel_Boss* pInstance = new CLevel_Boss(pGraphic_Device);

	if (FAILED(pInstance->Initialize(pLevelData)))
	{
		MSG_BOX("Failed to Created : CLevel_Boss");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Boss::Free()
{
}
