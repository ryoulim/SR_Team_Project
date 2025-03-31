#include "Level_RaceThird.h"

#include "GameInstance.h"
#include "Statue.h"
#include "Map.h"
#include "PlayerOnBoat.h"
#include "Camera.h"

CLevel_RaceThird::CLevel_RaceThird(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel { pGraphic_Device }
{
}

HRESULT CLevel_RaceThird::Initialize(CLevelData* pLevelData)
{
	if (FAILED(__super::Initialize(pLevelData)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Statue(TEXT("Layer_RaceLandscape"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Terrain(TEXT("Layer_RaceTerrain"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Pawn(TEXT("Layer_Pawn"))))
		return E_FAIL;

	return S_OK;
}

void CLevel_RaceThird::Update(_float fTimeDelta)
{
}

HRESULT CLevel_RaceThird::Render()
{
	SetWindowText(g_hWnd, TEXT("레이싱 third레벨입니다."));

	return S_OK;
}

HRESULT CLevel_RaceThird::Ready_Layer_Terrain(const _wstring& strLayerTag)
{
	CMap::DESC MapDesc{};
	MapDesc.vScale = _float3(1.f, 1.f, 1.f);
	MapDesc.eLevelID = LEVEL_RACETHIRD;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACETHIRD, TEXT("Prototype_GameObject_RaceTerrain"),
		LEVEL_RACETHIRD, strLayerTag, &MapDesc)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Sky"),
		LEVEL_RACETHIRD, strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_RaceThird::Ready_Layer_Statue(const _wstring& strLayerTag)
{
	CStatue::DESC desc = {};

	desc.vAngle		= _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	desc.vInitPos	= _float3(300.f, 15.f, 5000.f);
	desc.vScale		= _float3(72.f, 30.f, 10000.f);
	desc.eLevelID = LEVEL_RACETHIRD;
	
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACETHIRD, TEXT("Prototype_GameObject_Rock"),
		LEVEL_RACETHIRD, strLayerTag, &desc)))
		return E_FAIL;

	desc.vAngle		= _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	desc.vInitPos	= _float3(600.f, 15.f, 5000.f);
	desc.vScale		= _float3(72.f, 30.f, 10000.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACETHIRD, TEXT("Prototype_GameObject_Rock"),
		LEVEL_RACETHIRD, strLayerTag, &desc)))
		return E_FAIL;

	desc.vAngle = _float3(D3DXToRadian(0.f), D3DXToRadian(90.f), D3DXToRadian(0.f));
	desc.vInitPos = _float3(600.f, 265.f, 0.f);
	desc.vScale = _float3(600.f, 500.f, 600.f);

	for (_uint i = 0; i < 25; i++)
	{
		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACETHIRD, TEXT("Prototype_GameObject_Forest"),
			LEVEL_RACETHIRD, strLayerTag, &desc)))
			return E_FAIL;

		desc.vInitPos.x += 5.f;
		desc.vInitPos.z += 400.f;
	}

	desc.vAngle = _float3(D3DXToRadian(0.f), D3DXToRadian(-90.f), D3DXToRadian(0.f));
	desc.vInitPos = _float3(300.f, 265.f, 0.f);
	desc.vScale = _float3(600.f, 500.f, 600.f);

	for (_uint i = 0; i < 25; i++)
	{
		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACETHIRD, TEXT("Prototype_GameObject_Forest"),
			LEVEL_RACETHIRD, strLayerTag, &desc)))
			return E_FAIL;

		desc.vInitPos.x -= 5.f;
		desc.vInitPos.z += 400.f;
	}

	desc.vAngle = _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	desc.vInitPos = _float3(450.f, 100.f, 10000.f);
	desc.vScale = _float3(350.f, 200.f, 200.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACETHIRD, TEXT("Prototype_GameObject_RaceGate"),
		LEVEL_RACETHIRD, strLayerTag, &desc)))
		return E_FAIL;


	desc.vAngle = _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	desc.vInitPos = _float3(450.f, 30.f, -2500.f);
	desc.vScale = _float3(30.f, 30.f, 5000.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACETHIRD, TEXT("Prototype_GameObject_RaceCylinder"),
		LEVEL_RACETHIRD, strLayerTag, &desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_RaceThird::Ready_Layer_Pawn(const _wstring& strLayerTag)
{
	//이 레벨의 플레이어 생성위치
	_float3 vInitPosition = { 450.f, 17.f, -3000.f };

	// 플레이어가 있는지 체크하고 있으면 위치만 변경해줌.
	auto pPlayer = m_pGameInstance->Find_Object(LEVEL_STATIC, TEXT("Layer_Pawn"));
	if (pPlayer)
	{
		static_cast<CTransform*>(pPlayer->Find_Component(TEXT("Com_Transform")))
			->Set_State(CTransform::STATE_POSITION, vInitPosition);
		return S_OK;
	}

	return S_OK;
}

CLevel_RaceThird* CLevel_RaceThird::Create(LPDIRECT3DDEVICE9 pGraphic_Device, class CLevelData* pLevelData)
{
	CLevel_RaceThird* pInstance = new CLevel_RaceThird(pGraphic_Device);

	if (FAILED(pInstance->Initialize(pLevelData)))
	{
		MSG_BOX("Failed to Created : CLevel_RaceThird");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_RaceThird::Free()
{
	__super::Free();
}
