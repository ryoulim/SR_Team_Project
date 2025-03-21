// �� Ŭ���� �̸� : TestMonster
// �θ� Ŭ���� �̸� : Monster

#include "Greater.h"

CGreater::CGreater(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster{ pGraphic_Device }
{
}

CGreater::CGreater(const CGreater& Prototype)
	: CMonster(Prototype)
{
}

HRESULT CGreater::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGreater::Initialize(void* pArg)
{

	//��ġ, ũ���ʱ�ȭ, ������Ʈ ����
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	//�ִϸ��̼�(��������)
	m_fAnimationMaxFrame = 3.f;
	m_fAnimationSpeed = 7.f;

	return S_OK;
}

void CGreater::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CGreater::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CGreater::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CGreater::Render()
{
	return __super::Render();

	//Ư���� �� ������ �Ұ� �ִ� ��� ��
}

HRESULT CGreater::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

CGreater* CGreater::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	//������Ÿ���� �⺻����
	CGreater* pInstance = new CGreater(pGraphic_Device);

	//�⺻ ����
	pInstance->m_eLevelID		= LEVEL_GAMEPLAY;
	pInstance->m_szTextureID	= TEXT("Greater_Run");
	pInstance->m_szBufferType	= TEXT("Rect");

	//�Ӽ�
	pInstance->m_iHP			= 120;
	pInstance->m_iAttackPower	= 15;
	pInstance->m_iDefense		= 2;
	pInstance->m_fSpeed			= 12.f;
	pInstance->m_vScale			= { 20.f, 50.f, 1.f };
	pInstance->m_eBehavior		= MODE::MODE_WAIT;

	//�μӼ�
	pInstance->m_strDialogue	= "Great..Great...?";
	pInstance->m_strSound		= "SoundFilePath";

	pInstance->m_vDropItems.push_back("GreatDropItem");


	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CGreater");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGreater::Clone(void* pArg)
{
	CGreater* pInstance = new CGreater(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CGreater");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGreater::Free()
{
	__super::Free();
}