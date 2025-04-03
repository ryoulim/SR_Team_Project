#include "Transform.h"
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
	m_fMaxLength = m_vHalfScale.Length();
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

	if (rayDir.x < 0.0f) swap(t1.x, t2.x);
	if (rayDir.y < 0.0f) swap(t1.y, t2.y);
	if (rayDir.z < 0.0f) swap(t1.z, t2.z);

	_float3 tMin = min(t1, t2);
	_float3 tMax = max(t1, t2);

	_float tNear = max(max(tMin.x, tMin.y), tMin.z);
	_float tFar = min(min(tMax.x, tMax.y), tMax.z);

	if (tNear > tFar || tFar < 0)
		return FALSE;

	m_vLast_Collision_Pos = rayOrigin + rayDir * tNear;
	
	if (abs(tNear - t1.x) < FLT_EPSILON)
		m_vLast_Collision_Depth = { rayDir.x < 0.f ? 1.f : -1.f, 0.f, 0.f };
	else if (abs(tNear - t1.y) < FLT_EPSILON)
		m_vLast_Collision_Depth = { 0.f, rayDir.y < 0.f ? 1.f : -1.f, 0.f };
	else if (abs(tNear - t1.z) < FLT_EPSILON)
		m_vLast_Collision_Depth = { 0.f, 0.f, rayDir.z < 0.f ? 1.f : -1.f };

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

void CCollider_AABB_Cube::Update_Rotation(const _float3& radEuler)
{
	//_float3 half = m_vHalfScale;

	//const _float RAD90 = PI * 0.5f;
	//const _float RAD180 = PI;
	//const _float RAD270 = PI * 1.5f;

	//const _float epsilon = FLT_EPSILON * 10.f;

	//// X축 회전: Y-Z 스왑
	//if (fabs(radEuler.x - RAD90) < epsilon || fabsf(radEuler.x - RAD270) < epsilon)
	//	swap(half.y, half.z);

	//// Y축 회전: X-Z 스왑
	//if (fabs(radEuler.y - RAD90) < epsilon || fabsf(radEuler.y - RAD270) < epsilon)
	//	swap(half.x, half.z);

	//// Z축 회전: X-Y 스왑
	//if (fabs(radEuler.z - RAD90) < epsilon || fabsf(radEuler.z - RAD270) < epsilon)
	//	swap(half.x, half.y);

	//m_tInfo.vMinPos = m_tInfo.vCenter - half;
	//m_tInfo.vMaxPos = m_tInfo.vCenter + half;

	D3DXMATRIX rot;
	D3DXMatrixRotationYawPitchRoll(&rot, radEuler.y, radEuler.x, radEuler.z);

	_float3 center = m_tInfo.vCenter;
	_float3 half = m_vHalfScale;

	// 8개 꼭짓점 오프셋
	_float3 corners[8] = {
		{ -half.x, -half.y, -half.z },
		{  half.x, -half.y, -half.z },
		{ -half.x,  half.y, -half.z },
		{  half.x,  half.y, -half.z },
		{ -half.x, -half.y,  half.z },
		{  half.x, -half.y,  half.z },
		{ -half.x,  half.y,  half.z },
		{  half.x,  half.y,  half.z },
	};

	_float3 min = { FLT_MAX, FLT_MAX, FLT_MAX };
	_float3 max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };

	for (int i = 0; i < 8; ++i)
	{
		D3DXVECTOR3 rotated;
		D3DXVec3TransformCoord(&rotated, &corners[i], &rot);

		D3DXVECTOR3 world = rotated + center;

		min.x = min(min.x, world.x);
		min.y = min(min.y, world.y);
		min.z = min(min.z, world.z);

		max.x = max(max.x, world.x);
		max.y = max(max.y, world.y);
		max.z = max(max.z, world.z);
	}

	m_tInfo.vMinPos = min;
	m_tInfo.vMaxPos = max;
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
