// 내 클래스 이름 : Trigger
// 부모 클래스 이름 : GameObject

#include "Trigger.h"
#include "GameInstance.h"

CTrigger::CTrigger(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CTrigger::CTrigger(const CTrigger& Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CTrigger::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTrigger::Initialize(void* pArg)
{
	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (pArg != nullptr)
	{
		DESC* pDesc = static_cast<DESC*>(pArg);

		if (nullptr == m_pGameInstance->Find_Objects(LEVEL_GAMEPLAY, pDesc->LayerTag))
			return S_SKIP;
		m_pTargets = *m_pGameInstance->Find_Objects(LEVEL_GAMEPLAY, pDesc->LayerTag);
		for (auto pGameObject : m_pTargets)
			Safe_AddRef(pGameObject);
	}

	return S_OK;
}

void CTrigger::Priority_Update(_float fTimeDelta)
{

}

EVENT CTrigger::Update(_float fTimeDelta)
{
	if (m_bDead)
		return EVN_DEAD;

	return EVN_NONE;
}

void CTrigger::Late_Update(_float fTimeDelta)
{

}

HRESULT CTrigger::Render()
{
	return S_OK;
}

HRESULT CTrigger::Ready_Components(void* pArg)
{
	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), pArg)))
		return E_FAIL;

	if (pArg != nullptr)
	{
		DESC* pDesc = static_cast<DESC*>(pArg);
		m_pTransformCom->QurternionRotation(pDesc->vAngle);
		m_pTransformCom->Scaling(pDesc->vScale);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, (pDesc->vInitPos));
	}

	CCollider::DESC ColliderDesc{};
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vScale = m_pTransformCom->Compute_Scaled();
	ColliderDesc.pOwner = this;
	ColliderDesc.iColliderGroupID = CG_TRIGGER;
	ColliderDesc.iColliderID = CI_TRIGGER;

	auto& vAngle = static_cast<DESC*>(pArg)->vAngle;

	/* For.Com_Collider */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB_Cube"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

void CTrigger::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	for (auto& pGameObject : m_pTargets)
	{
		pGameObject->On_Trigger();
	}
	m_bDead = true;
}

CTrigger* CTrigger::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTrigger* pInstance = new CTrigger(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTrigger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTrigger::Clone(void* pArg)
{
	CTrigger* pInstance = new CTrigger(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CTrigger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTrigger::Free()
{
	__super::Free();


	for (auto pGameObject : m_pTargets)
		Safe_Release(pGameObject);


	Safe_Release(m_pTransformCom);
	Safe_Release(m_pColliderCom);
}