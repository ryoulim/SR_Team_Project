// �� Ŭ���� �̸� : TestMonster
// �θ� Ŭ���� �̸� : Monster

#include "Nukemutant.h"
#include "FXMgr.h"
#include "MonsterNormalBullet.h"

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
	//������Ÿ���� �⺻����
	m_szTextureID = TEXT("Nukemutant_Walk");
	m_szBufferType = TEXT("Rect");

	//�Ӽ�
	m_iHP = 50;
	m_iMaxHP = 50;
	m_iAttackPower = 15;
	m_iDefense = 0;
	m_fSpeed = 15.f;
	m_vScale = { 27.f, 56.f, 1.f };
	m_eState = MODE::MODE_IDLE;

	m_fDetectiveDistance = 300.f;

	//�μӼ�
	m_strDialogue = "Nuke..Nu...";
	m_strSound = "SoundFilePath";

	m_vDropItems.push_back("NukemutantDropItem");

	return S_OK;
}

HRESULT CNukemutant::Initialize(void* pArg)
{
	//��ġ, ũ���ʱ�ȭ, ������Ʈ ����
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_fDivOffset = 45.f;
	//�ִϸ��̼�(��������)
	m_fAnimationMaxFrame = 4.f;
	m_fAnimationSpeed = 5.f;
	m_iState = STATE_MOVE;

	m_fBulletCooldown = 1.f;
	m_fCooldownTime = 2.f;
	m_fAttackTime = 0.5f;
	return S_OK;
}

void CNukemutant::Priority_Update(_float fTimeDelta)
{
	Set_Animation();
	__super::Priority_Update(fTimeDelta);
}

EVENT CNukemutant::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CNukemutant::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	if (m_eState == MODE::MODE_BATTLE)
		m_iDegree = 0;
	Resize_Texture(0.5f);
}

HRESULT CNukemutant::Render()
{
	return __super::Render();

	//Ư���� �� ������ �Ұ� �ִ� ��� ��
}


void CNukemutant::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	__super::On_Collision_NormalMonster(MyColliderID, OtherColliderID);
}

void CNukemutant::MonsterTick(_float dt)
{
	//���º�ȭ
	switch (m_eState)
	{
	case MODE::MODE_IDLE:
		if (IsPlayerDetected())	// ���� �Ÿ� ���� ���
		{
			m_bFoundPlayer = true;
			//�÷��̾� �߰� �� �ൿ
			cout << "nukemutant �÷��̾� ����!!" << endl;
			m_eState = MODE::MODE_DETECTIVE;
		}
		break;

	case MODE::MODE_DETECTIVE:
		//�÷��̾� ���� ������ �� ���� Ž��
		m_fRaycastTicker += dt;
		if (m_fRaycastTicker > 0.5f)
		{
			if (IsMonsterAbleToAttack())	// RayPicking���� �÷��̾�� ���� ���� ��ֹ� üũ
			{
				m_bFoundPlayer = true;
				m_eState = MODE::MODE_READY;
			}
		}
		break;

	case MODE::MODE_READY:
		//���� �� �غ�  ( ���� ���� ������ �ʿ� )
		m_fRaycastTicker += dt;
		if (m_fRaycastTicker > 0.5f)
		{
			if (!IsMonsterAbleToAttack())
			{
				m_eState = MODE::MODE_DETECTIVE;
				break;
			}
		}
		// �غ� ������
		if (m_isReadyToAttack)
			m_eState = MODE::MODE_BATTLE;
		break;

	case MODE::MODE_BATTLE:
		//���� ���� �� �ൿ
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

	case MODE::MODE_RETURN:
		m_bFoundPlayer = false;
		//������ġ�� ���ư��� IDLE�� ���°� ���Ѵ�.
		break;
	}

#ifdef _CONSOL
	auto now = steady_clock::now();
	auto elapsed = duration_cast<milliseconds>(now - g_LastLogTime).count();

	if (elapsed >= 1000)
	{
		// 1�� �̻� �����ٸ� ���
		cout << "[����Ʈ]\t�÷��̾���� �Ÿ� : " << m_fCurDistance << endl;
		cout << "[����Ʈ]\t���� : ";
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

	case MODE::MODE_RETURN:
		DoReturn(dt);
		break;
	}
}

void CNukemutant::DoDetect(_float dt)
{
	// ���� ���� �Ÿ� �̳��� �� / ���� ���� �� �߰� ���� �Ÿ��� ��
	ChasePlayer(dt, 50.f);
	m_eCurMonsterState = STATE_MOVE;
}


_bool CNukemutant::IsMonsterAbleToAttack()
{
	// ���� ����ĳ�������� �÷��̾�� ���� ���� ��ֹ� ���� üũ
	m_fRaycastTicker = 0.f;
	if (m_fCurDistance > m_fAttackDistance)
		return false;
	return Raycast_Player();
}

void CNukemutant::DoReady(_float dt)
{
	m_fCooldownDuration += dt;
	if (m_fCooldownDuration >= m_fCooldownTime)
	{
		m_isReadyToAttack = true;
		m_fBulletCooldownElapsed = m_fBulletCooldown;
		m_fCooldownDuration = 0.f;
	}
	m_fAnimationFrame = 0.f;
	_float3 TargetPos = *static_cast<CTransform*>(m_pTargetPlayer->Find_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);
	m_pTransformCom->LookAt(TargetPos);
}

void CNukemutant::DoBattle(_float dt)
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

void CNukemutant::DoIdle(_float dt)
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

void CNukemutant::AttackPattern(_float dt)
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
		MonsterNormalBullet_iDesc.fSpeedPerSec = 300.f;
		MonsterNormalBullet_iDesc.fRotationPerSec = RADIAN(180.f);
		MonsterNormalBullet_iDesc.vScale = { 6.f, 3.f, 0.f };
		MonsterNormalBullet_iDesc.vPosition = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float3 vRight = *m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		vRight.Normalize();
		// �ѱ� ��ġ�� ������ ��ġ�� look ���͸� ����Ͽ� ���
		MonsterNormalBullet_iDesc.vPosition += vRight * 11.f;
		MonsterNormalBullet_iDesc.vPosition.y += 13.f;

		/*�׽�Ʈ��*/
		MonsterNormalBullet_iDesc.szTextureID = TEXT("FleshBullet");
		MonsterNormalBullet_iDesc.bAnimation = true;
		MonsterNormalBullet_iDesc.bGravity = true;
		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_MonsterNormalBullet"),
			LEVEL_GAMEPLAY, L"Layer_MonsterBullet", &MonsterNormalBullet_iDesc)))
			return;

		m_fBulletCooldownElapsed = 0.f;
	}
}

void CNukemutant::ChasePlayer(_float dt, _float fChaseDist)
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

HRESULT CNukemutant::Ready_Components(void* pArg)
{
	Ready_Textures();
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;


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
			m_fAnimationMaxFrame = _float(MAX_ATTACK) + _float(m_iAttackAnimVariation);
			m_fAnimationFrame = _float(m_iAttackAnimVariation);
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