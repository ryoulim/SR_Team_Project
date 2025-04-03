// �� Ŭ���� �̸� : Player
// �θ� Ŭ���� �̸� : Pawn

#include "Player.h"
#include "GameInstance.h"
#include "CameraManager.h"
#include "FXMgr.h"

// ����� ��Ŭ���
//#include "Weapon_LoverBoy.h"
#include "UI_Manager.h"

#define DASH_TIME 0.25f
#define JUST_DASH_TIME 0.15f
#define DASH_SPEED 1500.f
#define DASH_COOLTIME 0.7f

// �����ð�
#define INVINCIBILITY_FRAMES 0.3f

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
	m_tInfo.iHP = 100;
	m_tInfo.iArmor = 0;
	m_fDashTimer = -DASH_COOLTIME;

	m_szTextureID = TEXT("MyCube");
	m_szBufferType = TEXT("Cube");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	Init_Camera_Link();

	Add_Weapons();

	CUI_Manager::Get_Instance()->Init_UI_To_Player(&m_tInfo);

	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	m_pGameInstance->Set_Listener_Position(m_pTransformCom, *m_pTransformCom->Get_State(CTransform::STATE_POSITION) - m_vPrePosition);

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
	Update_Camera_Link();

	m_Weapons[m_iCurWeaponIndex]->Priority_Update(fTimeDelta);
	__super::Priority_Update(fTimeDelta);
}

EVENT CPlayer::Update(_float fTimeDelta)
{
#ifdef _CONSOL
	_float3 vPosition = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	printf("�÷��̾� X : %f\n", vPosition.x);
	printf("�÷��̾� Y : %f\n", vPosition.y);
	printf("�÷��̾� Z : %f\n", vPosition.z);

#endif
	if (!m_bActive)
		return EVN_NONE;

	// �ǰ� �� �����ð� ���
	if (m_bOnHit)
	{
		m_fOnHitTimer += fTimeDelta;
		if (m_fOnHitTimer > DASH_COOLTIME)
		{
			m_bOnHit = FALSE;
			m_fOnHitTimer = 0.f;
		}
	}

	m_Weapons[m_iCurWeaponIndex]->Update(fTimeDelta);

	return __super::Update(fTimeDelta);
}

void CPlayer::Late_Update(_float fTimeDelta)
{
	m_vPrePosition = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);

	if (!m_bActive)
		return;

	m_Weapons[m_iCurWeaponIndex]->Late_Update(fTimeDelta);

	
	m_pCollider->Update_Collider();

	if (m_bOnLadder)
		m_bOnLadder = FALSE;
	else
		m_pGravityCom->Update(fTimeDelta);

	__super::Late_Update(fTimeDelta);	
}

HRESULT CPlayer::Render()
{
	if(m_bActive)
		return m_Weapons[m_iCurWeaponIndex]->Render();
	else
		return __super::Render();
}

void CPlayer::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	switch (OtherColliderID)
	{
	case CI_BLOCK_INVISIBLE:
		return;
		break;

	case CI_TRIGGER:
		Change_Level();
		break;

	case CI_ITEM_AMMO_CHAINGUN:
		m_Weapons[1]->Replenish_Ammo(5);
		break;
		
	case CI_ITEM_AMMO_DISPENSER_SCATTER:
		m_Weapons[2]->Replenish_Ammo(5);
		break;

	case CI_ITEM_AMMO_DISPENSER_CANNON:
		m_Weapons[2]->Replenish_Ammo(5);
		break;

	case CI_ITEM_AMMO_LOVERBOY:
		m_Weapons[0]->Replenish_Ammo(5);
		break;
	
	case CI_BLOCK_COMMON:
	{
		_float3 vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float3 vPos2 = *m_pCameraTransform->Get_State(CTransform::STATE_POSITION);

		_float3 Depth = m_pCollider->Get_Last_Collision_Depth();
		if (Depth.y != 0)
			int a = 1;
		vPos += Depth;
		vPos2 += Depth;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
		m_pCameraTransform->Set_State(CTransform::STATE_POSITION, vPos2);

		m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_pCameraTransform->Get_WorldMatrix_Inverse());
		break;
	}

		/* ���ͷ��� */
	case CI_INTERACTIVE_LAVA:
		On_Hit(5);
		break;

	case CI_INTERACTIVE_LADDER:
		m_bOnLadder = TRUE;
		m_pGravityCom->Stop_Jump();
		break;

	case CI_INTERACTIVE_SECURITY:
		// ī��Ű UI�� Ȱ��ȭ ��Ų��.
		// ���� EŰ�� �����ٸ�
		// 1. ���� �ö��
		// 2. ���Ŀ� ���� ���� << �ٵ� �̰� �� ���� ������Ʈ�ʿ��� �ϸ� �Ǵ°� �ƴϳ�?
		break;

		/* ���� ����ü*/
	case CI_MONSTER_SHOTGUNNER:
		On_Hit(10);
		break;
	case CI_MONSTER_CULTIST:
		On_Hit(10);
		break;
	case CI_MONSTER_GREATER:
		On_Hit(10);
		break;
	case CI_MONSTER_DEACON:
		On_Hit(10);
		break;
	case CI_MONSTER_NUKEMUTANT:
		On_Hit(10);
		break;
	case CI_MONSTER_ARCHANGEL:
		On_Hit(10);

		/* ���� ����*/
		break;
	case CI_MONSTER_MECHSECT:
		On_Hit(10);
		break;
	case CI_MONSTER_WENTEKO:
		On_Hit(10);
		break;

		/*���� ��ų*/
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

	WeaponDesc.fSpeedPerSec = 3000.f;
	m_Weapons.push_back(
		static_cast<CWeapon*>(m_pGameInstance->Clone_Prototype(
			PROTOTYPE::TYPE_GAMEOBJECT, LEVEL_STATIC,
			TEXT("Prototype_GameObject_Weapon_Chaingun"), &WeaponDesc)));

	WeaponDesc.fSpeedPerSec = 2600.f;
	m_Weapons.push_back(
		static_cast<CWeapon*>(m_pGameInstance->Clone_Prototype(
			PROTOTYPE::TYPE_GAMEOBJECT, LEVEL_STATIC,
			TEXT("Prototype_GameObject_Weapon_Dispenser"), &WeaponDesc)));

	CUI_Manager::Get_Instance()->Change_Weapon(m_Weapons[m_iCurWeaponIndex]->Get_Info());
}

void CPlayer::Key_Input(_float fTimeDelta)
{
	_bool bTriger{};

	// �̵�����
	if (KEY_PRESSING(DIK_W))
	{
		m_pGravityCom->Go_Straight_On_Terrain(fTimeDelta);
		bTriger = TRUE;
	}
	if (KEY_PRESSING(DIK_S))
	{
		m_pGravityCom->Go_Backward_On_Terrain(fTimeDelta);
		bTriger = TRUE;
	}
	if (KEY_PRESSING(DIK_A))
	{
		m_pGravityCom->Go_Left_On_Terrain(fTimeDelta);
		bTriger = TRUE;
	}
	if (KEY_PRESSING(DIK_D))
	{
		m_pGravityCom->Go_Right_On_Terrain(fTimeDelta);
		bTriger = TRUE;
	}
	if (bTriger)
		m_Weapons[m_iCurWeaponIndex]->Walk(fTimeDelta);

	// ����
	if (KEY_DOWN(DIK_SPACE))
	{
		m_pGravityCom->Jump(33.f);
	}
	// �뽬
	if (KEY_DOWN(DIK_LSHIFT)			&&
		!m_bDash						&&
		m_fDashTimer < -DASH_COOLTIME	&&
		m_vPrePosition != *m_pTransformCom->Get_State(CTransform::STATE_POSITION))
	{
		FX_MGR->PlayerDash(m_eLevelID);

		m_vDashDirection = (*m_pTransformCom->Get_State(CTransform::STATE_POSITION)
			- m_vPrePosition).Normalize() * DASH_SPEED;
		m_vDashDirection.y = 0.f;

		_float fTargetFov{};
		if (0 <= m_pTransformCom->Get_State(CTransform::STATE_LOOK)->Dot(m_vDashDirection))
			fTargetFov = RADIAN(50.f);
		else
			fTargetFov = RADIAN(70.f);

		m_pCameraManager->Zoom(fTargetFov, DASH_TIME * 0.5f);
		m_bDash = TRUE;
		m_fDashTimer = 0.f;
	}

	// ���� ��ü
	bTriger = FALSE;
	if (KEY_DOWN(DIK_Q))
	{
		m_iCurWeaponIndex--;
		if (m_iCurWeaponIndex < 0)
			m_iCurWeaponIndex = m_iMaxWeaponIndex;
		CUI_Manager::Get_Instance()->Change_Weapon(m_Weapons[m_iCurWeaponIndex]->Get_Info());
		bTriger = TRUE;
	}
	if (KEY_DOWN(DIK_E))
	{
		m_iCurWeaponIndex++;
		if (m_iMaxWeaponIndex < m_iCurWeaponIndex)
			m_iCurWeaponIndex = 0;
		CUI_Manager::Get_Instance()->Change_Weapon(m_Weapons[m_iCurWeaponIndex]->Get_Info());
		bTriger = TRUE;
	}

	if (bTriger)
	{
		m_Weapons[m_iCurWeaponIndex]->Set_State(CWeapon::ST_OPENING);
	}

	// ���� Ű ����
	m_Weapons[m_iCurWeaponIndex]->Key_Input();
}

void CPlayer::Init_Camera_Link()
{
	//ī�޶� �Ŵ��� ������
	m_pCameraManager = CAMERA_MANAGER;
	Safe_AddRef(m_pCameraManager);

	// FPS ī�Ŷ� ����
	auto FPS_Camera = m_pCameraManager->Get_Camera(CCameraManager::FPS);

	// FPS ī�޶��� Ʈ������ ������ �޾Ƶ�
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
	m_fDashTimer += fTimeDelta;
	if (m_fDashTimer > DASH_TIME)
	{
		m_bDash = FALSE;
		m_pCameraManager->Zoom(RADIAN(60.f), DASH_TIME * 2.f);
	}

	_float3 vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float fFloorY = m_pGravityCom->Get_FloorY();

	vPos += (m_vDashDirection * fTimeDelta);

	if (vPos.y < fFloorY)
		vPos.y = fFloorY;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION,vPos);

	m_vDashDirection *= 0.85f;
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

void CPlayer::On_Hit(_uint iDamage)
{
	if (m_bOnHit)
		return;

	iDamage -= m_tInfo.iArmor;
	m_tInfo.iArmor -= iDamage;
	if (m_tInfo.iArmor < 0)
		m_tInfo.iArmor = 0;
	m_tInfo.iHP -= iDamage;

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
}