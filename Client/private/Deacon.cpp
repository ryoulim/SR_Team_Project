// �� Ŭ���� �̸� : TestMonster
// �θ� Ŭ���� �̸� : Monster

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
	//������Ÿ���� �⺻����
	m_szTextureID = TEXT("Deacon_Walk");
	m_szBufferType = TEXT("Rect");

	//�Ӽ�
	m_iHP = 30;
	m_iMaxHP = 30;
	m_iAttackPower = 5;
	m_iDefense = 1;
	m_fSpeed = 13.f;
	m_vScale = { 52.f, 75.f, 1.f };
	m_eState = MODE::MODE_IDLE;

	m_fDetectiveDistance = 500.f;

	//�μӼ�
	m_strDialogue = "Deacon..Deacon...";
	m_strSound = "SoundFilePath";

	m_vDropItems.push_back("DeaconDropItem");

	return S_OK;
}

HRESULT CDeacon::Initialize(void* pArg)
{

	//��ġ, ũ���ʱ�ȭ, ������Ʈ ����
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_fDivOffset = 45.f;
	//�ִϸ��̼�(��������)
	m_fAnimationMaxFrame = 0.f;
	m_fAnimationSpeed = 5.f;
	m_iState = STATE_FLY;
	m_fCooldownTime = 0.f;
	return S_OK;
}

void CDeacon::Priority_Update(_float fTimeDelta)
{
	Set_Animation();
	__super::Priority_Update(fTimeDelta);
}

EVENT CDeacon::Update(_float fTimeDelta)
{
	if (m_bDead) // ��� üũ
	{
		m_fDeadBodyCounter += fTimeDelta;
		if (m_fDeadBodyCounter > 5.f)
			return EVN_DEAD;
	}

	if (KEY_DOWN(DIK_RBRACKET)) // ��� ���� Ȯ�ο�
	{
		int i = m_eCurMonsterState;
		i++;
		m_eCurMonsterState = MONSTER_STATE(i);
		if (m_eCurMonsterState == STATE_END)
			m_eCurMonsterState = MONSTER_STATE(0);
	}

	if (m_bActive)
	{
		MonsterTick(fTimeDelta);
	}

	return __super::Update(fTimeDelta);
}

void CDeacon::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CDeacon::Render()
{
	return __super::Render();

	//Ư���� �� ������ �Ұ� �ִ� ��� ��
} 

HRESULT CDeacon::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;
	Ready_Textures();

	return S_OK;
}

void CDeacon::On_Collision(_uint MyColliderID, _uint OtherColliderID)
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
		//�� ��� ��Ʋ��� ����
		if (m_eState != MODE::MODE_RETURN)
			m_eState = MODE::MODE_BATTLE;

		//��ġŽ��
		_float3 vImpactPos = CalculateEffectPos();

		//���� ���
		if (0 >= m_iHP)
		{
			CFXMgr::Get_Instance()->SpawnCustomExplosion(vImpactPos, LEVEL_GAMEPLAY, _float3{ 130.f, 160.f, 1.f }, TEXT("PC_Explosion"), 14);
			m_bDead = true;

			return;
		}

		// ����Ʈ ����
		m_iHP += -50;
		CFXMgr::Get_Instance()->SpawnBlood(vImpactPos, LEVEL_GAMEPLAY);
	}
}

_bool CDeacon::Raycast_Player() // �ð� �� �ֱ� ������ // �θ�� �ű��
{
	_float3 TargetPos = *static_cast<CTransform*>(m_pTargetPlayer->Find_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);

	_float3 vMyPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_float3 vDir = TargetPos - vMyPos;
	_uint colID = {};

	m_pGameInstance->Raycast(vMyPos, vDir, FLT_MAX, { CG_PAWN, CG_BLOCK }, colID);

	return CI_PLAYER(colID);
}

void CDeacon::MonsterTick(_float dt)
{
	//���º�ȭ
	switch (m_eState)
	{
	case MODE::MODE_IDLE:
		if (IsPlayerDetected())	// ���� �Ÿ� ���� ���
		{
			//�÷��̾� �߰� �� �ൿ
			cout << "Deacon �÷��̾� ����!!" << endl;
			m_eState = MODE::MODE_DETECTIVE;
		}
		break;

	case MODE::MODE_DETECTIVE:
		//�÷��̾� ���� ������ �� ���� Ž��
		m_fRaycastTicker += dt;
		if (m_fRaycastTicker > 0.5f)
		{
			if (IsMonsterAbleToAttack())	// RayPicking���� �÷��̾�� ���� ���� ��ֹ� X <- ���� ���� ���°� ��
				m_eState = MODE::MODE_READY;
		}
		break;

	case MODE::MODE_READY:
		//���� �� �غ�  ( ���� ���� ������ �ʿ� )
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
			m_bCoolingDown = false;
		}
		m_fRaycastTicker += dt;
		if (m_fRaycastTicker > 0.5f)
		{
			if (false == IsMonsterAbleToAttack())
			{
				m_fCooldownDuration = 0.f;
				m_eState = MODE::MODE_DETECTIVE;
			}
		}

		break;

	case MODE::MODE_RETURN:
		//������ġ�� ���ư��� IDLE�� ���°� ���Ѵ�.
		break;
	}

#ifdef _DEBUG
	auto now = steady_clock::now();
	auto elapsed = duration_cast<milliseconds>(now - g_LastLogTime).count();

	if (elapsed >= 1000)
	{
		// 1�� �̻� �����ٸ� ���
		//cout << "������ ���� : " << GetMonsterStateName(m_eState) << '\n';
		cout << "[����]\t�÷��̾���� �Ÿ� : " << m_fCurDistance << endl;
		cout << "[����]\t���� : ";
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

void CDeacon::DoDetect(_float dt)
{
	// ���� ���� �Ÿ� �̳��� �� / ���� ���� �� �߰� ���� �Ÿ��� ��
	ChasePlayer(dt, 50.f);
	m_eCurMonsterState = STATE_FLY;
}


_bool CDeacon::IsMonsterAbleToAttack()
{
	// ���� ����ĳ�������� �÷��̾�� ���� ���� ��ֹ� ���� üũ
	if (m_fCurDistance > 200.f)
		return false;
	//if (m_fRaycastTicker > 0.5f)
	{
		m_fRaycastTicker = 0.f;
		if (true == Raycast_Player())
			return true;
		else
			return false;
	}
}

void CDeacon::DoReady(_float dt)
{
	m_fCooldownDuration += dt;
	if (m_fCooldownDuration >= m_fCooldownTime)
		m_isReadyToAttack = true;

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
			SetRandomDirection();                  // ȸ���� ���� ����
			m_fIdleWaitElapsed = 0.f;
			m_eIdlePhase = EIdlePhase::IDLE_TURN; // ������ ȸ���Ϸ� ����
		}
		break;

	case EIdlePhase::IDLE_TURN:
	{
		m_eCurMonsterState = STATE_FLY;
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

void CDeacon::AttackPattern(_float dt)
{
	// ���� ���� ���� �ۼ��ϴ� ��
	// ����̶� �Ϲݰ���������
	// Archangel Ư������ ����
	// Wenteko ���� �� �굵 ����
	m_eCurMonsterState = STATE_ATTACK;

	m_fSpawnNormalBullet += dt;
	m_fAttackTimer += dt;
	if (m_fAttackTimer >= 1.f)
	{
		m_bCoolingDown = true;
	}
	if (m_fSpawnNormalBullet >= 0.2f)
	{
		// 0.2�ʸ��� �߻�
		CMonsterNormalBullet::DESC MonsterNormalBullet_iDesc{};
		MonsterNormalBullet_iDesc.fSpeedPerSec = 60.f;
		MonsterNormalBullet_iDesc.fRotationPerSec = RADIAN(180.f);
		MonsterNormalBullet_iDesc.vScale = { 10.f, 10.f, 0.f };
		MonsterNormalBullet_iDesc.vPosition = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_MonsterNormalBullet"),
			LEVEL_GAMEPLAY, L"Layer_MonsterBullet", &MonsterNormalBullet_iDesc)))
			return;

		m_fSpawnNormalBullet = 0.f;
	}
}

void CDeacon::ChasePlayer(_float dt, _float fChaseDist)
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
			m_fAnimationSpeed = 8.f;
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
		if (!m_bDead)
			m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		if (m_fAnimationFrame >= m_fAnimationMaxFrame)
		{
			m_bDead = true;
			m_fAnimationFrame = 0.f;
		}
		break;
	}
	return S_OK;
}


CDeacon* CDeacon::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	//������Ÿ���� �⺻����
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
}