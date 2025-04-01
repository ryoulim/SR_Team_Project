#include "Ttakkeun_i.h"
#include "FXMgr.h"
#include "MonsterBullet.h"
#include "FlyEffect.h"
#include "GrenadeBullet.h"
#include "CameraManager.h"
#include "MonsterGuidBullet.h"
#include "Sprite.h"

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
	//프로토타입의 기본정의
	m_szTextureID = TEXT("Ttakkeun_i_Walk");
	m_szBufferType = TEXT("Rect");

	//속성
	m_iHP = 500;
	m_iMaxHP = 500;
	m_iAttackPower = 10;
	m_iDefense = 3;
	m_fSpeed = 100.f;
	m_vScale = { 150.f, 147.f, 1.f };
	m_eState = MODE::MODE_IDLE;

	m_fDetectiveDistance = 400.f;

	//부속성
	m_strDialogue = "Boss..Boss...";
	m_strSound = "SoundFilePath";

	m_vDropItems.push_back("BossDropItem");

	return S_OK;
}

HRESULT CTtakkeun_i::Initialize(void* pArg)
{
	/* 카메라를 알고 있어라 */
	m_pCamera = CAMERA_MANAGER;
	Safe_AddRef(m_pCamera);


	m_fDivOffset = 22.5f;
	// 보스랑 잡몹 텍스쳐 갯수가 달라서 별도 지정
	//애니메이션(수정예정)
	m_iState = (_uint)(MONSTER_STATE::STATE_WALK);
	m_fAnimationMaxFrame = (_float)(STATE_MAXFRAME::MAX_WALK);
	m_fAnimationSpeed = 17.f;
	m_eCurFlyingDirection = UP;
	m_ePrevFlyingDirection = UP;


	//위치, 크기초기화, 컴포넌트 부착
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

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
	if (m_bDie)
		return EVN_DEAD;

	if (m_bDead)
	{
		_float3 vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		vPos.x += GetRandomFloat(-50.f, 50.f);
		vPos.y += GetRandomFloat(-50.f, 50.f);
		vPos.z += GetRandomFloat(-50.f, 50.f);

		m_fCallTimer += fTimeDelta;
		m_fTotalTime += fTimeDelta;

		if (m_fCallTimer >= 0.2f)
		{
			FX_MGR->SpawnCustomExplosion(vPos, LEVEL_GAMEPLAY, _float3{ 60.f, 100.f, 1.f }, TEXT("Effect_Explorer"), 24);
			m_fCallTimer = 0.f;
		}
		if (m_fTotalTime >= 3.f)
		{
			_float3 vImpactPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			FX_MGR->SpawnCustomExplosion(vImpactPos, LEVEL_GAMEPLAY, _float3{ 200.f, 250.f, 1.f }, TEXT("Effect_Explor"), 32);
			m_bDie = true;
		}
	}
	

	if (m_pBossEffect)
		static_cast<CFlyEffect*>(m_pBossEffect)->SetPosition(*m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	if (m_bActive)
	{
		//m_pGravityCom->Update(fTimeDelta);
		MonsterTick(fTimeDelta);
	}

	return EVN_NONE;
}

void CTtakkeun_i::Late_Update(_float fTimeDelta)
{
	//플레이어 감지 업데이트
	PlayerDistance();
	CalculateVectorToPlayer();
	//IsPlayerDetected();

	if (m_eCurMonsterState != STATE_FLY &&
		m_eCurMonsterState != STATE_FLY_ATTACK &&
		!m_bJumpStart)
	{
		m_pGravityCom->Update(fTimeDelta);
	}

	Compute_ViewAngle();
	//콜라이더 업데이트
	m_pCollider->Update_Collider();

	//렌더그룹 업데이트
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
		return;
	Set_TextureType();
	Resize_Texture(0.35f);
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

	Ready_Textures();

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

	/* BOMB */
	for (_uint i = 0; i < BOSS_DEGREE::D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Boss_Bomb_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(m_eLevelID, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Boss_Bomb_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_BOMB][i])))))
			return E_FAIL;
	}

	/* MISSILE */
	for (_uint i = 0; i < BOSS_DEGREE::D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Boss_Missile_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(m_eLevelID, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Boss_Missile_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_MISSILE][i])))))
			return E_FAIL;
	}

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
			m_fAnimationMaxFrame = _float(MAX_WALK);
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
			m_fAnimationMaxFrame = _float(MAX_JUMP);
			m_fAnimationSpeed = 3.f;
			break;
		case Client::CTtakkeun_i::STATE_STAY:
			m_fAnimationMaxFrame = 1.f;
			m_fAnimationSpeed = 0.f;
			m_iState = (_uint)(STATE_WALK);
			break;
		case Client::CTtakkeun_i::STATE_MISSILE: //두개
			m_fAnimationMaxFrame = _float(MAX_GROUNDATK);
			m_fAnimationSpeed = 20.f;
			break;
		case Client::CTtakkeun_i::STATE_BOMB: //꼬리
			m_fAnimationMaxFrame = _float(MAX_GROUNDATK);
			m_fAnimationSpeed = 20.f;
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
			
			//내려찍기 패턴(점프모션에 스탑하려고)
			if (m_bJumpStart)
			{
				if (m_bJumpEnd)
					m_fAnimationFrame += fTimeDelta * 0.1f;
					
				//2프레임 고정
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


	case Client::CTtakkeun_i::STATE_BOMB:
		m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		if (m_fAnimationFrame >= m_fAnimationMaxFrame)
			m_fAnimationFrame = 0.f;
		break;


	case Client::CTtakkeun_i::STATE_MISSILE:
		m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		if (m_fAnimationFrame >= m_fAnimationMaxFrame)
			m_fAnimationFrame = 0.f;
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
	_float fAttackRange = 400.f;
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
			m_iRandom = GetRandomInt(0, 5);
			m_bCoolingDown = false;
			m_fCooldownTime = 0.f;
		}
		return;
	}

	//공격하라~
	BasicAttackSet(dt);
}

void CTtakkeun_i::BasicAttackSet(_float dt)
{
	/* [ 따끈이의 공격패턴 ] */
	
	//m_iRandom = 5;
	switch (m_iRandom)
	{
	case 0: 
		/* 0. 화염방사 */
		FireAttack(dt);	
		break;
	case 1:
		/* 1. 날아오르라 주작이여 */
		FlyAttack(dt);
		break;
	case 2:
		/* 2. 바운스 볼 */
		BounceBall(dt);
		break;
	case 3:
		/* 3. 내리찍기 */
		JumpAttack(dt);
		break;
	case 4:
		/* 4. 공작 미사일발사 */
		MissileAttack(dt);
		break;
	case 5:
		/* 5. 몬스터 소환 */
		SpawnAttack(dt);
		break;
	}
}


void CTtakkeun_i::MissileAttack(_float dt)
{
	/* [ 공작 미사일 패턴 ] */

	/* 1. 딱 한번 공작모양으로 미사일을 스폰한다. */
	/* 2. 따끈이는 일정시간 가만히 있는다. */

	m_fAttackTimer += dt;
	if (m_fAttackTimer >= 3.f)
	{
		//3초가 넘어가면 다른 행동시작
		m_bRotateAnimation = true;
		m_eCurMonsterState = STATE_WALK;
		StartCooldown(dt, 1.5f, 3.f);
		m_bDoOnce = false;
		m_fAttackTimer = 0.f;
		return;
	}

	/* 룩벡터 회전 */
	_float3 vPlayerPos = *static_cast<CTransform*>(m_pTargetPlayer->Find_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);
	_float3 vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vToPlayer = vPlayerPos - vPos;
	_float3 vLook = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	bool bRotated = m_pTransformCom->RotateToDirection(vLook, vToPlayer, 60.f, dt);

	m_bRotateAnimation = false;
	m_eCurMonsterState = STATE_MISSILE;

	//미사일 발사
	if (!m_bDoOnce)
	{
		SpawnGuidMissile();
		m_bDoOnce = true;
	}

}

void CTtakkeun_i::SpawnAttack(_float dt)
{
	/* [ 다콘 소환 패턴 ] */

	/* 1. 다콘을 허공에서 소환한다. */
	/* 2. 따끈이는 일정시간 가만히 있는다. */

	m_fAttackTimer += dt;
	if (m_fAttackTimer >= 3.f)
	{
		//3초가 넘어가면 다른 행동시작
		m_eCurMonsterState = STATE_WALK;
		m_bRotateAnimation = true;
		StartCooldown(dt, 2.f, 3.f);
		m_bDoOnce = false;
		m_fAttackTimer = 0.f;
		return;
	}

	m_bRotateAnimation = false;
	m_eCurMonsterState = STATE_BOMB;

	/* 룩벡터 회전 */
	_float3 vPlayerPos = *static_cast<CTransform*>(m_pTargetPlayer->Find_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);
	_float3 vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vToPlayer = vPlayerPos - vPos;
	_float3 vLook = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	bool bRotated = m_pTransformCom->RotateToDirection(vLook, vToPlayer, 60.f, dt);

	//다콘 소환
	if (!m_bDoOnce)
	{
		for (int i = 0; i < 8; i++)
		{
			_float3 vRandomPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			vRandomPos.x += GetRandomFloat(-300.f, 300.f);
			vRandomPos.y += GetRandomFloat(100.f, 300.f);
			vRandomPos.z += GetRandomFloat(-300.f, 300.f);

			SpawnDeacon(vRandomPos);
			SpawnDeaconEffect(vRandomPos);
		}
		m_bDoOnce = true;
	}
}

void CTtakkeun_i::FireAttack(_float dt)
{
	// 어택 쿨타임
	m_fAttackTimer += dt;
	if (m_fAttackTimer >= 2.f)
	{
		StartCooldown(dt, 2.f, 2.5f);
		m_fAttackTimer = 0.f;
		return;
	}

	/* 화염방사 패턴 */

	/* 1. 플레이어에게 접근한다(조금 빠르게) */
	_float3 vPlayerPos = *static_cast<CTransform*>(m_pTargetPlayer->Find_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);
	vPlayerPos.y = m_vReturnPos.y;
	_float3 vMyPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	//(고개를 돌린다)
	_float3 vDir = vPlayerPos - vMyPos;
	float fDistance = vDir.Length();
	vDir.Normalize();

	_float3 vLook = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	bool bRotated = m_pTransformCom->RotateToDirection(vLook, vDir, 5.f, dt);

	m_pTransformCom->ChaseCustom(vPlayerPos, dt, 100.f, 150.f);
	/* 2. 화염을 발사한다! */
	FX_MGR->FireAttack(vMyPos, LEVEL_GAMEPLAY, m_iNum);

}

void CTtakkeun_i::BounceBall(_float dt)
{
	/* 바운스볼 패턴 */

	/* 1. 따끈이는 고정된 상태로 플레이어를 빌보드한다. */
	m_bRotateAnimation = false;
	m_eCurMonsterState = STATE_BOMB;
	
	/* 룩벡터 회전 */
	_float3 vPlayerPos = *static_cast<CTransform*>(m_pTargetPlayer->Find_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);
	_float3 vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vToPlayer = vPlayerPos - vPos;
	_float3 vLook = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	bool bRotated = m_pTransformCom->RotateToDirection(vLook, vToPlayer, 60.f, dt);

	/* 2. 일정시간마다 바운스볼을 플레이어에게 3방향 발사한다. */
	m_fBounceTime += dt;
	if (m_fBounceTime >= 1.f)
	{
		//여기에 소환 함수 호출하면 끝!
		SpawnBounce();
		m_iBounceCount++;
		m_fBounceTime = 0.f;
		return;
	}

	/* 3. 총 4번을 발사하면 쿨다운함수를 호출한다 */
	if (m_iBounceCount > 3)
	{
		m_eCurMonsterState = STATE_WALK;
		StartCooldown(dt, 1.f, 2.f);
		m_bRotateAnimation = true;
		m_fBounceTime = 0;
		m_iBounceCount = 0;
	}
}

void CTtakkeun_i::JumpAttack(_float dt)
{
	if (m_iJumpCount > 2)
	{
		m_iJumpCount = 0;
		StartCooldown(dt, 1.f, 2.f);
		return;
	}

	/* [ 공중으로 점프를 한 뒤, 플레이어 방향으로 3번 내리꼽는다 ] */

	/* 필요변수 */
	_float3 vPlayerPos = *static_cast<CTransform*>(m_pTargetPlayer->Find_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);
	_float3 vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vToPlayer = vPlayerPos - vPos;
	m_bJumpStart = true;
	m_eCurMonsterState = STATE_JUMP;


	/* 1. 점프모션을 취한 뒤에 타이밍 맞춰 날아오른다. */

	if (m_eCurMonsterState == STATE_JUMP && m_fAnimationFrame == 2.f && !m_bJumpEnd)
	{
		//날아오르고 플레이어 쪽으로 조금 다가와야할거같음.
		m_bIsFly = m_pTransformCom->Go_UpCustom(dt, 400.f, 300.f);
		if (!m_bIsFly)
		{
			//고개를 돌린다.
			_float3 vLook = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);
			bool bRotated = m_pTransformCom->RotateToDirection(vLook, vToPlayer, 60.f, dt);

			_float3 TargetPos = *static_cast<CTransform*>(m_pTargetPlayer->Find_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);
			m_pTransformCom->ChaseWithOutY(TargetPos,dt,80.f,500.f);
		}
		//연결해줄 변수 true
		m_bIsLink = true;
	}

	/* 2. 내려오면서 블럭이랑 충돌하면 파티클이 비산한다. */
	if (m_bIsFly && m_bIsLink)//연결 true
	{
		//바닥 레이의 float 보다 크면 낙하
		m_pGravityCom->Raycast_StandAble_Obj();
		if (vPos.y - 73.5f > m_pGravityCom->Get_FloorY())
		{
			m_bJumpEnd = true;

			// 중력 가속 유지
			m_fFallSpeed += 3000.f * dt;

			vPos.y -= m_fFallSpeed * dt;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

		}
		else
		{
			//이펙트 생성 && 카메라 쉐이킹
			vPos.y = 30.f;
			FX_MGR->JumpAttack(vPos, LEVEL_GAMEPLAY);
			m_pCamera->Shake_Camera();

			//몬스터가 착지한 후 점프리셋
			vPos.y = 77.f;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
			m_fFallSpeed = 0.f;
			m_iJumpCount++;

			m_eCurMonsterState = STATE_WALK;

			m_bJumpStart = false;
			m_bJumpEnd = false;

			//연결 false
			m_bIsLink = false;

			//재귀해야징~
			JumpAttack(dt);
		}
	}

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
		if (m_pBossEffect)
		{
			static_cast<CFlyEffect*>(m_pBossEffect)->SetDead();
			m_pBossEffect = nullptr;
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

void CTtakkeun_i::StartCooldown(_float dt, _float fMin, _float fMax)
{
	m_bCoolingDown = true;
	m_fCooldownDuration = GetRandomFloat(fMin, fMax); // 쿨다운 시간 랜덤
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
			m_eLevelID, L"Layer_MonsterBullet", &MonsterBullet_iDesc)))
			return;


		m_fSpawnMissile = 0.f;
	}
}

void CTtakkeun_i::SpawnGuidMissile()
{
	// 중심 방향 (위쪽)
	_float3 vBaseDir = { 0.f, 1.f, 0.f };

	// 부채꼴 각도 설정
	int missileCount = 5;
	float totalAngleDeg = 60.f;
	float angleStep = totalAngleDeg / (missileCount - 1); // 예: 15도 간격
	float startAngle = -totalAngleDeg * 0.5f; // 예: -30도 시작

	for (int i = 0; i < missileCount; ++i)
	{
		float angleDeg = startAngle + angleStep * i;
		float angleRad = RADIAN(angleDeg);

		// 위쪽 방향 기준으로 Z축 회전 (좌우로 퍼지게)
		_float3 vRotated = Rotate_Z(vBaseDir, angleRad);
		
		CMonsterGuidBullet::DESC MonsterGuidBullet_iDesc{};
		MonsterGuidBullet_iDesc.fSpeedPerSec = 60.f;
		MonsterGuidBullet_iDesc.fRotationPerSec = RADIAN(180.f);
		MonsterGuidBullet_iDesc.vScale = { 30.f, 30.f, 1.f };
		MonsterGuidBullet_iDesc.vPosition = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		MonsterGuidBullet_iDesc.vPosition.y += 30;
		MonsterGuidBullet_iDesc.vDir = vRotated;

		_float3 vLook = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		vLook.Normalize();

		MonsterGuidBullet_iDesc.vPosition.x -= vLook.x * 10.f;
		MonsterGuidBullet_iDesc.vPosition.y -= vLook.y * 10.f;
		MonsterGuidBullet_iDesc.vPosition.z -= vLook.z * 10.f;

		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_MonsterGuidBullet"),
			m_eLevelID, L"Layer_MonsterBullet", &MonsterGuidBullet_iDesc)))
			return;
	}
}

void CTtakkeun_i::SpawnBounce()
{
	//현재 포지션
	_float3 fMyPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	fMyPos.y += 30.f;

	//발사방향
	_float3 fMyLook = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	fMyLook.Normalize();

	float fAngleOffsets[3] = { -30.f, 0.f, 30.f };

	for (int i = 0; i < 3; ++i)
	{
		float rad = RADIAN(fAngleOffsets[i]);
		_float3 vDir = Rotate_Y(fMyLook, rad); // Y축 기준으로 회전

		CGrenadeBullet::DESC BulletDesc{};
		BulletDesc.fSpeedPerSec = 300.f;
		BulletDesc.vScale = { 20.f,20.f,20.f };

		BulletDesc.vPosition = fMyPos;
		BulletDesc.szTextureTag = TEXT("MonsterBounce");
		BulletDesc.vLook = vDir;

		BulletDesc.ColliderGroup = CG_MBULLET;
		BulletDesc.fInitJumpPower = 30.f;
		BulletDesc.fTimeLimit = 2.f;
		BulletDesc.bAnimation = true;

		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_GrenadeBullet"),
			m_eLevelID, TEXT("Layer_Bullet"), &BulletDesc)))
			return;
	}
}

void CTtakkeun_i::SpawnDeacon(_float3 vPos)
{
	CMonster::DESC Deacon_iDesc{};
	Deacon_iDesc.vPosition = vPos;
	Deacon_iDesc.fSpeedPerSec = 60.f;
	Deacon_iDesc.fRotationPerSec = RADIAN(180.f);
	Deacon_iDesc.vActive = true;
	Deacon_iDesc.vReturnPos = vPos;
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Deacon"),
		m_eLevelID, L"Layer_Monster", &Deacon_iDesc)))
		return;
}

void CTtakkeun_i::SpawnDeaconEffect(_float3 vPos)
{
	CSprite::DESC ExplosionDesc{};
	ExplosionDesc.vInitPos = vPos;
	ExplosionDesc.vScale = { 200.f,200.f,1.f };
	ExplosionDesc.bLoop = false;
	ExplosionDesc.fMaxFrame = 8;
	ExplosionDesc.fRotationPerSec = RADIAN(180.f);
	ExplosionDesc.fSpeedPerSec = 100.f;
	ExplosionDesc.szTextureTag = TEXT("DeaconSpawn");

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Sprite"),
		m_eLevelID, L"Layer_Effect", &ExplosionDesc)))
		return;
}

_float3 CTtakkeun_i::Rotate_Y(_float3 vDir, float rad)
{
	float cosA = cosf(rad);
	float sinA = sinf(rad);

	_float3 result;
	result.x = vDir.x * cosA - vDir.z * sinA;
	result.y = vDir.y;
	result.z = vDir.x * sinA + vDir.z * cosA;
	return result;
}

_float3 CTtakkeun_i::Rotate_Z(_float3 vDir, float rad)
{
	float cosA = cosf(rad);
	float sinA = sinf(rad);

	return 
	{
		vDir.x * cosA - vDir.y * sinA,
		vDir.x * sinA + vDir.y * cosA,
		vDir.z
	};
	return _float3();
}

void CTtakkeun_i::FlyEffect()
{
	//비행 파티클
	CPSystem::DESC FlyEffect_iDesc{};
	FlyEffect_iDesc.vPosition = { 0.f, 0.f, 0.f };
	FlyEffect_iDesc.szTextureTag = TEXT("PC_Small_Smoke");
	FlyEffect_iDesc.iParticleNums = 200;
	FlyEffect_iDesc.fSize = 7.f;
	FlyEffect_iDesc.fMaxFrame = 20.f;

	CGameObject* pObject = nullptr;
	CGameObject** ppOut = &pObject;
	if (FAILED(m_pGameInstance->Add_GameObjectReturn(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_FlyEffect"),
		m_eLevelID, L"Layer_Particle", ppOut, &FlyEffect_iDesc)))
		return;

	m_pBossEffect = *ppOut;
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
		//그 즉시 배틀모드 진입
		if (m_eState != MODE::MODE_RETURN)
			m_eState = MODE::MODE_BATTLE;

		//위치탐색
		_float3 vImpactPos = CalculateEffectPos();

		//몬스터 사망
		if (0 >= m_iHP)
		{
			m_bActive = false;

			if (m_pBossEffect)
			{
				static_cast<CFlyEffect*>(m_pBossEffect)->SetDead();
				m_pBossEffect = nullptr;
			}

			m_bDead = true;

			return;
		}

		// 이펙트 생성
		m_iHP += -10;
		FX_MGR->SpawnBlood(vImpactPos, LEVEL_GAMEPLAY);
	}
	
}

CTtakkeun_i* CTtakkeun_i::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTtakkeun_i* pInstance = new CTtakkeun_i(pGraphic_Device);

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

	Safe_Release(m_pCamera);
}