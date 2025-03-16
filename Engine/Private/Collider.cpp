#include "Collider.h"

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
	return S_OK;
}

_bool CCollider::Check_Intersect(const CCollider* Collider)
{
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
	default:
		return FALSE;
	}

	return FALSE;
}

void CCollider::Free()
{
	__super::Free();
}