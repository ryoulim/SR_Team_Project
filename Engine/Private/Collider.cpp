#include "Collider.h"
#include "GameObject.h"
#include "Transform.h"
#include "GameInstance.h"

_float3 CCollider::m_vLast_Collision_Depth{};
_float3 CCollider::m_vLast_Collision_Pos{};

CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent{ pGraphic_Device }
{
}

CCollider::CCollider(const CCollider& Prototype)
	: CComponent(Prototype)
	, m_eType{ Prototype.m_eType}
{
}

HRESULT CCollider::Initialize_Prototype(COLLIDER_TYPE Type)
{
	m_eType = Type;
	return S_OK;
}

HRESULT CCollider::Initialize(void* pArg)
{
	if (pArg == nullptr)
	{
		MSG_BOX("콜라이더 pArg에 nullptr을 넣어주면 어떡해...");
		return E_FAIL;
	}

	DESC* pDesc = static_cast<DESC*>(pArg);
	m_pOwner = pDesc->pOwner;
	//Safe_AddRef(m_pOwner); 순환참조 때문에 어쩔수가 없다 쉬팔
	m_pTransform = pDesc->pTransform;
	Safe_AddRef(m_pTransform);
	m_vOffSet = pDesc->vOffSet;
	m_iColliderID = pDesc->iColliderID;

	Update_Scale(pDesc->vScale);
	Update_Collider();
	m_pGameInstance->Add_Collider(this, pDesc->iColliderGroupID);
	return S_OK;
}

_bool CCollider::Check_Intersect(const CCollider* Collider)
{
	if ((Collider->Get_Pos() - Get_Pos()).Length() >
		Get_MaxLength() + Collider->Get_MaxLength())
		return FALSE;

	switch (Collider->Get_Type())
	{
	case AABB_CUBE:
		return Intersect_With_AABB_Cube(Collider);
	case OBB_CUBE:
		return Intersect_With_OBB_Cube(Collider);
	case SPHERE:
		return Intersect_With_Sphere(Collider);
	case CAPSULE:
		return Intersect_With_Capsule(Collider);
	case LINE:
		return Intersect_With_Line(Collider);
	case RECT:
		return Intersect_With_Rect(Collider);
	default:
		return FALSE;
	}

	return FALSE;
}

void CCollider::Free()
{
	__super::Free();
	Safe_Release(m_pTransform);
	//Safe_Release(m_pOwner); 난 최선을 다했어
}