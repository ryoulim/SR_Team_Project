// 내 클래스 이름 : Player
// 부모 클래스 이름 : Pawn

#include "Player.h"
#include "GameInstance.h"
#include "CameraManager.h"
#include "FXMgr.h"

/* 샷건을 트리거로 몬스터 생성해주고싶어서 헤더 추가 해놓겠습니다 감사합니다 */
#include "Monster.h"

// 무기들 인클루드
//#include "Weapon_LoverBoy.h"
#include "Weapon_Dispenser.h"
#include "UI_Manager.h"
#define UIMGR CUI_Manager::Get_Instance()
#define PRINT_DIALOG(Message) UIMGR->Insert_DialogQueue(Message)

#define DASH_TIME 0.25f
#define JUST_DASH_TIME 0.15f
//이게 근데 타이머판정 말고 그 콜라이더 판정도 있어서 약간 특정상황에서 후해지긴함
#define DASH_SPEED 900.f
#define DASH_DECAY 0.05f
#define DASH_COOLTIME 0.7f

#define DODGE_TIMESCALE 0.1f

// 무적시간

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CPawn{ pGraphic_Device }
{
}

CPlayer::CPlayer(const CPlayer& Prototype)
	: CPawn(Prototype)
{
}

HRESULT CPlayer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{
	m_eType = COMMON;
	m_fDashTimer = -DASH_COOLTIME;

	m_szTextureID = TEXT("MyCube");
	m_szBufferType = TEXT("Cube");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	Init_Camera_Link();

	Add_Weapons();

	m_pSoundCom->SetVolume(0.45f);
	m_pSoundCom->SetVolume("stone_jump001", 0.8f);
	m_pSoundCom->SetVolume("stone_jump002", 0.8f);
	m_pSoundCom->SetVolume("stone_jump003", 0.8f);
	m_pSoundCom->SetVolume("fastwhoosh", 0.35f);
	m_pSoundCom->SetVolume("Dash", 0.6f);
	m_pSoundCom->SetVolume("Dodge", 1.0f);

	m_pSoundCom->SetVolume("stone_jump001", 1.f);
	m_pSoundCom->SetVolume("stone_jump002", 1.f);
	m_pSoundCom->SetVolume("stone_jump003", 1.f);

	m_pSoundCom->SetVolume("stone_land001", 0.8f);
	m_pSoundCom->SetVolume("stone_land002", 0.8f);
	m_pSoundCom->SetVolume("stone_land003", 0.8f);

	m_pSoundCom->SetVolume("TimeTo",0.35f);
	m_pSoundCom->Play("TimeTo");

	m_pLeftHand = new CLeftHand(m_pGraphic_Device);
	if (FAILED(m_pLeftHand->Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	//fTimeDelta = m_pGameInstance->Get_TimeDelta(TEXT("Timer_60"));

	if (m_bDash)
	{
		Update_Dash(fTimeDelta);
	}
	else if (m_bOnLadder)
	{
		Ladder(fTimeDelta);
	}
	else
	{
		m_fDashTimer -= fTimeDelta;
		Key_Input(fTimeDelta);
	}

	m_Weapons[m_iCurWeaponIndex]->Priority_Update(fTimeDelta);
	__super::Priority_Update(fTimeDelta);
}

EVENT CPlayer::Update(_float fTimeDelta)
{
	if (!m_bActive)
		return EVN_NONE;

#ifdef _CONSOL
	if (KEY_DOWN(DIK_Z))
	{
		_float3 vPosition = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		printf("플레이어 좌표 : { %.2f, %.2f, %.2f }\n", vPosition.x, vPosition.y, vPosition.z);
	}
#endif

	//fTimeDelta = m_pGameInstance->Get_TimeDelta(TEXT("Timer_60"));

	// 피격 후 무적시간 계산
	if (m_bMoveLeftHand)
	{
		if (m_pLeftHand->Move(fTimeDelta))
			m_bMoveLeftHand = false;
	}

	m_Weapons[m_iCurWeaponIndex]->Update(fTimeDelta);

	return __super::Update(fTimeDelta);
}

void CPlayer::Late_Update(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	//fTimeDelta = m_pGameInstance->Get_TimeDelta(TEXT("Timer_60"));

	m_vPrePosition = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);

	m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, m_pLeftHand);

	m_Weapons[m_iCurWeaponIndex]->Late_Update(fTimeDelta);
	
	m_pCollider->Update_Collider();

	//if (m_bOnLadder)
	//	m_bOnLadder = FALSE;
	//else

	m_pGravityCom->Update(fTimeDelta);
	if (m_pGravityCom->isJumpEnd())
	{
		switch (rand() % 3)
		{
		case 0:
			m_pSoundCom->Play("stone_land001");
			break;
		case 1:
			m_pSoundCom->Play("stone_land002");
			break;
		case 2:
			m_pSoundCom->Play("stone_land003");
			break;
		}
		m_fWalkSoundTimer = -0.1f;
	}


	Update_Camera_Link();

	__super::Late_Update(fTimeDelta);	
}

HRESULT CPlayer::Render()
{
	m_pLeftHand->Render();

	if(m_bActive)
		return m_Weapons[m_iCurWeaponIndex]->Render();
	else
		return __super::Render();
}

void CPlayer::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	if (MyColliderID == CI_PLAYER_PRE)
	{
		On_Just_Dodge();
		return;
	}

	m_pSoundCom->SetVolume(0.3f);
	m_pSoundCom->SetVolume("ArmorPiece", 0.15f);

	switch (OtherColliderID)
	{
	case CI_BLOCK_COMMON:
	{
		_float3 vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float3 vPos2 = *m_pCameraTransform->Get_State(CTransform::STATE_POSITION);

		_float3 Depth = m_pCollider->Get_Last_Collision_Depth();

		vPos += Depth;
		vPos2 += Depth;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
		m_pCameraTransform->Set_State(CTransform::STATE_POSITION, vPos2);

		//m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_pCameraTransform->Get_WorldMatrix_Inverse());
		break;
	}

	case CI_BLOCK_INVISIBLE:
		return;
		break;

	case CI_TRIGGER:
		if (m_pGameInstance->Get_CurrentLevelIndex() != LEVEL_GAMEPLAY)
			Change_Level();
		break;

		/* 아이템 */
	case CI_ITEM_CHAINGUN:
		/* 좀 그렇긴 하네 */
		m_iMaxWeaponIndex = 2;
		UIMGR->Add_Weapon(); // 이거 무슨 무기가 증가했는지는 모르고 웨폰맥스인덱스 ++해주는 로직마냥 ui도 걍 ++로 증가시키기만 해서 중간에 무기를 안줍거나 그러면 꼬입니다요..
		PRINT_DIALOG("picked up Chaingun");
		m_pSoundCom->Play("Weapon");
		break;

	case CI_ITEM_DISPENSER:
	{
		++m_iMaxWeaponIndex;
		UIMGR->Add_Weapon();
		PRINT_DIALOG("picked up Dispenser");
		m_pSoundCom->Play("Weapon");

		/* 몬스터 소환 좀 하겠습니다,, */
		if (m_eLevelID == LEVEL_INDOOR)
		{
			/* 앙찬겔 각자 자리에 */
			SPAWN_ARCHANGEL_ALT(611.f, 100.f, 690.f, LEVEL_INDOOR, false);
			SPAWN_ARCHANGEL_ALT(1950.f, 100.f, 650.f, LEVEL_INDOOR, false);
			SPAWN_ARCHANGEL_ALT(1780.f, 100.f, 1400.f, LEVEL_INDOOR, false);

			/* 두번째 광장 좀비 */
			SPAWN_NUKEMUTANT_ALT(2114.f, 50.f, 545.f, LEVEL_INDOOR);
			SPAWN_NUKEMUTANT_ALT(1961.f, 50.f, 427.f, LEVEL_INDOOR);
			SPAWN_NUKEMUTANT_ALT(2180.f, 50.f, 600.f, LEVEL_INDOOR);

			/* 첫번째 광장 좀비 */
			SPAWN_NUKEMUTANT_ALT(600.f, 50.f, 591.f, LEVEL_INDOOR);
			SPAWN_NUKEMUTANT_ALT(500.f, 50.f, 511.f, LEVEL_INDOOR);
			SPAWN_NUKEMUTANT_ALT(500.f, 50.f, 591.f, LEVEL_INDOOR);

			/* 기둥 뒤 사내 */
			SPAWN_SHOTGUNNER_ALT(2012.f, 50.f, 464.f, LEVEL_INDOOR, true);
			SPAWN_SHOTGUNNER_ALT(1874.f, 50.f, 464.f, LEVEL_INDOOR, true);
							 
			/* 출구 쪽 대기 */
			SPAWN_SHOTGUNNER_ALT(1765.f, 50.f, 1050.f, LEVEL_INDOOR, true);
			SPAWN_SHOTGUNNER_ALT(1776.f, 50.f, 1100.f, LEVEL_INDOOR, true);
			SPAWN_SHOTGUNNER_ALT(1718.f, 50.f, 1050.f, LEVEL_INDOOR, true);
		}

		break;
	}
	case CI_ITEM_AMMO_CHAINGUN:
		m_Weapons[2]->Replenish_Ammo(100);
		PRINT_DIALOG("ammo for Chaingun x 100");
		m_pSoundCom->Play("Ammo_ChainGun");
		break;
		
	case CI_ITEM_AMMO_DISPENSER_SCATTER:
		m_Weapons[1]->Replenish_Ammo(10);
		PRINT_DIALOG("shells for Disperser x 10");
		m_pSoundCom->Play("Ammo_Dispenser_Scatter");
		break;

	case CI_ITEM_AMMO_DISPENSER_CANNON:
		static_cast<CWeapon_Dispenser*>(m_Weapons[2])->Replenish_GrenadeAmmo(10);
		PRINT_DIALOG("grenades for Disperser x 10");
		m_pSoundCom->Play("Ammo_Dispenser_Cannon");
		break;

	case CI_ITEM_AMMO_LOVERBOY:
		m_Weapons[0]->Replenish_Ammo(20);
		PRINT_DIALOG("ammo for LoverBoy x 20");
		m_pSoundCom->Play("Ammo_LoverBoy");
		break;

	case CI_ITEM_HEALKIT:
		m_tInfo.iHP += 10;
		if (m_tInfo.iHP > 100)
			m_tInfo.iHP = 100;
		FX_MGR->SpawnHealEffect(m_eLevelID);
		PRINT_DIALOG("health kit: +10% HP ");
		m_pSoundCom->Play("HealKit");
		break;

	case CI_ITEM_ARMOR_PIECE:
		m_tInfo.iArmor += 1;
		if (m_tInfo.iArmor > 100)
			m_tInfo.iArmor = 100;
		PRINT_DIALOG("armor fragment x 1");
		m_pSoundCom->Play("ArmorPiece");
		break;

	case CI_ITEM_ARMOR_FULL:
		m_tInfo.iArmor += 100;
		if (m_tInfo.iArmor > 100)
			m_tInfo.iArmor = 100;
		PRINT_DIALOG("full armor pack !");
		m_pSoundCom->Play("Armor");
		break;

	case CI_ITEM_CARDKEY:
		m_bHaveCardkey = true;
		PRINT_DIALOG("picked up a CARDKEY.");
		m_pSoundCom->Play("CardKey");
		break;
	
		/* 인터렉션 */
	case CI_INTERACTIVE_LAVA:
		On_Hit(5);
		break;

	case CI_INTERACTIVE_LADDER:
		m_bOnLadder = TRUE;
		m_pGravityCom->Stop_Jump();
		break;

	case CI_INTERACTIVE_SECURITY:
		// 카드키 UI를 활성화 시킨다.
		// 만약 E키를 눌렀다면
		// 1. 손이 올라옴
		// 2. 이후에 문이 열림 << 근데 이거 두 개다 오브젝트쪽에서 하면 되는거 아니냐?
		/* 개추 ㅋㅋㅋ */
		break;

	case CI_INTERACTIVE_DOOR:
	{
		_float3 vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float3 vPos2 = *m_pCameraTransform->Get_State(CTransform::STATE_POSITION);

		_float3 Depth = m_pCollider->Get_Last_Collision_Depth();

		vPos += Depth;
		vPos2 += Depth;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
		m_pCameraTransform->Set_State(CTransform::STATE_POSITION, vPos2);
		break;
	}
		/* 몬스터 투사체*/
	case CI_MONSTER_SHOTGUNNER:
		On_Hit(3);
		break;
	case CI_MONSTER_CULTIST:
		On_Hit(4);
		break;
	case CI_MONSTER_GREATER:
		On_Hit(5);
		break;
	case CI_MONSTER_DEACON:
		On_Hit(6);
		break;
	case CI_MONSTER_NUKEMUTANT:
		On_Hit(7);
		break;
	case CI_MONSTER_ARCHANGEL:
		On_Hit(10);

		/* 몬스터 몸박*/
		break;
	case CI_MONSTER_MECHSECT:
		On_Hit(10);
		break;
	case CI_MONSTER_WENTEKO:
		On_Hit(10);
		break;

		/*보스 스킬*/
	case CI_BOSS_FIRE:
		On_Hit(10);
		break;
	case CI_BOSS_BULLET:
		On_Hit(10);
		break; 
	case CI_BOSS_JUMP:
		On_Hit(10);
		break;
	case CI_BOSS_GUIDBULLET:
		On_Hit(10);
		break;
	case CI_BOSS_TANGTANG:
		On_Hit(10);
		break;
	default:
		break;
	}
}

void CPlayer::Set_Level(LEVEL ID)
{
	__super::Set_Level(ID);

	if(!m_Weapons.empty()) 
		CUI_Manager::Get_Instance()->Change_Weapon(m_Weapons[m_iCurWeaponIndex]->Get_Info(), m_iCurWeaponIndex);
}

HRESULT CPlayer::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	/* For.Com_Gravity */
	CGravity::DESC GravityDesc{};
	GravityDesc.pTransformCom = m_pTransformCom;
	GravityDesc.fTimeIncreasePerSec = 8.2f;
	GravityDesc.fMaxFallSpeedPerSec = 840.f;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Gravity"),
		TEXT("Com_Gravity"), reinterpret_cast<CComponent**>(&m_pGravityCom), &GravityDesc)))
		return E_FAIL;

	/* Com_Sound */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Sound_Player"),
		TEXT("Com_Sound"), reinterpret_cast<CComponent**>(&m_pSoundCom))))
			return E_FAIL;

	return S_OK;
}

void CPlayer::Add_Weapons()
{
	CWeapon::DESC WeaponDesc{};
	WeaponDesc.pPlayerTransform = m_pTransformCom;
	WeaponDesc.eLevelID = m_eLevelID;

	WeaponDesc.fSpeedPerSec = 600.f;
	m_Weapons.push_back(
		static_cast<CWeapon*>(m_pGameInstance->Clone_Prototype(
			PROTOTYPE::TYPE_GAMEOBJECT, LEVEL_STATIC,
			TEXT("Prototype_GameObject_Weapon_LoverBoy"), &WeaponDesc)));

	WeaponDesc.fSpeedPerSec = 2600.f;
	m_Weapons.push_back(
		static_cast<CWeapon*>(m_pGameInstance->Clone_Prototype(
			PROTOTYPE::TYPE_GAMEOBJECT, LEVEL_STATIC,
			TEXT("Prototype_GameObject_Weapon_Dispenser"), &WeaponDesc)));

	WeaponDesc.fSpeedPerSec = 3000.f;
	m_Weapons.push_back(
		static_cast<CWeapon*>(m_pGameInstance->Clone_Prototype(
			PROTOTYPE::TYPE_GAMEOBJECT, LEVEL_STATIC,
			TEXT("Prototype_GameObject_Weapon_Chaingun"), &WeaponDesc)));

	CUI_Manager::Get_Instance()->Change_Weapon(m_Weapons[m_iCurWeaponIndex]->Get_Info(), m_iCurWeaponIndex);
	CUI_Manager::Get_Instance()->Init_Weapons(&m_Weapons);

	//////// 무기 탄약 정보들
	m_Weapons[0]->Get_Info(); // Loverboy
	m_Weapons[1]->Get_Info(); // ChainGun
	static_cast<CWeapon_Dispenser*>(m_Weapons[2])->Get_Shell_Info(); // Dispensor_Shell
	static_cast<CWeapon_Dispenser*>(m_Weapons[2])->Get_Grenade_Info(); // Dispensor_Grenade
}

void CPlayer::Key_Input(_float fTimeDelta)
{
	_bool bTriger{};

	// 이동관련
	_float3 vMoveDir{};
	if (KEY_PRESSING(DIK_W))
	{
		vMoveDir += m_pGravityCom->Get_Terrain_Front_Dir();
		bTriger = TRUE;
	}
	if (KEY_PRESSING(DIK_S))
	{
		vMoveDir -= m_pGravityCom->Get_Terrain_Front_Dir();
		bTriger = TRUE;
	}
	if (KEY_PRESSING(DIK_A))
	{
		vMoveDir -= m_pGravityCom->Get_Terrain_Right_Dir();
		bTriger = TRUE;
	}
	if (KEY_PRESSING(DIK_D))
	{
		vMoveDir += m_pGravityCom->Get_Terrain_Right_Dir();
		bTriger = TRUE;
	}
	if (bTriger)
	{
		vMoveDir.Normalize();
		m_pTransformCom->Go_Dir(vMoveDir, fTimeDelta);

		if(!m_pGravityCom->isJump())
			m_Weapons[m_iCurWeaponIndex]->Walk(fTimeDelta);

		m_fWalkSoundTimer += fTimeDelta;
		if (m_fWalkSoundTimer >= 0.39f && 
			!m_pGravityCom->isJump())
		{ 
			switch (rand() % 3)
			{
			case 0:
				m_pSoundCom->Play("stone_run001");
				break;
			case 1:
				m_pSoundCom->Play("stone_run002");
				break;
			case 2:
				m_pSoundCom->Play("stone_run003");
				break;
			}
			m_fWalkSoundTimer = 0.f;
		}
	}

	// 점프
	if (KEY_DOWN(DIK_SPACE) &&
		!m_pGravityCom->isJump())
	{
		m_pGravityCom->Jump(33.f);
		switch (rand() % 3)
		{
		case 0:
			m_pSoundCom->Play("stone_jump001");
			break;
		case 1:
			m_pSoundCom->Play("stone_jump002");
			break;
		case 2:
			m_pSoundCom->Play("stone_jump003");
			break;
		}
	}
	// 대쉬
	if (KEY_DOWN(DIK_LSHIFT)			&&
		!m_bDash						&&
		m_fDashTimer < -DASH_COOLTIME	&&
		bTriger)
	{
		// 이펙트 생성
		FX_MGR->PlayerDash(m_eLevelID);

		// 대쉬 스피드 설정
		m_pTransformCom->Set_SpeedPerSec(DASH_SPEED);

		// 이동방향 판별
		if (m_pGravityCom->isJump())
			vMoveDir.y = 0.f;
		vMoveDir.y = max(vMoveDir.y, 0.f);

		m_vDashDirection = vMoveDir.Normalize();
		
		m_pSoundCom->Play("fastwhoosh");
		m_pSoundCom->Play("Dash");
		// FOV 관련
		//_float fTargetFov{};
		//if (0 <= m_pTransformCom->Get_State(CTransform::STATE_LOOK)->Dot(m_vDashDirection))
		//	fTargetFov = RADIAN(50.f);
		//else
		//	fTargetFov = RADIAN(70.f);
		//m_pCameraManager->Zoom(fTargetFov, DASH_TIME * 0.5f);

		// 저스트 회피 콜라이더
		m_pPrePosCollider->Update_Collider();

		// 대쉬 초기화
		m_bDash = TRUE;
		m_fDashTimer = 0.f;
	}

	if (KEY_DOWN(DIK_LCONTROL) &&
		!m_bDash && ! m_pGravityCom->isJump() &&
		m_fDashTimer < -DASH_COOLTIME)
	{
		// 이펙트 생성
		FX_MGR->PlayerDash(m_eLevelID);

		// 이동방향 판별
		m_pGravityCom->Jump(60.f);
		//m_vDashDirection = {0.f,1.f,0.f};

		// 저스트 회피 콜라이더
		m_pPrePosCollider->Update_Collider();
		// 대쉬 초기화
		m_bDash = TRUE;
		m_fDashTimer = 0.f;

		//m_pSoundCom->Play("fastwhoosh")
		m_pSoundCom->Play("Dash");
		switch (rand() % 3)
		{
		case 0:
			m_pSoundCom->Play("stone_jump001");
			break;
		case 1:
			m_pSoundCom->Play("stone_jump002");
			break;
		case 2:
			m_pSoundCom->Play("stone_jump003");
			break;
		}
	}

	if (0 == m_iMaxWeaponIndex)
		return;

	_int iMouseWheelZ = m_pGameInstance->Get_DIMMoveState(DIMM_WHEEL);

	// 무기 교체
	if (iMouseWheelZ > 0)
	{
		m_iCurWeaponIndex--;
		if (m_iCurWeaponIndex < 0)
			m_iCurWeaponIndex = m_iMaxWeaponIndex;
		UIMGR->Change_Weapon(m_Weapons[m_iCurWeaponIndex]->Get_Info(), m_iCurWeaponIndex);
		UIMGR->Render_WeaponUI();
	}
	else if (iMouseWheelZ < 0)
	{
		m_iCurWeaponIndex++;
		if (m_iMaxWeaponIndex < m_iCurWeaponIndex)
			m_iCurWeaponIndex = 0;
		UIMGR->Change_Weapon(m_Weapons[m_iCurWeaponIndex]->Get_Info(), m_iCurWeaponIndex);
		UIMGR->Render_WeaponUI();
	}

	if (iMouseWheelZ)
	{
		m_Weapons[m_iCurWeaponIndex]->Set_State(CWeapon::ST_OPENING);
	}
}

void CPlayer::Init_Camera_Link()
{
	//카메라 매니저 가져옴
	m_pCameraManager = CAMERA_MANAGER;
	Safe_AddRef(m_pCameraManager);

	// FPS 카매라 뺴옴
	auto FPS_Camera = m_pCameraManager->Get_Camera(CCameraManager::FPS);

	// FPS 카메라의 트랜스폼 정보를 받아둠
	m_pCameraTransform = static_cast<CTransform*>(FPS_Camera->Find_Component(TEXT("Com_Transform")));
	Safe_AddRef(m_pCameraTransform);
}

void CPlayer::Update_Camera_Link()
{
	_float3 Scale = m_pTransformCom->Compute_Scaled();

	const _float3* pCameraRight = m_pCameraTransform->Get_State(CTransform::STATE_RIGHT);

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT,
		*pCameraRight * Scale.x);

	m_pTransformCom->Set_State(CTransform::STATE_LOOK,
		pCameraRight->Cross({ 0.f,1.f,0.f }) * Scale.z);

	m_pCameraTransform->Set_State(CTransform::STATE_POSITION,
		*m_pTransformCom->Get_State(CTransform::STATE_POSITION) + _float3{ 0.f,Scale.y * 0.5f ,0.f });
}

void CPlayer::Update_Dash(_float fTimeDelta)
{
	if (m_byJustDodgeFlag == 1)
	{
		if (m_fJustDodgeTimer >= 0.5f)
		{
			m_pGameInstance->Set_TimeScale(TEXT("Timer_60"), 1.f);
			m_byJustDodgeFlag = -1;
			m_bOnHit = FALSE;
			m_pCameraManager->Zoom(RADIAN(60.f), 0.2f);
			m_pCameraManager->Tilt(-m_fTiltAngle, 0.1f);

			//const _float3& vCameraLook = m_pCameraTransform->Get_State(CTransform::STATE_LOOK)->Normalize();
			//_float3 vRight = _VUp.Cross(vCameraLook);

			//m_pCameraTransform->Set_State(CTransform::STATE_RIGHT, vRight);
			//m_pCameraTransform->Set_State(CTransform::STATE_UP, vCameraLook.Cross(vRight));
		}

		m_fJustDodgeTimer += fTimeDelta * (1.f / DODGE_TIMESCALE);
		m_pTransformCom->Move(m_vDashDirection * 210.f * fTimeDelta); // 상숫값 : 저스트 회피시의 이동 속도
		return;
	}

	// 대쉬 끝났는지 판단
	m_fDashTimer += fTimeDelta;

	if (m_fDashTimer > DASH_TIME)
	{
		m_bDash = FALSE;
		m_byJustDodgeFlag = 0;
		//m_pCameraManager->Zoom(RADIAN(60.f), DASH_TIME * 2.f);
		m_pTransformCom->Set_SpeedPerSec(m_fInitSpeed);
		return;
	}

	if (m_fDashTimer < JUST_DASH_TIME)
		m_pGameInstance->Intersect(CG_PAWN_PRE, CG_MBULLET);

	m_pTransformCom->Go_Dir(m_vDashDirection, fTimeDelta);
	m_pTransformCom->Mul_SpeedPerSec(powf(DASH_DECAY, fTimeDelta));
}

void CPlayer::Ladder(_float fTimeDelta)
{
	if (KEY_PRESSING(DIK_W))
	{
		m_pTransformCom->Go_Down(fTimeDelta);
	}

	if (KEY_PRESSING(DIK_S))
	{
		m_pTransformCom->Go_Up(fTimeDelta);
	}
}

void CPlayer::On_Just_Dodge()
{
	// 1 = 회피중
	// 0 = 회피중 아님
	// -1 = 회피 끝났음(재 진입 방지)
	if (m_byJustDodgeFlag)
		return;

	m_pSoundCom->Play("Dodge");
	// 저스트 회피
	m_byJustDodgeFlag = 1;
	m_fJustDodgeTimer = 0.f;
	m_pGameInstance->Set_TimeScale(TEXT("Timer_60"), DODGE_TIMESCALE);
	CUI_Manager::Get_Instance()->Set_Face(CPortrait::PORTRAIT_HYPER);
	/*
	* 기울이는 방향 판정
	앞	Look과 내적 +
	뒤	Look과 내적 -
	우	Right과 내적 +
	좌	Right과 내적 -
	*/

	const _float3& vCameraLook = m_pCameraTransform->Get_State(CTransform::STATE_LOOK)->Normalize();
	const _float3& vCameraRight = m_pCameraTransform->Get_State(CTransform::STATE_RIGHT)->Normalize();

	_float fFwdDot = m_vDashDirection.Dot(vCameraLook);
	_float fRightDot = m_vDashDirection.Dot(vCameraRight);

	// 우선순위로 분기
	if (fabs(fFwdDot) > fabs(fRightDot))
	{
		m_fTiltAngle = 0.f;
		if (fFwdDot > 0.f)
			m_pCameraManager->Zoom(RADIAN(50.f), 0.5f * DODGE_TIMESCALE); // 앞 회피 = 확대
		else
			m_pCameraManager->Zoom(RADIAN(70.f), 0.5f * DODGE_TIMESCALE); // 뒤 회피 = 축소
	}
	else
	{
		if (fRightDot > 0.f)
			m_fTiltAngle = -RADIAN(12.f);
		else
			m_fTiltAngle = RADIAN(12.f);
		 
		m_pCameraManager->Tilt(m_fTiltAngle, 0.5f * DODGE_TIMESCALE); 
	}

}

void CPlayer::On_Hit(_int iDamage)
{
	if (m_bOnHit)
		return;

	if (m_bDash &&
		m_fDashTimer < JUST_DASH_TIME)
	{
		On_Just_Dodge();
		m_bOnHit = TRUE;
		return;
	}

	_float3 vCollisionDepth = CCollider::Get_Last_Collision_Depth();
	vCollisionDepth.y = 0.f;
	vCollisionDepth.Normalize();
	
	_float3 vLook = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	vLook.y = 0.f;
	vLook.Normalize();

	// 각도 구하기
	_float fRadian = atan2f(vLook.x * vCollisionDepth.z - vLook.z * vCollisionDepth.x,
		vLook.x * vCollisionDepth.x + vLook.z * vCollisionDepth.z);


	switch (rand() % 3)
	{
	case 0:
		m_pSoundCom->Play("hurt01");
		break;
	case 1:
		m_pSoundCom->Play("hurt02");
		break;
	case 2:
		m_pSoundCom->Play("hurt03");
		break;
	}

	m_tInfo.iArmor -= iDamage;
	FX_MGR->SpawnHitEffect(m_eLevelID);
	FX_MGR->Damage_Indicator(m_eLevelID,fRadian);
	m_pCameraManager->Shake_Camera(0.5f, 0.25f, 200.f, 80.f);
	CUI_Manager::Get_Instance()->Set_Face(CPortrait::PORTRAIT_ANGER);

	if (m_tInfo.iArmor <= 0)
	{
		// 음수니까 더해줘야겠지
		m_tInfo.iHP += m_tInfo.iArmor;
		m_tInfo.iArmor = 0;
		//플레이어는 안죽는다
	}

	m_bOnHit = TRUE;
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlayer* pInstance = new CPlayer(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer::Clone(void* pArg)
{
	CPlayer* pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();

	for (auto& Weapon : m_Weapons)
		Safe_Release(Weapon);
	m_Weapons.clear();

	Safe_Release(m_pGravityCom);
	Safe_Release(m_pCameraManager);
	Safe_Release(m_pCameraTransform);
	Safe_Release(m_pSoundCom);

	Safe_Release(m_pLeftHand);
}

CPlayer::CLeftHand::CLeftHand(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{

}

HRESULT CPlayer::CLeftHand::Initialize(void* pArg)
{
	// 여기서 컴포넌트 갖고오기 
	m_pTextureCom = dynamic_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_COMPONENT,
		LEVEL_STATIC, TEXT("Prototype_Component_Texture_LeftHandCard")));
	if (nullptr == m_pTextureCom)
		return E_FAIL;

	m_pVIBufferCom = dynamic_cast<CVIBuffer*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_COMPONENT,
		LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect")));
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	m_pTransformCom = dynamic_cast<CTransform*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_COMPONENT,
		LEVEL_STATIC, TEXT("Prototype_Component_Transform")));
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	m_pTransformCom->Set_SpeedPerSec(800.f);
	m_pTransformCom->Scaling(_float3(105.f, 243.f, 1.f) * 1.6f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(-350.f, -(g_iWinSizeY *  0.5f) - 200.f, 0.95f));

	m_fDepth = UI_PLAYER;

	return S_OK;
}

_bool CPlayer::CLeftHand::Move(_float fTimeDelta)
{
	/* 무브해라를 플레이어가 시켜주니까 폴스를 여기서 못해준다.*/
	/* fTimeAcc도 초기화를 못시켜줌 */

	/* byState == 0 올라가기, 1 멈춰있기, 2 내려가기 */

	if(m_byState == 0)
	{
		m_pTransformCom->Go_Up(fTimeDelta);
		if (m_pTransformCom->Get_State(CTransform::STATE_POSITION)->y >= -200.f)
			m_byState = 1;
	}	
	else if(m_byState == 1)
	{
		m_fTimeAcc += fTimeDelta;

		if (m_fTimeAcc >= 0.5f)
			m_byState = 2;
	}
	else if (m_byState == 2)
	{
		m_pTransformCom->Go_Down(fTimeDelta);
		if (m_pTransformCom->Get_State(CTransform::STATE_POSITION)->y <= -(g_iWinSizeY * 0.5f) - 200.f)
		{
			m_byState = 0;
			m_fTimeAcc = 0.f;
			return true;
		}
	}

	return false;
}

HRESULT CPlayer::CLeftHand::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Resource(static_cast<_uint>(0))))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return S_OK;
}

void CPlayer::CLeftHand::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
}
