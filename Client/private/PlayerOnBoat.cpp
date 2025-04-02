#include "PlayerOnBoat.h"
#include "CameraManager.h"

#include "PBState_Accel.h"
#include "PBState_Normal.h"
#include "PBState_Decel.h"
#include "PBState_Lerp.h"

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

	m_pState[DECEL] = new CPBState_Decel();
	m_pState[NORMAL] = new CPBState_Normal();
	m_pState[LERP] = new CPBState_Lerp();
	m_pState[ACCEL] = new CPBState_Accel();

	m_pCurState = m_pState[NORMAL];

	return S_OK;
}

void CPlayerOnBoat::Priority_Update(_float fTimeDelta)
{
	//이전 상태와 현재 상태가 다르다면 Enter 실행
	if (m_eCurState != m_ePreState)
	{
		m_pCurState->Enter(this, fTimeDelta);
		m_ePreState = m_eCurState;
	}

	//Exectue는 무조건 실행
	m_pCurState->Execute(this, fTimeDelta);

	Update_Camera_Link();

	__super::Priority_Update(fTimeDelta);
}

EVENT CPlayerOnBoat::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CPlayerOnBoat::Late_Update(_float fTimeDelta)
{
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
	if (KEY_PRESSING(DIK_A))
	{
		m_pTransformCom->Go_LeftOnRace(fTimeDelta);
	}
	if (KEY_PRESSING(DIK_D))
	{
		m_pTransformCom->Go_RightOnRace(fTimeDelta);
	}
}

_float CPlayerOnBoat::Compute_StartPosX()
{
	return m_pTransformCom->Get_State(CTransform::STATE_POSITION)->x;
}

_float CPlayerOnBoat::Compute_CurPosZ()
{
	return m_pTransformCom->Get_State(CTransform::STATE_POSITION)->z;
}

void CPlayerOnBoat::Move_Lerp(_float fStartPosX, _float fTime)
{
	_float3 vStartPos = { fStartPosX, 17.f, 9000.f };
	_float3 vEndPos = { 450.f, 17.f, 9800.f };

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Lerp(vStartPos, vEndPos, fTime));
}

void CPlayerOnBoat::Set_State(STATE eState)
{
	m_pCurState = m_pState[eState];
	m_eCurState = eState;
}

void CPlayerOnBoat::Go_Straight(_float fTimeDelta)
{
	m_pTransformCom->Go_Straight(fTimeDelta);
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

	for (size_t i = DECEL; i < NON; ++i)
		Safe_Release(m_pState[i]);
}
