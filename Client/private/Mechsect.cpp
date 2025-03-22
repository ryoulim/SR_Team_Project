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

	//�ִϸ��̼�(��������)
	m_fAnimationMaxFrame = 4.f;
	m_fAnimationSpeed = 8.f;

	return S_OK;
}

void CMechsect::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CMechsect::Update(_float fTimeDelta)
{
	if (m_bDead)
		return EVN_DEAD;
	if (m_bActive)
		MonsterTick(fTimeDelta);
	return __super::Update(fTimeDelta);
}

void CMechsect::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CMechsect::Render()
{
	Render_DebugFOV();
	return __super::Render();

	//Ư���� �� ������ �Ұ� �ִ� ��� ��
}

HRESULT CMechsect::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CMechsect::On_Collision(CGameObject* pCollisionedObject, const _wstring& strLayerTag)
{
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