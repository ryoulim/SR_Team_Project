// �� Ŭ���� �̸� : TestMonster
// �θ� Ŭ���� �̸� : Monster

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
	//������Ÿ���� �⺻����
	m_szTextureID = TEXT("Cultist_Run");
	m_szBufferType = TEXT("Rect");

	//�Ӽ�
	m_iHP = 150;
	m_iMaxHP = 150;
	m_iAttackPower = 8;
	m_iDefense = 1;
	m_fSpeed = 10.f;
	m_vScale = { 36.5f, 62.2f, 1.f };
	m_eState = MODE::MODE_IDLE;

	m_fDetectiveDistance = 300.f;

	//�μӼ�
	m_strDialogue = "Cultist..Cultist...";
	m_strSound = "SoundFilePath";

	m_vDropItems.push_back("CultistDropItem");

	return S_OK;
}

HRESULT CCultist::Initialize(void* pArg)
{
	//��ġ, ũ���ʱ�ȭ, ������Ʈ ����
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	// �Ӹ� �ݶ��̴� 
/* �ݶ��̵� ������Ʈ */
	DESC* pDesc = static_cast<DESC*>(pArg);
	CCollider::DESC ColliderDesc{};
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vOffSet = { 0.f, 62.2f * 0.5f - 7.f, 0.f }; // y���� * 0.5 - �Ӹ���ġy��ǥ + ����ũ��?
	ColliderDesc.vScale = { 6.f, 0.f, 0.f }; // ������ ũ��
	ColliderDesc.pOwner = this;
	ColliderDesc.iColliderGroupID = CG_MONSTER_HEAD;
	ColliderDesc.iColliderID = CI_MON_HEAD;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider_head"), reinterpret_cast<CComponent**>(&m_pHeadCollider), &ColliderDesc)))
		return E_FAIL;

	m_pCollider->Update_OffSet({ 0.f, -7.f, 0.f });
	m_pCollider->Update_Scale({ 36.5f, 62.2f - 7.f, 1.f });


	m_fDivOffset = 45.f;
	//�ִϸ��̼�(��������)
	m_fAnimationMaxFrame = 4.f;
	m_fAnimationSpeed = 5.f;
	m_iState = STATE_MOVE;

	m_fBulletCooldown = 0.1f;
	m_fCooldownTime = 0.7f;
	m_fAttackTime = 1.f;

	return S_OK;
}

void CCultist::Priority_Update(_float fTimeDelta)
{
	Set_Animation();
	__super::Priority_Update(fTimeDelta);
}

EVENT CCultist::Update(_float fTimeDelta)
{
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

	//Ư���� �� ������ �Ұ� �ִ� ��� ��
}


void CCultist::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	__super::On_Collision_NormalMonster(MyColliderID, OtherColliderID);
}

void CCultist::MonsterTick(_float dt)
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
void CCultist::DoDetect(_float dt)
{
	// ���� ���� �Ÿ� �̳��� �� / ���� ���� �� �߰� ���� �Ÿ��� ��
	ChasePlayer(dt, 50.f);
	m_eCurMonsterState = STATE_MOVE;
}


//_bool CCultist::IsMonsterAbleToAttack()
//{
//	// ���� ����ĳ�������� �÷��̾�� ���� ���� ��ֹ� ���� üũ
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

void CCultist::AttackPattern(_float dt)
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
		MonsterNormalBullet_iDesc.iColliderID = CI_MONSTER_CULTIST;
		MonsterNormalBullet_iDesc.fSpeedPerSec = 1000.f;
		MonsterNormalBullet_iDesc.fRotationPerSec = RADIAN(180.f);
		MonsterNormalBullet_iDesc.vScale = { 1.f, 3.f, 1.f };
		MonsterNormalBullet_iDesc.vPosition = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float3 vRight = *m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		vRight.Normalize();
		// �ѱ� ��ġ�� ������ ��ġ�� look ���͸� ����Ͽ� ���
		MonsterNormalBullet_iDesc.vPosition += vRight * 11.f;
		MonsterNormalBullet_iDesc.vPosition.y += 13.f;

		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_MonsterNormalBullet"),
			LEVEL_GAMEPLAY, L"Layer_MonsterBullet", &MonsterNormalBullet_iDesc)))
			return;

		m_fBulletCooldownElapsed = 0.f;
	}
}

void CCultist::ChasePlayer(_float dt, _float fChaseDist)
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


HRESULT CCultist::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	Ready_Textures();

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
}