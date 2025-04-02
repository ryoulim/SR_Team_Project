// 내 클래스 이름 : TestMonster
// 부모 클래스 이름 : Monster

#include "Greater.h"
#include "FXMgr.h"
#include "MonsterNormalBullet.h"

CGreater::CGreater(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster{ pGraphic_Device }
{
}

CGreater::CGreater(const CGreater& Prototype)
	: CMonster(Prototype)
{
}

HRESULT CGreater::Initialize_Prototype()
{
	//프로토타입의 기본정의
	m_szTextureID = TEXT("Greater_Run");
	m_szBufferType = TEXT("Rect");

	//속성
	m_iHP = 120;
	m_iMaxHP = 120;
	m_iAttackPower = 15;
	m_iDefense = 2;
	m_fSpeed = 12.f;
	m_vScale = { 28.f, 68.2f, 1.f };
	m_eState = MODE::MODE_IDLE;

	m_fDetectiveDistance = 500.f;

	//부속성
	m_strDialogue = "Great..Great...?";
	m_strSound = "SoundFilePath";

	m_vDropItems.push_back("GreatDropItem");

	return S_OK;
}

HRESULT CGreater::Initialize(void* pArg)
{

	//위치, 크기초기화, 컴포넌트 부착
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_fDivOffset = 45.f;
	//애니메이션(수정예정)
	m_fAnimationMaxFrame = 4.f;
	m_fAnimationSpeed = 5.f;
	m_iState = STATE_MOVE;
	m_fBulletCooldown = 0.2f;
	m_fCooldownTime = 0.5f;
	m_fAttackTime = 1.f;

	return S_OK;
}

void CGreater::Priority_Update(_float fTimeDelta)
{
	Set_Animation();
	__super::Priority_Update(fTimeDelta);
}

EVENT CGreater::Update(_float fTimeDelta)
{
	if (KEY_DOWN(DIK_RBRACKET))
	{
		int i = m_eCurMonsterState;
		i++;
		m_eCurMonsterState = MONSTER_STATE(i);
		if (m_eCurMonsterState == STATE_END)
			m_eCurMonsterState = MONSTER_STATE(0);
	}
	return __super::Update(fTimeDelta);
}

void CGreater::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	if (false == m_bRotateAnimation)
		m_iDegree = 0;
	Resize_Texture(0.4f);
}

HRESULT CGreater::Render()
{
	return __super::Render();

	//특별히 더 렌더링 할게 있는 경우 ↓
}


void CGreater::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	if (CI_BLOCK(OtherColliderID))
	{
		m_pCollider->Get_Last_Collision_Pos();

		_float3 vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		_float3 Depth = m_pCollider->Get_Last_Collision_Depth();
		if (Depth.y != 0)
			int a = 1;
		vPos += Depth;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	}
	else if (CI_WEAPON(OtherColliderID))
	{
		// 탐색 시작
		if (!m_bFoundPlayer)
			m_eState = MODE::MODE_DETECTIVE;

		//위치탐색
		_float3 vImpactPos = CalculateEffectPos();

		// 이펙트 생성
		m_iHP += -50;
		FX_MGR->SpawnBlood(vImpactPos, LEVEL_GAMEPLAY);

		//몬스터 사망
		if (0 >= m_iHP)
		{
			//이펙트 혹시몰라서 그냥 주석만 해둠
			//FX_MGR->SpawnCustomExplosion(vImpactPos, LEVEL_GAMEPLAY, _float3{ 130.f, 160.f, 1.f }, TEXT("PC_Explosion"), 14);
			m_bDead = true;
			m_eState = MODE_DEAD;
			_float3 TargetPos = *static_cast<CTransform*>(m_pTargetPlayer->Find_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);
			m_pTransformCom->LookAt(TargetPos);

			// 뒤로 넉백
			m_bKnockBack = true;

			return;
		}

		// 이펙트 생성
		FX_MGR->SpawnBlood(vImpactPos, LEVEL_GAMEPLAY);

		__super::On_Collision(MyColliderID, OtherColliderID);
	}
}

void CGreater::MonsterTick(_float dt)
{
	//상태변화
	switch (m_eState)
	{
	case MODE::MODE_IDLE:
		if (IsPlayerDetected())	// 감지 거리 기준 계산
		{
			m_bFoundPlayer = true;
			//플레이어 발견 시 행동
			cout << "Shotgunner 플레이어 감지!!" << endl;
			m_eState = MODE::MODE_DETECTIVE;
		}
		break;

	case MODE::MODE_DETECTIVE:
		//플레이어 공격 가능할 때 까지 탐색
		m_fRaycastTicker += dt;
		if (m_fRaycastTicker > 0.5f)
		{
			if (IsMonsterAbleToAttack())	// RayPicking으로 플레이어와 몬스터 사이 장애물 체크
			{
				m_bFoundPlayer = true;
				m_eState = MODE::MODE_READY;
			}
		}
		break;

	case MODE::MODE_READY:
		//공격 할 준비  ( 장전 등의 딜레이 필요 )
		m_fRaycastTicker += dt;
		if (m_fRaycastTicker > 0.5f)
		{
			if (!IsMonsterAbleToAttack())
			{
				m_eState = MODE::MODE_DETECTIVE;
				break;
			}
		}
		// 준비 끝나면
		if (m_isReadyToAttack)
			m_eState = MODE::MODE_BATTLE;
		break;

	case MODE::MODE_BATTLE:
		//실제 공격 시 행동
		if (m_bCoolingDown)
		{
			m_fCooldownDuration = 0.f;
			m_eState = MODE::MODE_READY;
			m_isReadyToAttack = false;
			m_bCoolingDown = false;
		}
		m_fRaycastTicker += dt;
		if (m_fRaycastTicker > 0.5f)
		{
			if (false == IsMonsterAbleToAttack())
			{
				m_fCooldownDuration = 0.f;
				m_eState = MODE::MODE_DETECTIVE;
				m_isReadyToAttack = false;
				m_bCoolingDown = false;
			}
		}
		break;

	case MODE::MODE_DEAD:
		break;
	case MODE::MODE_RETURN:
		m_bFoundPlayer = false;
		//본래위치로 돌아가고 IDLE로 상태가 변한다.
		break;
	}

#ifdef _CONSOL
	auto now = steady_clock::now();
	auto elapsed = duration_cast<milliseconds>(now - g_LastLogTime).count();

	if (elapsed >= 1000)
	{
		// 1초 이상 지났다면 출력
		cout << "[그레이터]\t플레이어와의 거리 : " << m_fCurDistance << endl;
		cout << "[그레이터]\t상태 : ";
		switch (m_eState)
		{
		case Client::CMonster::MODE_IDLE:
			cout << "IDLE";
			break;
		case Client::CMonster::MODE_READY:
			cout << "READY";
			break;
		case Client::CMonster::MODE_BATTLE:
			cout << "BATTLE";
			break;
		case Client::CMonster::MODE_DETECTIVE:
			cout << "DETECTIVE";
			break;
		case Client::CMonster::MODE_RETURN:
			cout << "RETURN";
			break;
		case Client::CMonster::MODE_END:
			cout << "UNKNOWN";
			break;
		default:
			cout << "UNKNOWN";
			break;
		}
		cout << endl;
		g_LastLogTime = now;
	}
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

void CGreater::DoDetect(_float dt)
{
	// 감지 가능 거리 이내일 때 / 감지 상태 중 추격 가능 거리일 때
	ChasePlayer(dt, 50.f);
	m_eCurMonsterState = STATE_MOVE;
}


_bool CGreater::IsMonsterAbleToAttack()
{
	// 여기 레이캐스팅으로 플레이어와 몬스터 사이 장애물 유무 체크
	m_fRaycastTicker = 0.f;
	if (m_fCurDistance > m_fAttackDistance)
		return false;
	return Raycast_Player();
}

void CGreater::DoReady(_float dt)
{
	m_fCooldownDuration += dt;
	if (m_fCooldownDuration >= m_fCooldownTime)
	{
		m_isReadyToAttack = true;
		m_fBulletCooldownElapsed = 0.4f;
		m_fCooldownDuration = 0.f;
	}
	m_fAnimationFrame = 0.f;
	_float3 TargetPos = *static_cast<CTransform*>(m_pTargetPlayer->Find_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);
	m_pTransformCom->LookAt(TargetPos);
}

void CGreater::DoBattle(_float dt)
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

void CGreater::DoIdle(_float dt)
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

void CGreater::AttackPattern(_float dt)
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
		MonsterNormalBullet_iDesc.fSpeedPerSec = 900.f;
		MonsterNormalBullet_iDesc.fRotationPerSec = RADIAN(180.f);
		MonsterNormalBullet_iDesc.vScale = { 1.f, 3.f, 0.f };
		MonsterNormalBullet_iDesc.vPosition = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float3 vRight = *m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		vRight.Normalize();
		// 총구 위치를 몬스터의 위치와 look 벡터를 사용하여 계산
		MonsterNormalBullet_iDesc.vPosition = MonsterNormalBullet_iDesc.vPosition + vRight * 6.f;
		MonsterNormalBullet_iDesc.vPosition.y += 14.5f;

		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_MonsterNormalBullet"),
			LEVEL_GAMEPLAY, L"Layer_MonsterBullet", &MonsterNormalBullet_iDesc)))
			return;

		m_fBulletCooldownElapsed = 0.f;
	}
}

void CGreater::ChasePlayer(_float dt, _float fChaseDist)
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


HRESULT CGreater::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	Ready_Textures();

	return S_OK;
}

HRESULT CGreater::Set_Animation()
{
	if (m_eCurMonsterState != m_ePrevMonsterState)
	{
		m_ePrevMonsterState = m_eCurMonsterState;
		m_fAnimationFrame = 0.f;
		m_iState = (_uint)(m_eCurMonsterState);
		switch (m_eCurMonsterState)
		{
		case Client::CGreater::STATE_MOVE:
			m_fAnimationMaxFrame = _float(MAX_MOVERUN);
			m_fAnimationSpeed = 10.f;
			break;
		case Client::CGreater::STATE_RUN:
			m_fAnimationMaxFrame = _float(MAX_MOVERUN);
			m_fAnimationSpeed = 10.f;
			break;
		case Client::CGreater::STATE_ATTACK:
			m_fAnimationMaxFrame = _float(MAX_ATTACK);
			m_fAnimationSpeed = 14.f;
			break;
		case Client::CGreater::STATE_STAY:
			m_fAnimationMaxFrame = 1.f;
			m_fAnimationSpeed = 0.f;
			m_iState = (_uint)(STATE_MOVE);
			break;
		case Client::CGreater::STATE_DEAD:
			m_fAnimationMaxFrame = _float(MAX_DEAD);
			m_fAnimationSpeed = 8.f;
			m_bRotateAnimation = false;
			break;
		}
	}
	return S_OK;
}

HRESULT CGreater::Animate_Monster(_float fTimeDelta)
{
	if (m_fAnimationMaxFrame < 2.f)
		return S_OK;

	switch (m_eCurMonsterState)
	{
	case Client::CGreater::STATE_STAY:
		m_bRotateAnimation = true;
		return S_OK;
	case Client::CGreater::STATE_MOVE:
		m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		if (m_fAnimationFrame >= m_fAnimationMaxFrame)
			m_fAnimationFrame = 0.f;
		m_bRotateAnimation = true;
		break;
	case Client::CGreater::STATE_RUN:
		m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		if (m_fAnimationFrame >= m_fAnimationMaxFrame)
			m_fAnimationFrame = 0.f;
		m_bRotateAnimation = true;
		break;
	case Client::CGreater::STATE_ATTACK:
		m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		if (m_fAnimationFrame >= m_fAnimationMaxFrame)
			m_fAnimationFrame = 0.f;
		m_bRotateAnimation = true;
		break;
	case Client::CGreater::STATE_DEAD:
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

HRESULT CGreater::Ready_Textures()
{
	/* Move */
	for (_uint i = 0; i < D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Greater_Move_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(LEVEL_STATIC, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Greater_Move_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_MOVE][i])))))
			return E_FAIL;
	}
	/* Run */
	for (_uint i = 0; i < D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Greater_Run_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(LEVEL_STATIC, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Greater_Run_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_RUN][i])))))
			return E_FAIL;
	}
	/* Attack */
	for (_uint i = 0; i < D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Greater_Attack_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(LEVEL_STATIC, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Greater_Attack_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_ATTACK][i])))))
			return E_FAIL;
	}

	/* DEATH */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_Greater_Dead",
		_wstring(TEXT("Com_Texture")) + L"_Greater_Death", reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_DEAD][0])))))
		return E_FAIL;
	return S_OK;
}


CGreater* CGreater::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGreater* pInstance = new CGreater(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CGreater");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGreater::Clone(void* pArg)
{
	CGreater* pInstance = new CGreater(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CGreater");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGreater::Free()
{
	__super::Free();
}