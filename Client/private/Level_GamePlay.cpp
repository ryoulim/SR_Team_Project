#include "Level_GamePlay.h"
#include "Level_Loading.h"
#include "Explosion.h"
#include "Firework.h"
#include "GameInstance.h"
#include "Flatform.h"
#include "Monster.h"
#include "FXMgr.h"

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

	//if (FAILED(Ready_Layer_Item(TEXT("Layer_Item"))))
	//	return E_FAIL;

	//if(FAILED(Ready_Light()))
	//	return E_FAIL;

	if (FAILED(Load_Map(LEVEL_GAMEPLAY, TEXT("MapData.txt"))))
		return E_FAIL;

	ShowCursor(FALSE);


	return S_OK;
}

void CLevel_GamePlay::Update(_float fTimeDelta)
{
	CFXMgr::Get_Instance()->SpawnMultipleExplosions(fTimeDelta, LEVEL_GAMEPLAY);
	CFXMgr::Get_Instance()->SpawnMultipleExplosions2(fTimeDelta, LEVEL_GAMEPLAY);
	CFXMgr::Get_Instance()->SpawnMultipleExplosions3(fTimeDelta, LEVEL_GAMEPLAY);

	//m_pGameInstance->Intersect(LEVEL_GAMEPLAY, TEXT("Layer_Pawn"), TEXT("Layer_Statue"));
	//m_pGameInstance->Intersect(LEVEL_GAMEPLAY, TEXT("Layer_Pawn"), TEXT("Layer_Monster"));
	m_pGameInstance->Intersect(LEVEL_GAMEPLAY, TEXT("Layer_Pawn"), TEXT("Layer_Block"));
	//m_pGameInstance->Intersect(LEVEL_GAMEPLAY, TEXT("Layer_Pawn"), TEXT("Layer_BackGround"));
	//m_pGameInstance->Intersect(LEVEL_GAMEPLAY, TEXT("Layer_PBullet"), TEXT("Layer_Monster"));
	//m_pGameInstance->Intersect(LEVEL_GAMEPLAY, TEXT("Layer_PBullet"), TEXT("Layer_Statue"));


	if (KEY_DOWN(DIK_F11))
	{
		if (FAILED(m_pGameInstance->Change_Level(LEVEL_LOADING,
			CLevel_Loading::Create(m_pGraphic_Device, LEVEL_RACE))))
			return;
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

	CGravity::DESC GravityDesc{};
	GravityDesc.vTerrainScale = pTerrainTransform->Compute_Scaled();
	GravityDesc.pTerrainVtxPos = pTerrainBuffer->Get_VertexPos();
	GravityDesc.iTerrainVtxNumX = pTerrainBuffer->Get_NumVerticesX();
  	GravityDesc.iTerrainVtxNumZ = pTerrainBuffer->Get_NumVerticesX();
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
	//CFXMgr::Get_Instance()->SpawnGunFire(_float3(5.f, -5.f, 20.f), LEVEL_GAMEPLAY);
	//CFXMgr::Get_Instance()->SpawnBulletTracer(_float3(5.f, -5.f, 50.f), LEVEL_GAMEPLAY);

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
		LEVEL_GAMEPLAY, strLayerTag, &FlatformDESC)))
		return E_FAIL;


	//따끈이
	CMonster::DESC Ttakkeun_iDesc{};
	Ttakkeun_iDesc.vPosition = { 100.f, 66.f, -250.f };
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Ttakkeun_i"),
		LEVEL_GAMEPLAY, strLayerTag, &Ttakkeun_iDesc)))
		return E_FAIL;

	//웬테코
	CMonster::DESC Wenteko_iDesc{};
	Wenteko_iDesc.vPosition = { 100.f, 40.f, -100.f };
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Wenteko"),
		LEVEL_GAMEPLAY, strLayerTag, &Wenteko_iDesc)))
		return E_FAIL;

	//샷거너
	CMonster::DESC Shotgunner_iDesc{};
	Shotgunner_iDesc.vPosition = { 180.f, 20.f, -120.f };
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Shotgunner"),
		LEVEL_GAMEPLAY, strLayerTag, &Shotgunner_iDesc)))
		return E_FAIL;

	//누크뮤턴트
	CMonster::DESC Nukemutant_iDesc{};
	Nukemutant_iDesc.vPosition = { 250.f, 30.f, -120.f };
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Nukemutant"),
		LEVEL_GAMEPLAY, strLayerTag, &Nukemutant_iDesc)))
		return E_FAIL;

	//거미쉐끼
	CMonster::DESC Mechsect_iDesc{};
	Mechsect_iDesc.vPosition = { 330.f, 10.f, -120.f };
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Mechsect"),
		LEVEL_GAMEPLAY, strLayerTag, &Mechsect_iDesc)))
		return E_FAIL;

	//그리터
	CMonster::DESC Greater_iDesc{};
	Greater_iDesc.vPosition = { 410.f, 20.f, -120.f };
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Greater"),
		LEVEL_GAMEPLAY, strLayerTag, &Greater_iDesc)))
		return E_FAIL;

	//다콘
	CMonster::DESC Deacon_iDesc{};
	Deacon_iDesc.vPosition = { 220.f, 50.f, -250.f };
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Deacon"),
		LEVEL_GAMEPLAY, strLayerTag, &Deacon_iDesc)))
		return E_FAIL;

	//컬티스트
	CMonster::DESC Cultist_iDesc{};
	Cultist_iDesc.vPosition = { 320.f, 30.f, -250.f };
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Cultist"),
		LEVEL_GAMEPLAY, strLayerTag, &Cultist_iDesc)))
		return E_FAIL;

	//알찬겔
	CMonster::DESC Archangel_iDesc{};
	Archangel_iDesc.vPosition = { 420.f, 80.f, -250.f };
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Archangel"),
		LEVEL_GAMEPLAY, strLayerTag, &Archangel_iDesc)))
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
	_int iNumTile = {};
	/* 불러오기용 변수 */
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

			// 큐브인지 렉트인지 분기 필요함
 			if (Prototype == TEXT("Prototype_GameObject_BackGround"))
			{
				CGameObject* pGameObject = m_pGameInstance->Find_Object(iLevelIdx, TEXT("Layer_BackGround"), iNumTile++);
				if (nullptr != pGameObject)
				{
					D3DVERTEXBUFFER_DESC VBDesc = {};
					bResult = ReadFile(hFile, &VBDesc, sizeof(D3DVERTEXBUFFER_DESC), &dwByte, NULL);

					CVIBuffer* pVIBuffer = dynamic_cast<CVIBuffer*>(pGameObject->Find_Component(TEXT("Com_VIBuffer")));

					if (nullptr != pVIBuffer)
					{
						LPDIRECT3DVERTEXBUFFER9 pLoadVB = { nullptr };
						void* pData = { nullptr };
						if (FAILED(m_pGraphic_Device->CreateVertexBuffer(VBDesc.Size, VBDesc.Usage, VBDesc.FVF, VBDesc.Pool, &pLoadVB, nullptr)))
							continue;

						if (SUCCEEDED(pLoadVB->Lock(0, 0, &pData, 0)))
						{
							bResult = ReadFile(hFile, pData, VBDesc.Size, &dwByte, NULL);
							pLoadVB->Unlock();

							pVIBuffer->Set_VertexBuffer(pLoadVB);
							Safe_Release(pLoadVB);
						}
					}
				}

			}
			else if (Prototype == TEXT("Prototype_GameObject_Block"))
			{

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