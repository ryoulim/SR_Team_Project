// �� Ŭ���� �̸� : TestMonster
// �θ� Ŭ���� �̸� : Monster

#include "Cultist.h"
#include "FXMgr.h"

CCultist::CCultist(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster{ pGraphic_Device }
{
}

CCultist::CCultist(const CCultist& Prototype)
	: CMonster(Prototype)
{
}

HRESULT CCultist::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCultist::Initialize(void* pArg)
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

void CCultist::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CCultist::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CCultist::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CCultist::Render()
{
	return __super::Render();

	//Ư���� �� ������ �Ұ� �ִ� ��� ��
}

HRESULT CCultist::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCultist::Ready_Textures()
{
	for (_uint i = 0; i < D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Cultist_Move_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(m_eLevelID, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Cultist_Move_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_MOVE][i])))))
			return E_FAIL;
	}
	return S_OK;
}

void CCultist::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	//��ġŽ��
	_float3 vImpactPos = CalculateEffectPos();

	//���� ���
	if (0 >= m_iHP)
	{
		CFXMgr::Get_Instance()->SpawnCustomExplosion(vImpactPos, LEVEL_GAMEPLAY, _float3{ 60.f, 80.f, 1.f }, TEXT("PC_Explosion"), 14);
		m_bDead = true;

		return;
	}

	// ����Ʈ ����
	m_iHP += -50;
	CFXMgr::Get_Instance()->SpawnBlood(vImpactPos, LEVEL_GAMEPLAY);
}

CCultist* CCultist::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	//������Ÿ���� �⺻����
	CCultist* pInstance = new CCultist(pGraphic_Device);

	//�⺻ ����
	pInstance->m_eLevelID		= LEVEL_GAMEPLAY;
	pInstance->m_szTextureID	= TEXT("Cultist_Run");
	pInstance->m_szBufferType	= TEXT("Rect");

	//�Ӽ�
	pInstance->m_iHP			= 150;
	pInstance->m_iAttackPower	= 8;
	pInstance->m_iDefense		= 1;
	pInstance->m_fSpeed			= 10.f;
	pInstance->m_vScale			= { 27.f, 56.f, 1.f };
	pInstance->m_eState		= MODE::MODE_IDLE;
	pInstance->m_fDetectiveDistance = 300.f;

	//�μӼ�
	pInstance->m_strDialogue	= "Cultist..Cultist...";
	pInstance->m_strSound		= "SoundFilePath";

	pInstance->m_vDropItems.push_back("CultistDropItem");


	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCultist");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCultist::Clone(void* pArg)
{
	CCultist* pInstance = new CCultist(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CCultist");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCultist::Free()
{
	__super::Free();
}