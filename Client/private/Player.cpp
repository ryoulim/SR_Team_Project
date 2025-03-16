// 내 클래스 이름 : Player
// 부모 클래스 이름 : Pawn

#include "Player.h"
#include "GameInstance.h"
#include "CameraManager.h"

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

	m_pFPSCameraTransform = static_cast<CTransform*>(FPS_Camera->Find_Component(TEXT("Com_Transform")));
	Safe_AddRef(m_pFPSCameraTransform);

	if (pArg != nullptr)
	{
		m_fMouseSensor = static_cast<DESC*>(pArg)->fMouseSensor;
	}
	m_pGravityCom->Set_JumpOption(CGravity::JUMPDESC{ 13.2f, 1140.f});

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
	}
	__super::Priority_Update(fTimeDelta);
}

EVENT CPlayer::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CPlayer::Late_Update(_float fTimeDelta)
{
	if (m_bFpsMode)
	{
		m_pGravityCom->Update(fTimeDelta);

		_float3 Scale = m_pTransformCom->Compute_Scaled();

		m_pTransformCom->Set_State(CTransform::STATE_RIGHT,
			*m_pFPSCameraTransform->Get_State(CTransform::STATE_RIGHT) * Scale.x);

		m_pTransformCom->Set_State(CTransform::STATE_UP,
			*m_pFPSCameraTransform->Get_State(CTransform::STATE_UP) * Scale.y);

		m_pTransformCom->Set_State(CTransform::STATE_LOOK,
			*m_pFPSCameraTransform->Get_State(CTransform::STATE_LOOK) * Scale.z);

		m_pFPSCameraTransform->Set_State(CTransform::STATE_POSITION,
			*m_pTransformCom->Get_State(CTransform::STATE_POSITION) + _float3{ 0.f,Scale.y * 0.5f ,0.f });

		__super::Late_Update(fTimeDelta);
	}
}

HRESULT CPlayer::Render()
{
	return __super::Render();
}

void CPlayer::On_Collision()
{
	//m_pCollider->Get_Last_Collision_Pos();

	_float3 vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vPos2 = *m_pFPSCameraTransform->Get_State(CTransform::STATE_POSITION);

	_float3 Depth = m_pCollider->Get_Last_Collision_Depth();

	vPos += Depth;
	vPos2 += Depth;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	m_pFPSCameraTransform->Set_State(CTransform::STATE_POSITION, vPos2);
	
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
		m_pTransformCom->Go_Left(fTimeDelta);
	}
	if (KEY_PRESSING('D'))
	{
		m_pTransformCom->Go_Right(fTimeDelta);
	}
	if (KEY_DOWN(VK_TAB))
	{
		m_bBouseFixMod = !m_bBouseFixMod;
	}
	if (KEY_DOWN(VK_SPACE))
	{
		m_pGravityCom->Jump(50.f);
	}
}

void CPlayer::Mouse_Move()
{
	_float		fMouseMoveX = { static_cast<_float>(m_pGameInstance->Get_DIMMoveState(DIMM_X)) };
	_float		fMouseMoveY = { static_cast<_float>(m_pGameInstance->Get_DIMMoveState(DIMM_Y)) };

	_float3		vRotationAxis = (*m_pFPSCameraTransform->Get_State(CTransform::STATE_RIGHT) * fMouseMoveY)
		+ (*m_pFPSCameraTransform->Get_State(CTransform::STATE_UP) * fMouseMoveX);

	_float fAngle = RADIAN(_float3(fMouseMoveX, fMouseMoveY, 0).Length() * m_fMouseSensor);

	_float3 vLook = *m_pFPSCameraTransform->Get_State(CTransform::STATE_LOOK);

	_float4x4 matRot{ vRotationAxis,fAngle };
	vLook.TransformNormal(matRot);
	m_pFPSCameraTransform->LookAt(*m_pFPSCameraTransform->Get_State(CTransform::STATE_POSITION) + vLook);
}

void CPlayer::Mouse_Fix()
{
	POINT		ptMouse{ g_iWinSizeX >> 1, g_iWinSizeY >> 1 };

	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
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
	Safe_Release(m_pCameraManager);
	Safe_Release(m_pFPSCameraTransform);
}