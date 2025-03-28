#include "Ttakkeun_i.h"
#include "FXMgr.h"
#include "MonsterBullet.h"
#include "FlyEffect.h"
#include "GrenadeBullet.h"

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
	//��ġ, ũ���ʱ�ȭ, ������Ʈ ����
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_isReadyMonster = true;
	m_fDivOffset = 22.5f;
	// ������ ��� �ؽ��� ������ �޶� ���� ����
	Ready_Textures();
	
	//�ִϸ��̼�(��������)
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
			CFXMgr::Get_Instance()->SpawnCustomExplosion(vPos, LEVEL_GAMEPLAY, _float3{ 60.f, 100.f, 1.f }, TEXT("Effect_Explorer"), 24);
			m_fCallTimer = 0.f;
		}
		if (m_fTotalTime >= 3.f)
		{
			_float3 vImpactPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			CFXMgr::Get_Instance()->SpawnCustomExplosion(vImpactPos, LEVEL_GAMEPLAY, _float3{ 200.f, 250.f, 1.f }, TEXT("Effect_Explor"), 32);
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
	//�÷��̾� ���� ������Ʈ
	PlayerDistance();
	CalculateVectorToPlayer();
	IsPlayerDetected();

	if (m_eCurMonsterState != STATE_FLY &&
		m_eCurMonsterState != STATE_FLY_ATTACK &&
		!m_bJumpStart)
	{
		m_pGravityCom->Update(fTimeDelta);
	}

	Compute_ViewAngle();
	//�ݶ��̴� ������Ʈ
	m_pCollider->Update_Collider();

	//�����׷� ������Ʈ
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
		return;

	//__super::Late_Update(fTimeDelta);
#ifdef _DEBUG
	auto now = steady_clock::now();
	auto elapsed = duration_cast<milliseconds>(now - g_LastLogTime).count();
	
	if (elapsed >= 1000) 
	{
		// 1�� �̻� �����ٸ� ���
		//cout << "������ ���� : " << GetMonsterStateName(m_eState) << '\n';
		//cout << "�÷��̾���� �Ÿ� : " << m_fCurDistance << endl;
		cout << "������ �������� : " << m_iRandom << endl;
		g_LastLogTime = now;
	}
#endif
}

HRESULT CTtakkeun_i::Render()
{
	
	return __super::Render();

	//Ư���� �� ������ �Ұ� �ִ� ��� ��
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
			//���ƿ����� ����
			if (m_bFlyAttack)
				m_eCurMonsterState = STATE_FLY;
			else if (!m_bFlyAttack && !m_bJumpStart)
				m_fAnimationFrame += fTimeDelta * 0.1f;
			
			//������� ����(������ǿ� ��ž�Ϸ���)
			if (m_bJumpStart)
			{
				if (m_bJumpEnd)
					m_fAnimationFrame += fTimeDelta * 0.1f;
					
				//2������ ����
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
			SetRandomDirection();                  // ȸ���� ���� ����
			m_fIdleWaitElapsed = 0.f;
			m_eIdlePhase = EIdlePhase::IDLE_TURN; // ������ ȸ���Ϸ� ����
		}
		break;

	case EIdlePhase::IDLE_TURN:
	{
		m_eCurMonsterState = STATE_WALK;
		_float3 vLook = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);

		bool bRotated = m_pTransformCom->RotateToDirection(vLook, m_vDirection, 5.f, dt);
		if (bRotated)  // ȸ�� �Ϸ� ��ȣ
		{
			m_eIdlePhase = EIdlePhase::IDLE_MOVE;
		}
		break;
	}
	}
}

void CTtakkeun_i::DoBattle(_float dt)
{
	// 1. �÷��̾���� �Ÿ� ���
	_float fAttackRange = 400.f;
	_float fChaseRange = 700.f;

	//���� ���߰������̶��
	if (m_bFlyAttack)
	{
		FlyAttack(dt);
		return;
	}

	// 2. �Ÿ� ���� �б�
	if (m_fCurDistance < fAttackRange)
	{
		// ������ ����
		AttackPattern(dt);
	}
	else
	{
		//�����̴� ���� ���մϴ�. (�߰�)
		ChasePlayer(dt);
	}
}

void CTtakkeun_i::AttackPattern(_float dt)
{
	// ��ٿ� ���̸� �������� ����
	if (m_bCoolingDown)
	{
		m_fCooldownTime += dt;

		if (m_fCooldownTime >= m_fCooldownDuration)
		{
			m_iRandom = GetRandomInt(0, 3);
			m_bCoolingDown = false;
			m_fCooldownTime = 0.f;
		}
		return;
	}

	//�����϶�~
	BasicAttackSet(dt);
}

void CTtakkeun_i::BasicAttackSet(_float dt)
{
	/* �������� ��������[ 1������ ] */
	m_iRandom = 2;
	switch (m_iRandom)
	{
	case 0: 
		/* 1. ȭ����� */
		FireAttack(dt);	
		break;
	case 1:
		/* 2. ���ƿ����� �����̿� */
		FlyAttack(dt);
		break;
	case 2:
		/* 3. �ٿ �� */
		BounceBall(dt);
		break;
	case 3:
		/* 4. ������� */
		JumpAttack(dt);
		break;
	case 4:
		/* 5. ���� �̻��Ϲ߻� */
		MissileAttack(dt);
		break;
	case 5:
		/* 6. ���� ��ȯ */
		SpawnAttack(dt);
		break;
	}
}


void CTtakkeun_i::MissileAttack(_float dt)
{
}

void CTtakkeun_i::SpawnAttack(_float dt)
{
}

void CTtakkeun_i::FireAttack(_float dt)
{
	// ���� ��Ÿ��
	m_fAttackTimer += dt;
	if (m_fAttackTimer >= 2.f)
	{
		StartCooldown(dt, 2.f, 2.5f);
		m_fAttackTimer = 0.f;
		return;
	}

	/* ȭ����� ���� */

	/* 1. �÷��̾�� �����Ѵ�(���� ������) */
	_float3 vPlayerPos = *static_cast<CTransform*>(m_pTargetPlayer->Find_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);
	vPlayerPos.y = m_vReturnPos.y;
	_float3 vMyPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	//(���� ������)
	_float3 vDir = vPlayerPos - vMyPos;
	float fDistance = vDir.Length();
	vDir.Normalize();

	_float3 vLook = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	bool bRotated = m_pTransformCom->RotateToDirection(vLook, vDir, 5.f, dt);

	m_pTransformCom->ChaseCustom(vPlayerPos, dt, 100.f, 150.f);
	/* 2. ȭ���� �߻��Ѵ�! */
	CFXMgr::Get_Instance()->FireAttack(vMyPos, LEVEL_GAMEPLAY);

}

void CTtakkeun_i::BounceBall(_float dt)
{
	/* �ٿ�� ���� */

	/* 1. �����̴� ������ ���·� �÷��̾ �������Ѵ�. */
	m_bRotateAnimation = false;
	
	/* �躤�� ȸ�� */
	_float3 vPlayerPos = *static_cast<CTransform*>(m_pTargetPlayer->Find_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);
	_float3 vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vToPlayer = vPlayerPos - vPos;
	_float3 vLook = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	bool bRotated = m_pTransformCom->RotateToDirection(vLook, vToPlayer, 60.f, dt);

	/* 2. �����ð����� �ٿ���� �÷��̾�� 3���� �߻��Ѵ�. */
	m_fBounceTime += dt;
	if (m_fBounceTime >= 1.f)
	{
		//���⿡ ��ȯ �Լ� ȣ���ϸ� ��!
		SpawnBounce();
		m_iBounceCount++;
		m_fBounceTime = 0.f;
		return;
	}

	/* 3. �� 4���� �߻��ϸ� ��ٿ��Լ��� ȣ���Ѵ� */
	if (m_iBounceCount > 3)
	{
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

	/* [ �������� ������ �� ��, �÷��̾� �������� 3�� �����Ŵ´� ] */

	/* �ʿ亯�� */
	_float3 vPlayerPos = *static_cast<CTransform*>(m_pTargetPlayer->Find_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);
	_float3 vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vToPlayer = vPlayerPos - vPos;
	m_bJumpStart = true;
	m_eCurMonsterState = STATE_JUMP;


	/* 1. ��������� ���� �ڿ� Ÿ�̹� ���� ���ƿ�����. */

	if (m_eCurMonsterState == STATE_JUMP && m_fAnimationFrame == 2.f && !m_bJumpEnd)
	{
		//���ƿ����� �÷��̾� ������ ���� �ٰ��;��ҰŰ���.
		m_bIsFly = m_pTransformCom->Go_UpCustom(dt, 400.f, 300.f);
		if (!m_bIsFly)
		{
			//���� ������.
			_float3 vLook = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);
			bool bRotated = m_pTransformCom->RotateToDirection(vLook, vToPlayer, 60.f, dt);

			_float3 TargetPos = *static_cast<CTransform*>(m_pTargetPlayer->Find_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);
			m_pTransformCom->ChaseWithOutY(TargetPos,dt,80.f,500.f);
		}
		//�������� ���� true
		m_bIsLink = true;
	}

	/* 2. �������鼭 ���̶� �浹�ϸ� ��ƼŬ�� ����Ѵ�. */
	if (m_bIsFly && m_bIsLink)//���� true
	{
		//�ٴ� ������ float ���� ũ�� ����
		m_pGravityCom->Raycast_StandAble_Obj();
		if (vPos.y - 73.5f > m_pGravityCom->Get_FloorY())
		{
			m_bJumpEnd = true;

			// �߷� ���� ����
			m_fFallSpeed += 3000.f * dt;

			vPos.y -= m_fFallSpeed * dt;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

		}
		else
		{
			//����Ʈ ���� && ī�޶� ����ŷ
			vPos.y = 30.f;
			CFXMgr::Get_Instance()->JumpAttack(vPos, LEVEL_GAMEPLAY);

			//���Ͱ� ������ �� ��������
			vPos.y = 77.f;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
			m_fFallSpeed = 0.f;
			m_iJumpCount++;

			m_eCurMonsterState = STATE_WALK;

			m_bJumpStart = false;
			m_bJumpEnd = false;

			//���� false
			m_bIsLink = false;

			//����ؾ�¡~
			JumpAttack(dt);
		}
	}

}

void CTtakkeun_i::FlyAttack(_float dt)
{
	//�� ��ġ
	_float3 vMyPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	
	//���ƿ����� ����
	bool isFly = false;

	if (m_eCurMonsterState == STATE_WALK)
	{
		//���� �� Fly �� ������Ʈ�� �ٲ��ش�.
		m_eCurMonsterState = STATE_JUMP;
		m_bFlyAttack = true;

		//����Ʈ ��ȯ
		if (!m_bDoOnce)
		{
			FlyEffect();
			m_bDoOnce = true;
		}
	}

	//���Ͱ� �������� �������̱��� ���ƿ�����.
	if (m_eCurMonsterState == STATE_FLY || m_eCurMonsterState == STATE_FLY_ATTACK)
	{
		isFly = m_pTransformCom->Go_UpCustom(dt, 100.f, 250.f);
	}

	//���Ͱ� m_vReturnPos �� �������� ���߿��� ��۹�� ����.
	if (isFly)
	{
		if (m_pBossEffect)
		{
			static_cast<CFlyEffect*>(m_pBossEffect)->SetDead();
			m_pBossEffect = nullptr;
		}

		_float fRotationSpeed = 60.f;

		// ���� �� ���� ��������
		_float3 vLook = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);

		/* [ ȸ������ ���� ] */
		_float fAngleRad = RADIAN(fRotationSpeed * dt);

		_float cosA = cosf(fAngleRad);
		_float sinA = sinf(fAngleRad);

		_float3 vRotatedLook;
		vRotatedLook.x = vLook.x * cosA - vLook.z * sinA;
		vRotatedLook.z = vLook.x * sinA + vLook.z * cosA;
		vRotatedLook.y = vLook.y;

		//�躤�� ������Ʈ
		vRotatedLook.Normalize();
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, vRotatedLook);

		// �������� �̵�
		_float fSpeed = 600.f;
		_float3 vForward = vRotatedLook * fSpeed * dt;

		//���͹��� ����
		_float3 vToCenter = m_vReturnPos - vMyPos;
		vToCenter.y = 0.f;
		vToCenter.Normalize();

		// õõ�� �߽� ������ �б�
		_float fDriftSpeed = 100.f;
		_float3 vDrift = vToCenter * fDriftSpeed * dt;

		// ���� ��ġ = ȸ�� + �߽� �� �̵�
		_float3 vNewPos = vMyPos + vForward + vDrift;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vNewPos);
	}

	/* ���Ͱ� �÷��̾ ���ؼ� �̻����� �߻��Ѵ� */
	if (isFly)
	{
		//3�� �߻� �� ���� ����
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

		//fly ���¿��� 2�ʸ��� �̻��� �߻� �Լ��� ȣ���Ѵ�.
		m_fFlyAttack += dt;

		if (m_fFlyAttack >= 4.f)
		{
			// 4�ʸ��� �ʱ�ȭ
			m_fFlyAttack = 0.f;
			++m_iMissileCount;
		}

		if (m_fFlyAttack >= 2.f) // 2~4�� ����
		{
			/* ���ݸ������ �ٲٰ� �̻����� �߻��Ѵ� */
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
	m_fCooldownDuration = GetRandomFloat(fMin, fMax); // ��ٿ� �ð� ����
	m_fCooldownTime = 0.f;
}

void CTtakkeun_i::SpawnMissile(_float dt)
{
	m_fSpawnMissile += dt;

	if (m_fSpawnMissile >= 0.2f)
	{
		// 0.2�ʸ��� �߻�
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

void CTtakkeun_i::SpawnBounce()
{
	//���� ������
	_float3 fMyPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	fMyPos.y += 30.f;

	//�߻����
	_float3 fMyLook = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	fMyLook.Normalize();

	float fAngleOffsets[3] = { -30.f, 0.f, 30.f };

	for (int i = 0; i < 3; ++i)
	{
		float rad = RADIAN(fAngleOffsets[i]);
		_float3 vDir = Rotate_Y(fMyLook, rad); // Y�� �������� ȸ��

		CGrenadeBullet::DESC BulletDesc{};
		BulletDesc.fSpeedPerSec = 300.f;
		BulletDesc.vScale = { 20.f,20.f,20.f };

		BulletDesc.vPosition = fMyPos;
		BulletDesc.szTextureTag = TEXT("MonsterBounce");
		BulletDesc.vLook = vDir;

		BulletDesc.ColliderGroup = CG_MBULLET;
		BulletDesc.fInitJumpPower = 30.f;
		BulletDesc.fTimeLimit = 2.f;

		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_GrenadeBullet"),
			LEVEL_GAMEPLAY, TEXT("Layer_Bullet"), &BulletDesc)))
			return;
	}
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

void CTtakkeun_i::FlyEffect()
{
	//���� ��ƼŬ
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

	m_pBossEffect = *ppOut;
}

void CTtakkeun_i::DoReturn(_float dt)
{
	// ���� ��ġ
	_float3 vMyPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	// ���� ���
	_float3 vDir = m_vReturnPos - vMyPos;
	float fDistance = vDir.Length();
	vDir.Normalize();

	//������������ ���ϱ�
	_float3 vLook = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	bool bRotated = m_pTransformCom->RotateToDirection(vLook, vDir, 5.f, dt);

	if (bRotated)  // ȸ�� �Ϸ� ��ȣ
	{
		// �ʹ� ������ �̵� ����
		if (fDistance < 10.0f) // ���� ���� ���� ���
		{
			m_eState = MODE::MODE_IDLE;
			m_eCurMonsterState = STATE_WALK;
			return;
		}

		// �̵� ó�� (dt ���)
		float fSpeed = 300.f;
		_float3 vMove = vDir * fSpeed * dt;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, (vMyPos + vMove));
	}
}


void CTtakkeun_i::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	//�� ��� ��Ʋ��� ����
	if(m_eState != MODE::MODE_RETURN)
		m_eState = MODE::MODE_BATTLE;

	//��ġŽ��
	_float3 vImpactPos = CalculateEffectPos();

	//���� ���
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

	// ����Ʈ ����
	m_iHP += -10;
	CFXMgr::Get_Instance()->SpawnBlood(vImpactPos, LEVEL_GAMEPLAY);
}

CTtakkeun_i* CTtakkeun_i::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	//������Ÿ���� �⺻����
	CTtakkeun_i* pInstance = new CTtakkeun_i(pGraphic_Device);

	//�⺻ ����
	pInstance->m_eLevelID		= LEVEL_GAMEPLAY;
	pInstance->m_szTextureID	= TEXT("Ttakkeun_i_Walk");
	pInstance->m_szBufferType	= TEXT("Rect");

	//�Ӽ�
	pInstance->m_iHP			= 500;
	pInstance->m_iMaxHP			= 500;
	pInstance->m_iAttackPower	= 10;
	pInstance->m_iDefense		= 3;
	pInstance->m_fSpeed			= 100.f;
	pInstance->m_vScale			= { 150.f, 147.f, 1.f };
	pInstance->m_eState			= MODE::MODE_IDLE;
	pInstance->m_fDetectiveDistance = 400.f;

	//�μӼ�
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