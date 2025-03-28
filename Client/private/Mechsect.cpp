// �� Ŭ���� �̸� : TestMonster
// �θ� Ŭ���� �̸� : Monster

#include "Mechsect.h"
#include "FXMgr.h"

CMechsect::CMechsect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster{ pGraphic_Device }
{
}

CMechsect::CMechsect(const CMechsect& Prototype)
	: CMonster(Prototype)
{
}

HRESULT CMechsect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMechsect::Initialize(void* pArg)
{

	//��ġ, ũ���ʱ�ȭ, ������Ʈ ����
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_fDivOffset = 45.f;
	m_isReadyMonster = true;
	Ready_Textures();
	//�ִϸ��̼�(��������)
	m_fAnimationMaxFrame = 4.f;
	m_fAnimationSpeed = 5.f;
	m_iState = STATE_MOVE;

	return S_OK;
}

void CMechsect::Priority_Update(_float fTimeDelta)
{
	Set_Animation();
	__super::Priority_Update(fTimeDelta);
}

EVENT CMechsect::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CMechsect::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CMechsect::Render()
{
	return __super::Render();

	//Ư���� �� ������ �Ұ� �ִ� ��� ��
}

HRESULT CMechsect::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMechsect::Ready_Textures()
{
	/* Move */
	for (_uint i = 0; i < D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Mechsect_Move_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(m_eLevelID, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Mechsect_Move_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_MOVE][i])))))
			return E_FAIL;
	}

	/* Jump */
	for (_uint i = 0; i < D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Mechsect_Jump_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(m_eLevelID, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Mechsect_Jump_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_JUMP][i])))))
			return E_FAIL;
	}

	/* Death */
	if (FAILED(__super::Add_Component(m_eLevelID, L"Prototype_Component_Texture_Mechsect_Dead",
		_wstring(TEXT("Com_Texture")) + L"_Mechsect_Dead", reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_DEAD][0])))))
		return E_FAIL;


	return S_OK;
}


HRESULT CMechsect::Set_Animation()
{
	if (m_eCurMonsterState != m_ePrevMonsterState)
	{
		m_ePrevMonsterState = m_eCurMonsterState;
		m_fAnimationFrame = 0.f;
		m_iState = (_uint)(m_eCurMonsterState);
		switch (m_eCurMonsterState)
		{
		case Client::CMechsect::STATE_MOVE:
			m_fAnimationMaxFrame = _float(MAX_MOVERUN);
			m_fAnimationSpeed = 10.f;
			break;
		case Client::CMechsect::STATE_JUMP:
			m_fAnimationMaxFrame = _float(MAX_MOVERUN);
			m_fAnimationSpeed = 10.f;
			break;
		case Client::CMechsect::STATE_STAY:
			m_fAnimationMaxFrame = 1.f;
			m_fAnimationSpeed = 0.f;
			m_iState = (_uint)(STATE_MOVE);
			break;
		case Client::CMechsect::STATE_DEAD:
			m_fAnimationMaxFrame = _float(MAX_DEAD);
			m_fAnimationSpeed = 8.f;
			m_bRotateAnimation = false;
			break;
		}
	}
	return S_OK;
}

HRESULT CMechsect::Animate_Monster(_float fTimeDelta)
{
	if (m_fAnimationMaxFrame < 2.f)
		return S_OK;

	switch (m_eCurMonsterState)
	{
	case Client::CMechsect::STATE_STAY:
		m_bRotateAnimation = true;
		return S_OK;
	case Client::CMechsect::STATE_MOVE:
		m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		if (m_fAnimationFrame >= m_fAnimationMaxFrame)
			m_fAnimationFrame = 0.f;
		m_bRotateAnimation = true;
		break;
	case Client::CMechsect::STATE_JUMP:
		m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		if (m_fAnimationFrame >= m_fAnimationMaxFrame)
			m_fAnimationFrame = 0.f;
		m_bRotateAnimation = true;
		break;
	case Client::CMechsect::STATE_DEAD:
		m_fAnimationFrame += fTimeDelta * m_fAnimationSpeed;
		if (m_fAnimationFrame >= m_fAnimationMaxFrame)
			m_fAnimationFrame = m_fAnimationMaxFrame - 1.f;
		m_bRotateAnimation = false;
		break;
	}
	return S_OK;
}


void CMechsect::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	//�� ��� ��Ʋ��� ����
	m_eState = MODE::MODE_BATTLE;

	//��ġŽ��
	_float3 vImpactPos = CalculateEffectPos();

	//���� ���
	if (0 >= m_iHP)
	{
		CFXMgr::Get_Instance()->SpawnCustomExplosion(vImpactPos, LEVEL_GAMEPLAY, _float3{ 70.f, 80.f, 1.f }, TEXT("Effect_Explorer"), 24);
		m_bDead = true;

		return;
	}

	// ����Ʈ ����
	m_iHP += -50;
	CFXMgr::Get_Instance()->SpawnBlood(vImpactPos, LEVEL_GAMEPLAY);
}

CMechsect* CMechsect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	//������Ÿ���� �⺻����
	CMechsect* pInstance = new CMechsect(pGraphic_Device);

	//�⺻ ����
	pInstance->m_eLevelID		= LEVEL_GAMEPLAY;
	pInstance->m_szTextureID	= TEXT("Mechsect_Walk");
	pInstance->m_szBufferType	= TEXT("Rect");

	//�Ӽ�
	pInstance->m_iHP			= 50;
	pInstance->m_iMaxHP			= 50;
	pInstance->m_iAttackPower	= 2;
	pInstance->m_iDefense		= 1;
	pInstance->m_fSpeed			= 20.f;
	pInstance->m_vScale			= { 45.f, 20.f, 1.f };
	pInstance->m_eState		= MODE::MODE_IDLE;
	pInstance->m_fDetectiveDistance = 300.f;

	//�μӼ�
	pInstance->m_strDialogue	= "Mechs..Mec...";
	pInstance->m_strSound		= "SoundFilePath";

	pInstance->m_vDropItems.push_back("MechsectDropItem");


	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMechsect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMechsect::Clone(void* pArg)
{
	CMechsect* pInstance = new CMechsect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CMechsect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMechsect::Free()
{
	__super::Free();
}