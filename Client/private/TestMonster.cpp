// 내 클래스 이름 : TestMonster
// 부모 클래스 이름 : Monster

#include "TestMonster.h"

CTestMonster::CTestMonster(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster{ pGraphic_Device }
{
}

CTestMonster::CTestMonster(const CTestMonster& Prototype)
	: CMonster(Prototype)
{
}

HRESULT CTestMonster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTestMonster::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_GAMEPLAY;
	m_szTextureID = TEXT("TestMonster");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CTestMonster::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CTestMonster::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CTestMonster::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CTestMonster::Render()
{
	return __super::Render();
}

HRESULT CTestMonster::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	DESC* pDesc = static_cast<DESC*>(pArg);
	CCollider_Capsule::DESC ColliderDesc{};
	ColliderDesc.pTransform = m_pTransformCom;

	/* For.Com_Collider */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Capsule"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pCollider), &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

CTestMonster* CTestMonster::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTestMonster* pInstance = new CTestMonster(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTestMonster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTestMonster::Clone(void* pArg)
{
	CTestMonster* pInstance = new CTestMonster(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CTestMonster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTestMonster::Free()
{
	__super::Free();
}