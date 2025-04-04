#include "PlayerOnBoat.h"
#include "CameraManager.h"

#include "PBState_Accel.h"
#include "PBState_Normal.h"
#include "PBState_Decel.h"
#include "PBState_Lerp.h"
#include "WaterBoat.h"

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


	/* [ 물보라 파티클 01번 ] */
	CPSystem::DESC WaterBoatDesc{};
	WaterBoatDesc.vPosition = { 0.f, 0.f, 0.f };
	WaterBoatDesc.szTextureTag = TEXT("WaterBoat");
	WaterBoatDesc.iParticleNums = 50;
	WaterBoatDesc.fMaxFrame = 1.f;
	WaterBoatDesc.fSize = 0.06f;
	WaterBoatDesc.fNum = static_cast<DESC*>(pArg)->fSpeedPerSec;

	CGameObject* pObject = nullptr;
	CGameObject** ppOut = &pObject;
	if (FAILED(m_pGameInstance->Add_GameObjectReturn(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_WaterBoat"),
		m_eLevelID, L"Layer_Particle", ppOut, &WaterBoatDesc)))
		return E_FAIL;

	m_pWaterBoatEffect_01 = *ppOut;

	/* [ 물보라 파티클 02번 ] */
	CPSystem::DESC WaterBoatDesc2{};
	WaterBoatDesc2.vPosition = { 0.f, 0.f, 0.f };
	WaterBoatDesc2.szTextureTag = TEXT("WaterBoat");
	WaterBoatDesc2.iParticleNums = 200;
	WaterBoatDesc2.fMaxFrame = 1.f;
	WaterBoatDesc2.fSize = 0.03f;
	WaterBoatDesc2.fNum = static_cast<DESC*>(pArg)->fSpeedPerSec;

	CGameObject* pObject2 = nullptr;
	CGameObject** ppOut2 = &pObject2;
	if (FAILED(m_pGameInstance->Add_GameObjectReturn(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_WaterBoat"),
		m_eLevelID, L"Layer_Particle", ppOut2, &WaterBoatDesc2)))
		return E_FAIL;

	m_pWaterBoatEffect_02 = *ppOut2;

	/* [ 물보라 파티클 03번 ] */
	CPSystem::DESC WaterBoatDesc3{};
	WaterBoatDesc3.vPosition = { 0.f, 0.f, 0.f };
	WaterBoatDesc3.szTextureTag = TEXT("WaterBoat");
	WaterBoatDesc3.iParticleNums = 1000;
	WaterBoatDesc3.fMaxFrame = 1.f;
	WaterBoatDesc3.fSize = 0.01f;
	WaterBoatDesc3.fNum = static_cast<DESC*>(pArg)->fSpeedPerSec;
	//WaterBoatDesc2.

	CGameObject* pObject3 = nullptr;
	CGameObject** ppOut3 = &pObject2;
	if (FAILED(m_pGameInstance->Add_GameObjectReturn(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_WaterBoat"),
		m_eLevelID, L"Layer_Particle", ppOut3, &WaterBoatDesc3)))
		return E_FAIL;

	m_pWaterBoatEffect_03 = *ppOut3;

	return S_OK;
}

void CPlayerOnBoat::Priority_Update(_float fTimeDelta)
{
	Key_Input(fTimeDelta);

	Update_Camera_Link();

	__super::Priority_Update(fTimeDelta);
}

EVENT CPlayerOnBoat::Update(_float fTimeDelta)
{
	//이전 상태와 현재 상태가 다르다면 Enter 실행
	if (m_eCurState != m_ePreState)
	{
		m_pCurState->Enter(this, fTimeDelta);
		m_ePreState = m_eCurState;
	}

	//Exectue는 무조건 실행
	m_pCurState->Execute(this, fTimeDelta);

	m_pCollider->Update_Collider();

	/* 따라오는 파티클 */
	if (m_pWaterBoatEffect_01)
	{
		static_cast<CWaterBoat*>(m_pWaterBoatEffect_01)->SetPosition(*m_pTransformCom->Get_State(CTransform::STATE_POSITION));
		static_cast<CWaterBoat*>(m_pWaterBoatEffect_02)->SetPosition(*m_pTransformCom->Get_State(CTransform::STATE_POSITION));
		static_cast<CWaterBoat*>(m_pWaterBoatEffect_03)->SetPosition(*m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	}

	/* 통로로 들어갈 때 파티클을 제거하자 */
	if (m_pCurState == m_pState[ACCEL])
	{
		/* [ 이펙트를 반납하고 가시오 ] */
		if (m_pWaterBoatEffect_01)
		{
			static_cast<CWaterBoat*>(m_pWaterBoatEffect_01)->SetDead();
			static_cast<CWaterBoat*>(m_pWaterBoatEffect_02)->SetDead();
			static_cast<CWaterBoat*>(m_pWaterBoatEffect_03)->SetDead();
			m_pWaterBoatEffect_01 = nullptr;
			m_pWaterBoatEffect_02 = nullptr;
			m_pWaterBoatEffect_03 = nullptr;
		}
	}


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

void CPlayerOnBoat::StoreSlidingSpeed(_float fSpeed)
{
	m_fSlidingSpeed = fSpeed;
}

_float CPlayerOnBoat::GetSlidingSpeed()
{
	return m_fSlidingSpeed;
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
