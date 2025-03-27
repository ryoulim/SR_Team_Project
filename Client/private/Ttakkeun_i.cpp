#include "Ttakkeun_i.h"
#include "FXMgr.h"
#include "MonsterBullet.h"
#include "FlyEffect.h"

CTtakkeun_i::CTtakkeun_i(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster{ pGraphic_Device }
{
}

CTtakkeun_i::CTtakkeun_i(const CTtakkeun_i& Prototype)
	: CMonster(Prototype)
{
}

HRESULT CTtakkeun_i::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTtakkeun_i::Initialize(void* pArg)
{
	//위치, 크기초기화, 컴포넌트 부착
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_isReadyMonster = true;
	m_fDivOffset = 22.5f;
	// 보스랑 잡몹 텍스쳐 갯수가 달라서 별도 지정
	Ready_Textures();
	
	//애니메이션(수정예정)
	m_iState = (_uint)(MONSTER_STATE::STATE_WALK);
	m_fAnimationMaxFrame = (_float)(STATE_MAXFRAME::MAX_WALK);
	m_fAnimationSpeed = 17.f;
	//_float3 vOrigSize = {};
	//m_pTextureMap[m_iState][m_iDegree]->Get_TextureSize(static_cast<_uint>(m_fAnimationFrame), &m_vScale);
	//m_pTransformCom->Scaling(m_vScale);
	m_eCurFlyingDirection = UP;
	m_ePrevFlyingDirection = UP;

	m_pGravityCom->Set_Height(110.f);

	return S_OK;
}

void CTtakkeun_i::Priority_Update(_float fTimeDelta)
{
	Set_Animation();
	__super::Priority_Update(fTimeDelta);
}

EVENT CTtakkeun_i::Update(_float fTimeDelta)
{
	if (m_pFlyEffect)
		static_cast<CFlyEffect*>(m_pFlyEffect)->SetPosition(*m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	return __super::Update(fTimeDelta);
}

void CTtakkeun_i::Late_Update(_float fTimeDelta)
{
	//플레이어 감지 업데이트
	PlayerDistance();
	CalculateVectorToPlayer();
	IsPlayerDetected();

	if (m_eCurMonsterState != STATE_FLY &&
		m_eCurMonsterState != STATE_FLY_ATTACK)
	{
		m_pGravityCom->Update(fTimeDelta);
	}

	Compute_ViewAngle();

	//콜라이더 업데이트
	m_pCollider->Update_Collider();

	//렌더그룹 업데이트
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
		return;

	//__super::Late_Update(fTimeDelta);
#ifdef _DEBUG
	auto now = steady_clock::now();
	auto elapsed = duration_cast<milliseconds>(now - g_LastLogTime).count();
	
	if (elapsed >= 1000) 
	{
		// 1초 이상 지났다면 출력
		//cout << "따끈이 상태 : " << GetMonsterStateName(m_eState) << '\n';
		//cout << "플레이어와의 거리 : " << m_fCurDistance << endl;
		cout << "따끈이 랜덤변수 : " << m_iRandom << endl;
		g_LastLogTime = now;
	}
#endif
}

HRESULT CTtakkeun_i::Render()
{
	
	return __super::Render();

	//특별히 더 렌더링 할게 있는 경우 ↓
}

HRESULT CTtakkeun_i::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTtakkeun_i::Ready_Textures()
{
	/* WALK */
	for (_uint i = 0; i < BOSS_DEGREE::D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Boss_Walk_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
   		if (FAILED(__super::Add_Component(m_eLevelID, sPrototypeTag,
	 		_wstring(TEXT("Com_Texture")) + L"_Boss_Walk_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_WALK][i])))))
       		return E_FAIL;
	}

	/* FLY */
	for (_uint i = 0; i < BOSS_DEGREE::D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Boss_Fly_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(m_eLevelID, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Boss_Fly_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_FLY][i])))))
			return E_FAIL;
	}

	/* FLY_ATTACK */
	for (_uint i = 0; i < BOSS_DEGREE::D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Boss_Fly_Attack_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(m_eLevelID, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Boss_Fly_Attack_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_FLY_ATTACK][i])))))
			return E_FAIL;
	}

	/* JUMP */
	for (_uint i = 0; i < BOSS_DEGREE::D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Boss_Jump_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(m_eLevelID, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Boss_Jump_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_JUMP][i])))))
			return E_FAIL;
	}



	//if (FAILED(__super::Add_Component(m_eLevelID, _wstring(TEXT("Prototype_Component_Texture_")) + m_szTextureID,
	//	TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
	//	return E_FAIL;

 	return S_OK;
}

HRESULT CTtakkeun_i::Set_Animation()
{
	if (m_eCurMonsterState != m_ePrevMonsterState)
	{
		m_ePrevMonsterState = m_eCurMonsterState;
		m_fAnimationFrame = 0.f;
		m_iState = (_uint)(m_eCurMonsterState);
		switch (m_eCurMonsterState)
		{
		case Client::CTtakkeun_i::STATE_WALK:
			m_fAnimationMaxFrame = MAX_WALK;
			m_fAnimationSpeed = 15.f;
			break;
		case Client::CTtakkeun_i::STATE_FLY:
			m_fAnimationFrame = _float(m_eCurFlyingDirection);
			//m_eCurFlyingDirection = UP;
			break;
		case Client::CTtakkeun_i::STATE_FLY_ATTACK:
			//m_eCurFlyingDirection = UP;
			m_fAnimationFrame = _float(m_eCurFlyingDirection) * 2.f;
			m_fAnimationSpeed = 10.f;
			break;
		case Client::CTtakkeun_i::STATE_JUMP:
			m_fAnimationMaxFrame = MAX_JUMP;
			m_fAnimationSpeed = 3.f;
			break;
		case Client::CTtakkeun_i::STATE_STAY:
			m_fAnimationMaxFrame = 1.f;
			m_fAnimationSpeed = 0.f;
			m_iState = (_uint)(STATE_WALK);
			break;
		default:
			break;
		}
	}
	if (m_eCurFlyingDirection != m_ePrevFlyingDirection)
	{
		m_ePrevFlyingDirection = m_eCurFlyingDirection;
		m_fAnimationFrame = _float(m_eCurFlyingDirection) * 2.f;
	}

	return S_OK;
}

HRESULT CTtakkeun_i::Animate_Monster(_float fTimeDelta)
{
	if (m_fAnimationMaxFrame < 2.f)
		return S_OK;

	switch (m_eCurMonsterState)
	{
	case Client::CTtakkeun_i::STATE_WALK:
		m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		if (m_fAnimationFrame >= m_fAnimationMaxFrame)
			m_fAnimationFrame = 0.f;
		break;

	case Client::CTtakkeun_i::STATE_STAY:
		m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		if (m_fAnimationFrame >= m_fAnimationMaxFrame)
			m_fAnimationFrame = 0.f;
		break;

	case Client::CTtakkeun_i::STATE_FLY:
		if (m_iDegree != 0 && m_iDegree != 8 && m_bCW == true)
		{
			if (m_eCurFlyingDirection == LEFT)
				m_fAnimationFrame = (_float)(RIGHT);
			else if (m_eCurFlyingDirection == RIGHT)
				m_fAnimationFrame = (_float)(LEFT);
		}
		else
			m_fAnimationFrame = (_float)(m_eCurFlyingDirection);
		break;


	case Client::CTtakkeun_i::STATE_FLY_ATTACK:
		m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		if (m_fAnimationFrame >= (_float)(m_eCurFlyingDirection) * 2.f + 2.f)
			m_fAnimationFrame = (_float)(m_eCurFlyingDirection) * 2.f;
		break;


	case Client::CTtakkeun_i::STATE_JUMP:
		m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		if (m_fAnimationFrame < 1.f)
			m_fAnimationFrame += fTimeDelta * 0.2f;
		else if (m_fAnimationFrame < 2.f)
			m_fAnimationFrame += fTimeDelta * 6.f;
		else if (m_fAnimationFrame < 3.f)
		{
			//날아오르라 패턴
			if (m_bFlyAttack)
				m_eCurMonsterState = STATE_FLY;
			else if (!m_bFlyAttack && !m_bJumpStart)
				m_fAnimationFrame += fTimeDelta * 0.1f;
			
			//내려찍기 패턴
			if (m_bJumpStart)
			{
				m_fAnimationFrame = 2.f;
				break;
			}
			else if (!m_bFlyAttack && !m_bJumpStart)
				m_fAnimationFrame += fTimeDelta * 0.1f;
		}
		else if (m_fAnimationFrame < 4.f)
			m_fAnimationFrame += fTimeDelta * 6.f;
		else if (m_fAnimationFrame < 5.f)
			m_fAnimationFrame += fTimeDelta * 0.05f;
		else if (m_fAnimationFrame >= 5.f)
			m_eCurMonsterState = STATE_WALK;
			
		break;
	}


	return S_OK;
}

void CTtakkeun_i::DoIdle(_float dt)
{
	switch (m_eIdlePhase)
	{
	case EIdlePhase::IDLE_MOVE:
	{
		m_eCurMonsterState = STATE_WALK;
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
		//m_eCurMonsterState = STATE_STAY;

		if (m_fIdleWaitElapsed >= m_fIdleWaitTime)
		{
			SetRandomDirection();                  // 회전할 방향 설정
			m_fIdleWaitElapsed = 0.f;
			m_eIdlePhase = EIdlePhase::IDLE_TURN; // 다음엔 회전하러 간다
		}
		break;

	case EIdlePhase::IDLE_TURN:
	{
		m_eCurMonsterState = STATE_WALK;
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

void CTtakkeun_i::DoBattle(_float dt)
{
	// 1. 플레이어와의 거리 계산
	_float fAttackRange = 350.f;
	_float fChaseRange = 700.f;

	//만약 공중공격중이라면
	if (m_bFlyAttack)
	{
		FlyAttack(dt);
		return;
	}

	// 2. 거리 기준 분기
	if (m_fCurDistance < fAttackRange)
	{
		// 가까우면 공격
		AttackPattern(dt);
	}
	else
	{
		//따끈이는 리턴 안합니다. (추격)
		ChasePlayer(dt);
	}
}

void CTtakkeun_i::AttackPattern(_float dt)
{
	// 쿨다운 중이면 진행하지 않음
	if (m_bCoolingDown)
	{
		m_fCooldownTime += dt;

		if (m_fCooldownTime >= m_fCooldownDuration)
		{
			m_iRandom = GetRandomInt(0, 1);
			m_bCoolingDown = false;
			m_fCooldownTime = 0.f;
		}
		return;
	}

	bool isPhase2 = (m_iHP <= m_iMaxHP / 2);
	int iRand = GetRandomInt(0, 99);

	/* 따끈이의 공격패턴[ 2페이즈 ] */
	if (isPhase2)
	{
		// 2페이즈일 때는 2페이즈 전용 패턴이 더 높은 확률로 등장
		if (iRand < 20)
			/* 1. 레이저 빔 */
			LazerAttack(dt);      // 20% 확률
		else if (iRand < 40)
			/* 2. 공작 유도탄 */
			MissileAttack(dt);    // 20% 확률
		else if (iRand < 60)
			/* 3. 몬스터 소환(다콘) */
			SpawnAttack(dt);      // 20% 확률
		else
			BasicAttackSet(dt); // 20% 확률로 1페이즈 패턴
	}
	else
	{
		// 1페이즈일 때는 그냥 1페이즈 패턴만 사용
		BasicAttackSet(dt);
	}
}

void CTtakkeun_i::BasicAttackSet(_float dt)
{
	/* 따끈이의 공격패턴[ 1페이즈 ] */

	m_iRandom = 2;

	switch (m_iRandom)
	{
	case 0: 
		/* 1. 화염방사 */
		FireAttack(dt);	
		break;
	case 1:
		/* 2. 날아오르라 주작이여 */
		FlyAttack(dt);
		break;
	case 2:
		/* 3. 내리찍기 */
		JumpAttack(dt);
		break;
	}
}

void CTtakkeun_i::LazerAttack(_float dt)
{
}

void CTtakkeun_i::MissileAttack(_float dt)
{
}

void CTtakkeun_i::SpawnAttack(_float dt)
{
}

void CTtakkeun_i::FireAttack(_float dt)
{
	// 어택 쿨타임
	m_fAttackTimer += dt;
	if (m_fAttackTimer >= 2.f)
	{
		StartCooldown(dt);
		m_fAttackTimer = 0.f;
		return;
	}

	/* 화염방사 패턴 */

	/* 1. 플레이어에게 접근한다(조금 빠르게) */
	_float3 vPlayerPos = *static_cast<CTransform*>(m_pTargetPlayer->Find_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);
	vPlayerPos.y = m_vReturnPos.y;
	_float3 vMyPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_float3 vDir = vPlayerPos - vMyPos;
	float fDistance = vDir.Length();
	vDir.Normalize();

	_float3 vLook = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	bool bRotated = m_pTransformCom->RotateToDirection(vLook, vDir, 5.f, dt);

	m_pTransformCom->ChaseCustom(vPlayerPos, dt, 100.f, 150.f);
	/* 2. 화염을 발사한다! */
	CFXMgr::Get_Instance()->FireAttack(vMyPos, LEVEL_GAMEPLAY);

}

void CTtakkeun_i::JumpAttack(_float dt)
{
	/* [ 공중으로 점프를 한 뒤, 플레이어 방향으로 3번 내리꼽는다 ] */

	//내리꼽을때 상자가 있으면 어쩌지 -> 내리꼽는 로직을 짤때 시간으로 짜야할듯

	/* 필요변수 */
	_bool bIsFly = false;
	_bool bIsTop = false;
	m_bJumpStart = true;
	m_eCurMonsterState = STATE_JUMP;


	/* 1. 점프모션을 취한 뒤에 타이밍 맞춰 날아오른다. */

	if (m_eCurMonsterState == STATE_JUMP && m_fAnimationFrame == 2.f)
	{
		//날아오르고 플레이어 쪽으로 조금 다가와야할거같음.
		bIsFly = m_pTransformCom->Go_UpCustom(dt, 400.f, 300.f);
		if (!bIsFly)
		{
			_float3 TargetPos = *static_cast<CTransform*>(m_pTargetPlayer->Find_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);
			m_pTransformCom->ChaseWithOutY(TargetPos,dt,50.f,200.f);
		}
	}

	/* 2. 내려오면서 블럭이랑 충돌하면 파티클이 비산한다. */

}

void CTtakkeun_i::FlyAttack(_float dt)
{
	//내 위치
	_float3 vMyPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	
	//날아오르기 변수
	bool isFly = false;

	if (m_eCurMonsterState == STATE_WALK)
	{
		//점프 후 Fly 로 스테이트를 바꿔준다.
		m_eCurMonsterState = STATE_JUMP;
		m_bFlyAttack = true;

		//이펙트 소환
		if (!m_bDoOnce)
		{
			FlyEffect();
			m_bDoOnce = true;
		}
	}

	//몬스터가 공중으로 일정높이까지 날아오른다.
	if (m_eCurMonsterState == STATE_FLY || m_eCurMonsterState == STATE_FLY_ATTACK)
	{
		isFly = m_pTransformCom->Go_UpCustom(dt, 100.f, 250.f);
	}

	//몬스터가 m_vReturnPos 를 중점으로 공중에서 뱅글뱅글 돈다.
	if (isFly)
	{
		if (m_pFlyEffect)
		{
			static_cast<CFlyEffect*>(m_pFlyEffect)->SetDead();
			m_pFlyEffect = nullptr;
		}

		_float fRotationSpeed = 60.f;

		// 현재 룩 벡터 가져오기
		_float3 vLook = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);

		/* [ 회전수식 적용 ] */
		_float fAngleRad = RADIAN(fRotationSpeed * dt);

		_float cosA = cosf(fAngleRad);
		_float sinA = sinf(fAngleRad);

		_float3 vRotatedLook;
		vRotatedLook.x = vLook.x * cosA - vLook.z * sinA;
		vRotatedLook.z = vLook.x * sinA + vLook.z * cosA;
		vRotatedLook.y = vLook.y;

		//룩벡터 업데이트
		vRotatedLook.Normalize();
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, vRotatedLook);

		// 전방으로 이동
		_float fSpeed = 600.f;
		_float3 vForward = vRotatedLook * fSpeed * dt;

		//센터방향 벡터
		_float3 vToCenter = m_vReturnPos - vMyPos;
		vToCenter.y = 0.f;
		vToCenter.Normalize();

		// 천천히 중심 쪽으로 밀기
		_float fDriftSpeed = 100.f;
		_float3 vDrift = vToCenter * fDriftSpeed * dt;

		// 최종 위치 = 회전 + 중심 쪽 이동
		_float3 vNewPos = vMyPos + vForward + vDrift;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vNewPos);
	}

	/* 몬스터가 플레이어를 향해서 미사일을 발사한다 */
	if (isFly)
	{
		//3번 발사 시 패턴 종료
		if (3 <= m_iMissileCount)
		{
			m_iRandom = GetRandomInt(0, 1);
			m_eCurMonsterState = STATE_FLY;
			m_eCurFlyingDirection = UP;
			m_eState = MODE::MODE_RETURN;
			m_iMissileCount = 0;
			m_bFlyAttack = false;
			m_bDoOnce = false;
			isFly = false;
			return;
		}

		//fly 상태에서 2초마다 미사일 발사 함수를 호출한다.
		m_fFlyAttack += dt;

		if (m_fFlyAttack >= 4.f)
		{
			// 4초마다 초기화
			m_fFlyAttack = 0.f;
			++m_iMissileCount;
		}

		if (m_fFlyAttack >= 2.f) // 2~4초 구간
		{
			/* 공격모션으로 바꾸고 미사일을 발사한다 */
			m_eCurMonsterState = STATE_FLY_ATTACK;
			m_eCurFlyingDirection = RIGHT;
			m_bRotateAnimation = false;

			SpawnMissile(dt);
			return;
		}
		m_eCurMonsterState = STATE_FLY;
		m_eCurFlyingDirection = UP;
		m_bRotateAnimation = true;
	}
}

void CTtakkeun_i::StartCooldown(_float dt)
{
	m_bCoolingDown = true;
	m_fCooldownDuration = GetRandomFloat(1.5f, 2.5f); // 쿨다운 시간 랜덤
	m_fCooldownTime = 0.f;
}

void CTtakkeun_i::SpawnMissile(_float dt)
{
	m_fSpawnMissile += dt;

	if (m_fSpawnMissile >= 0.2f)
	{
		// 0.2초마다 발사
		CMonsterBullet::DESC MonsterBullet_iDesc{};
		MonsterBullet_iDesc.fSpeedPerSec = 60.f;
		MonsterBullet_iDesc.fRotationPerSec = RADIAN(180.f);
		MonsterBullet_iDesc.vScale = { 10.f, 10.f, 0.f };
		MonsterBullet_iDesc.vPosition = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_MonsterBullet"),
			LEVEL_GAMEPLAY, L"Layer_MonsterBullet", &MonsterBullet_iDesc)))
			return;


		m_fSpawnMissile = 0.f;
	}
}

void CTtakkeun_i::FlyEffect()
{
	//비행 파티클
	CPSystem::DESC FlyEffect_iDesc{};
	FlyEffect_iDesc.vPosition = { 0.f, 0.f, 0.f };
	FlyEffect_iDesc.szTextureTag = TEXT("PC_Small_Smoke");
	FlyEffect_iDesc.iParticleNums = 100;
	FlyEffect_iDesc.fSize = 1.f;
	FlyEffect_iDesc.fMaxFrame = 20.f;
	FlyEffect_iDesc.fLifeTime = GetRandomFloat(1.f, 3.f);

	CGameObject* pObject = nullptr;
	CGameObject** ppOut = &pObject;
	if (FAILED(m_pGameInstance->Add_GameObjectReturn(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_FlyEffect"),
		LEVEL_GAMEPLAY, L"Layer_Particle", ppOut, &FlyEffect_iDesc)))
		return;

	m_pFlyEffect = *ppOut;
}

void CTtakkeun_i::DoReturn(_float dt)
{
	// 현재 위치
	_float3 vMyPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	// 방향 계산
	_float3 vDir = m_vReturnPos - vMyPos;
	float fDistance = vDir.Length();
	vDir.Normalize();

	//원래방향으로 턴하기
	_float3 vLook = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	bool bRotated = m_pTransformCom->RotateToDirection(vLook, vDir, 5.f, dt);

	if (bRotated)  // 회전 완료 신호
	{
		// 너무 가까우면 이동 종료
		if (fDistance < 10.0f) // 도착 판정 오차 허용
		{
			m_eState = MODE::MODE_IDLE;
			m_eCurMonsterState = STATE_WALK;
			return;
		}

		// 이동 처리 (dt 고려)
		float fSpeed = 300.f;
		_float3 vMove = vDir * fSpeed * dt;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, (vMyPos + vMove));
	}
}


void CTtakkeun_i::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	//그 즉시 배틀모드 진입
	m_eState = MODE::MODE_BATTLE;

	//위치탐색
	_float3 vImpactPos = CalculateEffectPos();

	//몬스터 사망
	if (0 >= m_iHP)
	{
		CFXMgr::Get_Instance()->SpawnCustomExplosion(vImpactPos, LEVEL_GAMEPLAY, _float3{ 120.f, 160.f, 1.f }, TEXT("Effect_Explor"), 32);
		m_bDead = true;

		return;
	}

	// 이펙트 생성
	m_iHP += -10;
	CFXMgr::Get_Instance()->SpawnBlood(vImpactPos, LEVEL_GAMEPLAY);
}

CTtakkeun_i* CTtakkeun_i::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	//프로토타입의 기본정의
	CTtakkeun_i* pInstance = new CTtakkeun_i(pGraphic_Device);

	//기본 정보
	pInstance->m_eLevelID		= LEVEL_GAMEPLAY;
	pInstance->m_szTextureID	= TEXT("Ttakkeun_i_Walk");
	pInstance->m_szBufferType	= TEXT("Rect");

	//속성
	pInstance->m_iHP			= 500;
	pInstance->m_iMaxHP			= 500;
	pInstance->m_iAttackPower	= 10;
	pInstance->m_iDefense		= 3;
	pInstance->m_fSpeed			= 100.f;
	pInstance->m_vScale			= { 150.f, 147.f, 1.f };
	pInstance->m_eState			= MODE::MODE_IDLE;
	pInstance->m_fDetectiveDistance = 400.f;

	//부속성
	pInstance->m_strDialogue	= "Boss..Boss...";
	pInstance->m_strSound		= "SoundFilePath";

	pInstance->m_vDropItems.push_back("BossDropItem");


	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTtakkeun_i");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTtakkeun_i::Clone(void* pArg)
{
	CTtakkeun_i* pInstance = new CTtakkeun_i(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CTtakkeun_i");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTtakkeun_i::Free()
{
	__super::Free();
}