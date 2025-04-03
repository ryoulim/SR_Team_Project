// 내 클래스 이름 : TestMonster
// 부모 클래스 이름 : Monster

#include "Mechsect.h"
#include "FXMgr.h"
#include "MonsterNormalBullet.h"

CMechsect::CMechsect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster{ pGraphic_Device }
{
}

CMechsect::CMechsect(const CMechsect& Prototype)
	: CMonster(Prototype)
{
}

HRESULT CMechsect::Initialize_Prototype()
{
	//프로토타입의 기본정의
	m_szTextureID = TEXT("Mechsect_Walk");
	m_szBufferType = TEXT("Rect");

	//속성
	m_iHP = 50;
	m_iMaxHP = 50;
	m_iAttackPower = 2;
	m_iDefense = 1;
	m_fSpeed = 20.f;
	m_vScale = { 43.2f, 24.8f, 1.f };
	//108 62
	m_eState = MODE::MODE_IDLE;

	m_fDetectiveDistance = 300.f;

	//부속성
	m_strDialogue = "Mechs..Mec...";
	m_strSound = "SoundFilePath";

	m_vDropItems.push_back("MechsectDropItem");

	return S_OK;
}

HRESULT CMechsect::Initialize(void* pArg)
{
	//위치, 크기초기화, 컴포넌트 부착
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_fDivOffset = 45.f;
	//애니메이션(수정예정)
	m_fAnimationMaxFrame = 4.f;
	m_fAnimationSpeed = 5.f;
	m_iState = STATE_MOVE;
	m_fCooldownTime = 1.f;     // 공격 쉬는 텀
	m_fBulletCooldown = 0.4f;	// 총알 발사 쿨
	m_fAttackTime = 0.35f;		// 공격 시간
	return S_OK;
}

void CMechsect::Priority_Update(_float fTimeDelta)
{
	Set_Animation();
	__super::Priority_Update(fTimeDelta);
}

EVENT CMechsect::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CMechsect::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	if (m_bRotateAnimation == false)
		m_iDegree = 0;
	Resize_Texture(0.4f);
}

HRESULT CMechsect::Render()
{
	return __super::Render();

	//특별히 더 렌더링 할게 있는 경우 ↓
}

void CMechsect::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	__super::On_Collision_NormalMonster(MyColliderID, OtherColliderID);
}

void CMechsect::MonsterTick(_float dt)
{
	//상태변화
	switch (m_eState)
	{
	case MODE::MODE_IDLE:
		State_Change_IDLE(dt);
		break;

	case MODE::MODE_DETECTIVE:
		State_Change_DETECTIVE(dt);
		break;

	case MODE::MODE_READY:
		State_Change_READY(dt);
		break;

	case MODE::MODE_BATTLE:
		State_Change_BATTLE(dt);
		break;

	case MODE::MODE_DEAD:
		break;

	case MODE::MODE_RETURN:
		//본래위치로 돌아가고 IDLE로 상태가 변한다.
		m_bFoundPlayer = false;
		break;
	}

#ifdef _CONSOL
	Debug_Output();
#endif

	// 상태행동(액션)
	switch (m_eState)
	{
	case MODE::MODE_IDLE:
		DoIdle(dt);
		break;

	case MODE::MODE_DETECTIVE:
		DoDetect(dt);
		break;

	case MODE::MODE_READY:
		DoReady(dt);
		break;

	case MODE::MODE_BATTLE:
		DoBattle(dt);
		break;

	case MODE::MODE_DEAD:
		m_eCurMonsterState = STATE_DEAD;
		DoDead(dt);
		break;

	case MODE::MODE_RETURN:
		DoReturn(dt);
		break;
	}
}

void CMechsect::DoDetect(_float dt)
{
	// 감지 가능 거리 이내일 때 / 감지 상태 중 추격 가능 거리일 때
	ChasePlayer(dt, 50.f);
	m_bJumpEnd = false;
	m_eCurMonsterState = STATE_MOVE;
}


void CMechsect::DoReady(_float dt)
{
	m_bJumpEnd = false;
	m_fCooldownDuration += dt;
	if (m_fCooldownDuration >= m_fCooldownTime)
	{
		m_isReadyToAttack = true;
		m_fCooldownDuration = 0.f;
	}
	m_fAnimationFrame = 0.f;
	_float3 TargetPos = *static_cast<CTransform*>(m_pTargetPlayer->Find_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);
	m_pTransformCom->LookAt(TargetPos);
}

void CMechsect::DoBattle(_float dt)
{
	_float fChaseDistance = 500.f;
	if (m_fCurDistance > fChaseDistance)
	{
		m_eState = MODE::MODE_RETURN;
		return;
	}
	else
	{
		AttackPattern(dt);
	}
}

void CMechsect::DoIdle(_float dt)
{
	switch (m_eIdlePhase)
	{
	case EIdlePhase::IDLE_MOVE:
	{
		m_eCurMonsterState = STATE_MOVE;
		m_fWanderElapsed += dt;

		m_pTransformCom->Go_Straight(dt);

		if (m_fWanderElapsed >= m_fWanderTime)
		{
			m_fWanderElapsed = 0.f;
			m_eIdlePhase = EIdlePhase::IDLE_WAIT;
		}
		break;
	}
	case EIdlePhase::IDLE_WAIT:
		m_fIdleWaitElapsed += dt;
		m_eCurMonsterState = STATE_STAY;

		if (m_fIdleWaitElapsed >= m_fIdleWaitTime)
		{
			SetRandomDirection();                  // 회전할 방향 설정
			m_fIdleWaitElapsed = 0.f;
			m_eIdlePhase = EIdlePhase::IDLE_TURN; // 다음엔 회전하러 간다
		}
		break;

	case EIdlePhase::IDLE_TURN:
	{
		m_eCurMonsterState = STATE_STAY;
		_float3 vLook = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);

		bool bRotated = m_pTransformCom->RotateToDirection(vLook, m_vDirection, 5.f, dt);
		if (bRotated)  // 회전 완료 신호
		{
			m_eIdlePhase = EIdlePhase::IDLE_MOVE;
		}
		break;
	}
	}
}

void CMechsect::AttackPattern(_float dt)
{
	// 실제 공격 패턴 작성하는 곳
	m_eCurMonsterState = STATE_JUMP;

	if (!m_pGravityCom->isJump())
	{
		m_pGravityCom->Jump(30.f);
	}
	if (m_fAnimationSpeed < 1.f)
	{
		m_fAttackTimer += dt;
		if (m_fAttackTimer >= 0.3f)
		{
			m_bCoolingDown = true;
			m_fAttackTimer = 0.f;
		}
	}
	else
		m_pTransformCom->Go_Straight(dt);
	//if (!m_bJump && m_fAnimationFrame >= 1.f)
	//{
	//	m_bJump = true;
	//	m_pGravityCom->Jump(30.f);
	//}

	//if (m_bJump && !m_pGravityCom->isJump())
	//{
	//	m_bJumpEnd = true;
	//}

	//if (m_bJumpEnd)
	//{
	//	m_fJumpFinished += dt;
	//	if (m_fJumpFinished >= 0.3f)
	//	{
	//		m_fJumpFinished = 0.f;
	//		m_bJump = false;
	//		m_bJumpEnd = false;
	//		//m_eState = MODE_READY;
	//		m_bCoolingDown = true;
	//	}
	//}
	//else
	//{
	//	m_pTransformCom->Go_Straight(dt);
	//}

}

void CMechsect::ChasePlayer(_float dt, _float fChaseDist)
{
	//타겟을 350거리까지 추격한다.
	_float3 TargetPos = *static_cast<CTransform*>(m_pTargetPlayer->Find_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);

	// 현재 위치
	_float3 vMyPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	// 방향 계산
	_float3 vDir = TargetPos - vMyPos;
	float fDistance = vDir.Length();
	vDir.Normalize();

	//원래방향으로 턴하기
	_float3 vLook = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	bool bRotated = m_pTransformCom->RotateToDirection(vLook, vDir, 5.f, dt);
	m_pTransformCom->ChaseWithOutY(TargetPos, dt, fChaseDist, 150.f);
}



HRESULT CMechsect::Ready_Components(void* pArg)
{
	/* 렉트 버퍼 컴포넌트 */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, _wstring(TEXT("Prototype_Component_VIBuffer_")) + m_szBufferType,
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* 트랜스폼 컴포넌트 */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), pArg)))
		return E_FAIL;

	/* 위치, 스케일 초기화 */
	if (pArg != nullptr)
	{
		DESC* pDesc = static_cast<DESC*>(pArg);

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, pDesc->vPosition);
		m_pTransformCom->Scaling(m_vScale);
		m_bActive = pDesc->vActive;
		m_vReturnPos = pDesc->vReturnPos;
		m_iNum = pDesc->iNums;
		m_eLevelID = pDesc->eLevel;
		m_fDetectiveDistance = pDesc->fDetectiveDistance;
		m_fAttackDistance = pDesc->fAttackDistance;
	}

	/* 콜라이드 컴포넌트 */
	DESC* pDesc = static_cast<DESC*>(pArg);
	CCollider_Capsule::DESC ColliderDesc{};
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vOffSet = {};
	ColliderDesc.vScale = m_pTransformCom->Compute_Scaled();
	ColliderDesc.pOwner = this;
	ColliderDesc.iColliderGroupID = CG_MONSTER;
	ColliderDesc.iColliderID = CI_MONSTER_MECHSECT;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Capsule"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pCollider), &ColliderDesc)))
		return E_FAIL;

	if (m_bActive)
	{
		CGravity::DESC GravityDesc{};
		GravityDesc.pTransformCom = m_pTransformCom;
		GravityDesc.fTimeIncreasePerSec = 8.2f;
		GravityDesc.fMaxFallSpeedPerSec = 840.f;
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Gravity"),
			TEXT("Com_Gravity"), reinterpret_cast<CComponent**>(&m_pGravityCom), &GravityDesc)))
			return E_FAIL;
	}

	Ready_Textures();

	return S_OK;
}

HRESULT CMechsect::Ready_Textures()
{
	/* Move */
	for (_uint i = 0; i < D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Mechsect_Move_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(LEVEL_STATIC, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Mechsect_Move_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_MOVE][i])))))
			return E_FAIL;
	}

	/* Jump */
	for (_uint i = 0; i < D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Mechsect_Jump_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(LEVEL_STATIC, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Mechsect_Jump_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_JUMP][i])))))
			return E_FAIL;
	}

	/* Death */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_Mechsect_Dead",
		_wstring(TEXT("Com_Texture")) + L"_Mechsect_Dead", reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_DEAD][0])))))
		return E_FAIL;


	return S_OK;
}


HRESULT CMechsect::Set_Animation()
{
	if (m_eCurMonsterState != m_ePrevMonsterState)
	{
		m_ePrevMonsterState = m_eCurMonsterState;
		m_fAnimationFrame = 0.f;
		m_iState = (_uint)(m_eCurMonsterState);
		switch (m_eCurMonsterState)
		{
		case Client::CMechsect::STATE_MOVE:
			m_fAnimationMaxFrame = _float(MAX_MOVERUN);
			m_fAnimationSpeed = 10.f;
			break;
		case Client::CMechsect::STATE_JUMP:
			m_fAnimationMaxFrame = _float(MAX_MOVERUN);
			m_fAnimationSpeed = 6.f;
			break;
		case Client::CMechsect::STATE_STAY:
			m_fAnimationMaxFrame = 1.f;
			m_fAnimationSpeed = 0.f;
			m_iState = (_uint)(STATE_MOVE);
			break;
		case Client::CMechsect::STATE_DEAD:
			m_fAnimationMaxFrame = _float(MAX_DEAD);
			m_fAnimationSpeed = 8.f;
			m_bRotateAnimation = false;
			break;
		}
	}
	return S_OK;
}

HRESULT CMechsect::Animate_Monster(_float fTimeDelta)
{
	if (m_fAnimationMaxFrame < 2.f)
		return S_OK;

	switch (m_eCurMonsterState)
	{
	case Client::CMechsect::STATE_STAY:
		m_bRotateAnimation = true;
		return S_OK;
	case Client::CMechsect::STATE_MOVE:
		m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		if (m_fAnimationFrame >= m_fAnimationMaxFrame)
			m_fAnimationFrame = 0.f;
		m_bRotateAnimation = true;
		break;
	case Client::CMechsect::STATE_JUMP:
		m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		if (m_fAnimationFrame >= m_fAnimationMaxFrame - 2.f)
		{
			m_fAnimationFrame = m_fAnimationMaxFrame - 2.f;
			m_fAnimationSpeed = 0.f;
		}
		if (!m_pGravityCom->isJump() && m_fAnimationSpeed < 1.f)
		{
			m_fAnimationFrame = m_fAnimationMaxFrame - 1.f; // 착지?
			m_bJumpEnd = true;
		}
		m_bRotateAnimation = true;
		break;
	case Client::CMechsect::STATE_DEAD:
		m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		if (m_fAnimationFrame >= m_fAnimationMaxFrame)
			m_fAnimationFrame = m_fAnimationMaxFrame - 1.f;
		m_bRotateAnimation = false;
		break;
	}
	return S_OK;
}

CMechsect* CMechsect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMechsect* pInstance = new CMechsect(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMechsect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMechsect::Clone(void* pArg)
{
	CMechsect* pInstance = new CMechsect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CMechsect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMechsect::Free()
{
	__super::Free();
}