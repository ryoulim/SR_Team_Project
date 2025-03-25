// �� Ŭ���� �̸� : TestMonster
// �θ� Ŭ���� �̸� : Monster

#include "Nukemutant.h"
#include "FXMgr.h"

CNukemutant::CNukemutant(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster{ pGraphic_Device }
{
}

CNukemutant::CNukemutant(const CNukemutant& Prototype)
	: CMonster(Prototype)
{
}

HRESULT CNukemutant::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CNukemutant::Initialize(void* pArg)
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

void CNukemutant::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CNukemutant::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CNukemutant::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CNukemutant::Render()
{
	return __super::Render();

	//Ư���� �� ������ �Ұ� �ִ� ��� ��
}

HRESULT CNukemutant::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CNukemutant::Ready_Textures()
{
	/* ATTACK */
	//for (_uint i = 0; i < D_END; i++)
	//{
	//	_wstring sPrototypeTag = L"Prototype_Component_Texture_Nukemutant_Attack_";
	//	_uint num = static_cast<_uint>(i * m_fDivOffset);
	//	_tchar buf[32];
	//	_itow_s((int)num, buf, 10);
	//	sPrototypeTag += buf;
	//	if (FAILED(__super::Add_Component(m_eLevelID, sPrototypeTag,
	//		_wstring(TEXT("Com_Texture")) + L"_Nukemutant_Attack_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_ATTACK][i])))))
	//		return E_FAIL;
	//}

	/* DEATH */
	//for (_uint i = 0; i < D_END; i++)
	//{
	//	_wstring sPrototypeTag = L"Prototype_Component_Texture_Nukemutant_Death_";
	//	_uint num = static_cast<_uint>(0);
	//	_tchar buf[32];
	//	_itow_s((int)num, buf, 10);
	//	sPrototypeTag += buf;
	//	if (FAILED(__super::Add_Component(m_eLevelID, sPrototypeTag,
	//		_wstring(TEXT("Com_Texture")) + L"_Nukemutant_Death_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_DEATH][0])))))
	//		return E_FAIL;
	//}

	/* MOVE */
	for (_uint i = 0; i < D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Nukemutant_Move_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(m_eLevelID, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Nukemutant_Move_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_MOVE][i])))))
			return E_FAIL;
	}
	return S_OK;
}

void CNukemutant::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	//�� ��� ��Ʋ��� ����
	m_eState = MODE::MODE_BATTLE;

	//��ġŽ��
	_float3 vImpactPos = CalculateEffectPos();

	//���� ���
	if (0 >= m_iHP)
	{
		CFXMgr::Get_Instance()->SpawnCustomExplosion(vImpactPos, LEVEL_GAMEPLAY, _float3{ 120.f, 160.f, 1.f }, TEXT("Effect_Explorer"), 24);
		m_bDead = true;

		return;
	}

	// ����Ʈ ����
	m_iHP += -50;
	CFXMgr::Get_Instance()->SpawnBlood(vImpactPos, LEVEL_GAMEPLAY);
}

CNukemutant* CNukemutant::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	//������Ÿ���� �⺻����
	CNukemutant* pInstance = new CNukemutant(pGraphic_Device);

	//�⺻ ����
	pInstance->m_eLevelID		= LEVEL_GAMEPLAY;
	pInstance->m_szTextureID	= TEXT("Nukemutant_Walk");
	pInstance->m_szBufferType	= TEXT("Rect");

	//�Ӽ�
	pInstance->m_iHP			= 50;
	pInstance->m_iMaxHP			= 50;
	pInstance->m_iAttackPower	= 15;
	pInstance->m_iDefense		= 0;
	pInstance->m_fSpeed			= 15.f;
	pInstance->m_vScale			= { 27.f, 56.f, 1.f };
	pInstance->m_eState		= MODE::MODE_IDLE;
	pInstance->m_fDetectiveDistance = 300.f;

	//�μӼ�
	pInstance->m_strDialogue	= "Nuke..Nu...";
	pInstance->m_strSound		= "SoundFilePath";

	pInstance->m_vDropItems.push_back("NukemutantDropItem");


	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CNukemutant");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CNukemutant::Clone(void* pArg)
{
	CNukemutant* pInstance = new CNukemutant(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CNukemutant");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNukemutant::Free()
{
	__super::Free();
}