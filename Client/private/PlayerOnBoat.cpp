#include "PlayerOnBoat.h"
#include "CameraManager.h"
#include "PBState.h"
#include "WaterBoat.h"
#include "UI_Manager.h"
#include "PlayerMissile.h"

#define BULLET_COOLTIME 0.5f

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

	m_pState[DECEL] = new CPBState_Decel(this);
	m_pState[NORMAL] = new CPBState_Normal(this);
	m_pState[LERP] = new CPBState_Lerp(this);
	m_pState[ACCEL] = new CPBState_Accel(this);

	m_pCurState = m_pState[NORMAL];

	m_fWaterSpeed = static_cast<DESC*>(pArg)->fSpeedPerSec;


	//셰이더 장착
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Particle"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

void CPlayerOnBoat::Priority_Update(_float fTimeDelta)
{
	//Key_Input(fTimeDelta);

#ifdef _CONSOL
	_float3 vPosition = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	printf("플레이어 좌표 : { %.2f, %.2f, %.2f }\n", vPosition.x, vPosition.y, vPosition.z);
#endif

	//이전 상태와 현재 상태가 다르다면 Enter 실행
	if (m_eCurState != m_ePreState)
	{
		m_pCurState->Enter(fTimeDelta);
		m_ePreState = m_eCurState;

		if (m_pCurState == m_pState[NORMAL])
			SpawnWaterParticle(m_fWaterSpeed);
	}

	//Exectue는 무조건 실행
	m_pCurState->Execute(fTimeDelta);

	Update_Camera_Link();

#pragma region 파티클 
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

#pragma endregion

	__super::Priority_Update(fTimeDelta);
}

EVENT CPlayerOnBoat::Update(_float fTimeDelta)
{
	m_fBulletTimer += fTimeDelta;
	return __super::Update(fTimeDelta);
}

void CPlayerOnBoat::Late_Update(_float fTimeDelta)
{
	const _float3& vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	if (vPos.z > 13000.f)
		Change_Level();

	m_vPrePos = vPos;

	m_pCollider->Update_Collider();

	m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLEND, this);

	__super::Late_Update(fTimeDelta);
}

HRESULT CPlayerOnBoat::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 254);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Resource(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	/* -----------------------그림자----------------------------- */
	
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, false);

	_float3 vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);	vPos.z += -30.f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Resource(1)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	vPos.z += 30.f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);

	return S_OK;
}

void CPlayerOnBoat::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	if (OtherColliderID == CI_TRIGGER)
		Change_Level();
}

void CPlayerOnBoat::Key_Input(_float fTimeDelta)
{
	// 키 타이머 -> 왼쪽 = 음수, 오른쪽 = 양수
	_float fAccelRate = 2.5f; // 클수록 더 빠르게 가속됨
	_float fSpeed = 0.f;


	if (KEY_PRESSING(DIK_W))
	{
		m_fSpeedRatio += fTimeDelta;
		m_fSpeedRatio = max(m_fSpeedRatio, 1.f); // 최소 -1
		m_pTransformCom->Set_SpeedPerSec(m_fSpeedRatio * RACE_SPEED_PER_SEC);
	}
	if (KEY_PRESSING(DIK_S))
	{
		m_fSpeedRatio *= powf(0.5f, fTimeDelta * 5.f); // 감쇠율 조절 가능
		if (fabsf(m_fSpeedRatio) < 0.05f)
			m_fSpeedRatio = 0.05f;

		m_pTransformCom->Set_SpeedPerSec(m_fSpeedRatio * RACE_SPEED_PER_SEC);
	}
	if (KEY_PRESSING(DIK_A))
	{
		m_fKeyTimer -= fTimeDelta;
		m_fKeyTimer = max(m_fKeyTimer, -1.f); // 최소 -1
		m_pTransformCom->Rotation({ 0.f,0.f,1.f }, RADIAN(11.f));
	}
	else if (KEY_PRESSING(DIK_D))
	{
		m_fKeyTimer += fTimeDelta;
		m_fKeyTimer = min(m_fKeyTimer, 1.f); // 최대 1
		m_pTransformCom->Rotation({ 0.f,0.f,1.f }, RADIAN(-11.f));
	}
	else
	{
		// 자연스럽게 0으로 수렴
		m_fKeyTimer *= powf(0.5f, fTimeDelta * 5.f); // 감쇠율 조절 가능
		if (fabsf(m_fKeyTimer) < 0.01f)
			m_fKeyTimer = 0.f;
		m_pTransformCom->Rotation({ 0.f,0.f,1.f }, 0.f);
	}
	CUI_Manager::Get_Instance()->Set_RacingSpeed(_int(fabsf(m_fKeyTimer) * 30));

	// 지수 함수: y = sign(x) * (1 - e^(-a * |x|)) 
	_float fSign = (m_fKeyTimer >= 0.f) ? 1.f : -1.f;
	_float fExpFactor = 1.f - expf(-fAccelRate * fabsf(m_fKeyTimer));
	fSpeed = fSign * fExpFactor * RACE_SPEED_PER_SEC;
	m_pTransformCom->Move({ fSpeed, 0.f, 0.f }, fTimeDelta);

	_float3 vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	// 카메라를 살짝 늦게 따라오게 하고
	// 카메라도 기울이고
	// 부딪혔을때 살짝 흔들고
	// 뭐시기 뭐 카메라연출해야할듯?
	if (vPos.x < 315.f)
	{
		vPos.x = 315.f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
		m_fKeyTimer = 0.f;
	}
	if (585.f < vPos.x)
	{
		vPos.x = 585.f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
		m_fKeyTimer = 0.f;
	}


	if (MOUSE_PRESSING(DIMK_LBUTTON))
	{
		Create_Bullet();
	}
}

void CPlayerOnBoat::Key_Input2(_float fTimeDelta)
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

void CPlayerOnBoat::SpawnWaterParticle(_float fWaterSpeed)
{
	/* [ 물보라 파티클 01번 ] */
	CPSystem::DESC WaterBoatDesc{};
	WaterBoatDesc.vPosition = { 0.f, 0.f, 0.f };
	WaterBoatDesc.szTextureTag = TEXT("WaterBoat");
	WaterBoatDesc.iParticleNums = 150;
	WaterBoatDesc.fMaxFrame = 1.f;
	WaterBoatDesc.fSize = 0.06f;
	WaterBoatDesc.fNum = fWaterSpeed;

	CGameObject* pObject = nullptr;
	CGameObject** ppOut = &pObject;
	if (FAILED(m_pGameInstance->Add_GameObjectReturn(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_WaterBoat"),
		m_eLevelID, L"Layer_Particle", ppOut, &WaterBoatDesc)))
		return;

	m_pWaterBoatEffect_01 = *ppOut;

	/* [ 물보라 파티클 02번 ] */
	CPSystem::DESC WaterBoatDesc2{};
	WaterBoatDesc2.vPosition = { 0.f, 0.f, 0.f };
	WaterBoatDesc2.szTextureTag = TEXT("WaterBoat");
	WaterBoatDesc2.iParticleNums = 500;
	WaterBoatDesc2.fMaxFrame = 1.f;
	WaterBoatDesc2.fSize = 0.03f;
	WaterBoatDesc2.fNum = fWaterSpeed;

	CGameObject* pObject2 = nullptr;
	CGameObject** ppOut2 = &pObject2;
	if (FAILED(m_pGameInstance->Add_GameObjectReturn(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_WaterBoat"),
		m_eLevelID, L"Layer_Particle", ppOut2, &WaterBoatDesc2)))
		return;

	m_pWaterBoatEffect_02 = *ppOut2;

	/* [ 물보라 파티클 03번 ] */
	CPSystem::DESC WaterBoatDesc3{};
	WaterBoatDesc3.vPosition = { 0.f, 0.f, 0.f };
	WaterBoatDesc3.szTextureTag = TEXT("WaterBoat");
	WaterBoatDesc3.iParticleNums = 1000;
	WaterBoatDesc3.fMaxFrame = 1.f;
	WaterBoatDesc3.fSize = 0.01f;
	WaterBoatDesc3.fNum = fWaterSpeed;

	CGameObject* pObject3 = nullptr;
	CGameObject** ppOut3 = &pObject2;
	if (FAILED(m_pGameInstance->Add_GameObjectReturn(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_WaterBoat"),
		m_eLevelID, L"Layer_Particle", ppOut3, &WaterBoatDesc3)))
		return;

	m_pWaterBoatEffect_03 = *ppOut3;

}

void CPlayerOnBoat::Create_Bullet()
{
	if (BULLET_COOLTIME > m_fBulletTimer)
		return;

	CPlayerMissile::DESC MissileDesc{};
	MissileDesc.fSpeedPerSec = RACE_PBULLET_SPEED;
	MissileDesc.vPosition = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	MissileDesc.vScale = { 20.f,20.f,20.f };
	MissileDesc.iColliderID = CI_MISSILE;
	MissileDesc.vLook = RACE_PBULLET_DIR;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PlayerMissile"),
		m_eLevelID, TEXT("Layer_Bullet"), &MissileDesc)))
		return;

	m_fBulletTimer = 0.f;
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
	Safe_Release(m_pShaderCom);

	for (size_t i = DECEL; i < NON; ++i)
		Safe_Release(m_pState[i]);
}
