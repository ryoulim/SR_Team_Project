// 내 클래스 이름 : TestMonster
// 부모 클래스 이름 : Monster

#include "Cultist.h"
#include "FXMgr.h"
#include "MonsterNormalBullet.h"

CCultist::CCultist(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster{ pGraphic_Device }
{
}

CCultist::CCultist(const CCultist& Prototype)
	: CMonster(Prototype)
{
}

HRESULT CCultist::Initialize_Prototype()
{
	//프로토타입의 기본정의
	m_szTextureID = TEXT("Cultist_Run");
	m_szBufferType = TEXT("Rect");

	//속성
	m_iHP = 150;
	m_iMaxHP = 150;
	m_iAttackPower = 8;
	m_iDefense = 1;
	m_fSpeed = 10.f;
	m_vScale = { 36.5f, 62.2f, 1.f };
	m_eState = MODE::MODE_IDLE;

	m_fDetectiveDistance = 300.f;

	//부속성
	m_strDialogue = "Cultist..Cultist...";
	m_strSound = "SoundFilePath";

	m_vDropItems.push_back("CultistDropItem");

	return S_OK;
}

HRESULT CCultist::Initialize(void* pArg)
{
	//위치, 크기초기화, 컴포넌트 부착
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	m_fDivOffset = 45.f;
	//애니메이션(수정예정)
	m_fAnimationMaxFrame = 4.f;
	m_fAnimationSpeed = 5.f;
	m_iState = STATE_MOVE;

	m_fBulletCooldown = 0.1f;
	m_fCooldownTime = 0.7f;
	m_fAttackTime = 1.f;


	/* [ 사운드 설정칸 ] */
	m_pSoundCom->Set3DState(100.f, 1000.f);
	m_pSoundCom->SetVolume("Chacing", 0.4f);
	m_pSoundCom->SetVolume("Die", 0.4f);
	m_pSoundCom->SetVolume("Hit", 0.4f);
	m_pSoundCom->SetVolume("Attack", 0.4f);

	return S_OK;
}

void CCultist::Priority_Update(_float fTimeDelta)
{
	_float3 vCurPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_pSoundCom->Update3DPosition(vCurPos);

	Set_Animation();
	__super::Priority_Update(fTimeDelta);
}

EVENT CCultist::Update(_float fTimeDelta)
{
	if (m_bDead && !m_bDeadSound)
	{
		m_pSoundCom->Play("Die");
		m_bDeadSound = true;
	}
	return __super::Update(fTimeDelta);
}

void CCultist::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	if (false == m_bRotateAnimation)
		m_iDegree = 0;
	Resize_Texture(0.4f);
}

HRESULT CCultist::Render()
{
	return __super::Render();

	//특별히 더 렌더링 할게 있는 경우 ↓
}


void CCultist::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	__super::On_Collision_NormalMonster(MyColliderID, OtherColliderID);
}

void CCultist::MonsterTick(_float dt)
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
void CCultist::DoDetect(_float dt)
{
	// 감지 가능 거리 이내일 때 / 감지 상태 중 추격 가능 거리일 때
	ChasePlayer(dt, 50.f);
	m_eCurMonsterState = STATE_MOVE;
}


//_bool CCultist::IsMonsterAbleToAttack()
//{
//	// 여기 레이캐스팅으로 플레이어와 몬스터 사이 장애물 유무 체크
//	m_fRaycastTicker = 0.f;
//	if (m_fCurDistance > m_fAttackDistance)
//		return false;
//	return Raycast_Player();
//}

void CCultist::DoReady(_float dt)
{
	m_fCooldownDuration += dt;
	if (m_fCooldownDuration >= m_fCooldownTime)
	{
		m_bFirstBullet = true;
		m_isReadyToAttack = true;
		m_fBulletCooldownElapsed = 0.4f;
		m_fCooldownDuration = 0.f;
	}
	m_fAnimationFrame = 0.f;
	_float3 TargetPos = *static_cast<CTransform*>(m_pTargetPlayer->Find_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);
	m_pTransformCom->LookAt(TargetPos);
}

void CCultist::DoBattle(_float dt)
{
	_float fChaseDistance = 500.f;
	if (m_fCurDistance > fChaseDistance)
	{
		m_eState = MODE::MODE_RETURN;
		return;
	}
	else
		AttackPattern(dt);
}

void CCultist::DoIdle(_float dt)
{
	switch (m_eIdlePhase)
	{
	case EIdlePhase::IDLE_MOVE:
	{
		if (m_bStandby)
		{
			m_eIdlePhase = EIdlePhase::IDLE_WAIT;
			break;
		}

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

void CCultist::AttackPattern(_float dt)
{
	// 실제 공격 패턴 작성하는 곳
	// 잡몹이라 일반공격정도만
	// Archangel 특수공격 있음
	// Wenteko 넣을 시 얘도 있음
	m_eCurMonsterState = STATE_ATTACK;

	m_fBulletCooldownElapsed += dt;
	m_fAttackTimer += dt;
	if (m_fAttackTimer >= m_fAttackTime)
	{
		m_bCoolingDown = true;
		m_fAttackTimer = 0.f;
	}
	if (m_fBulletCooldownElapsed >= m_fBulletCooldown)
	{
		_float3 TargetPos = *static_cast<CTransform*>(m_pTargetPlayer->Find_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);
		m_pTransformCom->LookAt(TargetPos);
		CMonsterNormalBullet::DESC MonsterNormalBullet_iDesc{};
		MonsterNormalBullet_iDesc.iColliderID = CI_MONSTER_CULTIST;
		MonsterNormalBullet_iDesc.fSpeedPerSec = 1000.f;
		MonsterNormalBullet_iDesc.fRotationPerSec = RADIAN(180.f);
		MonsterNormalBullet_iDesc.vScale = { 1.f, 3.f, 1.f };
		MonsterNormalBullet_iDesc.vPosition = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float3 vRight = *m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		vRight.Normalize();
		// 총구 위치를 몬스터의 위치와 look 벡터를 사용하여 계산
		MonsterNormalBullet_iDesc.vPosition += vRight * 11.f;
		MonsterNormalBullet_iDesc.vPosition.y += 13.f;
		if (m_bFirstBullet)
		{
			MonsterNormalBullet_iDesc.bCollision = true;
			m_bFirstBullet = false;
		}
		else
			MonsterNormalBullet_iDesc.bCollision = false;

		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_MonsterNormalBullet"),
			LEVEL_GAMEPLAY, L"Layer_MonsterBullet", &MonsterNormalBullet_iDesc)))
			return;

		if (!m_pSoundCom->IsPlaying("Attack"))
			m_pSoundCom->Play("Attack");

		m_fBulletCooldownElapsed = 0.f;
	}
}

void CCultist::ChasePlayer(_float dt, _float fChaseDist)
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


HRESULT CCultist::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	Ready_Textures();

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Sound_Cultist"),
		TEXT("Com_Sound"), reinterpret_cast<CComponent**>(&m_pSoundCom))))
		return E_FAIL;

	// 머리 콜라이더 
/* 콜라이드 컴포넌트 */
	DESC* pDesc = static_cast<DESC*>(pArg);
	CCollider::DESC ColliderDesc{};
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vOffSet = { 0.f, 62.2f * 0.5f - 7.f, 0.f }; // y길이 * 0.5 - 머리위치y좌표 + 지름크기?
	ColliderDesc.vScale = { 6.f, 0.f, 0.f }; // 반지름 크기
	ColliderDesc.pOwner = this;
	ColliderDesc.iColliderGroupID = CG_MONSTER_HEAD;
	ColliderDesc.iColliderID = CI_MON_HEAD;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider_head"), reinterpret_cast<CComponent**>(&m_pHeadCollider), &ColliderDesc)))
		return E_FAIL;

	m_pCollider->Update_OffSet({ 0.f, -7.f, 0.f });
	m_pCollider->Update_Scale({ 36.5f, 62.2f - 7.f, 1.f });

	return S_OK;
}

HRESULT CCultist::Ready_Textures()
{
	for (_uint i = 0; i < D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Cultist_Move_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(LEVEL_STATIC, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Cultist_Move_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_MOVE][i])))))
			return E_FAIL;
	}
	for (_uint i = 0; i < D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Cultist_Run_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(LEVEL_STATIC, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Cultist_Run_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_RUN][i])))))
			return E_FAIL;
	}
	for (_uint i = 0; i < D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Cultist_Attack_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(LEVEL_STATIC, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Cultist_Attack_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_ATTACK][i])))))
			return E_FAIL;
	}
	if (FAILED(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_Cultist_Dead",
		_wstring(TEXT("Com_Texture")) + L"_Cultist_Dead", reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_DEAD][0])))))
		return E_FAIL;

	return S_OK;
}

HRESULT CCultist::Set_Animation()
{
	if (m_eCurMonsterState != m_ePrevMonsterState)
	{
		m_ePrevMonsterState = m_eCurMonsterState;
		m_fAnimationFrame = 0.f;
		m_iState = (_uint)(m_eCurMonsterState);
		switch (m_eCurMonsterState)
		{
		case Client::CCultist::STATE_MOVE:
			m_fAnimationMaxFrame = _float(MAX_MOVERUN);
			m_fAnimationSpeed = 10.f;
			break;
		case Client::CCultist::STATE_RUN:
			m_fAnimationMaxFrame = _float(MAX_MOVERUN);
			m_fAnimationSpeed = 10.f;
			break;
		case Client::CCultist::STATE_ATTACK:
			m_fAnimationMaxFrame = _float(MAX_ATTACK);
			m_fAnimationSpeed = 20.f;
			break;
		case Client::CCultist::STATE_STAY:
			m_fAnimationMaxFrame = 1.f;
			m_fAnimationSpeed = 0.f;
			m_iState = (_uint)(STATE_MOVE);
			break;
		case Client::CCultist::STATE_DEAD:
			m_fAnimationMaxFrame = _float(MAX_DEAD);
			m_fAnimationSpeed = 13.f;
			m_bRotateAnimation = false;
			break;
		}
	}
	return S_OK;
}

HRESULT CCultist::Animate_Monster(_float fTimeDelta)
{
	if (m_fAnimationMaxFrame < 2.f)
		return S_OK;

	switch (m_eCurMonsterState)
	{
	case Client::CCultist::STATE_STAY:
		m_bRotateAnimation = true;
		return S_OK;
	case Client::CCultist::STATE_MOVE:
		m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		if (m_fAnimationFrame >= m_fAnimationMaxFrame)
			m_fAnimationFrame = 0.f;
		m_bRotateAnimation = true;
		break;
	case Client::CCultist::STATE_RUN:
		m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		if (m_fAnimationFrame >= m_fAnimationMaxFrame)
			m_fAnimationFrame = 0.f;
		m_bRotateAnimation = true;
		break;
	case Client::CCultist::STATE_ATTACK:
		m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		if (m_fAnimationFrame >= m_fAnimationMaxFrame)
			m_fAnimationFrame = 0.f;
		m_bRotateAnimation = true;
		break;
	case Client::CCultist::STATE_DEAD:
		m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		if (m_fAnimationFrame >= m_fAnimationMaxFrame)
		{
			m_fAnimationFrame = m_fAnimationMaxFrame - 1.f;
			m_fAnimationSpeed = 0.f;
		}
		break;
	}
	return S_OK;
}


CCultist* CCultist::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCultist* pInstance = new CCultist(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCultist");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCultist::Clone(void* pArg)
{
	CCultist* pInstance = new CCultist(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CCultist");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCultist::Free()
{
	__super::Free();

	Safe_Release(m_pSoundCom);
}