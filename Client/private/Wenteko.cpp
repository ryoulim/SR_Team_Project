// �� Ŭ���� �̸� : TestMonster
// �θ� Ŭ���� �̸� : Monster

#include "Wenteko.h"
#include "FXMgr.h"

CWenteko::CWenteko(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster{ pGraphic_Device }
{
}

CWenteko::CWenteko(const CWenteko& Prototype)
	: CMonster(Prototype)
{
}

HRESULT CWenteko::Initialize_Prototype()
{
	//������Ÿ���� �⺻����
	m_szTextureID = TEXT("Wenteko_Walk");
	m_szBufferType = TEXT("Rect");

	//�Ӽ�
	m_iHP = 100;
	m_iMaxHP = 100;
	m_iAttackPower = 5;
	m_iDefense = 1;
	m_fSpeed = 8.f;
	m_vScale = { 52.f, 65.6f, 1.f };
	// 130 164
	m_eState = MODE::MODE_IDLE;
	m_fDetectiveDistance = 300.f;

	//�μӼ�
	m_strDialogue = "Wente..Wente...";
	m_strSound = "SoundFilePath";

	m_vDropItems.push_back("WentenkoDrop");

	return S_OK;
}

HRESULT CWenteko::Initialize(void* pArg)
{
	//��ġ, ũ���ʱ�ȭ, ������Ʈ ����
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	/* �ݶ��̵� ������Ʈ - �밡�� */
	CCollider::DESC ColliderDesc{};
	ColliderDesc.pTransform = m_pTransformCom; // 50��
	ColliderDesc.vOffSet = { 0.f, 20.8f, 0.f };	// y���� * 0.5 - �Ӹ���ġy��ǥ + ������ũ��?
	ColliderDesc.vScale = { 8.f, 0.f, 0.f };					// ������ ũ��
	ColliderDesc.pOwner = this;
	ColliderDesc.iColliderGroupID = CG_MONSTER_HEAD;
	ColliderDesc.iColliderID = CI_MON_HEAD;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider_head"), reinterpret_cast<CComponent**>(&m_pHeadCollider), &ColliderDesc)))
		return E_FAIL;

	/* �ݶ��̵� ������Ʈ  - �������ݿ� Ÿ�� ���� */
	CCollider::DESC AttackColliderDesc{};
	AttackColliderDesc.pTransform = m_pTransformCom;
	AttackColliderDesc.vOffSet = { 0.f, -1000.f, 0.f }; // �ƹ�ư �ʱ⿡ �ָ� ������ �ȸ´´ٴϱ��
	AttackColliderDesc.vScale = { 40.f, 0.f, 0.f };				
	AttackColliderDesc.pOwner = this;
	AttackColliderDesc.iColliderGroupID = CG_MONSTER;
	AttackColliderDesc.iColliderID = CI_MONSTER_WENTEKO;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider_CloseAttack"), reinterpret_cast<CComponent**>(&m_pAttackCollider), &AttackColliderDesc)))
		return E_FAIL;


	m_fDivOffset = 45.f;
	//�ִϸ��̼�(��������)
	m_fAnimationMaxFrame = 4.f;
	m_fAnimationSpeed = 5.f;
	m_iState = STATE_MOVE;
	m_fCooldownTime = 0.8f;     // ���� ���� ��
	//m_fBulletCooldown = 0.03f;	// �Ѿ� �߻� ��
	//m_fAttackTime = 0.2f;		// ���� �ð�
	return S_OK;
}

void CWenteko::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CWenteko::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CWenteko::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	_float3 vLook = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	vLook.Normalize();
	if (m_isCloseAttack)
	{
		_float3 vCLook = vLook;
		vCLook *= 30.f;
		vCLook.y += 10.f;
		m_pAttackCollider->Update_OffSet(vCLook);
		m_pAttackCollider->Update_Collider();  // ���� ���� �ÿ��� �ݶ��̴� ������Ʈ 
	}
	else
	{
		m_pAttackCollider->Update_OffSet({0.f, -10000.f, 0.f});
		//m_pAttackCollider->Update_Collider();  // ���� ���� �ÿ��� �ݶ��̴� ������Ʈ 
	}
	if (!m_bDead)
	{
		vLook *= 16.f;
		vLook.y += 20.8f;
		m_pHeadCollider->Update_OffSet(vLook);
	}
	else
	{
		m_pAttackCollider->Update_Scale({ 0.f, 0.f, 0.f });	
	}

	if (m_bRotateAnimation == false)
		m_iDegree = 0;
	Resize_Texture(0.4f);
}

HRESULT CWenteko::Render()
{
#ifdef _COLLIDERRENDER
	if (m_pAttackCollider)
		m_pAttackCollider->Render();
#endif
	return __super::Render();

	//Ư���� �� ������ �Ұ� �ִ� ��� ��
}


void CWenteko::MonsterTick(_float dt)
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

void CWenteko::DoDetect(_float dt)
{
	// ���� ���� �Ÿ� �̳��� �� / ���� ���� �� �߰� ���� �Ÿ��� ��
	ChasePlayer(dt, 50.f);
	m_eCurMonsterState = STATE_MOVE;
}

void CWenteko::JumpPattern(_float dt)
{
	switch (m_eJumpState)
	{
	case Client::CWenteko::JUMP_BEFORE:
		m_fJumpFinished = 0.f;
		m_fAnimationFrame = 0.f;
		m_pGravityCom->Jump(25.f);
		m_eJumpState = JUMP_ING;
		break;

	case Client::CWenteko::JUMP_ING:
		m_fAnimationFrame += dt * m_fAnimationSpeed;
		if (m_fAnimationFrame >= 4.f)
			m_fAnimationFrame = 4.f;
		m_pTransformCom->Go_Straight(dt * 10.f);
		if (!m_pGravityCom->isJump())
			m_eJumpState = JUMP_AFTER;
		break;

	case Client::CWenteko::JUMP_AFTER:
		m_fAnimationFrame = 5.f;
		m_fJumpFinished += dt;
		if (m_fJumpFinished >= 0.2f)
			m_bCoolingDown = true;
		break;

	default:
		break;
	}
}

void CWenteko::ClosePattern(_float dt)
{
	// ���������ϱ�
	m_eCurMonsterState = STATE_ATTACK;
	
	m_fAnimationFrame += dt * m_fAnimationSpeed;
	m_pTransformCom->Go_StraightWithoutY(dt * 3.f);
	m_isCloseAttack = true;
	if (m_fAnimationFrame >= m_fAnimationMaxFrame)
	{
		if (m_bOneMoreAttack)
		{
			m_fAnimationFrame = 0.f;
			m_bCoolingDown = true;
			m_isCloseAttack = false;
			m_eCurMonsterState = STATE_STAY;
		}
		else
		{
			m_fAnimationFrame = 0.f;
			m_bOneMoreAttack = true;
		}
	}
}

void CWenteko::DoReady(_float dt)
{
	m_fCooldownDuration += dt;
	if (m_fCooldownDuration >= m_fCooldownTime)
	{
		m_isReadyToAttack = true;
		if (m_fCurDistance < 200.f)
			m_eAttackPattern = ATTACK_NORMAL;
		else
			m_eAttackPattern = ATTACK_JUMP;
		//rand() % 2 == 0 ? m_eAttackPattern = ATTACK_JUMP : m_eAttackPattern = ATTACK_NORMAL;
		m_fCooldownDuration = 0.f;
		m_bJump = false;
		m_isCloseAttack = false;
		m_bOneMoreAttack = false;
		m_eJumpState = JUMP_BEFORE;
	}
	m_fAnimationFrame = 0.f;
	_float3 TargetPos = *static_cast<CTransform*>(m_pTargetPlayer->Find_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);
	m_pTransformCom->LookAt(TargetPos);
}

void CWenteko::DoBattle(_float dt)
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

void CWenteko::DoIdle(_float dt)
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

void CWenteko::AttackPattern(_float dt)
{
	switch (m_eAttackPattern)
	{
	case Client::CWenteko::ATTACK_JUMP:
		m_eCurMonsterState = STATE_JUMP;
		JumpPattern(dt);
		break;

	case Client::CWenteko::ATTACK_NORMAL:
		m_eCurMonsterState = STATE_ATTACK;
		ClosePattern(dt);
		break;

	default:
		break;
	}
}

void CWenteko::ChasePlayer(_float dt, _float fChaseDist)
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

HRESULT CWenteko::Ready_Components(void* pArg)
{
	/* ��Ʈ ���� ������Ʈ */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, _wstring(TEXT("Prototype_Component_VIBuffer_")) + m_szBufferType,
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Ʈ������ ������Ʈ */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), pArg)))
		return E_FAIL;

	/* ��ġ, ������ �ʱ�ȭ */
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

	/* �ݶ��̵� ������Ʈ */
	DESC* pDesc = static_cast<DESC*>(pArg);
	CCollider_Capsule::DESC ColliderDesc{};
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vOffSet = {};
	ColliderDesc.vScale = m_pTransformCom->Compute_Scaled();
	ColliderDesc.pOwner = this;
	ColliderDesc.iColliderGroupID = CG_MONSTER;
	ColliderDesc.iColliderID = CI_MONSTER_WENTEKO;

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

HRESULT CWenteko::Ready_Textures()
{
	/* Move */
	for (_uint i = 0; i < D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Wenteko_Move_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(LEVEL_STATIC, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Wenteko_Move_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_MOVE][i])))))
			return E_FAIL;
	}

	/* Jump */
	for (_uint i = 0; i < D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Wenteko_Jump_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(LEVEL_STATIC, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Wenteko_Jump_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_JUMP][i])))))
			return E_FAIL;
	}

	/* Attack */
	for (_uint i = 0; i < D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Wenteko_Attack_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(LEVEL_STATIC, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Wenteko_Attack_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_ATTACK][i])))))
			return E_FAIL;
	}

	/* Death */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_Wenteko_Dead",
		_wstring(TEXT("Com_Texture")) + L"_Wenteko_Dead", reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_DEAD][0])))))
		return E_FAIL;

	return S_OK;
}

HRESULT CWenteko::Set_Animation()
{
	if (m_eCurMonsterState != m_ePrevMonsterState)
	{
		m_ePrevMonsterState = m_eCurMonsterState;
		m_fAnimationFrame = 0.f;
		m_iState = (_uint)(m_eCurMonsterState);
		switch (m_eCurMonsterState)
		{
		case Client::CWenteko::STATE_MOVE:
			m_fAnimationMaxFrame = _float(MAX_MOVERUN);
			m_fAnimationSpeed = 12.f;
			break;
		case Client::CWenteko::STATE_JUMP:
			m_fAnimationMaxFrame = _float(MAX_MOVERUN);
			m_fAnimationSpeed = 13.f;
			break;
		case Client::CWenteko::STATE_ATTACK:
			m_fAnimationMaxFrame = _float(MAX_ATTACK);
			m_fAnimationSpeed = 18.f;
			break;
		case Client::CWenteko::STATE_STAY:
			m_fAnimationMaxFrame = 1.f;
			m_fAnimationSpeed = 0.f;
			m_iState = (_uint)(STATE_MOVE);
			break;
		case Client::CWenteko::STATE_DEAD:
			m_fAnimationMaxFrame = _float(MAX_DEAD);
			m_fAnimationSpeed = 10.f;
			m_bRotateAnimation = false;
			break;
		}
	}
	return S_OK;
}

HRESULT CWenteko::Animate_Monster(_float fTimeDelta)
{
	if (m_fAnimationMaxFrame < 2.f)
		return S_OK;

	switch (m_eCurMonsterState)
	{
	case Client::CWenteko::STATE_STAY:
		m_bRotateAnimation = true;
		return S_OK;
	case Client::CWenteko::STATE_MOVE:
		m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		if (m_fAnimationFrame >= m_fAnimationMaxFrame)
			m_fAnimationFrame = 0.f;
		m_bRotateAnimation = true;
		break;
	case Client::CWenteko::STATE_JUMP:
		//	m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		//	if (!m_pGravityCom->isJump())// && m_fAnimationSpeed < 1.f)
		//	{
		//		if(m_fAnimationFrame < 2.5f)
		//			m_fAnimationFrame = 3.f;
		//		//m_fAnimationFrame = m_fAnimationMaxFrame - 1.f; // ����?
		//		//m_bJumpEnd = true;
		//	}
		//	else if (m_fAnimationFrame >= 2.f)
		//	{
		//		m_fAnimationFrame = 2.f;
		//		//m_fAnimationSpeed = 0.f;
		//	}
		//	m_bRotateAnimation = true;
		break;
	case Client::CWenteko::STATE_DEAD:
		m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		if (m_fAnimationFrame >= m_fAnimationMaxFrame)
			m_fAnimationFrame = m_fAnimationMaxFrame - 1.f;
		m_bRotateAnimation = false;
		break;
	}
	return S_OK;
}

void CWenteko::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	__super::On_Collision_NormalMonster(MyColliderID, OtherColliderID);
}

CWenteko* CWenteko::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CWenteko* pInstance = new CWenteko(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CWenteko");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWenteko::Clone(void* pArg)
{
	CWenteko* pInstance = new CWenteko(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CWenteko");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWenteko::Free()
{
	__super::Free();
	Safe_Release(m_pAttackCollider);
}