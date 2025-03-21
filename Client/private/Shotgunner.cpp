// �� Ŭ���� �̸� : TestMonster
// �θ� Ŭ���� �̸� : Monster

#include "Shotgunner.h"

CShotgunner::CShotgunner(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster{ pGraphic_Device }
{
}

CShotgunner::CShotgunner(const CShotgunner& Prototype)
	: CMonster(Prototype)
{
}

HRESULT CShotgunner::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CShotgunner::Initialize(void* pArg)
{

	//��ġ, ũ���ʱ�ȭ, ������Ʈ ����
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	//�ִϸ��̼�(��������)
	m_fAnimationMaxFrame = 3.f;
	m_fAnimationSpeed = 7.f;

	return S_OK;
}

void CShotgunner::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CShotgunner::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CShotgunner::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CShotgunner::Render()
{
	return __super::Render();

	//Ư���� �� ������ �Ұ� �ִ� ��� ��
}

HRESULT CShotgunner::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

CShotgunner* CShotgunner::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	//������Ÿ���� �⺻����
	CShotgunner* pInstance = new CShotgunner(pGraphic_Device);

	//�⺻ ����
	pInstance->m_eLevelID		= LEVEL_GAMEPLAY;
	pInstance->m_szTextureID	= TEXT("Shotgunner_Walk");
	pInstance->m_szBufferType	= TEXT("Rect");

	//�Ӽ�
	pInstance->m_iHP			= 80;
	pInstance->m_iAttackPower	= 6;
	pInstance->m_iDefense		= 0;
	pInstance->m_fSpeed			= 4.f;
	pInstance->m_vScale			= { 27.f, 46.f, 1.f };
	pInstance->m_eBehavior		= MODE::MODE_WAIT;

	//�μӼ�
	pInstance->m_strDialogue	= "Gun..Gun...";
	pInstance->m_strSound		= "SoundFilePath";

	pInstance->m_vDropItems.push_back("ShotgunnerDropItem");


	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CShotgunner");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CShotgunner::Clone(void* pArg)
{
	CShotgunner* pInstance = new CShotgunner(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CShotgunner");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShotgunner::Free()
{
	__super::Free();
}