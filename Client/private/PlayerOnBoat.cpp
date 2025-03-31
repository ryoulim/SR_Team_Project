#include "PlayerOnBoat.h"
#include "CameraManager.h"

CPlayerOnBoat::CPlayerOnBoat(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CPawn { pGraphic_Device }
{
}

CPlayerOnBoat::CPlayerOnBoat(const CPlayerOnBoat& Prototype)
	: CPawn (Prototype)
{
}

HRESULT CPlayerOnBoat::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayerOnBoat::Initialize(void* pArg)
{
	m_szTextureID = TEXT("PlayerOnBoat");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	Init_Camera_Link();

	return S_OK;
}

void CPlayerOnBoat::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CPlayerOnBoat::Update(_float fTimeDelta)
{
	if (m_bDead)
		return EVN_DEAD;


	if (m_pTransformCom->Get_State(CTransform::STATE_POSITION)->z > 9500.f
		|| m_pTransformCom->Get_State(CTransform::STATE_POSITION)->z < 100.f)
	{
		m_pTransformCom->Go_Straight(fTimeDelta  * 5.f);

		//누적시켰던 시간 값 초기화
		if (m_fTime != 0.f)
			m_fTime = 0.f;

		//초기화
		if (m_fStartPosX != 0.f)
			m_fStartPosX = 0.f;
	}

	else if (m_pTransformCom->Get_State(CTransform::STATE_POSITION)->z > 9000.f)
	{
		//선형보간을 위한 시작지점 x값 저장, 시작지점 z값은 9000.f로 고정되므로 저장할 필요 없음
		if (m_fStartPosX == 0.f)
		{
			m_fStartPosX = m_pTransformCom->Get_State(CTransform::STATE_POSITION)->x;
		}
		
		//시간을 누적해서 전달
		m_fTime += fTimeDelta * 0.7f;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			{ LerpToHole(m_fStartPosX, 450.f, m_fTime),
			17.f,
			LerpToHole(9000.f, 10000.f, m_fTime) });
	}
	
	else
	{
		Key_Input(fTimeDelta);

	return __super::Update(fTimeDelta);
}

void CPlayerOnBoat::Late_Update(_float fTimeDelta)
{
	if (m_pTransformCom->Get_State(CTransform::STATE_POSITION)->x > 600.f)
	{
		
	}

	if (m_pTransformCom->Get_State(CTransform::STATE_POSITION)->x < 300.f)
	{

	}

	Update_Camera_Link();

	if (m_pTransformCom->Get_State(CTransform::STATE_POSITION)->z > 13000.f)
		Change_Level();

	m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);

	__super::Late_Update(fTimeDelta);
}

HRESULT CPlayerOnBoat::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 254);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return __super::Render();

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

void CPlayerOnBoat::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	if (OtherColliderID == CI_TRIGGER)
		Change_Level();
}

void CPlayerOnBoat::Key_Input(_float fTimeDelta)
{
	if (KEY_PRESSING(DIK_W))
	{
		m_pTransformCom->Go_Straight(fTimeDelta);
	}
	if (KEY_PRESSING(DIK_S))
	{
		m_pTransformCom->Go_Backward(fTimeDelta);
	}
	if (KEY_PRESSING(DIK_A))
	{
		m_pTransformCom->Go_Left(fTimeDelta);
	}
	if (KEY_PRESSING(DIK_D))
	{
		m_pTransformCom->Go_Right(fTimeDelta);
	}
}

void CPlayerOnBoat::Init_Camera_Link()
{
	//카메라 매니저 가져옴
	m_pCameraManager = CAMERA_MANAGER;
	Safe_AddRef(m_pCameraManager);

	// TPS 카매라 뺴옴
	auto TPS_Camera = m_pCameraManager->Get_Camera(CCameraManager::TPS);

	// TPS 카메라의 트랜스폼 정보를 받아둠
	m_pCameraTransform = static_cast<CTransform*>(TPS_Camera->Find_Component(TEXT("Com_Transform")));
	Safe_AddRef(m_pCameraTransform);
}

void CPlayerOnBoat::Update_Camera_Link()
{	
	//카메라의 위치를 (플레이어 위치 + @)
	m_pCameraTransform->Set_State(CTransform::STATE_POSITION,
		*m_pTransformCom->Get_State(CTransform::STATE_POSITION) 
		+ _float3(0.f, 20.f, -80.f));// -20 50
}

_float CPlayerOnBoat::LerpToHole(_float StartPos, _float TargetPos, _float fTimeDelta)
{
	return (TargetPos - StartPos) * fTimeDelta + StartPos;
}

CPlayerOnBoat* CPlayerOnBoat::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlayerOnBoat* pInstance = new CPlayerOnBoat(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPlayerOnBoat");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayerOnBoat::Clone(void* pArg)
{
	CPlayerOnBoat* pInstance = new CPlayerOnBoat(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CPlayerOnBoat");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayerOnBoat::Free()
{
	__super::Free();

	Safe_Release(m_pCameraManager);
	Safe_Release(m_pCameraTransform);
}
