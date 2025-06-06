#include "Level_RaceSecond.h"
#include "Level_Loading.h"

#include "GameInstance.h"
#include "Statue.h"
#include "Map.h"
#include "PlayerOnBoat.h"
#include "RaceBoss.h"
#include "CameraManager.h"
#include "UI_Manager.h"

#define CurLevel LEVEL_RACESECOND

CLevel_RaceSecond::CLevel_RaceSecond(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel { pGraphic_Device }
{
}

HRESULT CLevel_RaceSecond::Initialize(CLevelData* pLevelData)
{
	if (FAILED(__super::Initialize(pLevelData)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Statue(TEXT("Layer_RaceLandscape"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Terrain(TEXT("Layer_RaceTerrain"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Pawn(TEXT("Layer_Pawn"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_RaceBoss(TEXT("Layer_RaceBoss"))))
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pBGM = m_pGameInstance->Get_Single_Sound("antistatic");

	/* 남아있던 파티클 다 제거하고 시작하기 */
	m_pGameInstance->Release_Layer(LEVEL_STATIC, TEXT("Layer_Particle"));

#pragma region 재질 설정
	/* 기본적인 재질들 설정하기 */

	D3DMATERIAL9		MtrlDesc{};
	MtrlDesc.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	MtrlDesc.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	MtrlDesc.Emissive = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.f);

	// 장치에 재질을 설정한다
	m_pGraphic_Device->SetMaterial(&MtrlDesc);
#pragma endregion
	return S_OK;
}

void CLevel_RaceSecond::Update(_float fTimeDelta)
{
	Check_Collision();

	if (m_iNextLevel)
	{
		if (m_iNextLevel == LEVEL_UNDERGROUND)
		{
			m_pGameInstance->Release_Layer(LEVEL_STATIC, TEXT("Layer_RaceBoss"));
			m_pGameInstance->Release_Layer(LEVEL_STATIC, TEXT("Layer_RaceBossBullet"));
			m_pGameInstance->Release_Layer(LEVEL_STATIC, TEXT("Layer_Particle"));
			static_cast<CPawn*>(GET_PLAYER)->Set_Active(FALSE);
			m_pBGM->Stop();
		}

		m_pGameInstance->Change_Level(LEVEL_LOADING,
			CLevel_Loading::Create(m_pGraphic_Device, (LEVEL)m_iNextLevel));
	}
}

HRESULT CLevel_RaceSecond::Render()
{
	// 모든 랜더가 끝나고 들어오는 부분!! 
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, TRUE);

	SetWindowText(g_hWnd, TEXT("레이싱 second레벨입니다."));

	return S_OK;
}

HRESULT CLevel_RaceSecond::Ready_Layer_Camera()
{
	auto CameraManager = CAMERA_MANAGER;
	CameraManager->Switch(CCameraManager::TPS);
	CameraManager->Set_Mouse_Fix(TRUE);

	return S_OK;
}

HRESULT CLevel_RaceSecond::Ready_Layer_Terrain(const _wstring& strLayerTag)
{
	CMap::DESC MapDesc{};
	MapDesc.vScale = _float3(1.f, 1.f, 1.f);
	MapDesc.eLevelID = LEVEL_RACESECOND;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACESECOND, TEXT("Prototype_GameObject_RaceTerrain"),
		LEVEL_RACESECOND, strLayerTag, &MapDesc)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Sky"),
		LEVEL_RACESECOND, strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_RaceSecond::Ready_Layer_Statue(const _wstring& strLayerTag)
{
	CStatue::DESC desc = {};

	desc.vAngle = _float3(D3DXToRadian(0.f), D3DXToRadian(80.f), D3DXToRadian(0.f));
	desc.vInitPos = _float3(850.f, 300.f, -3000.f);
	desc.vScale = _float3(600.f, 600.f, 600.f);
	desc.eLevelID = LEVEL_RACESECOND;

	for (_uint i = 0; i < 25; i++)
	{
		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACESECOND, TEXT("Prototype_GameObject_Forest"),
			LEVEL_RACESECOND, strLayerTag, &desc)))
			return E_FAIL;

		desc.vInitPos.z += 600.f;
	}

	desc.vAngle = _float3(D3DXToRadian(0.f), D3DXToRadian(-80.f), D3DXToRadian(0.f));
	desc.vInitPos = _float3(50.f, 300.f, -3000.f);
	desc.vScale = _float3(600.f, 600.f, 600.f);
	desc.eLevelID = LEVEL_RACESECOND;

	for (_uint i = 0; i < 25; i++)
	{
		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACESECOND, TEXT("Prototype_GameObject_Forest"),
			LEVEL_RACESECOND, strLayerTag, &desc)))
			return E_FAIL;

		desc.vInitPos.z += 600.f;
	}


	desc.vAngle		= _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	desc.vInitPos	= _float3(250.f, 36.f, -3000.f);
	desc.vScale		= _float3(72.f, 120.f, 72.f);
	desc.eLevelID = LEVEL_RACESECOND;
	
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACESECOND, TEXT("Prototype_GameObject_RaceLandscape"),
		LEVEL_RACESECOND, strLayerTag, &desc)))
		return E_FAIL;

	desc.vAngle		= _float3(D3DXToRadian(0.f), D3DXToRadian(180.f), D3DXToRadian(0.f));
	desc.vInitPos	= _float3(650.f, 36.f, 10000.f);
	desc.vScale		= _float3(72.f, 120.f, 72.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACESECOND, TEXT("Prototype_GameObject_RaceLandscape"),
		LEVEL_RACESECOND, strLayerTag, &desc)))
		return E_FAIL;

	desc.vAngle = _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	desc.vInitPos = _float3(450.f, 100.f, 10000.f);
	desc.vScale = _float3(350.f, 200.f, 200.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACESECOND, TEXT("Prototype_GameObject_RaceGate"),
		LEVEL_RACESECOND, strLayerTag, &desc)))
		return E_FAIL;

	desc.vAngle = _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	desc.vInitPos = _float3(450.f, 30.f, -8000.f);
	desc.vScale = _float3(30.f, 30.f, 10000.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACESECOND, TEXT("Prototype_GameObject_RaceCylinder"),
		LEVEL_RACESECOND, strLayerTag, &desc)))
		return E_FAIL;

	desc.vAngle = _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	desc.vInitPos = _float3(450.f, 30.f, 12480.f);//12480
	desc.vScale = _float3(30.f, 30.f, 5000.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACESECOND, TEXT("Prototype_GameObject_RaceCylinder"),
		LEVEL_RACESECOND, strLayerTag, &desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_RaceSecond::Ready_Layer_Pawn(const _wstring& strLayerTag)
{
	//이 레벨의 플레이어 생성위치
	_float3 vInitPosition = { 450.f, 17.f, -10000.f };

	// 만약 플레이어가 있다면? 플레이어를 리스트에서 빼서
	auto pPlayer = static_cast<CPawn*>(GET_PLAYER);
	if (pPlayer)
	{
		// 만약 플레이어가 컴먼이라면 뒤로 뺸다.
		if (pPlayer->Get_Type() == CPawn::COMMON)
		{
			m_pGameInstance->Release_Layer(LEVEL_STATIC, strLayerTag);
			pPlayer->Set_Active(FALSE);
		}
		// 만약 플레이어가 보트라면 위치만 바꾸어준다.
		else
		{
			static_cast<CTransform*>(pPlayer->Find_Component(TEXT("Com_Transform")))
				->Set_State(CTransform::STATE_POSITION, vInitPosition);
			pPlayer->Set_Level(CurLevel);
			static_cast<CPlayerOnBoat*>(pPlayer)->Set_StartState(CPlayerOnBoat::ACCEL);
			return S_OK;
		}
	}

	/// 이 밑은 일반 플레이어 -> 보트 플레이어 전환시의 초기화 옵션
	CPlayerOnBoat::DESC PlayerOnBoatDesc = {};
	PlayerOnBoatDesc.vInitPos = vInitPosition;
	PlayerOnBoatDesc.vScale = _float3{ 223.f, 137.f, 223.f } *0.15f;
	PlayerOnBoatDesc.fRotationPerSec = RADIAN(180.f);
	PlayerOnBoatDesc.fSpeedPerSec = RACE_SPEED_PER_SEC;
	PlayerOnBoatDesc.eLevelID = CurLevel;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PlayerOnBoat"),
		LEVEL_STATIC, strLayerTag, &PlayerOnBoatDesc)))
		return E_FAIL;

	Safe_AddRef(pPlayer);
	m_pGameInstance->Push_GameObject(pPlayer, LEVEL_STATIC, strLayerTag);

	return S_OK;
}

HRESULT CLevel_RaceSecond::Ready_Layer_RaceBoss(const _wstring& strLayerTag)
{
	_float3 vInitPosition = { 0.f, 250.f, 0.f };

	auto pBoss = static_cast<CRaceBoss*>(m_pGameInstance->Find_Object(LEVEL_STATIC, TEXT("Layer_RaceBoss")));
	if (pBoss)
	{
		static_cast<CTransform*>(pBoss->Find_Component(TEXT("Com_Transform")))
			->Set_State(CTransform::STATE_POSITION, vInitPosition);
		pBoss->Set_StartState(CRaceBoss::IDLE);
		pBoss->Add_Collider();
		CUI_Manager::Get_Instance()->Set_RacingBoss_HP_Settings(pBoss->Get_HP(), 250);
		CUI_Manager::Get_Instance()->Start_Rendering_BossHPUI();
		return S_OK;
	}

	return S_OK;
}

HRESULT CLevel_RaceSecond::Ready_Layer_UI(const _wstring& strLayerTag)
{
	CUI::DESC Desc{};
	Desc.eLevelID = CurLevel;
	Desc.fDepth = _float(UI_HUD);
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

	Desc.vScale = _float3(240.f, 42.f, 1.f);
	Desc.vInitPos = _float3(0.f, g_iWinSizeY * 0.5f - 44.f, 0.1f);
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_BossHPBar"),
		Desc.eLevelID, strLayerTag, &Desc)))
		return E_FAIL;
	CUI_Manager::Get_Instance()->Initialize_BossHPUI(CurLevel);

	Desc.vScale = _float3(131.25f, 95.f, 1.f); // 1.25배됨
	Desc.vInitPos = _float3(0.f, g_iWinSizeY * -0.5f + 50.f, 0.1f);
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_RacingPanel"),
		Desc.eLevelID, strLayerTag, &Desc)))
		return E_FAIL;

	CUI_Manager::Get_Instance()->Initialize_RacingUI(CurLevel);


	/* ui생성 순서 중요 */
	// 과거의 나야 미안해 
	// 안미안해꺼져
	return S_OK;
}

void CLevel_RaceSecond::Check_Collision()
{
	m_pGameInstance->Intersect(CG_PAWN, CG_MBULLET);
	m_pGameInstance->Intersect(CG_MONSTER, CG_PBULLET);
	m_pGameInstance->Intersect(CG_BOSS, CG_PAWN);
}

CLevel_RaceSecond* CLevel_RaceSecond::Create(LPDIRECT3DDEVICE9 pGraphic_Device, class CLevelData* pLevelData)
{
	CLevel_RaceSecond* pInstance = new CLevel_RaceSecond(pGraphic_Device);

	if (FAILED(pInstance->Initialize(pLevelData)))
	{
		MSG_BOX("Failed to Created : CLevel_RaceSecond");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_RaceSecond::Free()
{
	__super::Free();

	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);

	Safe_Release(m_pBGM);

}
