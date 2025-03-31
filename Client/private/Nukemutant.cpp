// 내 클래스 이름 : TestMonster
// 부모 클래스 이름 : Monster

#include "Nukemutant.h"
#include "FXMgr.h"

CNukemutant::CNukemutant(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster{ pGraphic_Device }
{
}

CNukemutant::CNukemutant(const CNukemutant& Prototype)
	: CMonster(Prototype)
{
}

HRESULT CNukemutant::Initialize_Prototype()
{
	//프로토타입의 기본정의
	m_szTextureID = TEXT("Nukemutant_Walk");
	m_szBufferType = TEXT("Rect");

	//속성
	m_iHP = 50;
	m_iMaxHP = 50;
	m_iAttackPower = 15;
	m_iDefense = 0;
	m_fSpeed = 15.f;
	m_vScale = { 27.f, 56.f, 1.f };
	m_eState = MODE::MODE_IDLE;

	m_fDetectiveDistance = 300.f;

	//부속성
	m_strDialogue = "Nuke..Nu...";
	m_strSound = "SoundFilePath";

	m_vDropItems.push_back("NukemutantDropItem");

	return S_OK;
}

HRESULT CNukemutant::Initialize(void* pArg)
{

	//위치, 크기초기화, 컴포넌트 부착
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_fDivOffset = 45.f;
	//애니메이션(수정예정)
	m_fAnimationMaxFrame = 4.f;
	m_fAnimationSpeed = 5.f;
	m_iState = STATE_MOVE;
	m_fSpawnCooldown = 0.2f;
	m_fCooldownTime = 0.5f;
	return S_OK;
}

void CNukemutant::Priority_Update(_float fTimeDelta)
{
	Set_Animation();
	__super::Priority_Update(fTimeDelta);
}

EVENT CNukemutant::Update(_float fTimeDelta)
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

void CNukemutant::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CNukemutant::Render()
{
	return __super::Render();

	//특별히 더 렌더링 할게 있는 경우 ↓
}


//void CGreater::On_Collision(_uint MyColliderID, _uint OtherColliderID)
//{
//	if (CI_BLOCK(OtherColliderID))
//	{
//		m_pCollider->Get_Last_Collision_Pos();
//
//		_float3 vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
//
//		_float3 Depth = m_pCollider->Get_Last_Collision_Depth();
//		if (Depth.y != 0)
//			int a = 1;
//		vPos += Depth;
//
//		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
//	}
//	else if (CI_WEAPON(OtherColliderID))
//	{
//		//그 즉시 배틀모드 진입
//		if (m_eState != MODE::MODE_RETURN)
//			m_eState = MODE::MODE_BATTLE;
//
//		//위치탐색
//		_float3 vImpactPos = CalculateEffectPos();
//
//		//몬스터 사망
//		if (0 >= m_iHP)
//		{
//			CFXMgr::Get_Instance()->SpawnCustomExplosion(vImpactPos, LEVEL_GAMEPLAY, _float3{ 130.f, 160.f, 1.f }, TEXT("PC_Explosion"), 14);
//			m_bDead = true;
//
//			return;
//		}
//
//		// 이펙트 생성
//		m_iHP += -50;
//		CFXMgr::Get_Instance()->SpawnBlood(vImpactPos, LEVEL_GAMEPLAY);
//	}
//}
//
//void CGreater::MonsterTick(_float dt)
//{
//	//상태변화
//	switch (m_eState)
//	{
//	case MODE::MODE_IDLE:
//		if (IsPlayerDetected())	// 감지 거리 기준 계산
//		{
//			//플레이어 발견 시 행동
//			cout << "Shotgunner 플레이어 감지!!" << endl;
//			m_eState = MODE::MODE_DETECTIVE;
//		}
//		break;
//
//	case MODE::MODE_DETECTIVE:
//		//플레이어 공격 가능할 때 까지 탐색
//		m_fRaycastTicker += dt;
//		if (m_fRaycastTicker > 0.5f)
//		{
//			if (IsMonsterAbleToAttack())	// RayPicking으로 플레이어와 몬스터 사이 장애물 X <- 공격 가능 상태가 됨
//				m_eState = MODE::MODE_READY;
//		}
//		break;
//
//	case MODE::MODE_READY:
//		//공격 할 준비  ( 장전 등의 딜레이 필요 )
//		// 준비 끝나면
//		if (m_isReadyToAttack)
//			m_eState = MODE::MODE_BATTLE;
//		break;
//
//	case MODE::MODE_BATTLE:
//		//실제 공격 시 행동
//		if (m_bCoolingDown)
//		{
//			m_fCooldownDuration = 0.f;
//			m_eState = MODE::MODE_READY;
//			m_isReadyToAttack = false;
//			m_bCoolingDown = false;
//		}
//		m_fRaycastTicker += dt;
//		if (m_fRaycastTicker > 0.5f)
//		{
//			if (false == IsMonsterAbleToAttack())
//			{
//				m_fCooldownDuration = 0.f;
//				m_eState = MODE::MODE_DETECTIVE;
//				m_isReadyToAttack = false;
//				m_bCoolingDown = false;
//			}
//		}
//
//		break;
//
//	case MODE::MODE_RETURN:
//		//본래위치로 돌아가고 IDLE로 상태가 변한다.
//		break;
//	}
//
//#ifdef _DEBUG
//	auto now = steady_clock::now();
//	auto elapsed = duration_cast<milliseconds>(now - g_LastLogTime).count();
//
//	if (elapsed >= 1000)
//	{
//		// 1초 이상 지났다면 출력
//		cout << "[샷거너]\t플레이어와의 거리 : " << m_fCurDistance << endl;
//		cout << "[샷거너]\t상태 : ";
//		switch (m_eState)
//		{
//		case Client::CMonster::MODE_IDLE:
//			cout << "IDLE";
//			break;
//		case Client::CMonster::MODE_READY:
//			cout << "READY";
//			break;
//		case Client::CMonster::MODE_BATTLE:
//			cout << "BATTLE";
//			break;
//		case Client::CMonster::MODE_DETECTIVE:
//			cout << "DETECTIVE";
//			break;
//		case Client::CMonster::MODE_RETURN:
//			cout << "RETURN";
//			break;
//		case Client::CMonster::MODE_END:
//			cout << "UNKNOWN";
//			break;
//		default:
//			cout << "UNKNOWN";
//			break;
//		}
//		cout << endl;
//		g_LastLogTime = now;
//	}
//#endif
//
//	// 상태행동(액션)
//	switch (m_eState)
//	{
//	case MODE::MODE_IDLE:
//		DoIdle(dt);
//		break;
//
//	case MODE::MODE_DETECTIVE:
//		DoDetect(dt);
//		break;
//
//	case MODE::MODE_READY:
//		DoReady(dt);
//		break;
//
//	case MODE::MODE_BATTLE:
//		DoBattle(dt);
//		break;
//
//	case MODE::MODE_RETURN:
//		DoReturn(dt);
//		break;
//	}
//}
//
//void CGreater::DoDetect(_float dt)
//{
//	// 감지 가능 거리 이내일 때 / 감지 상태 중 추격 가능 거리일 때
//	ChasePlayer(dt, 50.f);
//	m_eCurMonsterState = STATE_MOVE;
//}
//
//
//_bool CGreater::IsMonsterAbleToAttack()
//{
//	// 여기 레이캐스팅으로 플레이어와 몬스터 사이 장애물 유무 체크
//	if (m_fCurDistance > 200.f)
//		return false;
//	//if (m_fRaycastTicker > 0.5f)
//	{
//		m_fRaycastTicker = 0.f;
//		if (true == Raycast_Player())
//			return true;
//		else
//			return false;
//	}
//}
//
//void CGreater::DoReady(_float dt)
//{
//	m_fCooldownDuration += dt;
//	if (m_fCooldownDuration >= m_fCooldownTime)
//		m_isReadyToAttack = true;
//	m_fAnimationFrame = 0.f;
//}
//
//void CGreater::DoBattle(_float dt)
//{
//	AttackPattern(dt);
//}
//
//void CGreater::DoIdle(_float dt)
//{
//	switch (m_eIdlePhase)
//	{
//	case EIdlePhase::IDLE_MOVE:
//	{
//		m_eCurMonsterState = STATE_MOVE;
//		m_fWanderElapsed += dt;
//
//		m_pTransformCom->Go_Straight(dt);
//
//		if (m_fWanderElapsed >= m_fWanderTime)
//		{
//			m_fWanderElapsed = 0.f;
//			m_eIdlePhase = EIdlePhase::IDLE_WAIT;
//		}
//		break;
//	}
//	case EIdlePhase::IDLE_WAIT:
//		m_fIdleWaitElapsed += dt;
//		m_eCurMonsterState = STATE_STAY;
//
//		if (m_fIdleWaitElapsed >= m_fIdleWaitTime)
//		{
//			SetRandomDirection();                  // 회전할 방향 설정
//			m_fIdleWaitElapsed = 0.f;
//			m_eIdlePhase = EIdlePhase::IDLE_TURN; // 다음엔 회전하러 간다
//		}
//		break;
//
//	case EIdlePhase::IDLE_TURN:
//	{
//		m_eCurMonsterState = STATE_STAY;
//		_float3 vLook = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);
//
//		bool bRotated = m_pTransformCom->RotateToDirection(vLook, m_vDirection, 5.f, dt);
//		if (bRotated)  // 회전 완료 신호
//		{
//			m_eIdlePhase = EIdlePhase::IDLE_MOVE;
//		}
//		break;
//	}
//	}
//}
//
//void CGreater::AttackPattern(_float dt)
//{
//	// 실제 공격 패턴 작성하는 곳
//	// 잡몹이라 일반공격정도만
//	// Archangel 특수공격 있음
//	// Wenteko 넣을 시 얘도 있음
//	m_eCurMonsterState = STATE_ATTACK;
//
//	m_fSpawnNormalBullet += dt;
//	m_fAttackTimer += dt;
//	if (m_fAttackTimer >= 2.f)
//	{
//		m_bCoolingDown = true;
//		m_fAttackTimer = 0.f;
//	}
//	if (m_fSpawnNormalBullet >= m_fSpawnCooldown)
//	{
//		_float3 TargetPos = *static_cast<CTransform*>(m_pTargetPlayer->Find_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);
//		m_pTransformCom->LookAt(TargetPos);
//		// 0.2초마다 발사
//		CMonsterNormalBullet::DESC MonsterNormalBullet_iDesc{};
//		MonsterNormalBullet_iDesc.fSpeedPerSec = 60.f;
//		MonsterNormalBullet_iDesc.fRotationPerSec = RADIAN(180.f);
//		MonsterNormalBullet_iDesc.vScale = { 10.f, 10.f, 0.f };
//		MonsterNormalBullet_iDesc.vPosition = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
//		MonsterNormalBullet_iDesc.vPosition.y += 10.f;
//
//		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_MonsterNormalBullet"),
//			LEVEL_GAMEPLAY, L"Layer_MonsterBullet", &MonsterNormalBullet_iDesc)))
//			return;
//
//		m_fSpawnNormalBullet = 0.f;
//	}
//}
//
//void CGreater::ChasePlayer(_float dt, _float fChaseDist)
//{
//	//타겟을 350거리까지 추격한다.
//	_float3 TargetPos = *static_cast<CTransform*>(m_pTargetPlayer->Find_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);
//
//	// 현재 위치
//	_float3 vMyPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
//
//	// 방향 계산
//	_float3 vDir = TargetPos - vMyPos;
//	float fDistance = vDir.Length();
//	vDir.Normalize();
//
//	//원래방향으로 턴하기
//	_float3 vLook = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);
//	bool bRotated = m_pTransformCom->RotateToDirection(vLook, vDir, 5.f, dt);
//	m_pTransformCom->ChaseWithOutY(TargetPos, dt, fChaseDist, 150.f);
//}



HRESULT CNukemutant::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	Ready_Textures();

	return S_OK;
}

HRESULT CNukemutant::Ready_Textures()
{
	/* MOVE */
	for (_uint i = 0; i < D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Nukemutant_Move_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(LEVEL_STATIC, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Nukemutant_Move_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_MOVE][i])))))
			return E_FAIL;
	}

	/* ATTACK */
	for (_uint i = 0; i < D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Nukemutant_Attack_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;	
		if (FAILED(__super::Add_Component(LEVEL_STATIC, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Nukemutant_Attack_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_ATTACK][i])))))
			return E_FAIL;
	}

	/* REVIVE */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_Nukemutant_Revive",
		_wstring(TEXT("Com_Texture")) + L"_Nukemutant_Revive", reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_REVIVE][0])))))
		return E_FAIL;

	/* DEAD */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_Nukemutant_Dead",
		_wstring(TEXT("Com_Texture")) + L"_Nukemutant_Dead", reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_DEAD][0])))))
		return E_FAIL;

	/* MOVE */
	//for (_uint i = 0; i < D_END; i++)
	//{
	//	_wstring sPrototypeTag = L"Prototype_Component_Texture_Nukemutant_Move_";
	//	_uint num = static_cast<_uint>(i * m_fDivOffset);
	//	_tchar buf[32];
	//	_itow_s((int)num, buf, 10);
	//	sPrototypeTag += buf;
	//	if (FAILED(__super::Add_Component(m_eLevelID, sPrototypeTag,
	//		_wstring(TEXT("Com_Texture")) + L"_Nukemutant_Move_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_MOVE][i])))))
	//		return E_FAIL;
	//}
	return S_OK;
}

HRESULT CNukemutant::Set_Animation()
{
	if (m_eCurMonsterState != m_ePrevMonsterState)
	{
		m_ePrevMonsterState = m_eCurMonsterState;
		m_fAnimationFrame = 0.f;
		m_iState = (_uint)(m_eCurMonsterState);
		switch (m_eCurMonsterState)
		{
		case Client::CNukemutant::STATE_MOVE:
			m_fAnimationMaxFrame = _float(MAX_MOVERUN);
			m_fAnimationSpeed = 10.f;
			break;
		case Client::CNukemutant::STATE_ATTACK:
			m_fAnimationMaxFrame = _float(MAX_ATTACK);
			m_fAnimationSpeed = 10.f;
			break;
		case Client::CNukemutant::STATE_STAY:
			m_fAnimationMaxFrame = 1.f;
			m_fAnimationSpeed = 0.f;
			m_iState = (_uint)(STATE_MOVE);
			break;
		case Client::CNukemutant::STATE_DEAD:
			m_fAnimationMaxFrame = _float(MAX_DEAD);
			m_fAnimationSpeed = 8.f;
			m_bRotateAnimation = false;
			break;
		case Client::CNukemutant::STATE_REVIVE:
			m_fAnimationMaxFrame = _float(MAX_REVIVE);
			m_fAnimationSpeed = 8.f;
			m_bRotateAnimation = false;
			break;
		}
	}
	return S_OK;
}

HRESULT CNukemutant::Animate_Monster(_float fTimeDelta)
{
	if (m_fAnimationMaxFrame < 2.f)
		return S_OK;

	switch (m_eCurMonsterState)
	{
	case Client::CNukemutant::STATE_STAY:
		m_bRotateAnimation = true;
		return S_OK;
	case Client::CNukemutant::STATE_MOVE:
		m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		if (m_fAnimationFrame >= m_fAnimationMaxFrame)
			m_fAnimationFrame = 0.f;
		m_bRotateAnimation = true;
		break;
	case Client::CNukemutant::STATE_ATTACK:
		m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		if (m_fAnimationFrame >= m_fAnimationMaxFrame)
			m_fAnimationFrame = 0.f;
		m_bRotateAnimation = true;
		break;
	case Client::CNukemutant::STATE_DEAD:
		m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		if (m_fAnimationFrame >= m_fAnimationMaxFrame)
			m_fAnimationFrame = m_fAnimationMaxFrame - 1.f;
		m_bRotateAnimation = false;
		break;
	case Client::CNukemutant::STATE_REVIVE:
		m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		if (m_fAnimationFrame >= m_fAnimationMaxFrame)
			m_eCurMonsterState = STATE_STAY;
		m_bRotateAnimation = false;
		break;
	}
	return S_OK;
}

CNukemutant* CNukemutant::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CNukemutant* pInstance = new CNukemutant(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CNukemutant");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CNukemutant::Clone(void* pArg)
{
	CNukemutant* pInstance = new CNukemutant(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CNukemutant");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNukemutant::Free()
{
	__super::Free();
}