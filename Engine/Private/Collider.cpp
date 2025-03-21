#include "Collider.h"
#include "Transform.h"

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
	m_pTransform = pDesc->pTransform;
	Safe_AddRef(m_pTransform);
	m_vOffSet = pDesc->vOffSet;

	Update_Scale(pDesc->vScale);
	Update_Collider();
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
}