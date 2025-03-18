// 내 클래스 이름 : Player
// 부모 클래스 이름 : Pawn

#include "Player.h"
#include "GameInstance.h"
#include "CameraManager.h"

// 무기들 인클루드
#include "Weapon_LoverBoy.h"

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
	m_eLevelID = LEVEL_GAMEPLAY;
	m_szTextureID = TEXT("MyCube");
	m_szBufferType = TEXT("Cube");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	
	m_pCameraManager = static_cast<CCameraManager*>(m_pGameInstance->Find_Object(m_eLevelID, TEXT("Layer_Camera")));
	Safe_AddRef(m_pCameraManager);

	auto FPS_Camera = m_pCameraManager->Get_Camera(CCameraManager::FPS);

	m_pCameraTransform = static_cast<CTransform*>(FPS_Camera->Find_Component(TEXT("Com_Transform")));
	Safe_AddRef(m_pCameraTransform);

	if (pArg != nullptr)
	{
		m_fMouseSensor = static_cast<DESC*>(pArg)->fMouseSensor;
	}
	m_pGravityCom->Set_JumpOption(CGravity::JUMPDESC{ 13.2f, 1140.f});

	m_Weapons.push_back(
		static_cast<CWeapon*>(m_pGameInstance->Clone_Prototype(
			PROTOTYPE::TYPE_GAMEOBJECT, LEVEL_GAMEPLAY, 
			TEXT("Prototype_GameObject_Weapon_LoverBoy"))));

	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
	if (KEY_DOWN(VK_F1))
	{
		m_bFpsMode = !m_bFpsMode;
		m_pCameraManager->Switch(m_bFpsMode);
	}
	if (m_bFpsMode)
	{
		Key_Input(fTimeDelta);
		if (!m_bBouseFixMod)
		{
			Mouse_Move();
			Mouse_Fix();
		}
		m_Weapons[m_iCurWeaponIndex]->Priority_Update(fTimeDelta);
		__super::Priority_Update(fTimeDelta);
	}
}

EVENT CPlayer::Update(_float fTimeDelta)
{
	if (!m_bFpsMode)
		return EVN_NONE;

	m_Weapons[m_iCurWeaponIndex]->Update(fTimeDelta);
	return __super::Update(fTimeDelta);
}

void CPlayer::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);

	if (!m_bFpsMode)
		return;

	Update_Camera_Link();

	m_pCollider->Update_Collider(m_pTransformCom);

	m_pGravityCom->Update(fTimeDelta);

	m_Weapons[m_iCurWeaponIndex]->Late_Update(fTimeDelta);

	__super::Late_Update(fTimeDelta);	
}

HRESULT CPlayer::Render()
{
	if(m_bFpsMode)
		return m_Weapons[m_iCurWeaponIndex]->Render();
	else
		return __super::Render();
}

void CPlayer::On_Collision(CGameObject* pCollisionedObject, const _wstring& strLayerTag)
{
	//m_pCollider->Get_Last_Collision_Pos();

	_float3 vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vPos2 = *m_pCameraTransform->Get_State(CTransform::STATE_POSITION);

	_float3 Depth = m_pCollider->Get_Last_Collision_Depth();

	vPos += Depth;
	vPos2 += Depth;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	m_pCameraTransform->Set_State(CTransform::STATE_POSITION, vPos2);
	
}

void CPlayer::Key_Input(_float fTimeDelta)
{
	if (KEY_PRESSING('W'))
	{
		m_pGravityCom->Go_Straight_On_Terrain(fTimeDelta);
	}
	if (KEY_PRESSING('S'))
	{
		m_pGravityCom->Go_Backward_On_Terrain(fTimeDelta);
	}
	if (KEY_PRESSING('A'))
	{
		m_pGravityCom->Go_Left_On_Terrain(fTimeDelta);
	}
	if (KEY_PRESSING('D'))
	{
		m_pGravityCom->Go_Right_On_Terrain(fTimeDelta);
	}
	if (KEY_DOWN(VK_TAB))
	{
		m_bBouseFixMod = !m_bBouseFixMod;
	}
	if (KEY_DOWN(VK_SPACE))
	{
		m_pGravityCom->Jump(50.f);
	}

	if (KEY_DOWN(VK_LBUTTON))
	{
		m_Weapons[m_iCurWeaponIndex]->Set_State(CWeapon::ST_W_ATK);
	}
	if (KEY_DOWN('R'))
	{
		m_Weapons[m_iCurWeaponIndex]->Set_State(CWeapon::ST_RELOAD);
	}
}

void CPlayer::Mouse_Move()
{
	_float		fMouseMoveX = { static_cast<_float>(m_pGameInstance->Get_DIMMoveState(DIMM_X)) };
	_float		fMouseMoveY = { static_cast<_float>(m_pGameInstance->Get_DIMMoveState(DIMM_Y)) };

	_float3		vRotationAxis = (*m_pCameraTransform->Get_State(CTransform::STATE_RIGHT) * fMouseMoveY)
		+ (*m_pCameraTransform->Get_State(CTransform::STATE_UP) * fMouseMoveX);

	_float fAngle = RADIAN(_float3(fMouseMoveX, fMouseMoveY, 0).Length() * m_fMouseSensor);

	_float3 vLook = *m_pCameraTransform->Get_State(CTransform::STATE_LOOK);

	_float4x4 matRot{ vRotationAxis,fAngle };
	vLook.TransformNormal(matRot);
	m_pCameraTransform->LookAt(*m_pCameraTransform->Get_State(CTransform::STATE_POSITION) + vLook);
}

void CPlayer::Mouse_Fix()
{
	POINT		ptMouse{ g_iWinSizeX >> 1, g_iWinSizeY >> 1 };

	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}

#include "TestBullet.h"
void CPlayer::Create_Bullet()
{
	CTestBullet::DESC BulletDesc{};
	BulletDesc.fSpeedPerSec = 8000.f;
	BulletDesc.vInitPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	BulletDesc.vScale = {3.f,3.f,3.f};
	BulletDesc.vLook = *m_pCameraTransform->Get_State(CTransform::STATE_LOOK);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_TestBullet"),
		LEVEL_GAMEPLAY, TEXT("Layer_PBullet"), &BulletDesc)))
		return;
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

	Safe_Release(m_pCameraManager);
	Safe_Release(m_pCameraTransform);
}