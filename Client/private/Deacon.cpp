// 내 클래스 이름 : TestMonster
// 부모 클래스 이름 : Monster

#include "Deacon.h"
#include "FXMgr.h"
#include "MonsterNormalBullet.h"

CDeacon::CDeacon(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster{ pGraphic_Device }
{
}

CDeacon::CDeacon(const CDeacon& Prototype)
	: CMonster(Prototype)
{
}

HRESULT CDeacon::Initialize_Prototype()
{
	//프로토타입의 기본정의
	m_szTextureID = TEXT("Deacon_Walk");
	m_szBufferType = TEXT("Rect");

	//속성
	m_iHP = 30;
	m_iMaxHP = 30;
	m_iAttackPower = 5;
	m_iDefense = 1;
	m_fSpeed = 13.f;
	m_vScale = { 32.f, 39.6f, 1.f };
	m_eState = MODE::MODE_IDLE;

	m_fDetectiveDistance = 500.f;
	m_fAttackDistance = 400.f;
	//부속성
	m_strDialogue = "Deacon..Deacon...";
	m_strSound = "SoundFilePath";

	m_vDropItems.push_back("DeaconDropItem");

	return S_OK;
}

HRESULT CDeacon::Initialize(void* pArg)
{
	//위치, 크기초기화, 컴포넌트 부착
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_fDivOffset = 45.f;
	//애니메이션(수정예정)
	m_fAnimationMaxFrame = 0.f;
	m_fAnimationSpeed = 5.f;
	m_iState = STATE_FLY;
	m_fCooldownTime = 0.5f;     // 공격 쉬는 텀
	m_fBulletCooldown = 0.2f;	// 총알 발사 쿨
	m_fAttackTime = 0.5f;		// 공격 시간

	/* [ 사운드 설정칸 ] */
	m_pSoundCom->Set3DState(100.f, 1000.f);
	m_pSoundCom->SetVolume("Attack", 0.4f);
	m_pSoundCom->SetVolume("Chacing", 0.4f);
	m_pSoundCom->SetVolume("Die", 0.4f);
	m_pSoundCom->SetVolume("Hit", 0.4f);
	return S_OK;
}

void CDeacon::Priority_Update(_float fTimeDelta)
{
	_float3 vCurPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_pSoundCom->Update3DPosition(vCurPos);

	if (m_eCurMonsterState == STATE_DEAD)
		int a = 0;

	Set_Animation();
	__super::Priority_Update(fTimeDelta);
}

EVENT CDeacon::Update(_float fTimeDelta)
{
	if (m_bDead && !m_bDeadSound)
	{
		m_pSoundCom->Play("Die");
		m_bDeadSound = true;
	}
	return __super::Update(fTimeDelta);
}

void CDeacon::Late_Update(_float fTimeDelta)
{
	/* 중력 업데이트 없어서 그냥 오버라이딩 함 */
	_float3	vTemp = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	CGameObject::Compute_ViewZ(&vTemp);

	PlayerDistance();
	CalculateVectorToPlayer();

	if (m_bDead) 
	{
		m_pGravityCom->Update(fTimeDelta);	
		_float3 vOrigSize = {};
		m_pTextureMap[m_iState][m_iDegree]->Get_TextureSize(static_cast<_uint>(m_fAnimationFrame), &vOrigSize);
		_float fComputedSizeYFromOrig = -vOrigSize.y * 0.5f + 20.f;
		auto newY = m_vScale.y - 20.f;
		//m_pCollider->Update_OffSet({ 0.f, -10.f, 0.f });
		m_pCollider->Update_OffSet({ 0.f, -10.f, 0.f });
		//m_pCollider->Update_Scale({ vOrigSize.x, 20.f, 1.f });
		m_pCollider->Update_Scale({ vOrigSize.x * 0.5f, 3.f, 1.f });

		if (m_pHeadCollider != nullptr)
			m_pHeadCollider->Update_OffSet({ 0.f,10000.f,0.f });
	}

	m_pCollider->Update_Collider();
	if (m_pHeadCollider != nullptr)
		m_pHeadCollider->Update_Collider();



	Compute_ViewAngle();
	Set_TextureType();

	if (false == m_bRotateAnimation)
		m_iDegree = 0;
	Resize_Texture(0.4f);

	const _float3& vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	if (m_pGameInstance->IsInFrustum(vPos, m_pTransformCom->Get_Radius()))
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLEND, this);
}

HRESULT CDeacon::Render()
{
	return __super::Render();

	//특별히 더 렌더링 할게 있는 경우 ↓
} 

HRESULT CDeacon::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;
	Ready_Textures();

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Sound_Deacon"),
		TEXT("Com_Sound"), reinterpret_cast<CComponent**>(&m_pSoundCom))))
		return E_FAIL;

	return S_OK;
}

void CDeacon::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	__super::On_Collision_NormalMonster(MyColliderID, OtherColliderID);
}

void CDeacon::MonsterTick(_float dt)
{
	//상태변화
	switch (m_eState)
	{
	case MODE::MODE_IDLE:
		if (IsPlayerDetected())	// 감지 거리 기준 계산
		{
			//플레이어 발견 시 행동
			m_bFoundPlayer = true;
			cout << "Deacon 플레이어 감지!!" << endl;
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
		//본래위치로 돌아가고 IDLE로 상태가 변한다.
		break;
	}

#ifdef _CONSOL
	auto now = steady_clock::now();
	auto elapsed = duration_cast<milliseconds>(now - g_LastLogTime).count();

	if (elapsed >= 1000)
	{
		// 1초 이상 지났다면 출력
		//cout << "따끈이 상태 : " << GetMonsterStateName(m_eState) << '\n';
		cout << "[다콘]\t플레이어와의 거리 : " << m_fCurDistance << endl;
		cout << "[다콘]\t상태 : ";
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

void CDeacon::DoDetect(_float dt)
{
	// 감지 가능 거리 이내일 때 / 감지 상태 중 추격 가능 거리일 때
	ChasePlayer(dt, 50.f);
	m_eCurMonsterState = STATE_FLY;
}

_bool CDeacon::IsMonsterAbleToAttack()
{
	m_fRaycastTicker = 0.f;
	if (m_fCurDistance > m_fAttackDistance)
		return false;
	return true;
	// 여기 레이캐스팅으로 플레이어와 몬스터 사이 장애물 유무 체크 였던것
	//return Raycast_Player();
}

void CDeacon::DoReady(_float dt)
{
	//m_fCooldownDuration += dt;
	//if (m_fCooldownDuration >= m_fCooldownTime)
	//	m_isReadyToAttack = true;

	m_fCooldownDuration += dt;
	if (m_fCooldownDuration >= m_fCooldownTime)
	{
		m_isReadyToAttack = true;
		m_fBulletCooldownElapsed = 0.4f;
		m_fCooldownDuration = 0.f;
	}
	m_eCurMonsterState = STATE_STAY;
	m_fAnimationFrame = 0.f;
	_float3 TargetPos = *static_cast<CTransform*>(m_pTargetPlayer->Find_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);
	m_pTransformCom->LookAt(TargetPos);

}

void CDeacon::DoBattle(_float dt)
{
	AttackPattern(dt);
}

void CDeacon::DoIdle(_float dt)
{
	switch (m_eIdlePhase)
	{
	case EIdlePhase::IDLE_MOVE:
	{
		m_eCurMonsterState = STATE_FLY;
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
		m_eCurMonsterState = STATE_FLY;
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

void CDeacon::AttackPattern(_float dt)
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

		// 0.2초마다 발사
		CMonsterNormalBullet::DESC MonsterNormalBullet_iDesc{};
		MonsterNormalBullet_iDesc.fSpeedPerSec = 800.f;
		MonsterNormalBullet_iDesc.fRotationPerSec = RADIAN(180.f);
		MonsterNormalBullet_iDesc.vScale = { 5.f, 5.f, 0.f };
		MonsterNormalBullet_iDesc.vPosition = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		MonsterNormalBullet_iDesc.vPosition.y += 10.f;

		_float3 vRight = *m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		vRight.Normalize();
		MonsterNormalBullet_iDesc.vPosition += vRight * 15.f * _float(m_iLeftRight);
		MonsterNormalBullet_iDesc.bCollision = true;

		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_MonsterNormalBullet"),
			LEVEL_GAMEPLAY, L"Layer_MonsterBullet", &MonsterNormalBullet_iDesc)))
			return;

		if (!m_pSoundCom->IsPlaying("Attack"))
			m_pSoundCom->Play("Attack");

		m_iLeftRight = m_iLeftRight * -1;

		m_fBulletCooldownElapsed = 0.f;
	}
}

void CDeacon::ChasePlayer(_float dt, _float fChaseDist)
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
	m_pTransformCom->Chase(TargetPos, dt, fChaseDist);
}

HRESULT CDeacon::Ready_Textures()
{
	/* FLY */
	for (_uint i = 0; i < D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Deacon_Fly_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(LEVEL_STATIC, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Deacon_Fly_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_FLY][i])))))
			return E_FAIL;
	}
	/* ATTACK */
	for (_uint i = 0; i < D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Deacon_Attack_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(LEVEL_STATIC, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Deacon_Attack_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_ATTACK][i])))))
			return E_FAIL;
	}
	/* DEAD */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_Deacon_Dead",
		_wstring(TEXT("Com_Texture")) + L"_Deacon_Dead", reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_DEAD][0])))))
		return E_FAIL;

	return S_OK;
}

HRESULT CDeacon::Set_Animation()
{
	if (m_eCurMonsterState != m_ePrevMonsterState)
	{
		m_ePrevMonsterState = m_eCurMonsterState;
		m_fAnimationFrame = 0.f;
		m_iState = (_uint)(m_eCurMonsterState);
		switch (m_eCurMonsterState)
		{
		case Client::CDeacon::STATE_FLY:
			m_fAnimationMaxFrame = 1.f;
			m_fAnimationFrame = _float(m_eCurFlyingDirection);
			m_fAnimationSpeed = 0.f;
			break;
		case Client::CDeacon::STATE_ATTACK:
			m_fAnimationMaxFrame = _float(MAX_ATTACK);
			m_fAnimationSpeed = 10.f;
			break;
		case Client::CDeacon::STATE_STAY:
			m_fAnimationMaxFrame = 1.f;
			m_fAnimationSpeed = 0.f;
			m_iState = (_uint)(STATE_FLY);
			break;
		case Client::CDeacon::STATE_DEAD:
			m_fAnimationMaxFrame = _float(MAX_DEAD);
			m_fAnimationSpeed = 9.f;
			m_bRotateAnimation = false;
			break;
		}
	}
	if (m_eCurFlyingDirection != m_ePrevFlyingDirection)
	{
		m_ePrevFlyingDirection = m_eCurFlyingDirection;
		m_fAnimationFrame = _float(m_eCurFlyingDirection);
	}

	return S_OK;
}

HRESULT CDeacon::Animate_Monster(_float fTimeDelta)
{
	if (m_fAnimationMaxFrame < 2.f)
		return S_OK;

	switch (m_eCurMonsterState)
	{
	case Client::CDeacon::STATE_STAY:
		return S_OK;
	case Client::CDeacon::STATE_FLY:
		if (m_iDegree != 0 && m_iDegree != 5 && m_bCW == true)
		{
			if (m_eCurFlyingDirection == LEFT)
				m_fAnimationFrame = (_float)(RIGHT);
			else if (m_eCurFlyingDirection == RIGHT)
				m_fAnimationFrame = (_float)(LEFT);
		}
		else
			m_fAnimationFrame = (_float)(m_eCurFlyingDirection);
		break;
	case Client::CDeacon::STATE_ATTACK:
		m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		if (m_fAnimationFrame >= m_fAnimationMaxFrame)
			m_fAnimationFrame = 0.f;
		break;
	case Client::CDeacon::STATE_DEAD:
		m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		if (m_fAnimationFrame >= m_fAnimationMaxFrame)
		{
			m_fAnimationFrame = m_fAnimationMaxFrame -1.f;
			m_fAnimationSpeed = 0.f;
		}
		break;
	}
	return S_OK;
}


CDeacon* CDeacon::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	//프로토타입의 기본정의
	CDeacon* pInstance = new CDeacon(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDeacon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDeacon::Clone(void* pArg)
{
	CDeacon* pInstance = new CDeacon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CDeacon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDeacon::Free()
{
	__super::Free();

	Safe_Release(m_pSoundCom);
}