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

	//�ִϸ��̼�(��������)
	m_fAnimationMaxFrame = 1.f;
	m_fAnimationSpeed = 0.f;

	return S_OK;
}

void CDeacon::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CDeacon::Update(_float fTimeDelta)
{
	if (m_bDead)
		return EVN_DEAD;

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
	pInstance->m_iAttackPower	= 5;
	pInstance->m_iDefense		= 1;
	pInstance->m_fSpeed			= 15.f;
	pInstance->m_vScale			= { 52.f, 75.f, 1.f };
	pInstance->m_eBehavior		= MODE::MODE_WAIT;

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