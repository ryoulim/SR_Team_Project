// �� Ŭ���� �̸� : TestMonster
// �θ� Ŭ���� �̸� : Monster

#include "Deacon.h"
#include "FXMgr.h"

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
	return S_OK;
}

HRESULT CDeacon::Initialize(void* pArg)
{

	//��ġ, ũ���ʱ�ȭ, ������Ʈ ����
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_fDivOffset = 45.f;
	Ready_Textures();
	//�ִϸ��̼�(��������)
	m_fAnimationMaxFrame = 0.f;
	m_fAnimationSpeed = 5.f;
	m_iState = STATE_FLY;

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

	return S_OK;
}

void CDeacon::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	//�� ��� ��Ʋ��� ����
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
		if (IsMonsterAbleToAttack())	// RayPicking���� �÷��̾�� ���� ���� ��ֹ� X <- ���� ���� ���°� ��
			m_eState = MODE::MODE_READY;
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
			m_eState = MODE::MODE_READY;
		break;

	case MODE::MODE_RETURN:
		//������ġ�� ���ư��� IDLE�� ���°� ���Ѵ�.
		break;
	}

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
	ChasePlayer(dt);

}

_bool CDeacon::IsMonsterAbleToAttack()
{
	// ���� ����ĳ�������� �÷��̾�� ���� ���� ��ֹ� ���� üũ
	
	// ��ֹ� ���� ���
	// return false;
	// ���� ���
	return true;
}

void CDeacon::DoReady(_float dt)
{
}

void CDeacon::DoBattle(_float dt)
{
	// 1. �÷��̾���� �Ÿ� ���
	_float fAttackRange = 200.f;
	_float fChaseRange = 500.f;

	// 2. �Ÿ� ���� �б�
	if (m_fCurDistance < fAttackRange)
	{
		// ������ ����
		AttackPattern(dt);
		//cout << "������ ���� ��Ÿ� ���Դϴ�." << endl;
	}
	else if (m_fCurDistance < fChaseRange)
	{
		// �߰� �Ÿ��� ����
		m_eState = MODE_DETECTIVE;

		ChasePlayer(dt);
		//cout << "���� �߰����Դϴ�!" << endl;
	}
	else
	{
		// �ʹ� �ָ� ���� ����
		m_eState = MODE_RETURN;
	}
}

void CDeacon::AttackPattern(_float dt)
{
	// ���� ���� ���� �ۼ��ϴ� ��
	// ����̶� �Ϲݰ���������
	// Archangel Ư������ ����
	// Wenteko ���� �� �굵 ����
}

void CDeacon::ChasePlayer(_float dt)
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
	m_pTransformCom->ChaseWithOutY(TargetPos, dt, 200.f, 100.f);
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
	m_pTransformCom->ChaseWithOutY(TargetPos, dt, fChaseDist, 100.f);
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
		if (FAILED(__super::Add_Component(m_eLevelID, sPrototypeTag,
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
		if (FAILED(__super::Add_Component(m_eLevelID, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Deacon_Attack_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_ATTACK][i])))))
			return E_FAIL;
	}
	/* DEAD */
	if (FAILED(__super::Add_Component(m_eLevelID, L"Prototype_Component_Texture_Deacon_Dead",
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

	//�⺻ ����
	pInstance->m_eLevelID		= LEVEL_GAMEPLAY;
	pInstance->m_szTextureID	= TEXT("Deacon_Walk");
	pInstance->m_szBufferType	= TEXT("Rect");

	//�Ӽ�
	pInstance->m_iHP			= 30;
	pInstance->m_iMaxHP			= 30;
	pInstance->m_iAttackPower	= 5;
	pInstance->m_iDefense		= 1;
	pInstance->m_fSpeed			= 15.f;
	pInstance->m_vScale			= { 52.f, 75.f, 1.f };
	pInstance->m_eState		= MODE::MODE_IDLE;
	pInstance->m_fDetectiveDistance = 300.f;

	//�μӼ�
	pInstance->m_strDialogue	= "Deacon..Deacon...";
	pInstance->m_strSound		= "SoundFilePath";

	pInstance->m_vDropItems.push_back("DeaconDropItem");


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