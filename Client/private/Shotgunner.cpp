// �� Ŭ���� �̸� : TestMonster
// �θ� Ŭ���� �̸� : Monster

#include "Shotgunner.h"
#include "FXMgr.h"

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
	m_fDivOffset = 45.f;
	m_isReadyMonster = true;
	Ready_Textures();
	//�ִϸ��̼�(��������)
	m_fAnimationMaxFrame = 3.f;
	m_fAnimationSpeed = 5.f;
	m_iState = STATE_ATTACK;

	return S_OK;
}

void CShotgunner::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CShotgunner::Update(_float fTimeDelta)
{
	if (m_bDead)
		return EVN_DEAD;

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

HRESULT CShotgunner::Ready_Textures()
{
	/* ATTACK */
	for (_uint i = 0; i < D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Shotgunner_Attack_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(m_eLevelID, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Shotgunner_Attack_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_ATTACK][i])))))
			return E_FAIL;
	}

	/* DEATH */
	//for (_uint i = 0; i < D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Shotgunner_Death_";
		_uint num = static_cast<_uint>(0);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(m_eLevelID, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Shotgunner_Death_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_DEATH][0])))))
			return E_FAIL;
	}

	/* MOVE */
	for (_uint i = 0; i < D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Shotgunner_Move_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(m_eLevelID, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Shotgunner_Move_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[STATE_MOVE][i])))))
			return E_FAIL;
	}

	return S_OK;
}

void CShotgunner::On_Collision(CGameObject* pCollisionedObject, const _wstring& strLayerTag)
{
	//��ġŽ��
	_float3 vImpactPos = CalculateEffectPos();

	//���� ���
	if (0 >= m_iHP)
	{
		CFXMgr::Get_Instance()->SpawnCustomExplosion(vImpactPos, LEVEL_GAMEPLAY, _float3{ 40.f, 80.f, 1.f }, TEXT("PC_Explosion"), 14);
		m_bDead = true;

		return;
	}

	// ����Ʈ ����
	m_iHP += -50;
	CFXMgr::Get_Instance()->SpawnBlood(vImpactPos, LEVEL_GAMEPLAY);
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