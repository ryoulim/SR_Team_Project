// �� Ŭ���� �̸� : TestMonster
// �θ� Ŭ���� �̸� : Monster

#include "Shotgunner.h"
#include "FXMgr.h"
#include "MonsterNormalBullet.h"

CShotgunner::CShotgunner(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster{ pGraphic_Device }
{
}

CShotgunner::CShotgunner(const CShotgunner& Prototype)
	: CMonster(Prototype)
{
}

HRESULT CShotgunner::Initialize_Prototype()
{
	//������Ÿ���� �⺻����
	m_szTextureID = TEXT("Shotgunner_Walk");
	m_szBufferType = TEXT("Rect");

	//�Ӽ�
	m_iHP = 80;
	m_iMaxHP = 80;
	m_iAttackPower = 6;
	m_iDefense = 0;
	m_fSpeed = 4.f;
	m_vScale = { 34.5f, 66.2f, 1.f };
	m_eState = MODE::MODE_IDLE;

	//�μӼ�
	m_strDialogue = "Gun..Gun...";
	m_strSound = "SoundFilePath";

	m_vDropItems.push_back("ShotgunnerDropItem");

	return S_OK;
}

HRESULT CShotgunner::Initialize(void* pArg)
{
	//��ġ, ũ���ʱ�ȭ, ������Ʈ ����
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	m_fDivOffset = 45.f;
	//�ִϸ��̼�(��������)
	m_pGravityCom->Set_Height(66.2f);
	m_fAnimationMaxFrame = 3.f;
	m_fAnimationSpeed = 5.f;
	m_iState = STATE_MOVE;
	m_fCooldownTime = 0.8f;     // ���� ���� ��
	m_fBulletCooldown = 0.4f;	// �Ѿ� �߻� ��
	m_fAttackTime = 0.35f;		// ���� �ð�
	return S_OK;				// m_fAttackTime���� �Ѿ� �߻��ϵ� �Ѿ��� m_fBulletCooldown��ŭ�� �������� ���
								// m_fAttackTime�� ������ m_fCooldownTime��ŭ ���� �ٽ� ������
}

void CShotgunner::Priority_Update(_float fTimeDelta)
{
	Set_Animation();
	__super::Priority_Update(fTimeDelta);
}

EVENT CShotgunner::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CShotgunner::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	if (m_bRotateAnimation == false)
		m_iDegree = 0;
	Resize_Texture(0.4f);
}

HRESULT CShotgunner::Render()
{
	return __super::Render();

	//Ư���� �� ������ �Ұ� �ִ� ��� ��
}

void CShotgunner::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	__super::On_Collision_NormalMonster(MyColliderID, OtherColliderID);
}

void CShotgunner::MonsterTick(_float dt)
{
	//���º�ȭ
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
		//������ġ�� ���ư��� IDLE�� ���°� ���Ѵ�.
		m_bFoundPlayer = false;
		break;
	}

#ifdef _CONSOL
	Debug_Output();
#endif

	// �����ൿ(�׼�)
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

void CShotgunner::DoDetect(_float dt)
{
	// ���� ���� �Ÿ� �̳��� �� / ���� ���� �� �߰� ���� �Ÿ��� ��
	ChasePlayer(dt, 50.f);
	m_eCurMonsterState = STATE_MOVE;
}


void CShotgunner::DoReady(_float dt)
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

void CShotgunner::DoBattle(_float dt)
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

void CShotgunner::DoIdle(_float dt)
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
			SetRandomDirection();                  // ȸ���� ���� ����
			m_fIdleWaitElapsed = 0.f;
			m_eIdlePhase = EIdlePhase::IDLE_TURN; // ������ ȸ���Ϸ� ����
		}
		break;

	case EIdlePhase::IDLE_TURN:
	{
		m_eCurMonsterState = STATE_STAY;
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

void CShotgunner::AttackPattern(_float dt)
{
	// ���� ���� ���� �ۼ��ϴ� ��
	// ����̶� �Ϲݰ���������
	// Archangel Ư������ ����
	// Wenteko ���� �� �굵 ����
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
		MonsterNormalBullet_iDesc.iColliderID = CI_MONSTER_SHOTGUNNER;
		MonsterNormalBullet_iDesc.fSpeedPerSec = 1000.f;
		// �Ѿ� �ӵ���
		MonsterNormalBullet_iDesc.fRotationPerSec = RADIAN(180.f);
		MonsterNormalBullet_iDesc.vScale = { 2.f, 2.f, 0.f };
		// �Ѿ� ũ����
		MonsterNormalBullet_iDesc.vPosition = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		MonsterNormalBullet_iDesc.vPosition.y += 15.f;
		_float3 vRight = *m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		vRight.Normalize();
		// �ѱ� ��ġ�� ������ ��ġ�� look ���͸� ����Ͽ� ���
		MonsterNormalBullet_iDesc.vPosition = MonsterNormalBullet_iDesc.vPosition + vRight * 11.f;
		// �Ѿ� ���� ��ġ ������

		for (size_t i = 0; i < 4; i++)
		{// �����̶� 4�� ���ÿ� �������� ����
			if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_MonsterNormalBullet"),
				m_eLevelID, L"Layer_MonsterBullet", &MonsterNormalBullet_iDesc)))
				return;
		}
		m_fBulletCooldownElapsed = 0.f;
	}

	/* ������ �ѹ� �� �� ���� �޽� */
}

void CShotgunner::ChasePlayer(_float dt, _float fChaseDist)
{
	//Ÿ���� 350�Ÿ����� �߰��Ѵ�.
	_float3 TargetPos = *static_cast<CTransform*>(m_pTargetPlayer->Find_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);

	// ���� ��ġ
	_float3 vMyPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	// ���� ���
	_float3 vDir = TargetPos - vMyPos;
	float fDistance = vDir.Length();
	vDir.Normalize();

	//������������ ���ϱ�
	_float3 vLook = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	bool bRotated = m_pTransformCom->RotateToDirection(vLook, vDir, 5.f, dt);
	m_pTransformCom->ChaseWithOutY(TargetPos, dt, fChaseDist, 150.f);
}

HRESULT CShotgunner::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	Ready_Textures();

	return S_OK;
}

HRESULT CShotgunner::Ready_Textures()
{
	/* ATTACK */
	for (_uint i = 0; i < D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Shotgunner_Attack_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(LEVEL_STATIC, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Shotgunner_Attack_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_ATTACK][i])))))
			return E_FAIL;
	}

	/* DEATH */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_Shotgunner_Death",
		_wstring(TEXT("Com_Texture")) + L"_Shotgunner_Death", reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_DEAD][0])))))
		return E_FAIL;

	/* MOVE */
	for (_uint i = 0; i < D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Shotgunner_Move_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(LEVEL_STATIC, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Shotgunner_Move_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_MOVE][i])))))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CShotgunner::Set_Animation()
{
	if (m_eCurMonsterState != m_ePrevMonsterState)
	{
		m_ePrevMonsterState = m_eCurMonsterState;
		m_fAnimationFrame = 0.f;
		m_iState = (_uint)(m_eCurMonsterState);
		switch (m_eCurMonsterState)
		{
		case Client::CShotgunner::STATE_MOVE:
			m_fAnimationMaxFrame = _float(MAX_MOVERUN);
			m_fAnimationSpeed = 10.f;
			break;
		case Client::CShotgunner::STATE_ATTACK:
			m_fAnimationMaxFrame = _float(MAX_ATTACK);
			m_fAnimationSpeed = 10.f;
			break;
		case Client::CShotgunner::STATE_STAY:
			m_fAnimationMaxFrame = 1.f;
			m_fAnimationSpeed = 0.f;
			m_iState = (_uint)(STATE_MOVE);
			break;
		case Client::CShotgunner::STATE_DEAD:
			m_fAnimationMaxFrame = _float(MAX_DEAD);
			m_fAnimationSpeed = 13.f;
			m_bRotateAnimation = false;
			break;
		}
	}
	return S_OK;
}

HRESULT CShotgunner::Animate_Monster(_float fTimeDelta)
{
	if (m_fAnimationMaxFrame < 2.f)
		return S_OK;

	switch (m_eCurMonsterState)
	{
	case Client::CShotgunner::STATE_STAY:
		m_bRotateAnimation = true;
		return S_OK;
	case Client::CShotgunner::STATE_MOVE:
		m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		if (m_fAnimationFrame >= m_fAnimationMaxFrame)
			m_fAnimationFrame = 0.f;
		m_bRotateAnimation = true;
		break;
	case Client::CShotgunner::STATE_ATTACK:
		m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		if (m_fAnimationFrame >= m_fAnimationMaxFrame)
			m_fAnimationFrame = 0.f;
		m_bRotateAnimation = true;
		break;
	case Client::CShotgunner::STATE_DEAD:
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

CShotgunner* CShotgunner::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CShotgunner* pInstance = new CShotgunner(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CShotgunner");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CShotgunner::Clone(void* pArg)
{
	CShotgunner* pInstance = new CShotgunner(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CShotgunner");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShotgunner::Free()
{
	__super::Free();
}