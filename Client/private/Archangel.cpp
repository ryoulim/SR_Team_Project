// �� Ŭ���� �̸� : TestMonster
// �θ� Ŭ���� �̸� : Monster

#include "Archangel.h"
#include "FXMgr.h"

CArchangel::CArchangel(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster{ pGraphic_Device }
{
}

CArchangel::CArchangel(const CArchangel& Prototype)
	: CMonster(Prototype)
{
}

HRESULT CArchangel::Initialize_Prototype()
{
	//������Ÿ���� �⺻����
	m_szTextureID = TEXT("Archangel_Walk");
	m_szBufferType = TEXT("Rect");

	//�Ӽ�
	m_iHP = 350;
	m_iMaxHP = 350;
	m_iAttackPower = 20;
	m_iDefense = 3;
	m_fSpeed = 13.f;
	m_vScale = { 61.f, 162.f, 1.f };
	m_eState = MODE::MODE_IDLE;

	m_fDetectiveDistance = 300.f;

	//�μӼ�
	m_strDialogue = "Archangel..Archangel...";
	m_strSound = "SoundFilePath";

	m_vDropItems.push_back("ArchangelDropItem");

	return S_OK;
}

HRESULT CArchangel::Initialize(void* pArg)
{
	//��ġ, ũ���ʱ�ȭ, ������Ʈ ����
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_fDivOffset = 45.f;
	//�ִϸ��̼�(��������)
	m_fAnimationMaxFrame = 4.f;
	m_fAnimationSpeed = 5.f;
	m_iState = STATE_MOVE;
	return S_OK;
}

void CArchangel::Priority_Update(_float fTimeDelta)
{
	Set_Animation();
	__super::Priority_Update(fTimeDelta);
}

EVENT CArchangel::Update(_float fTimeDelta)
{
	if (KEY_DOWN(DIK_RBRACKET))
	{
		int i = m_eCurMonsterState;
		i++;
		m_eCurMonsterState = MONSTER_STATE(i);
		if (m_eCurMonsterState == STATE_END)
			m_eCurMonsterState = MONSTER_STATE(0);
	}
	return __super::Update(fTimeDelta);
}

void CArchangel::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	Resize_Texture(0.8f);
	if (m_eState == MODE::MODE_BATTLE)
		m_iDegree = 0;
}

HRESULT CArchangel::Render()
{
	return __super::Render();

	//Ư���� �� ������ �Ұ� �ִ� ��� ��
}

HRESULT CArchangel::Ready_Components(void* pArg)
{
	Ready_Textures();
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CArchangel::Ready_Textures()
{
	/* MOVE */
	for (_uint i = 0; i < D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Archangel_Move_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(LEVEL_STATIC, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Archangel_Move_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_MOVE][i])))))
			return E_FAIL;
	}
	/* ATTACK */
	for (_uint i = 0; i < D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Archangel_Attack_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(LEVEL_STATIC, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Archangel_Attack_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_ATTACK][i])))))
			return E_FAIL;
	}
	/* FLY */
	for (_uint i = 0; i < D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Archangel_Fly_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(LEVEL_STATIC, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Archangel_Fly_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_FLY][i])))))
			return E_FAIL;
	}
	/* STAY */
	for (_uint i = 0; i < D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Archangel_Stay_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(LEVEL_STATIC, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Archangel_Stay_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_STAY][i])))))
			return E_FAIL;
	}

	/* DEATH */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_Archangel_Dead",
		_wstring(TEXT("Com_Texture")) + L"_Archangel_Dead", reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_DEAD][0])))))
		return E_FAIL;

	return S_OK;
}


HRESULT CArchangel::Set_Animation()
{
	if (m_eCurMonsterState != m_ePrevMonsterState)
	{
		m_ePrevMonsterState = m_eCurMonsterState;
		m_fAnimationFrame = 0.f;
		m_iState = (_uint)(m_eCurMonsterState);
		switch (m_eCurMonsterState)
		{
		case Client::CArchangel::STATE_MOVE:
			m_fAnimationMaxFrame = _float(MAX_MOVERUN);
			m_fAnimationSpeed = 10.f;
			break;
		case Client::CArchangel::STATE_FLY:
			m_fAnimationMaxFrame = _float(MAX_FLY);
			m_fAnimationSpeed = 10.f;
			break;
		case Client::CArchangel::STATE_ATTACK:
			m_fAnimationMaxFrame = _float(MAX_ATTACK);
			m_fAnimationSpeed = 10.f;
			break;
		case Client::CArchangel::STATE_STAY:
			m_fAnimationMaxFrame = _float(MAX_STAY);
			m_fAnimationSpeed = 0.f;
			break;
		case Client::CArchangel::STATE_DEAD:
			m_fAnimationMaxFrame = _float(MAX_DEAD);
			m_fAnimationSpeed = 8.f;
			m_bRotateAnimation = false;
			break;
		}
	}
	return S_OK;
}

HRESULT CArchangel::Animate_Monster(_float fTimeDelta)
{
	if (m_fAnimationMaxFrame < 2.f)
		return S_OK;

	switch (m_eCurMonsterState)
	{
	case Client::CArchangel::STATE_STAY:
		m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		if (m_fAnimationFrame >= m_fAnimationMaxFrame)
			m_fAnimationFrame = 0.f;
		m_bRotateAnimation = true;
		return S_OK;
	case Client::CArchangel::STATE_MOVE:
		m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		if (m_fAnimationFrame >= m_fAnimationMaxFrame)
			m_fAnimationFrame = 0.f;
		m_bRotateAnimation = true;
		break;
	case Client::CArchangel::STATE_FLY:
		m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		if (m_fAnimationFrame >= m_fAnimationMaxFrame)
			m_fAnimationFrame = 0.f;
		m_bRotateAnimation = true;
		break;
	case Client::CArchangel::STATE_ATTACK:
		m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		if (m_fAnimationFrame >= m_fAnimationMaxFrame)
			m_fAnimationFrame = 0.f;
		m_bRotateAnimation = true;
		break;
	case Client::CArchangel::STATE_DEAD:
		m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		if (m_fAnimationFrame >= m_fAnimationMaxFrame)
			m_fAnimationFrame = m_fAnimationMaxFrame - 1.f;
		m_bRotateAnimation = false;
		break;
	}
	return S_OK;
}


void CArchangel::On_Collision(_uint MyColliderID, _uint OtherColliderID)
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
		if (!m_bFoundPlayer)
			m_eState = MODE::MODE_DETECTIVE;

		//��ġŽ��
		_float3 vImpactPos = CalculateEffectPos();

		//���� ���
		if (0 >= m_iHP)
		{
			FX_MGR->SpawnCustomExplosion(vImpactPos, LEVEL_GAMEPLAY, _float3{ 130.f, 160.f, 1.f }, TEXT("PC_Explosion"), 14);
			m_bDead = true;

			return;
		}
		// ����Ʈ ����
		FX_MGR->SpawnBlood(vImpactPos, LEVEL_GAMEPLAY);
		__super::On_Collision(MyColliderID, OtherColliderID);
	}
}

CArchangel* CArchangel::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CArchangel* pInstance = new CArchangel(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CArchangel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CArchangel::Clone(void* pArg)
{
	CArchangel* pInstance = new CArchangel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CArchangel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CArchangel::Free()
{
	__super::Free();
}