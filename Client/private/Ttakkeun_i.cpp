#include "Ttakkeun_i.h"
#include "FXMgr.h"
#include "MonsterBullet.h"
#include "FlyEffect.h"
#include "GrenadeBullet.h"
#include "CameraManager.h"
#include "MonsterGuidBullet.h"
#include "Sprite.h"
#include "HitBox.h"

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
	//������Ÿ���� �⺻����
	m_szTextureID = TEXT("Ttakkeun_i_Walk");
	m_szBufferType = TEXT("Rect");

	//�Ӽ�
	m_iHP = 1500;
	m_iMaxHP = 1500;
	m_iAttackPower = 10;
	m_iDefense = 3;
	m_fSpeed = 100.f;
	m_vScale = { 150.f, 147.f, 1.f };
	m_eState = MODE::MODE_IDLE;

	m_fDetectiveDistance = 400.f;

	//�μӼ�
	m_strDialogue = "Boss..Boss...";
	m_strSound = "SoundFilePath";

	m_vDropItems.push_back("BossDropItem");

	return S_OK;
}

HRESULT CTtakkeun_i::Initialize(void* pArg)
{
	/* ī�޶� �˰� �־�� */
	m_pCamera = CAMERA_MANAGER;
	Safe_AddRef(m_pCamera);

	/* �ִϸ��̼� ���� �ʱ�ȭ */
	m_fDivOffset = 22.5f;
	m_iState = (_uint)(MONSTER_STATE::STATE_WALK);
	m_fAnimationMaxFrame = (_float)(STATE_MAXFRAME::MAX_WALK);
	m_fAnimationSpeed = 17.f;
	m_eCurFlyingDirection = UP;
	m_ePrevFlyingDirection = UP;

	//��ġ, ũ���ʱ�ȭ, ������Ʈ ����
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	/* ���� ����̸� ������ �Ųٷ� */
	if (m_iNum == 1)
		m_iRandom = 5;

	m_pTransformCom->Turn_Immediately({ 0.f, 1.f, 0.f }, RADIAN(180));
	m_vReturnPos.y = 77.f;
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

	if (m_bCutScene)
	{
		CutSceneAction(fTimeDelta);
	}

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
			m_pSoundCom->SetVolume("explosion", 0.5f);
			m_pSoundCom->Play("explosion");
			FX_MGR->SpawnCustomExplosion(vPos, LEVEL_GAMEPLAY, _float3{ 60.f, 100.f, 1.f }, TEXT("Effect_Explorer"), 24);
			m_fCallTimer = 0.f;
		}
		if (m_fTotalTime >= 3.f)
		{
			m_pSoundCom->SetVolume("Dead", 1.5f);
			m_pSoundCom->Play("Dead");
			_float3 vImpactPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			FX_MGR->SpawnCustomExplosion(vImpactPos, LEVEL_GAMEPLAY, _float3{ 200.f, 250.f, 1.f }, TEXT("Effect_Explor"), 32);
			m_bDie = true;
		}
	}
	

	if (m_pBossEffect)
		static_cast<CFlyEffect*>(m_pBossEffect)->SetPosition(*m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	if (m_bActive)
	{
		MonsterTick(fTimeDelta);
	}

	return EVN_NONE;
}

void CTtakkeun_i::Late_Update(_float fTimeDelta)
{
	//�÷��̾� ���� ������Ʈ
	PlayerDistance();
	CalculateVectorToPlayer();


	/* ��Ƽ�갡 ������ �߷��ݶ��̴��� Ȱ��ȭ�Ѵ� */
	if (m_bActive)
	{
		if (m_bCutSceneEnd)
		{
			CGravity::DESC GravityDesc{};
			GravityDesc.pTransformCom = m_pTransformCom;
			GravityDesc.fTimeIncreasePerSec = 8.2f;
			GravityDesc.fMaxFallSpeedPerSec = 840.f;
			if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Gravity"),
				TEXT("Com_Gravity"), reinterpret_cast<CComponent**>(&m_pGravityCom), &GravityDesc)))
				return;

			m_pGravityCom->Set_Height(110.f);

			m_bCutSceneEnd = false;
		}
	}

	if (m_eCurMonsterState != STATE_FLY &&
		m_eCurMonsterState != STATE_FLY_ATTACK &&
		!m_bJumpStart && m_bActive)
	{
		m_pGravityCom->Update(fTimeDelta);
	}

	Compute_ViewAngle();
	//�ݶ��̴� ������Ʈ
	m_pCollider->Update_Collider();

	//�����׷� ������Ʈ
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
		return;
	Set_TextureType();
	if (m_bRotateAnimation == false)
		m_iDegree = 0;
	Resize_Texture(0.35f);
	//__super::Late_Update(fTimeDelta);
#ifdef _CONSOL
	auto now = steady_clock::now();
	auto elapsed = duration_cast<milliseconds>(now - g_LastLogTime).count();
	
	if (elapsed >= 1000) 
	{
		// 1�� �̻� �����ٸ� ���
		//cout << "������ ���� : " << GetMonsterStateName(m_eState) << '\n';
		//cout << "�÷��̾���� �Ÿ� : " << m_fCurDistance << endl;
		//cout << "������ �������� : " << m_iRandom << endl;
		g_LastLogTime = now;
	}
#endif
}

HRESULT CTtakkeun_i::Render()
{
	/* [ ���� �ƽ��� ���۵����ʾҴٸ� ������ ���� ] */
	if (!m_bRender)
		return S_OK;
	
	return __super::Render();

	//Ư���� �� ������ �Ұ� �ִ� ��� ��
}

HRESULT CTtakkeun_i::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	/* [ �������� �Ҹ��� ã�Ƽ�.. ] */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Sound_Ttakkeun_i"),
		TEXT("Com_Sound"), reinterpret_cast<CComponent**>(&m_pSoundCom))))
		return E_FAIL;

	Ready_Textures();

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

	/* BOMB */
	for (_uint i = 0; i < BOSS_DEGREE::D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Boss_Bomb_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(m_eLevelID, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Boss_Bomb_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_BOMB][i])))))
			return E_FAIL;
	}

	/* MISSILE */
	for (_uint i = 0; i < BOSS_DEGREE::D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Boss_Missile_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(m_eLevelID, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Boss_Missile_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_MISSILE][i])))))
			return E_FAIL;
	}

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
		case Client::CTtakkeun_i::STATE_MISSILE: //�ΰ�
			m_fAnimationMaxFrame = _float(MAX_GROUNDATK);
			m_fAnimationSpeed = 20.f;
			break;
		case Client::CTtakkeun_i::STATE_BOMB: //����
			m_fAnimationMaxFrame = _float(MAX_GROUNDATK);
			m_fAnimationSpeed = 20.f;
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


	case Client::CTtakkeun_i::STATE_BOMB:
		m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		if (m_fAnimationFrame >= m_fAnimationMaxFrame)
			m_fAnimationFrame = 0.f;
		break;


	case Client::CTtakkeun_i::STATE_MISSILE:
		m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		if (m_fAnimationFrame >= m_fAnimationMaxFrame)
			m_fAnimationFrame = 0.f;
		break;
	}


	return S_OK;
}

void CTtakkeun_i::SpawnJumpHitBox(const _float3& _Position, const TCHAR* _szTextureTag, _float MaxTime, _bool _HitDead)
{
	CHitBox::DESC HitBoxDesc{};
	HitBoxDesc.vPosition = _Position;
	HitBoxDesc.vScale = { 130.f, 100.f, 130.f };
	HitBoxDesc.fSpeedPerSec = 60.f;
	HitBoxDesc.fRotationPerSec = RADIAN(180.f);
	HitBoxDesc.eID = CI_BOSS_JUMP;
	HitBoxDesc.szTextureTag = _szTextureTag;
	HitBoxDesc.bHitDead = _HitDead;
	HitBoxDesc.fMaxTime = MaxTime;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_HitBox"),
		m_eLevelID, L"Layer_HitBox", &HitBoxDesc)))
		return;

}

void CTtakkeun_i::SpawnFireHitBox(const _float3& _Position, const TCHAR* _szTextureTag, _float MaxTime, _bool _HitDead)
{
	CHitBox::DESC HitBoxDesc{};
	HitBoxDesc.vPosition = _Position;
	HitBoxDesc.vScale = { 100.f, 50.f, 100.f };
	HitBoxDesc.fSpeedPerSec = 60.f;
	HitBoxDesc.fRotationPerSec = RADIAN(180.f);
	HitBoxDesc.eID = CI_BOSS_FIRE;
	HitBoxDesc.szTextureTag = _szTextureTag;
	HitBoxDesc.bHitDead = _HitDead;
	HitBoxDesc.fMaxTime = MaxTime;

	CGameObject* pObject = nullptr;
	CGameObject** ppOut = &pObject;
	if (FAILED(m_pGameInstance->Add_GameObjectReturn(LEVEL_STATIC, TEXT("Prototype_GameObject_HitBox"),
		m_eLevelID, L"Layer_HitBox", ppOut, &HitBoxDesc)))
		return;

	m_pBossHitBox = *ppOut;
}

void CTtakkeun_i::DoIdle(_float dt)
{
	switch (m_eIdlePhase)
	{
	case EIdlePhase::IDLE_MOVE:
	{
		m_eCurMonsterState = STATE_WALK;
		m_fWanderElapsed += dt;
		
		if(!m_pSoundCom->IsPlaying("Walk"))
		{
			m_pSoundCom->SetVolume("Walk",0.2f);
			m_pSoundCom->Play("Walk");
		}
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
		if (!m_pSoundCom->IsPlaying("Walk"))
		{
			m_pSoundCom->SetVolume("Walk", 0.2f);
			m_pSoundCom->Play("Walk");
		}
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
			/* [ ù��° �ѹ����� ������ ������� ��ȸ�Ѵ� ] */
			if (!m_bDoPatternLeft && m_iNum == 0)
			{
				++m_iRandom;
				if (m_iRandom == 6)
				{
					m_iRandom = GetRandomInt(0, 5);
					m_bDoPatternLeft = true;
				}
				m_bCoolingDown = false;
			}
			if (!m_bDoPatternRight && m_iNum == 1)
			{
				--m_iRandom;
				if (m_iRandom == -1)
				{
					m_iRandom = GetRandomInt(0, 5);
					m_bDoPatternRight = true;
				}
				m_bCoolingDown = false;
			}
			else
			{
				m_iRandom = GetRandomInt(0, 5);
			}

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
	/* [ �������� �������� ] */
	
	switch (m_iRandom)
	{
	case 0: 
		/* 0. ȭ����� */
		FireAttack(dt);	
		break;
	case 1:
		/* 1. ���ƿ����� �����̿� */
		FlyAttack(dt);
		break;
	case 2:
		/* 2. �ٿ �� */
		BounceBall(dt);
		break;
	case 3:
		/* 3. ������� */
		JumpAttack(dt);
		break;
	case 4:
		/* 4. ���� �̻��Ϲ߻� */
		MissileAttack(dt);
		break;
	case 5:
		/* 5. ���� ��ȯ */
		SpawnAttack(dt);
		break;
	}
}


void CTtakkeun_i::MissileAttack(_float dt)
{
	/* [ ���� �̻��� ���� ] */

	/* 1. �� �ѹ� ���۸������ �̻����� �����Ѵ�. */
	/* 2. �����̴� �����ð� ������ �ִ´�. */

	m_fAttackTimer += dt;
	if (m_fAttackTimer >= 3.f)
	{
		//3�ʰ� �Ѿ�� �ٸ� �ൿ����
		m_bRotateAnimation = true;
		m_eCurMonsterState = STATE_WALK;
		StartCooldown(dt, 1.5f, 3.f);
		m_bDoOnce = false;
		m_fAttackTimer = 0.f;
		return;
	}

	/* �躤�� ȸ�� */
	_float3 vPlayerPos = *static_cast<CTransform*>(m_pTargetPlayer->Find_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);
	_float3 vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vToPlayer = vPlayerPos - vPos;
	_float3 vLook = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	bool bRotated = m_pTransformCom->RotateToDirection(vLook, vToPlayer, 60.f, dt);

	m_bRotateAnimation = false;
	m_eCurMonsterState = STATE_MISSILE;

	//�̻��� �߻�
	if (!m_bDoOnce)
	{
		m_pSoundCom->SetVolume("GuidMissile2", 0.3f);
		m_pSoundCom->Play("GuidMissile2");
		SpawnGuidMissile();
		m_bDoOnce = true;
	}
}

void CTtakkeun_i::SpawnAttack(_float dt)
{
	/* [ ���� ������ ������ 6������ �Ѿ�� �ٸ� ������ �����϶� ] */
	auto IsMonster = CGameInstance::Get_Instance()->Find_Objects(LEVEL_GAMEPLAY, L"Layer_Monster");
	if (IsMonster)
	{
		if (IsMonster->size() > 5)
		{
			m_iRandom = 3;
			return;
		}
	}

	/* [ ���� ��ȯ ���� ] */

	/* 1. ������ ������� ��ȯ�Ѵ�. */
	/* 2. �����̴� �����ð� ������ �ִ´�. */

	m_fAttackTimer += dt;
	if (m_fAttackTimer >= 3.f)
	{
		//3�ʰ� �Ѿ�� �ٸ� �ൿ����
		m_eCurMonsterState = STATE_WALK;
		m_bRotateAnimation = true;
		StartCooldown(dt, 2.f, 3.f);
		m_bDoOnce = false;
		m_fAttackTimer = 0.f;
		return;
	}

	m_bRotateAnimation = false;
	m_eCurMonsterState = STATE_BOMB;

	/* �躤�� ȸ�� */
	_float3 vPlayerPos = *static_cast<CTransform*>(m_pTargetPlayer->Find_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);
	_float3 vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vToPlayer = vPlayerPos - vPos;
	_float3 vLook = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	bool bRotated = m_pTransformCom->RotateToDirection(vLook, vToPlayer, 60.f, dt);

	//���� ��ȯ
	if (!m_bDoOnce)
	{
		m_pSoundCom->SetVolume("Spawn", 0.3f);
		m_pSoundCom->Play("Spawn");

		for (int i = 0; i < 4; i++)
		{
			_float3 vRandomPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			vRandomPos.x += GetRandomFloat(-300.f, 300.f);
			vRandomPos.y += GetRandomFloat(100.f, 300.f);
			vRandomPos.z += GetRandomFloat(-300.f, 300.f);

			SpawnDeacon(vRandomPos);
			SpawnDeaconEffect(vRandomPos);
		}
		m_bDoOnce = true;
	}
}

void CTtakkeun_i::FireAttack(_float dt)
{
	// ���� ��Ÿ��
	m_fAttackTimer += dt;
	if (m_fAttackTimer >= 2.f)
	{
		StartCooldown(dt, 2.f, 2.5f);
		m_fAttackTimer = 0.f;
		m_bDoOnce = false;
		return;
	}

	if (!m_pSoundCom->IsPlaying("Walk"))
	{
		m_pSoundCom->SetVolume("Walk", 0.2f);
		m_pSoundCom->Play("Walk");
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
	FX_MGR->FireAttack(vMyPos, LEVEL_GAMEPLAY, m_iNum);
	if (!m_pSoundCom->IsPlaying("FireAttack"))
	{
		m_pSoundCom->SetVolume("FireAttack", 0.4f);
		m_pSoundCom->Play("FireAttack");
	}

	/* [ �ݶ��̴� ���� ] */
	if (!m_bDoOnce)
	{
		//1. ȭ���߻�� ���ÿ� �ݶ��̴��� �ѹ��� ��ȯ�Ѵ�.
		_float3 vLook = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		_float3 vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		vLook.Normalize();
		vPos += vLook * 100.f;
		SpawnFireHitBox(vPos, TEXT("Cube"), 2.5f, false);
		m_bDoOnce = true;
	}

	if (m_pBossHitBox)
	{
		//2. ���� �ð��� �� �Ǿ� �����Ǿ��ٸ� ��۸������͸� �������ͷ� �ٲ��
		if (static_cast<CHitBox*>(m_pBossHitBox)->GetDead())
			m_pBossHitBox = nullptr;
	}

	if (m_pBossHitBox)
	{
		//3. �����Ͱ� ��ȿ�ϴٸ� ������ ������ ��� �������� �ٲ����
		auto vHitBoxPos = static_cast<CTransform*>(m_pBossHitBox->Find_Component(L"Com_Transform"));

		_float3 vLook = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		_float3 vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		vLook.Normalize();
		vPos += vLook * 100.0f;
		vHitBoxPos->Set_State(CTransform::STATE_POSITION, vPos);
	}
}

void CTtakkeun_i::BounceBall(_float dt)
{
	if (!m_pSoundCom->IsPlaying("Bounce"))
	{
		m_pSoundCom->SetVolume("Bounce", 0.2f);
		m_pSoundCom->Play("Bounce");
	}

	/* �ٿ�� ���� */

	/* 1. �����̴� ������ ���·� �÷��̾ �������Ѵ�. */
	m_bRotateAnimation = false;
	m_eCurMonsterState = STATE_BOMB;
	
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
		m_eCurMonsterState = STATE_WALK;
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
			if (!m_pSoundCom->IsPlaying("JumpAttack"))
			{
				m_pSoundCom->SetVolume("JumpAttack", 0.2f);
				m_pSoundCom->Play("JumpAttack");
			}
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
			m_pSoundCom->SetVolume("JumpAttack2", 0.4f);
			m_pSoundCom->Play("JumpAttack2");

			//����Ʈ ���� && ī�޶� ����ŷ
			vPos.y = 30.f;
			FX_MGR->JumpAttack(vPos, LEVEL_GAMEPLAY);
			FX_MGR->CutSceneSmoke(vPos, LEVEL_GAMEPLAY);
			SpawnJumpHitBox(vPos, TEXT("Cube"), 0.3f, true);
			m_pCamera->Shake_Camera();

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
		if (!m_pSoundCom->IsPlaying("Fly"))
		{
			m_pSoundCom->SetVolume("Fly", 0.4f);
			m_pSoundCom->Play("Fly");
		}
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
			StartCooldown(dt, 0.5f, 1.f);
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
		m_pSoundCom->SetVolume("Missile", 0.4f);
		m_pSoundCom->Play("Missile");

		// 0.2�ʸ��� �߻�
		CMonsterBullet::DESC MonsterBullet_iDesc{};
		MonsterBullet_iDesc.fSpeedPerSec = 60.f;
		MonsterBullet_iDesc.fRotationPerSec = RADIAN(180.f);
		MonsterBullet_iDesc.vScale = { 10.f, 10.f, 0.f };
		MonsterBullet_iDesc.vPosition = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_MonsterBullet"),
			m_eLevelID, L"Layer_MonsterBullet", &MonsterBullet_iDesc)))
			return;


		m_fSpawnMissile = 0.f;
	}
}

void CTtakkeun_i::SpawnGuidMissile()
{
	// �߽� ���� (����)
	_float3 vBaseDir = { 0.f, 1.f, 0.f };

	// ��ä�� ���� ����
	int missileCount = 5;
	float totalAngleDeg = 60.f;
	float angleStep = totalAngleDeg / (missileCount - 1); // ��: 15�� ����
	float startAngle = -totalAngleDeg * 0.5f; // ��: -30�� ����

	for (int i = 0; i < missileCount; ++i)
	{
		float angleDeg = startAngle + angleStep * i;
		float angleRad = RADIAN(angleDeg);

		// ���� ���� �������� Z�� ȸ�� (�¿�� ������)
		_float3 vRotated = Rotate_Z(vBaseDir, angleRad);
		
		CMonsterGuidBullet::DESC MonsterGuidBullet_iDesc{};
		MonsterGuidBullet_iDesc.fSpeedPerSec = 60.f;
		MonsterGuidBullet_iDesc.fRotationPerSec = RADIAN(180.f);
		MonsterGuidBullet_iDesc.vScale = { 30.f, 30.f, 1.f };
		MonsterGuidBullet_iDesc.vPosition = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		MonsterGuidBullet_iDesc.vPosition.y += 30;
		MonsterGuidBullet_iDesc.vDir = vRotated;

		_float3 vLook = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		vLook.Normalize();

		MonsterGuidBullet_iDesc.vPosition.x -= vLook.x * 10.f;
		MonsterGuidBullet_iDesc.vPosition.y -= vLook.y * 10.f;
		MonsterGuidBullet_iDesc.vPosition.z -= vLook.z * 10.f;

		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_MonsterGuidBullet"),
			m_eLevelID, L"Layer_MonsterBullet", &MonsterGuidBullet_iDesc)))
			return;
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

		BulletDesc.ColliderGroup = CG_BOSS;
		BulletDesc.fInitJumpPower = 30.f;
		BulletDesc.fTimeLimit = 2.f;
		BulletDesc.bAnimation = true;

		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_GrenadeBullet"),
			m_eLevelID, TEXT("Layer_Bullet"), &BulletDesc)))
			return;
	}
}

void CTtakkeun_i::SpawnDeacon(_float3 vPos)
{
	CMonster::DESC Deacon_iDesc{};
	Deacon_iDesc.vPosition = vPos;
	Deacon_iDesc.fSpeedPerSec = 60.f;
	Deacon_iDesc.fRotationPerSec = RADIAN(180.f);
	Deacon_iDesc.vActive = true;
	Deacon_iDesc.vReturnPos = vPos;
	Deacon_iDesc.fAttackDistance = 600.f;
	Deacon_iDesc.fDetectiveDistance = 800.f;
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Deacon"),
		m_eLevelID, L"Layer_Monster", &Deacon_iDesc)))
		return;
}

void CTtakkeun_i::SpawnDeaconEffect(_float3 vPos)
{
	CSprite::DESC ExplosionDesc{};
	ExplosionDesc.vInitPos = vPos;
	ExplosionDesc.vScale = { 200.f,200.f,1.f };
	ExplosionDesc.bLoop = false;
	ExplosionDesc.fMaxFrame = 8;
	ExplosionDesc.fRotationPerSec = RADIAN(180.f);
	ExplosionDesc.fSpeedPerSec = 100.f;
	ExplosionDesc.szTextureTag = TEXT("DeaconSpawn");

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_Sprite"),
		m_eLevelID, L"Layer_Effect", &ExplosionDesc)))
		return;
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

_float3 CTtakkeun_i::Rotate_Z(_float3 vDir, float rad)
{
	float cosA = cosf(rad);
	float sinA = sinf(rad);

	return 
	{
		vDir.x * cosA - vDir.y * sinA,
		vDir.x * sinA + vDir.y * cosA,
		vDir.z
	};
	return _float3();
}

void CTtakkeun_i::CutSceneAction(_float dt)
{
	/* [ �������� �ƾ� �Լ� ] */

	/* 1. ������Ʈ���� ȣ��Ǹ� �����ð��� ������. */

	_float m_fCutsceneDelay = 3.f;
	m_fAttackTimer += dt;

	if (m_fAttackTimer >= m_fCutsceneDelay)
	{
		m_bRender = true;
		/* 2. �����ð� �� ������ �Ʒ��� �����Ѵ�. */
		_float3 vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		if (vPos.y - 73.5f > 77.f)
		{
			// �߷� ���� ����
			m_fFallSpeed += 3000.f * dt;

			vPos.y -= m_fFallSpeed * dt;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

		}
		else
		{
			m_pSoundCom->SetVolume("JumpAttack", 0.5f);
			m_pSoundCom->Play("JumpAttack");

			//����Ʈ ���� && ī�޶� ����ŷ
			vPos.y = 30.f;
			FX_MGR->JumpAttack(vPos, LEVEL_GAMEPLAY);
			FX_MGR->CutSceneSmoke(vPos, LEVEL_GAMEPLAY);
			m_pCamera->Shake_Camera(0.5f,0.5f);

			//���Ͱ� ������ �� ��������
			vPos.y = 77.f;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
			m_fAttackTimer = 0.0f;
			m_bIsLink = true;
		}
	}
	if (m_bIsLink)
	{
		_float m_fCutsceneDelay = 0.7f;
		m_bCutSceneTime += dt;

		if (m_bCutSceneTime >= m_fCutsceneDelay)
		{
			m_fFallSpeed = 0.f;
			m_bCutScene = false;
			m_bCutSceneEnd = true;
			m_bActive = true;
			m_bCutSceneTime = 0.f;
		}
	}
}

void CTtakkeun_i::FlyEffect()
{
	//���� ��ƼŬ
	CPSystem::DESC FlyEffect_iDesc{};
	FlyEffect_iDesc.vPosition = { 0.f, 0.f, 0.f };
	FlyEffect_iDesc.szTextureTag = TEXT("PC_Small_Smoke");
	FlyEffect_iDesc.iParticleNums = 200;
	FlyEffect_iDesc.fSize = 7.f;
	FlyEffect_iDesc.fMaxFrame = 20.f;

	CGameObject* pObject = nullptr;
	CGameObject** ppOut = &pObject;
	if (FAILED(m_pGameInstance->Add_GameObjectReturn(LEVEL_STATIC, TEXT("Prototype_GameObject_PC_FlyEffect"),
		m_eLevelID, L"Layer_Particle", ppOut, &FlyEffect_iDesc)))
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
			m_bActive = false;

			if (m_pBossEffect)
			{
				static_cast<CFlyEffect*>(m_pBossEffect)->SetDead();
				m_pBossEffect = nullptr;
			}

			m_bDead = true;

			return;
		}

		//����Ʈ ����
		FX_MGR->SpawnBlood(vImpactPos, LEVEL_GAMEPLAY);
	}

	__super::On_Collision(MyColliderID, OtherColliderID);
	
}

CTtakkeun_i* CTtakkeun_i::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTtakkeun_i* pInstance = new CTtakkeun_i(pGraphic_Device);

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

	Safe_Release(m_pCamera);
	Safe_Release(m_pSoundCom);
}