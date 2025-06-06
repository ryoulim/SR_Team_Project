#include "Level_RaceFirst.h"
#include "Level_Loading.h"

#include "GameInstance.h"
#include "Statue.h"
#include "Map.h"
#include "PlayerOnBoat.h"
#include "CameraManager.h"
#include "RaceBoss.h"
#include "UI_Manager.h"
#include "StreetLampHead.h"
#include "PSystem.h"


#define CurLevel LEVEL_RACEFIRST

CLevel_RaceFirst::CLevel_RaceFirst(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel { pGraphic_Device }
{
}

HRESULT CLevel_RaceFirst::Initialize(CLevelData* pLevelData)
{
	if (FAILED(__super::Initialize(pLevelData)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera()))
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
	m_pBGM->Set_Volume(0.35f);
	if(!m_pBGM->IsPlaying())
		m_pBGM->Play();

	/* 남아있던 파티클 다 제거하고 시작하기 */
	m_pGameInstance->Release_Layer(LEVEL_STATIC, TEXT("Layer_Particle"));

#pragma region 재질 설정
	/* 기본적인 재질들 설정하기 */

	D3DMATERIAL9		MtrlDesc{};
	MtrlDesc.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	MtrlDesc.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	MtrlDesc.Emissive = D3DXCOLOR(0.45f, 0.45f, 0.45f, 1.f);

	// 장치에 재질을 설정한다
	m_pGraphic_Device->SetMaterial(&MtrlDesc);
#pragma endregion

	return S_OK;
}

void CLevel_RaceFirst::Update(_float fTimeDelta)
{
	Check_Collision();
	Manage_Light();

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

HRESULT CLevel_RaceFirst::Render()
{
	// 모든 랜더가 끝나고 들어오는 부분!! 
	//m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, TRUE);
	
	SetWindowText(g_hWnd, TEXT("레이싱 first레벨입니다."));

	return S_OK;
}

HRESULT CLevel_RaceFirst::Ready_Layer_Camera()
{
	auto CameraManager = CAMERA_MANAGER;
	CameraManager->Switch(CCameraManager::TPS);
	CameraManager->Set_Mouse_Fix(TRUE);

	return S_OK;
}

HRESULT CLevel_RaceFirst::Ready_Layer_Terrain(const _wstring& strLayerTag)
{
	CMap::DESC MapDesc{};
	MapDesc.vScale = _float3(1.f, 1.f, 1.f);
	MapDesc.eLevelID = LEVEL_RACEFIRST;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_RaceTerrain"),
		LEVEL_RACEFIRST, strLayerTag, &MapDesc)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Sky"),
		LEVEL_RACEFIRST, strLayerTag)))
		return E_FAIL;
	return S_OK;
}

HRESULT CLevel_RaceFirst::Ready_Layer_Statue(const _wstring& strLayerTag)
{
	CStatue::DESC desc = {};

	desc.vAngle		= _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	desc.vInitPos	= _float3(250.f, 36.f, -3000.f);
	desc.vScale		= _float3(72.f, 120.f, 72.f);
	desc.eLevelID = LEVEL_RACEFIRST;
	
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_RaceLandscape"),
		LEVEL_RACEFIRST, strLayerTag, &desc)))
		return E_FAIL;

	desc.vAngle		= _float3(D3DXToRadian(0.f), D3DXToRadian(180.f), D3DXToRadian(0.f));
	desc.vInitPos	= _float3(650.f, 36.f, 10000.f);
	desc.vScale		= _float3(72.f, 120.f, 72.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_RaceLandscape"),
		LEVEL_RACEFIRST, strLayerTag, &desc)))
		return E_FAIL;

	/*desc.vAngle		= _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	desc.vInitPos	= _float3(450.f, 250.f, 700.f);
	desc.vScale		= _float3(950.f, 500.f, 720.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_BuildingH"),
		LEVEL_RACEFIRST, strLayerTag, &desc)))
		return E_FAIL;*/

	desc.vAngle		= _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	desc.vInitPos	= _float3(1100.f, 250.f, 2200.f);
	desc.vScale		= _float3(800.f, 500.f, 720.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_BuildingU"),
		LEVEL_RACEFIRST, strLayerTag, &desc)))
		return E_FAIL;

	desc.vAngle		= _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	desc.vInitPos	= _float3(-200.f, 250.f, 2500.f);
	desc.vScale		= _float3(800.f, 500.f, 720.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_BuildingW"),
		LEVEL_RACEFIRST, strLayerTag, &desc)))
		return E_FAIL;

	desc.vAngle		= _float3(D3DXToRadian(0.f), D3DXToRadian(-90.f), D3DXToRadian(0.f));
	desc.vInitPos	= _float3(1100.f, 250.f, 3000.f);
	desc.vScale		= _float3(800.f, 500.f, 720.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_BuildingV"),
		LEVEL_RACEFIRST, strLayerTag, &desc)))
		return E_FAIL;

	desc.vAngle		= _float3(D3DXToRadian(0.f), D3DXToRadian(30.f), D3DXToRadian(0.f));
	desc.vInitPos	= _float3(-350.f, 250.f, 4000.f);
	desc.vScale		= _float3(800.f, 500.f, 720.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_BuildingU"),
		LEVEL_RACEFIRST, strLayerTag, &desc)))
		return E_FAIL;

	desc.vAngle		= _float3(D3DXToRadian(0.f), D3DXToRadian(90.f), D3DXToRadian(0.f));
	desc.vInitPos	= _float3(950.f, 250.f, 4500.f);
	desc.vScale		= _float3(800.f, 500.f, 720.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_BuildingV"),
		LEVEL_RACEFIRST, strLayerTag, &desc)))
		return E_FAIL;

	desc.vAngle		= _float3(D3DXToRadian(0.f), D3DXToRadian(-180.f), D3DXToRadian(0.f));
	desc.vInitPos	= _float3(-250.f, 250.f, 5500.f);
	desc.vScale		= _float3(800.f, 500.f, 720.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_BuildingU"),
		LEVEL_RACEFIRST, strLayerTag, &desc)))
		return E_FAIL;
	
	desc.vAngle		= _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	desc.vInitPos	= _float3(1150.f, 250.f, 6500.f);
	desc.vScale		= _float3(800.f, 500.f, 720.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_BuildingW"),
		LEVEL_RACEFIRST, strLayerTag, &desc)))
		return E_FAIL;

	desc.vAngle		= _float3(D3DXToRadian(0.f), D3DXToRadian(-90.f), D3DXToRadian(0.f));
	desc.vInitPos	= _float3(-120.f, 250.f, 7000.f);
	desc.vScale		= _float3(800.f, 500.f, 720.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_BuildingV"),
		LEVEL_RACEFIRST, strLayerTag, &desc)))
		return E_FAIL;

	desc.vAngle		= _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	desc.vInitPos	= _float3(-200.f, 250.f, 8200.f);
	desc.vScale		= _float3(800.f, 500.f, 720.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_BuildingU"),
		LEVEL_RACEFIRST, strLayerTag, &desc)))
		return E_FAIL;

	desc.vAngle = _float3(D3DXToRadian(0.f), D3DXToRadian(-90.f), D3DXToRadian(0.f));
	desc.vInitPos = _float3(1100.f, 250.f, 8500.f);
	desc.vScale = _float3(800.f, 500.f, 720.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_BuildingW"),
		LEVEL_RACEFIRST, strLayerTag, &desc)))
		return E_FAIL;

	desc.vAngle		= _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	desc.vInitPos	= _float3(450.f, 100.f, 10000.f);
	desc.vScale		= _float3(350.f, 200.f, 200.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_RaceGate"),
		LEVEL_RACEFIRST, strLayerTag, &desc)))
		return E_FAIL;

	desc.vAngle = _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	desc.vInitPos = _float3(450.f, 30.f, -8000.f);
	desc.vScale = _float3(30.f, 30.f, 10000.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_RaceCylinder"),
		LEVEL_RACEFIRST, strLayerTag, &desc)))
		return E_FAIL;

	desc.vAngle = _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	desc.vInitPos = _float3(450.f, 30.f, 12480.f);//12480
	desc.vScale = _float3(30.f, 30.f, 5000.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_RaceCylinder"),
		LEVEL_RACEFIRST, strLayerTag, &desc)))
		return E_FAIL;


#pragma region 가로등
	CStreetLampHead::DESC StreetLampDesc = {};

	StreetLampDesc.vAngle = _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	StreetLampDesc.vInitPos = _float3(150.f, 180.f, 250.f);//1200
	StreetLampDesc.vScale = _float3(15.f, 40.f, 0.f);
	StreetLampDesc.eLevelID = LEVEL_RACEFIRST;
	StreetLampDesc.iLightNumber = 0;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_StreetLampHead"),
		LEVEL_RACEFIRST, strLayerTag, &StreetLampDesc)))
		return E_FAIL;

	StreetLampDesc.vAngle = _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	StreetLampDesc.vInitPos = _float3(150.f, 80.f, 250.f);
	StreetLampDesc.vScale = _float3(15.f, 160.f, 0.f);
	StreetLampDesc.eLevelID = LEVEL_RACEFIRST;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_StreetLampBody"),
		LEVEL_RACEFIRST, strLayerTag, &StreetLampDesc)))
		return E_FAIL;

	//<플레이어와의 거리, 조명 구조체 번호>로 관리하는데 
	//처음엔 일단 본인의 위치z값을 넣어준다.
	m_LightData.push_back(new LIGHTDATA{StreetLampDesc.vInitPos.z, StreetLampDesc.iLightNumber});

	StreetLampDesc.vAngle = _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	StreetLampDesc.vInitPos = _float3(900.f, 180.f, 1600.f);
	StreetLampDesc.vScale = _float3(15.f, 40.f, 0.f);
	StreetLampDesc.eLevelID = LEVEL_RACEFIRST;
	StreetLampDesc.iLightNumber = 1;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_StreetLampHead"),
		LEVEL_RACEFIRST, strLayerTag, &StreetLampDesc)))
		return E_FAIL;

	StreetLampDesc.vAngle = _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	StreetLampDesc.vInitPos = _float3(900.f, 80.f, 1600.f);
	StreetLampDesc.vScale = _float3(15.f, 160.f, 0.f);
	StreetLampDesc.eLevelID = LEVEL_RACEFIRST;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_StreetLampBody"),
		LEVEL_RACEFIRST, strLayerTag, &StreetLampDesc)))
		return E_FAIL;

	m_LightData.push_back(new LIGHTDATA{ StreetLampDesc.vInitPos.z, StreetLampDesc.iLightNumber });

	StreetLampDesc.vAngle = _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	StreetLampDesc.vInitPos = _float3(0.f, 180.f, 2000.f);
	StreetLampDesc.vScale = _float3(15.f, 40.f, 0.f);
	StreetLampDesc.eLevelID = LEVEL_RACEFIRST;
	StreetLampDesc.iLightNumber = 2;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_StreetLampHead"),
		LEVEL_RACEFIRST, strLayerTag, &StreetLampDesc)))
		return E_FAIL;

	StreetLampDesc.vAngle = _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	StreetLampDesc.vInitPos = _float3(0.f, 80.f, 2000.f);
	StreetLampDesc.vScale = _float3(15.f, 160.f, 0.f);
	StreetLampDesc.eLevelID = LEVEL_RACEFIRST;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_StreetLampBody"),
		LEVEL_RACEFIRST, strLayerTag, &StreetLampDesc)))
		return E_FAIL;

	m_LightData.push_back(new LIGHTDATA{ StreetLampDesc.vInitPos.z, StreetLampDesc.iLightNumber });

	StreetLampDesc.vAngle = _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	StreetLampDesc.vInitPos = _float3(900.f, 180.f, 2500.f);
	StreetLampDesc.vScale = _float3(15.f, 40.f, 0.f);
	StreetLampDesc.eLevelID = LEVEL_RACEFIRST;
	StreetLampDesc.iLightNumber = 3;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_StreetLampHead"),
		LEVEL_RACEFIRST, strLayerTag, &StreetLampDesc)))
		return E_FAIL;

	StreetLampDesc.vAngle = _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	StreetLampDesc.vInitPos = _float3(900.f, 80.f, 2500.f);
	StreetLampDesc.vScale = _float3(15.f, 160.f, 0.f);
	StreetLampDesc.eLevelID = LEVEL_RACEFIRST;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_StreetLampBody"),
		LEVEL_RACEFIRST, strLayerTag, &StreetLampDesc)))
		return E_FAIL;

	m_LightData.push_back(new LIGHTDATA{ StreetLampDesc.vInitPos.z, StreetLampDesc.iLightNumber });

	StreetLampDesc.vAngle = _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	StreetLampDesc.vInitPos = _float3(150.f, 180.f, 3300.f);
	StreetLampDesc.vScale = _float3(15.f, 40.f, 0.f);
	StreetLampDesc.eLevelID = LEVEL_RACEFIRST;
	StreetLampDesc.iLightNumber = 4;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_StreetLampHead"),
		LEVEL_RACEFIRST, strLayerTag, &StreetLampDesc)))
		return E_FAIL;

	StreetLampDesc.vAngle = _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	StreetLampDesc.vInitPos = _float3(150.f, 80.f, 3300.f);
	StreetLampDesc.vScale = _float3(15.f, 160.f, 0.f);
	StreetLampDesc.eLevelID = LEVEL_RACEFIRST;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_StreetLampBody"),
		LEVEL_RACEFIRST, strLayerTag, &StreetLampDesc)))
		return E_FAIL;

	m_LightData.push_back(new LIGHTDATA{ StreetLampDesc.vInitPos.z, StreetLampDesc.iLightNumber });

	StreetLampDesc.vAngle = _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	StreetLampDesc.vInitPos = _float3(900.f, 180.f, 3800.f);
	StreetLampDesc.vScale = _float3(15.f, 40.f, 0.f);
	StreetLampDesc.eLevelID = LEVEL_RACEFIRST;
	StreetLampDesc.iLightNumber = 5;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_StreetLampHead"),
		LEVEL_RACEFIRST, strLayerTag, &StreetLampDesc)))
		return E_FAIL;

	StreetLampDesc.vAngle = _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	StreetLampDesc.vInitPos = _float3(900.f, 80.f, 3800.f);
	StreetLampDesc.vScale = _float3(15.f, 160.f, 0.f);
	StreetLampDesc.eLevelID = LEVEL_RACEFIRST;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_StreetLampBody"),
		LEVEL_RACEFIRST, strLayerTag, &StreetLampDesc)))
		return E_FAIL;

	m_LightData.push_back(new LIGHTDATA{ StreetLampDesc.vInitPos.z, StreetLampDesc.iLightNumber });

	StreetLampDesc.vAngle = _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	StreetLampDesc.vInitPos = _float3(0.f, 180.f, 4800.f);
	StreetLampDesc.vScale = _float3(15.f, 40.f, 0.f);
	StreetLampDesc.eLevelID = LEVEL_RACEFIRST;
	StreetLampDesc.iLightNumber = 6;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_StreetLampHead"),
		LEVEL_RACEFIRST, strLayerTag, &StreetLampDesc)))
		return E_FAIL;

	StreetLampDesc.vAngle = _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	StreetLampDesc.vInitPos = _float3(0.f, 80.f, 4800.f);
	StreetLampDesc.vScale = _float3(15.f, 160.f, 0.f);
	StreetLampDesc.eLevelID = LEVEL_RACEFIRST;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_StreetLampBody"),
		LEVEL_RACEFIRST, strLayerTag, &StreetLampDesc)))
		return E_FAIL;

	m_LightData.push_back(new LIGHTDATA{ StreetLampDesc.vInitPos.z, StreetLampDesc.iLightNumber });

	StreetLampDesc.vAngle = _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	StreetLampDesc.vInitPos = _float3(900.f, 180.f, 5400.f);
	StreetLampDesc.vScale = _float3(15.f, 40.f, 0.f);
	StreetLampDesc.eLevelID = LEVEL_RACEFIRST;
	StreetLampDesc.iLightNumber = 7;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_StreetLampHead"),
		LEVEL_RACEFIRST, strLayerTag, &StreetLampDesc)))
		return E_FAIL;

	StreetLampDesc.vAngle = _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	StreetLampDesc.vInitPos = _float3(900.f, 80.f, 5400.f);
	StreetLampDesc.vScale = _float3(15.f, 160.f, 0.f);
	StreetLampDesc.eLevelID = LEVEL_RACEFIRST;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_StreetLampBody"),
		LEVEL_RACEFIRST, strLayerTag, &StreetLampDesc)))
		return E_FAIL;

	m_LightData.push_back(new LIGHTDATA{ StreetLampDesc.vInitPos.z, StreetLampDesc.iLightNumber });

	StreetLampDesc.vAngle = _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	StreetLampDesc.vInitPos = _float3(100.f, 180.f, 6300.f);
	StreetLampDesc.vScale = _float3(15.f, 40.f, 0.f);
	StreetLampDesc.eLevelID = LEVEL_RACEFIRST;
	StreetLampDesc.iLightNumber = 8;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_StreetLampHead"),
		LEVEL_RACEFIRST, strLayerTag, &StreetLampDesc)))
		return E_FAIL;

	StreetLampDesc.vAngle = _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	StreetLampDesc.vInitPos = _float3(100.f, 80.f, 6300.f);
	StreetLampDesc.vScale = _float3(15.f, 160.f, 0.f);
	StreetLampDesc.eLevelID = LEVEL_RACEFIRST;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_StreetLampBody"),
		LEVEL_RACEFIRST, strLayerTag, &StreetLampDesc)))
		return E_FAIL;

	m_LightData.push_back(new LIGHTDATA{ StreetLampDesc.vInitPos.z, StreetLampDesc.iLightNumber });

	StreetLampDesc.vAngle = _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	StreetLampDesc.vInitPos = _float3(0.f, 180.f, 7400.f);
	StreetLampDesc.vScale = _float3(15.f, 40.f, 0.f);
	StreetLampDesc.eLevelID = LEVEL_RACEFIRST;
	StreetLampDesc.iLightNumber = 9;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_StreetLampHead"),
		LEVEL_RACEFIRST, strLayerTag, &StreetLampDesc)))
		return E_FAIL;

	StreetLampDesc.vAngle = _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	StreetLampDesc.vInitPos = _float3(0.f, 80.f, 7400.f);
	StreetLampDesc.vScale = _float3(15.f, 160.f, 0.f);
	StreetLampDesc.eLevelID = LEVEL_RACEFIRST;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_StreetLampBody"),
		LEVEL_RACEFIRST, strLayerTag, &StreetLampDesc)))
		return E_FAIL;

	m_LightData.push_back(new LIGHTDATA{ StreetLampDesc.vInitPos.z, StreetLampDesc.iLightNumber });

	StreetLampDesc.vAngle = _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	StreetLampDesc.vInitPos = _float3(900.f, 180.f, 7500.f);
	StreetLampDesc.vScale = _float3(15.f, 40.f, 0.f);
	StreetLampDesc.eLevelID = LEVEL_RACEFIRST;
	StreetLampDesc.iLightNumber = 10;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_StreetLampHead"),
		LEVEL_RACEFIRST, strLayerTag, &StreetLampDesc)))
		return E_FAIL;

	StreetLampDesc.vAngle = _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	StreetLampDesc.vInitPos = _float3(900.f, 80.f, 7500.f);
	StreetLampDesc.vScale = _float3(15.f, 160.f, 0.f);
	StreetLampDesc.eLevelID = LEVEL_RACEFIRST;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_StreetLampBody"),
		LEVEL_RACEFIRST, strLayerTag, &StreetLampDesc)))
		return E_FAIL;

	m_LightData.push_back(new LIGHTDATA{ StreetLampDesc.vInitPos.z, StreetLampDesc.iLightNumber });

	StreetLampDesc.vAngle = _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	StreetLampDesc.vInitPos = _float3(0.f, 180.f, 9200.f);
	StreetLampDesc.vScale = _float3(15.f, 40.f, 0.f);
	StreetLampDesc.eLevelID = LEVEL_RACEFIRST;
	StreetLampDesc.iLightNumber = 11;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_StreetLampHead"),
		LEVEL_RACEFIRST, strLayerTag, &StreetLampDesc)))
		return E_FAIL;

	StreetLampDesc.vAngle = _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	StreetLampDesc.vInitPos = _float3(0.f, 80.f, 9200.f);
	StreetLampDesc.vScale = _float3(15.f, 160.f, 0.f);
	StreetLampDesc.eLevelID = LEVEL_RACEFIRST;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_StreetLampBody"),
		LEVEL_RACEFIRST, strLayerTag, &StreetLampDesc)))
		return E_FAIL;

	m_LightData.push_back(new LIGHTDATA{ StreetLampDesc.vInitPos.z, StreetLampDesc.iLightNumber });

	StreetLampDesc.vAngle = _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	StreetLampDesc.vInitPos = _float3(900.f, 180.f, 9500.f);
	StreetLampDesc.vScale = _float3(15.f, 40.f, 0.f);
	StreetLampDesc.eLevelID = LEVEL_RACEFIRST;
	StreetLampDesc.iLightNumber = 12;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_StreetLampHead"),
		LEVEL_RACEFIRST, strLayerTag, &StreetLampDesc)))
		return E_FAIL;

	StreetLampDesc.vAngle = _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	StreetLampDesc.vInitPos = _float3(900.f, 80.f, 9500.f);
	StreetLampDesc.vScale = _float3(15.f, 160.f, 0.f);
	StreetLampDesc.eLevelID = LEVEL_RACEFIRST;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_StreetLampBody"),
		LEVEL_RACEFIRST, strLayerTag, &StreetLampDesc)))
		return E_FAIL;

	m_LightData.push_back(new LIGHTDATA{ StreetLampDesc.vInitPos.z, StreetLampDesc.iLightNumber });
#pragma endregion

	return S_OK;
}

HRESULT CLevel_RaceFirst::Ready_Layer_Statue2(const _wstring& strLayerTag)
{
	CStatue::DESC desc = {};

	desc.vAngle = _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	desc.vInitPos = _float3(450.f, 115.f, 500.f);
	desc.vScale = _float3(10.f, 30.f, 0.f);
	desc.eLevelID = LEVEL_RACEFIRST;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_StreetLampHead"),
		LEVEL_RACEFIRST, strLayerTag, &desc)))
		return E_FAIL;

	desc.vAngle = _float3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	desc.vInitPos = _float3(450.f, 50.f, 500.f);
	desc.vScale = _float3(10.f, 100.f, 0.f);
	desc.eLevelID = LEVEL_RACEFIRST;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_RACEFIRST, TEXT("Prototype_GameObject_StreetLampBody"),
		LEVEL_RACEFIRST, strLayerTag, &desc)))
		return E_FAIL;

	return S_OK;
}

#include "PlayerOnBoat.h"
HRESULT CLevel_RaceFirst::Ready_Layer_Pawn(const _wstring& strLayerTag)
{
	// 이 레벨 플레이어 생성위치 
	_float3 vInitPosition = { 450.f, 17.f, -1000.f };

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
			vInitPosition = { 450.f, 17.f, -10000.f };
			static_cast<CTransform*>(pPlayer->Find_Component(TEXT("Com_Transform")))
				->Set_State(CTransform::STATE_POSITION, vInitPosition);
			static_cast<CPawn*>(pPlayer)->Set_Level(CurLevel);
			static_cast<CPlayerOnBoat*>(pPlayer)->Set_StartState(CPlayerOnBoat::ACCEL);
			return S_OK;
		}
	}

	/// 이 밑은 일반 플레이어 -> 보트 플레이어 전환시의 초기화 옵션
	CPlayerOnBoat::DESC PlayerOnBoatDesc = {};
	PlayerOnBoatDesc.vInitPos = vInitPosition;
	PlayerOnBoatDesc.vScale = _float3{ 223.f, 137.f, 223.f } * 0.15f;
	PlayerOnBoatDesc.fRotationPerSec = RADIAN(180.f);
	PlayerOnBoatDesc.fSpeedPerSec = RACE_SPEED_PER_SEC;
	PlayerOnBoatDesc.eLevelID = CurLevel;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PlayerOnBoat"),
		LEVEL_STATIC, strLayerTag, &PlayerOnBoatDesc)))
		return E_FAIL;

	auto pBoatPlayer = static_cast<CPawn*>(GET_PLAYER);

	if (pPlayer)
	{
		pBoatPlayer->Link_Player_Data(*pPlayer);
		Safe_AddRef(pPlayer);
		m_pGameInstance->Push_GameObject(pPlayer, LEVEL_STATIC, strLayerTag);
	}

	return S_OK;
}

#include "RaceBoss.h"
HRESULT CLevel_RaceFirst::Ready_Layer_RaceBoss(const _wstring& strLayerTag)
{
	_float3 vInitPosition = { 0.f, 1000.f, -500.f };

	auto pBoss = static_cast<CRaceBoss*>(m_pGameInstance->Find_Object(LEVEL_STATIC, TEXT("Layer_RaceBoss")));
	if (pBoss)
	{
		static_cast<CTransform*>(pBoss->Find_Component(TEXT("Com_Transform")))
			->Set_State(CTransform::STATE_POSITION, vInitPosition);
		pBoss->Set_StartState(CRaceBoss::ENTRANCE);
		pBoss->Add_Collider(); 
		CUI_Manager::Get_Instance()->Set_RacingBoss_HP_Settings(pBoss->Get_HP(), 250);
		CUI_Manager::Get_Instance()->Start_Rendering_BossHPUI();
		return S_OK;
	}

	CRaceBoss::DESC RaceBossDesc = {};
	RaceBossDesc.fRotationPerSec = RADIAN(180.f);
	RaceBossDesc.fSpeedPerSec = RACE_SPEED_PER_SEC;
	RaceBossDesc.eLevelID = CurLevel;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_RaceBoss"),
		LEVEL_STATIC, strLayerTag, &RaceBossDesc)))
		return E_FAIL;

	pBoss = static_cast<CRaceBoss*>(m_pGameInstance->Find_Object(LEVEL_STATIC, TEXT("Layer_RaceBoss")));
	if (pBoss)
		CUI_Manager::Get_Instance()->Set_RacingBoss_HP_Settings(pBoss->Get_HP(), 250);
	CUI_Manager::Get_Instance()->Start_Rendering_BossHPUI();

	static_cast<CPlayerOnBoat*>(GET_PLAYER)->Set_RaceBossTransform(
		static_cast<CTransform*>(m_pGameInstance->Find_Object(LEVEL_STATIC, TEXT("Layer_RaceBoss"))->Find_Component(TEXT("Com_Transform"))));

	return S_OK;
}

HRESULT CLevel_RaceFirst::Ready_Layer_UI(const _wstring& strLayerTag)
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

#define MULTIPLIER 1.5f
	// 105 76
	Desc.vScale = _float3(105.f * MULTIPLIER, 76.f * MULTIPLIER, 1.f); // 1.25배됨
	Desc.vInitPos = _float3(0.f, g_iWinSizeY * -0.5f + 60.f, 0.1f);
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_RacingPanel"),
		Desc.eLevelID, strLayerTag, &Desc)))
		return E_FAIL;
	CUI_Manager::Get_Instance()->Initialize_RacingUI(CurLevel);


	/* ui생성 순서 중요 */
	// 과거의 나야 미안해 
	return S_OK;
}

void CLevel_RaceFirst::Check_Collision()
{
	m_pGameInstance->Intersect(CG_PAWN, CG_MBULLET);
	m_pGameInstance->Intersect(CG_MONSTER, CG_PBULLET);
	
	m_pGameInstance->Intersect(CG_BOSS, CG_PAWN);
}

void CLevel_RaceFirst::Manage_Light()
{
	if (m_LightData.empty())
		return;

	//플레이어의 위치 z값만 받아옴
	auto pPlayer = GET_PLAYER;
	_float fPlayerPosZ = GET_PLAYER_TRANSFORM->Get_State(CTransform::STATE_POSITION)->z;

	_uint iCount = 0;
	//한 번에 7개 까지만 켠다
	for (auto iter = m_LightData.begin(); iter != m_LightData.end(); ++iter)
	{
		if (iCount == 7)
			break;

		m_pGraphic_Device->LightEnable((*iter)->iLightNumber, TRUE);
		++iCount;
	}
		

	//제거할 데이터를 담기 위한 임시적인 변수
	LIGHTDATA* pLightData = nullptr;

	//맨 앞에 있는 조명과 플레이어의 거리가 일정 이상 떨어지면
	if (fPlayerPosZ - m_LightData.front()->fDistanceToPlayer > 3000.f)
	{
		//임시적인 변수에 담는다.
		pLightData = m_LightData.front();

		//조명을 끈다.
		m_pGraphic_Device->LightEnable(pLightData->iLightNumber, false);
		
		//맨 앞에 있는 데이터를 제거한다.
		m_LightData.pop_front();

		//할당 해제 한다.
		Safe_Delete(pLightData);
	}
}

CLevel_RaceFirst* CLevel_RaceFirst::Create(LPDIRECT3DDEVICE9 pGraphic_Device, class CLevelData* pLevelData)
{
	CLevel_RaceFirst* pInstance = new CLevel_RaceFirst(pGraphic_Device);

	if (FAILED(pInstance->Initialize(pLevelData)))
	{
		MSG_BOX("Failed to Created : CLevel_RaceFirst");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_RaceFirst::Free()
{
	__super::Free();

	for (auto& pLight : m_LightData)
		Safe_Delete(pLight);
	m_LightData.clear();

	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);

	Safe_Release(m_pBGM);
}
