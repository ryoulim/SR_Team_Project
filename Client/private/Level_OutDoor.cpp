#include "Level_OutDoor.h"

#include "GameInstance.h"
#include "Map.h"
#include "Statue.h"
#include "Dynamic_Camera.h"
#include "Player.h"
#include "Trigger.h"

#define CurLevel LEVEL_OUTDOOR

CLevel_OutDoor::CLevel_OutDoor(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel{pGraphic_Device}
{
}

HRESULT CLevel_OutDoor::Initialize(CLevelData* pLevelData)
{
	if (FAILED(Load_Map(LEVEL_OUTDOOR, TEXT("OutDoorMapData.txt"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;

	CUI_Manager::Get_Instance()->Initialize_GamePlayUI(CurLevel);

	if (FAILED(Ready_Layer_Pawn(TEXT("Layer_Pawn"))))
		return E_FAIL;

	CUI_Manager::Get_Instance()->Initialize_Player();


	return S_OK;
}

void CLevel_OutDoor::Update(_float fTimeDelta)
{

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
		MSG_BOX("���� ���� ����");
		return E_FAIL;
	}

	/* �ؽ���� �����ؼ� ������� �� �� �� ������Ʈ�� ������� ���� ( �����߰��� ���� )*/
	/* �迭�� �����ұ� �ͱ⵵ �ߴµ�, �������� �� �༮�� �� �� �༮�� �������� �Ű��⶧����,, */
	_int iNumTile{}, iNumBlock{}, iNumTriPil{}, iNumAniRect{}, iNumAniBlock{},
		iNumInviBlock{}, iNumAlphaRect{}, iNumAlphaBlock{};
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
				MSG_BOX("��ü ���� ����");
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


	/* ui���� ���� �߿�, player ���� ���� ȣ�� �߿�  */
	// ������ ���� �̾��� 
	return S_OK;
}

HRESULT CLevel_OutDoor::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	CAMERA_MANAGER->Mouse_Fix_Mode_Switch();

	return S_OK;
}

HRESULT CLevel_OutDoor::Ready_Layer_Pawn(const _wstring& strLayerTag)
{
	//�� ������ �÷��̾� ������ġ
	_float3 vInitPosition = { 1400.f, 150.f, 500.f };

	// �÷��̾ �ִ��� üũ�ϰ� ������ ��ġ�� ��������.
	auto pPlayer = GET_PLAYER;
	if (pPlayer)
	{
		static_cast<CTransform*>(pPlayer->Find_Component(TEXT("Com_Transform")))
			->Set_State(CTransform::STATE_POSITION, vInitPosition);
		return S_OK;
	}

	//������ ���� �����ؼ� �־���
	CPlayer::DESC PlayerDesc{};
	PlayerDesc.vInitPos = vInitPosition;
	PlayerDesc.vScale = { 20.f, 30.f, 20.f };
	PlayerDesc.fRotationPerSec = RADIAN(180.f);
	PlayerDesc.fSpeedPerSec = 150.f;
	PlayerDesc.eLevelID = LEVEL_STATIC;

	// ���� ���� �����Ҷ� ��𿡼� �����ϴ� ����ƽ�� �������ش�.
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Player"),
		LEVEL_STATIC, strLayerTag, &PlayerDesc)))
		return E_FAIL;

	return S_OK;
}

void CLevel_OutDoor::Check_Collision()
{
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
