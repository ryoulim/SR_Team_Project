#include "Collider_AABB_Cube.h"

CCollider_AABB_Cube::CCollider_AABB_Cube(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCollider{ pGraphic_Device }
{
}

CCollider_AABB_Cube::CCollider_AABB_Cube(const CCollider_AABB_Cube& Prototype)
	: CCollider(Prototype)
{
}

HRESULT CCollider_AABB_Cube::Initialize_Prototype()
{
	// 타입을 프로토타입 생성시에 확정하고 이후 바꾸지 못하도록 제한	
	return __super::Initialize_Prototype(AABB_CUBE);
}

void CCollider_AABB_Cube::Update_Collider()
{
	m_tInfo.vCenter = *m_pTransform->Get_State(CTransform::STATE_POSITION) + m_vOffSet;
	m_tInfo.vMinPos = m_tInfo.vCenter - m_vHalfScale;
	m_tInfo.vMaxPos = m_tInfo.vCenter + m_vHalfScale;
}

void CCollider_AABB_Cube::Update_Scale(const _float3& vScale)
{
	m_vHalfScale = vScale * 0.5f;
}

_bool CCollider_AABB_Cube::Intersect_With_AABB_Cube(const CCollider* pOther)
{
	auto pOtherInfo = dynamic_cast<const CCollider_AABB_Cube*>(pOther)->Get_Info();
	
	return (m_tInfo.vMaxPos.x > pOtherInfo->vMinPos.x && m_tInfo.vMinPos.x < pOtherInfo->vMaxPos.x) &&
		(m_tInfo.vMaxPos.y > pOtherInfo->vMinPos.y && m_tInfo.vMinPos.y < pOtherInfo->vMaxPos.y) &&
		(m_tInfo.vMaxPos.z > pOtherInfo->vMinPos.z && m_tInfo.vMinPos.z < pOtherInfo->vMaxPos.z);
}

_bool CCollider_AABB_Cube::Intersect_With_OBB_Cube(const CCollider* pOther)
{
	return _bool();
}

_bool CCollider_AABB_Cube::Intersect_With_Sphere(const CCollider* pOther)
{
	return _bool();
}

_bool CCollider_AABB_Cube::Intersect_With_Capsule(const CCollider* pOther)
{
	return _bool();
}

_bool CCollider_AABB_Cube::Intersect_With_Rect(const CCollider* pOther)
{
	return _bool();
}

_bool CCollider_AABB_Cube::Intersect_With_Line(const CCollider* pOther)
{
	return _bool();
}

_bool CCollider_AABB_Cube::RayCasting(const _float3& rayOrigin, const _float3& rayDir)
{
	_float3 invDir = { 1.0f / rayDir.x, 1.0f / rayDir.y, 1.0f / rayDir.z };

	_float3 t1 = _float3(m_tInfo.vMinPos - rayOrigin) * invDir;
	_float3 t2 = _float3(m_tInfo.vMaxPos - rayOrigin) * invDir;

	if (rayDir.x < 0.0f) std::swap(t1.x, t2.x);
	if (rayDir.y < 0.0f) std::swap(t1.y, t2.y);
	if (rayDir.z < 0.0f) std::swap(t1.z, t2.z);

	_float3 tMin = min(t1, t2);
	_float3 tMax = max(t1, t2);

	_float tNear = max(max(tMin.x, tMin.y), tMin.z);
	_float tFar = min(min(tMax.x, tMax.y), tMax.z);

	if (tNear > tFar || tFar < 0)
		return FALSE;

	m_vLast_Collision_Pos = rayOrigin + rayDir * tNear;

	if (tNear == tMin.x) m_vLast_Collision_Depth = { -1.0f, 0.0f, 0.0f };
	else if (tNear == tMax.x) m_vLast_Collision_Depth = { 1.0f, 0.0f, 0.0f };
	else if (tNear == tMin.y) m_vLast_Collision_Depth = { 0.0f, -1.0f, 0.0f };
	else if (tNear == tMax.y) m_vLast_Collision_Depth = { 0.0f, 1.0f, 0.0f };
	else if (tNear == tMin.z) m_vLast_Collision_Depth = { 0.0f, 0.0f, -1.0f };
	else if (tNear == tMax.z) m_vLast_Collision_Depth = { 0.0f, 0.0f, 1.0f };

	return TRUE;
}

_bool CCollider_AABB_Cube::RayCast_Downward(const _float3& rayOrigin)
{
	// 1. XZ 범위 안에 있는지 먼저 확인
	if (rayOrigin.x < m_tInfo.vMinPos.x || rayOrigin.x > m_tInfo.vMaxPos.x)
		return FALSE;
	if (rayOrigin.z < m_tInfo.vMinPos.z || rayOrigin.z > m_tInfo.vMaxPos.z)
		return FALSE;

	// 2. Ray가 AABB의 윗면보다 위에서 시작하는지 확인
	if (rayOrigin.y < m_tInfo.vMaxPos.y)
		return FALSE;

	// 3. 교차 발생: AABB의 "윗면"에 Ray가 닿음
	m_vLast_Collision_Pos = {
		rayOrigin.x,
		m_tInfo.vMaxPos.y,
		rayOrigin.z
	};

	m_vLast_Collision_Depth = { 0.0f, 1.0f, 0.0f }; // 윗면이므로 위 방향

	return TRUE;
}

CCollider_AABB_Cube* CCollider_AABB_Cube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCollider_AABB_Cube* pInstance = new CCollider_AABB_Cube(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCollider_AABB_Cube");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CCollider_AABB_Cube::Clone(void* pArg)
{
	CCollider_AABB_Cube* pInstance = new CCollider_AABB_Cube(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CCollider_AABB_Cube");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollider_AABB_Cube::Free()
{
	__super::Free();
}
