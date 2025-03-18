#include "Level_Test.h"

#include "GameInstance.h"
#include "Map.h"
#include "Statue.h"
#include "Dynamic_Camera.h"

CLevel_Test::CLevel_Test(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CLevel{pGraphic_Device}
{
}

HRESULT CLevel_Test::Initialize(CLevelData* pLevelData)
{
	if (FAILED(Ready_Layer_Terrain(TEXT("Layer_TestTerrain"))))
		return E_FAIL;

	if (FAILED(Load_Map(LEVEL_TEST, TEXT("MapData.txt"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	return S_OK;
}

void CLevel_Test::Update(_float fTimeDelta)
{
}

HRESULT CLevel_Test::Render()
{
	SetWindowText(g_hWnd, TEXT("테스트 레벨입니다."));
	return S_OK;
}

HRESULT CLevel_Test::Load_Map(_uint iLevelIdx, const _wstring& FileName)
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
	MSG_BOX("파일 로드 성공");

	return S_OK;
}

HRESULT CLevel_Test::Ready_Layer_Terrain(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_TEST, TEXT("Prototype_GameObject_TestTerrain"),
		LEVEL_TEST, strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Test::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	CDynamic_Camera::DESC tDesc = {};

	tDesc.fFov = 60.f;
	tDesc.fFar = 100.f;
	tDesc.fNear = 0.1f;
	tDesc.fSpeedPerSec = 100.f;
	tDesc.fRotationPerSec = D3DXToRadian(180.f);
	tDesc.vAt = _float3(0.f, 0.f, 0.f);
	tDesc.vEye = _float3(0.f, 40.f, 50.f);
	tDesc.fMouseSensor = 0.05f;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Dynamic_Camera"),
		LEVEL_TEST, strLayerTag, &tDesc)))
		return E_FAIL;

	return S_OK;
}

CLevel_Test* CLevel_Test::Create(LPDIRECT3DDEVICE9 pGraphic_Device,CLevelData* pLevelData)
{
	CLevel_Test* pInstance = new CLevel_Test(pGraphic_Device);

	if (FAILED(pInstance->Initialize(pLevelData)))
	{
		MSG_BOX("Failed to Created : CLevel_Test");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Test::Free()
{
	__super::Free();
}
