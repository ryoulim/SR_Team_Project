// �� Ŭ���� �̸� : TestMonster
// �θ� Ŭ���� �̸� : Monster

#include "Ttakkeun_i.h"
#include "FXMgr.h"

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

	return S_OK;
}

void CTtakkeun_i::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CTtakkeun_i::Update(_float fTimeDelta)
{
	if (KEY_DOWN(DIK_I))
	{
		m_eState = MODE::MODE_RETURN;
	}

	return __super::Update(fTimeDelta);
}

void CTtakkeun_i::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);



#ifdef _DEBUG
	auto now = steady_clock::now();
	auto elapsed = duration_cast<milliseconds>(now - g_LastLogTime).count();
	
	if (elapsed >= 1000) 
	{
		// 0.5�� �̻� �����ٸ� ���
		cout << "������ ���� : " << GetMonsterStateName(m_eState) << '\n';
		cout << "�÷��̾���� �Ÿ� : " << m_fCurDistance << endl;
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
	//for (_uint i = 0; i < BOSS_DEGREE::D_END; i++)
	//{
	//	_wstring sPrototypeTag = L"Prototype_Component_Texture_Boss_Fly_";
	//	_uint num = static_cast<_uint>(i * m_fDivOffset);
	//	_tchar buf[32];
	//	_itow_s((int)num, buf, 10);
	//	sPrototypeTag += buf;
	//	if (FAILED(__super::Add_Component(m_eLevelID, sPrototypeTag,
	//		_wstring(TEXT("Com_Texture")) + L"_Boss_Fly_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_FLY][i])))))
	//		return E_FAIL;
	//}

	/* FLY_ATTACK */
	//for (_uint i = 0; i < BOSS_DEGREE::D_END; i++)
	//{
	//	_wstring sPrototypeTag = L"Prototype_Component_Texture_Boss_Fly_Attack_";
	//	_uint num = static_cast<_uint>(i * m_fDivOffset);
	//	_tchar buf[32];
	//	_itow_s((int)num, buf, 10);
	//	sPrototypeTag += buf;
	//	if (FAILED(__super::Add_Component(m_eLevelID, sPrototypeTag,
	//		_wstring(TEXT("Com_Texture")) + L"_Boss_Fly_Attack_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_FLY_ATTACK][i])))))
	//		return E_FAIL;
	//}

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

	/* LAVA_ATTACK */
	for (_uint i = 0; i < BOSS_DEGREE::D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Boss_Lava_Attack_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(m_eLevelID, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Boss_Lava_Attack_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_LAVA_ATTACK][i])))))
			return E_FAIL;
	}


	//divein�� 0�� �̹��� ���� ****
	if (FAILED(__super::Add_Component(m_eLevelID, _wstring(TEXT("Prototype_Component_Texture_Boss_Lava_DiveIn")),
		_wstring(TEXT("Com_Texture")) + L"_Boss_Lava_DiveIn", reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_LAVA_DIVEIN][0])))))
		return E_FAIL;



	//if (FAILED(__super::Add_Component(m_eLevelID, _wstring(TEXT("Prototype_Component_Texture_")) + m_szTextureID,
	//	TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
	//	return E_FAIL;

 	return S_OK;
}

HRESULT CTtakkeun_i::Set_MaxFrame()
{
	m_iState = (_uint)(m_eMonsterState);
	switch (m_eMonsterState)
	{
	case Client::CTtakkeun_i::STATE_WALK:
		m_fAnimationMaxFrame = MAX_WALK;
		break;
	case Client::CTtakkeun_i::STATE_FLY:
		m_fAnimationMaxFrame = MAX_FLY;
		break;
	case Client::CTtakkeun_i::STATE_FLY_ATTACK:
		m_fAnimationMaxFrame = MAX_FLY_ATTACK;
		break;
	case Client::CTtakkeun_i::STATE_JUMP:
		m_fAnimationMaxFrame = MAX_JUMP;
		break;
	case Client::CTtakkeun_i::STATE_LAVA_ATTACK:
		m_fAnimationMaxFrame = MAX_LAVA_ATTACK;
		break;
	case Client::CTtakkeun_i::STATE_LAVA_DIVEIN:
		m_fAnimationMaxFrame = MAX_LAVA_DIVEIN;
		break;
	default:
		break;
	}

	return S_OK;
}

void CTtakkeun_i::DoBattle(_float dt)
{
	// 1. �÷��̾���� �Ÿ� ���
	_float fAttackRange = 250.f;
	_float fChaseRange = 700.f;

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
			m_bCoolingDown = false;
			m_fCooldownTime = 0.f;
		}
		return;
	}

	bool isPhase2 = (m_iHP <= m_iMaxHP / 2);
	int iRand = GetRandomInt(0, 99);

	/* �������� ��������[ 2������ ] */
	if (isPhase2)
	{
		// 2�������� ���� 2������ ���� ������ �� ���� Ȯ���� ����
		if (iRand < 20)
			/* 1. ������ �� */
			LazerAttack(dt);      // 20% Ȯ��
		else if (iRand < 40)
			/* 2. ���� ����ź */
			MissileAttack(dt);    // 20% Ȯ��
		else if (iRand < 60)
			/* 3. ���� ��ȯ(����) */
			SpawnAttack(dt);      // 20% Ȯ��
		else
			BasicAttackSet(dt); // 20% Ȯ���� 1������ ����
	}
	else
	{
		// 1�������� ���� �׳� 1������ ���ϸ� ���
		BasicAttackSet(dt);
	}
}

void CTtakkeun_i::BasicAttackSet(_float dt)
{
	/* �������� ��������[ 1������ ] */

	//int iRand = GetRandomInt(0, 2);
	int iRand = 0;

	switch (iRand)
	{
	case 0: 
		/* 1. ȭ����� */
		FireAttack(dt);
		break;
	case 1: 
		/* 2. ���ǳ�� */
		LavaAttack(dt);
		break;
	case 2:
		/* 3. ���ƿ����� �����̿� */
		FlyAttack(dt);
		break;
	}
}

void CTtakkeun_i::LazerAttack(_float dt)
{
}

void CTtakkeun_i::MissileAttack(_float dt)
{
}

void CTtakkeun_i::SpawnAttack(_float dt)
{
}

void CTtakkeun_i::FireAttack(_float dt)
{
	// 2�� �̻� ���������� isDone �����ϰ� ��
	m_fAttackTimer += dt;
	if (m_fAttackTimer >= 2.f)
	{
		StartCooldown(dt);
		m_fAttackTimer = 0.f;  // �ʿ��ϸ� �����ϰ�
		return;
	}

	/* ȭ����� ���� */

	/* 1. �÷��̾�� �����Ѵ�(���� ������) */
	_float3 vPlayerPos = *static_cast<CTransform*>(m_pTargetPlayer->Find_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);
	_float3 vMyPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_float3 vDir = vPlayerPos - vMyPos;
	float fDistance = vDir.Length();
	vDir.Normalize();

	_float3 vLook = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	bool bRotated = m_pTransformCom->RotateToDirection(vLook, vDir, 5.f, dt);

	m_pTransformCom->ChaseCustom(vPlayerPos, dt, 100.f, 150.f);
	/* 2. ȭ���� �߻��Ѵ�! */
	CFXMgr::Get_Instance()->FireAttack(vMyPos, LEVEL_GAMEPLAY);

}

void CTtakkeun_i::LavaAttack(_float dt)
{
}

void CTtakkeun_i::FlyAttack(_float dt)
{
}

void CTtakkeun_i::StartCooldown(_float dt)
{
	m_bCoolingDown = true;
	m_fCooldownDuration = GetRandomFloat(1.5f, 2.5f); // ��ٿ� �ð� ����
	m_fCooldownTime = 0.f;
}

void CTtakkeun_i::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	//�� ��� ��Ʋ��� ����
	m_eState = MODE::MODE_BATTLE;

	//��ġŽ��
	_float3 vImpactPos = CalculateEffectPos();

	//���� ���
	if (0 >= m_iHP)
	{
		CFXMgr::Get_Instance()->SpawnCustomExplosion(vImpactPos, LEVEL_GAMEPLAY, _float3{ 120.f, 160.f, 1.f }, TEXT("Effect_Explor"), 32);
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
	pInstance->m_fSpeed			= 60.f;
	pInstance->m_vScale			= { 150.f, 147.f, 1.f };
	pInstance->m_eState		= MODE::MODE_IDLE;
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