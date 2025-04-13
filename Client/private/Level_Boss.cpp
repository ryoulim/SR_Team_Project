#include "Level_Boss.h"

#include "GameInstance.h"
#include "Map.h"
#include "Statue.h"
#include "Dynamic_Camera.h"

#include "FXMgr.h"
#include "UI_Manager.h"

#define CurLevel LEVEL_BOSS

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

	CUI_Manager::Get_Instance()->Initialize_GamePlayUI(LEVEL_BOSS);

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
	SetWindowText(g_hWnd, TEXT("���� �����Դϴ�."));
	return S_OK;
}

HRESULT CLevel_Boss::Ready_Layer_Terrain(const _wstring& strLayerTag)
{
	CMap::DESC MapDesc{};
	MapDesc.eLevelID = LEVEL_BOSS;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_BOSS, TEXT("Prototype_GameObject_Terrain"),
		LEVEL_BOSS, strLayerTag, &MapDesc)))
		return E_FAIL;

	auto pTerrain = m_pGameInstance->
		Find_Object(LEVEL_BOSS, strLayerTag);
	auto pTerrainBuffer = static_cast<CVIBuffer_Terrain*>(pTerrain->
		Find_Component(TEXT("Com_VIBuffer")));
	auto pTerrainTransform = static_cast<CTransform*>(pTerrain->
		Find_Component(TEXT("Com_Transform")));


	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Sky"),
		LEVEL_BOSS, strLayerTag)))
		return E_FAIL;


	return S_OK;
}

HRESULT CLevel_Boss::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	CAMERA_MANAGER->Switch(CCameraManager::FPS);
	CAMERA_MANAGER->Set_Mouse_Fix(TRUE);

	return S_OK;
}

#include "MyCube.h"

HRESULT CLevel_Boss::Ready_Layer_Statue(const _wstring& strLayerTag)
{
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
	//�÷��� ����
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
	//�����
	FX_Manager->SpawnRain(LEVEL_GAMEPLAY);

	//�ҿ뵹��
	FX_Manager->SpawnSpher(_float3{ 800.f, 0.f, -200.f }, LEVEL_GAMEPLAY);

	//����̵�(�ұ��)
	FX_Manager->SpawnTornado(_float3{ 630.f, 25.f, -320.f }, LEVEL_GAMEPLAY);
	FX_Manager->SpawnTornado(_float3{ 760.f, 25.f, -100.f }, LEVEL_GAMEPLAY);
	FX_Manager->SpawnTornado(_float3{ 850.f, 25.f, -230.f }, LEVEL_GAMEPLAY);

	//��������
	FX_Manager->SpawnRava(_float3{ 800.f, 0.f, -200.f }, LEVEL_GAMEPLAY);

	return S_OK;
}

HRESULT CLevel_Boss::Ready_Layer_Effect(const _wstring& strLayerTag)
{
	return S_OK;
}

#include "Player.h"
HRESULT CLevel_Boss::Ready_Layer_Pawn(const _wstring& strLayerTag)
{
	//�� ������ �÷��̾� ������ġ
	_float3 vInitPosition = { 1400.f, 150.f, 200.f };

	auto pPlayer = static_cast<CPawn*>(GET_PLAYER);
	if (pPlayer)
		m_pGameInstance->Release_Layer(LEVEL_STATIC, strLayerTag);

	//������ ���� �����ؼ� �־���
	CPlayer::DESC PlayerDesc{};
	PlayerDesc.vInitPos = vInitPosition;
	PlayerDesc.vScale = { 20.f, 30.f, 20.f };
	PlayerDesc.fRotationPerSec = RADIAN(180.f);
	PlayerDesc.fSpeedPerSec = 150.f;
	PlayerDesc.eLevelID = CurLevel;

	// ���� ���� �����Ҷ� ��𿡼� �����ϴ� ����ƽ�� �������ش�.
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Player"),
		LEVEL_STATIC, strLayerTag, &PlayerDesc)))
		return E_FAIL;

	return S_OK;
}

#include "Ttakkeun_i.h"
HRESULT CLevel_Boss::Ready_Layer_Monster(const _wstring& strLayerTag)
{
	//�÷��� ����
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

	SpawnTtakkeun_i(_float3{ 1200.f, 100.f, 1500.f }, true, 0, LEVEL_GAMEPLAY);
	SpawnTtakkeun_i(_float3{ 1600.f, 100.f, 1500.f }, true, 1, LEVEL_GAMEPLAY);

	return S_OK;
}

HRESULT CLevel_Boss::Ready_Layer_UI(const _wstring& strLayerTag)
{
	CUI::DESC Desc{};
	Desc.eLevelID = LEVEL_BOSS;
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
		LEVEL_BOSS, strLayerTag, &tDesc)))
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
		MSG_BOX("���� ���� ����");
		return E_FAIL;
	}
	/* �ؽ���� �����ؼ� ������� �� �� �� ������Ʈ�� ������� ���� ( �����߰��� ���� )*/
	/* �迭�� �����ұ� �ͱ⵵ �ߴµ�, �������� �� �༮�� �� �� �༮�� �������� �Ű��⶧����,, */
	_int iNumTile{}, iNumBlock{}, iNumTriPil{}, iNumAniRect{}, iNumAniBlock{},
		iNumInviBlock{}, iNumLava{}, iNumAlphaRect{}, iNumAlphaBlock{}, iNumBossBridge{};
	/* �ҷ������ ���� */
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
				MSG_BOX("��ü ���� ����");
				return E_FAIL;
			}

			// ť������ ��Ʈ���� �б� �ʿ���
			if (Prototype == TEXT("Prototype_GameObject_BackGround"))
			{
				CGameObject* pGameObject = m_pGameInstance->Find_Object(iLevelIdx, Layertag, iNumTile++);
				if (nullptr != pGameObject)
				{
					if (FAILED(__super::Load_VertexBuffer(pGameObject, hFile, &dwByte)))
					{
						MSG_BOX("���ؽ� ���� �ε�����");
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
						MSG_BOX("���ؽ� ���� �ε�����");
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
						MSG_BOX("���ؽ� ���� �ε�����");
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
						MSG_BOX("���ؽ� ���� �ε�����");
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
						MSG_BOX("���ؽ� ���� �ε�����");
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
						MSG_BOX("���ؽ� ���� �ε�����");
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
						MSG_BOX("���ؽ� ���� �ε�����");
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
						MSG_BOX("���ؽ� ���� �ε�����");
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
						MSG_BOX("���ؽ� ���� �ε�����");
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
						MSG_BOX("���ؽ� ���� �ε�����");
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
				MSG_BOX("��ü ���� ����");
				return E_FAIL;
			}

			ZeroMemory(szPrototypeTag, sizeof(szPrototypeTag));
		}

	}

	CloseHandle(hFile);
	return S_OK;
}


void CLevel_Boss::SpawnTtakkeun_i(const _float3& _Position, _bool m_bActive, _int _iNum, LEVEL _eLevel)
{
	CMonster::DESC Ttakkeun_iDesc2{};
	Ttakkeun_iDesc2.vPosition = _Position;
	Ttakkeun_iDesc2.fSpeedPerSec = 60.f;
	Ttakkeun_iDesc2.fRotationPerSec = RADIAN(180.f);
	Ttakkeun_iDesc2.vActive = m_bActive;
	Ttakkeun_iDesc2.vReturnPos = _Position;
	Ttakkeun_iDesc2.iNums = _iNum;
	Ttakkeun_iDesc2.eLevel = _eLevel;
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_BOSS, TEXT("Prototype_GameObject_Ttakkeun_i"),
		_eLevel, L"Layer_Boss", &Ttakkeun_iDesc2)))
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
	__super::Free();
}
