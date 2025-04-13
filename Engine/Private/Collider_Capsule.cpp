#include "Transform.h"
#include "Collider_Capsule.h"
#include "Collider_AABB_Cube.h"
#include "Collider_OBB_Cube.h"
#include "Collider_Sphere.h"
#include "Collider_Rect.h"
#include "Collider_Line.h"

CCollider_Capsule::CCollider_Capsule(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCollider{ pGraphic_Device }
{
}

CCollider_Capsule::CCollider_Capsule(const CCollider_Capsule& Prototype)
	:CCollider(Prototype)
{
}

HRESULT CCollider_Capsule::Initialize_Prototype()
{
	// 타입을 프로토타입 생성시에 확정하고 이후 바꾸지 못하도록 제한	
	return __super::Initialize_Prototype(CAPSULE);
}

void CCollider_Capsule::Update_Collider()
{
	m_tInfo.vCenter = *m_pTransform->Get_State(CTransform::STATE_POSITION) + m_vOffSet;

#ifdef _COLLIDERRENDER
	m_pRenderTransform->Set_State(CTransform::STATE_POSITION, m_tInfo.vCenter);
#endif
}

void CCollider_Capsule::Update_Scale(const _float3& vScale)
{
	m_tInfo.fHeight = vScale.y;
	m_tInfo.fRadius = (vScale.x + vScale.z) * 0.25f;
	m_fMaxLength = m_tInfo.fHeight * 0.5f > m_tInfo.fRadius ?
		m_tInfo.fHeight * 0.5f : m_tInfo.fRadius;

#ifdef _COLLIDERRENDER
	m_pRenderTransform->Set_State(CTransform::STATE_RIGHT, _float3{1.f,0.f,0.f} * m_tInfo.fRadius * 2.f);
	m_pRenderTransform->Set_State(CTransform::STATE_UP, _float3{ 0.f,1.f,0.f } * m_tInfo.fHeight);
	m_pRenderTransform->Set_State(CTransform::STATE_LOOK, _float3{ 0.f,0.f,1.f } * m_tInfo.fRadius * 2.f);
#endif

}

_bool CCollider_Capsule::RayCasting(const _float3& rayOrigin, const _float3& rayDir)
{
	const _float3& center = m_tInfo.vCenter;
	const _float radius = m_tInfo.fRadius;
	const _float halfHeight = m_tInfo.fHeight * 0.5f;

	// 캡슐 세그먼트 (Y축)
	_float3 segA = center + _float3(0.f, halfHeight, 0.f);
	_float3 segB = center - _float3(0.f, halfHeight, 0.f);

	// 레이 방향
	const _float3& d1 = rayDir;                   // ray dir
	_float3 d2 = segB - segA;              // capsule axis
	_float3 r = rayOrigin - segA;

	_float a = d1.Dot(d1);
	_float e = d2.Dot(d2);
	_float f = d2.Dot(r);

	_float c = d1.Dot(r);
	_float b = d1.Dot(d2);

	_float denom = a * e - b * b;

	_float s = 0.f;
	if (fabs(denom) > 1e-6f)
		s = (b * f - c * e) / denom;
	s = max(s, 0.0f); // ray는 음수 방향 불허

	_float t = (b * s + f) / e;
	t = max(0.0f, min(1.0f, t));

	_float3 closestRay = rayOrigin + d1 * s;
	_float3 closestSeg = segA + d2 * t;

	_float3 diff = closestRay - closestSeg;
	_float distSqr = diff.Dot(diff);

	if (distSqr <= radius * radius)
	{
		m_vLast_Collision_Pos = closestRay;
		m_vLast_Collision_Depth = (diff.Length() > 1e-4f) ? diff.Normalize() : _float3(0.f, 1.f, 0.f);
		return TRUE;
	}

	return FALSE;
}

_bool CCollider_Capsule::Intersect_With_AABB_Cube(const CCollider* pOther)
{
	auto pCubeInfo = dynamic_cast<const CCollider_AABB_Cube*>(pOther)->Get_Info();

	// 캡슐의 중심
	_float3 capsuleCenter = m_tInfo.vCenter;
	_float capsuleMinY = capsuleCenter.y - (m_tInfo.fHeight * 0.5f);
	_float capsuleMaxY = capsuleCenter.y + (m_tInfo.fHeight * 0.5f);

	// AABB에서 캡슐의 중심과 가장 가까운 점을 찾기
	_float3 closestPointOnAABB;
	if (capsuleCenter.x < pCubeInfo->vMinPos.x) 
		closestPointOnAABB.x = pCubeInfo->vMinPos.x;
	else if (capsuleCenter.x > pCubeInfo->vMaxPos.x)
		closestPointOnAABB.x = pCubeInfo->vMaxPos.x;
	else 
		closestPointOnAABB.x = capsuleCenter.x;

	if (capsuleCenter.y < pCubeInfo->vMinPos.y) 
		closestPointOnAABB.y = pCubeInfo->vMinPos.y;
	else if (capsuleCenter.y > pCubeInfo->vMaxPos.y) 
		closestPointOnAABB.y = pCubeInfo->vMaxPos.y;
	else 
		closestPointOnAABB.y = capsuleCenter.y;

	if (capsuleCenter.z < pCubeInfo->vMinPos.z) 
		closestPointOnAABB.z = pCubeInfo->vMinPos.z;
	else if (capsuleCenter.z > pCubeInfo->vMaxPos.z) 
		closestPointOnAABB.z = pCubeInfo->vMaxPos.z;
	else 
		closestPointOnAABB.z = capsuleCenter.z;

	// 캡슐의 중심과 AABB의 가까운 점 사이의 거리가 반지름 보다 작으면 충돌
	_float3 diff = capsuleCenter - closestPointOnAABB;
	_float distanceSquared = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;

	if (distanceSquared >= SQUARE(m_tInfo.fRadius))
		return FALSE;

	// Y축 범위 체크
	// 캡슐의 천장이 AABB의 바닥보다 높거나,
	// 캡슐의 바닥이 AABB의 천장보다 낮으면 충돌
	_bool bCollision = !(capsuleMaxY < pCubeInfo->vMinPos.y || capsuleMinY > pCubeInfo->vMaxPos.y);

	if (bCollision)
	{
		// 충돌 깊이 계산
		_float3 moveDirection = { 0.0f, 0.0f, 0.0f };
		_float minPush = FLT_MAX;

		// X축 깊이 계산
		_float depthX = (capsuleCenter.x < pCubeInfo->vCenter.x) ?
			(pCubeInfo->vMinPos.x - (capsuleCenter.x + m_tInfo.fRadius)) :
			(pCubeInfo->vMaxPos.x - (capsuleCenter.x - m_tInfo.fRadius));
		if (fabs(depthX) < minPush)
		{
			minPush = fabs(depthX);
			moveDirection = { depthX, 0.0f, 0.0f };
		}

		//// Y축 깊이 계산
		// Y축은 계산 안함
		//_float depthY = (capsuleCenter.y < pCubeInfo->vCenter.y) ?
		//	(pCubeInfo->vMinPos.y - capsuleMaxY) :
		//	(pCubeInfo->vMaxPos.y - capsuleMinY);
		//if (fabs(depthY) < minPush)
		//{
		//	minPush = fabs(depthY);
		//	moveDirection = { 0.0f, depthY, 0.0f };
		//}

		// Z축 깊이 계산
		_float depthZ = (capsuleCenter.z < pCubeInfo->vCenter.z) ?
			(pCubeInfo->vMinPos.z - (capsuleCenter.z + m_tInfo.fRadius)) :
			(pCubeInfo->vMaxPos.z - (capsuleCenter.z - m_tInfo.fRadius));
		if (fabs(depthZ) < minPush)
		{
			moveDirection = { 0.0f, 0.0f, depthZ };
		}

		// 충돌 해결을 위한 이동값 설정
		m_vLast_Collision_Depth = moveDirection;
		m_vLast_Collision_Pos = m_tInfo.vCenter + moveDirection * 0.5f;
	}

	return bCollision;
}

_bool CCollider_Capsule::Intersect_With_OBB_Cube(const CCollider* pOther)
{
	const auto& pOBBInfo = dynamic_cast<const CCollider_OBB_Cube*>(pOther)->Get_Info();

	const _float3& obbCenter = pOBBInfo->vPosition;
	const _float3* obbAxes = pOBBInfo->vAxis;
	const _float3& obbHalfSize = pOBBInfo->vHalfScale;

	// 캡슐 중심 → OBB 로컬 좌표로 투영
	_float3 capsuleToOBB = m_tInfo.vCenter - obbCenter;
	_float3 closestPointOnOBB = obbCenter;

	for (int i = 0; i < 3; i++)
	{
		float distance = capsuleToOBB.Dot(obbAxes[i]);
		distance = max(-obbHalfSize[i], min(distance, obbHalfSize[i]));
		closestPointOnOBB += obbAxes[i] * distance;
	}

	// OBB 표면에서 캡슐 중심까지 거리
	_float3 diff = m_tInfo.vCenter - closestPointOnOBB;
	_float distSq = diff.Dot(diff);

	if (distSq >= SQUARE(m_tInfo.fRadius))
		return FALSE;

	float dist = sqrtf(distSq);
	float depth = m_tInfo.fRadius - dist;
	if (depth <= 0.f)
		return FALSE;

	_float3 normal = (dist > 0.f) ? diff / dist : _float3(1.f, 0.f, 0.f); // 정규화

	// Y축 양방향 밀림 방지: Y가 양수면 제거하고 정규화
	if (normal.y > 0.f)
		normal.y = 0.f;
	normal.Normalize();

	m_vLast_Collision_Depth = normal * depth;
	m_vLast_Collision_Pos = closestPointOnOBB + normal * m_tInfo.fRadius;

	return TRUE;
}

_bool CCollider_Capsule::Intersect_With_Sphere(const CCollider* pOther)
{
	auto pSphereInfo = dynamic_cast<const CCollider_Sphere*>(pOther)->Get_Info();

	// XZ 평면에서 최근접 점 찾기 (2D 거리 계산)
	_float2 capsuleXZ(m_tInfo.vCenter.x, m_tInfo.vCenter.z);
	_float2 sphereXZ(pSphereInfo->vPosition.x, pSphereInfo->vPosition.z);

	_float2 diffXZ = capsuleXZ - sphereXZ;
	_float distanceSquaredXZ = D3DXVec2LengthSq(&diffXZ);

	// 캡슐 반지름 + 구 반지름 범위 체크 (XZ 평면 충돌 여부)
	_float fRadiusSum = m_tInfo.fRadius + pSphereInfo->fRadius;
	if (distanceSquaredXZ > SQUARE(fRadiusSum))
		return FALSE; // XZ 평면에서 이미 충돌 안 함

	// Y축 범위에서 최근접 점 찾기 (클램핑)
	_float capsuleMinY = m_tInfo.vCenter.y - m_tInfo.fHeight * 0.5f;
	_float capsuleMaxY = m_tInfo.vCenter.y + m_tInfo.fHeight * 0.5f;

	_float fClosestY = max(capsuleMinY, min(m_tInfo.vCenter.y, capsuleMaxY));

	// 최근접 점과 구 중심 간의 3D 거리 계산
	_float3 vClosestPoint(m_tInfo.vCenter.x, fClosestY, m_tInfo.vCenter.z);
	_float3 vDiff = vClosestPoint - m_tInfo.vCenter;
	float distanceSquared = D3DXVec3LengthSq(&vDiff);

	_bool bCollision = distanceSquared <= SQUARE(fRadiusSum);

	if (bCollision)
	{
		m_vLast_Collision_Depth = pSphereInfo->vPosition - m_tInfo.vCenter;
	}

	// 거리 비교하여 충돌 여부 결정
	return bCollision;
}

_bool CCollider_Capsule::Intersect_With_Capsule(const CCollider* pOther)
{
	const auto& pOtherCapsuleInfo = *static_cast<const CCollider_Capsule*>(pOther)->Get_Info();

	const _float3& vCenterA = m_tInfo.vCenter;
	const _float3& vCenterB = pOtherCapsuleInfo.vCenter;
	const _float fHalfHeightA = m_tInfo.fHeight * 0.5f;
	const _float fHalfHeightB = pOtherCapsuleInfo.fHeight * 0.5f;
	const _float fRadiusA = m_tInfo.fRadius;
	const _float fRadiusB = pOtherCapsuleInfo.fRadius;

	// XZ 평면에서의 거리 계산 (Y축은 무시)
	_float3 vDiffXZ = _float3(vCenterA.x - vCenterB.x, 0, vCenterA.z - vCenterB.z);
	_float fDistanceSquaredXZ = vDiffXZ.Dot(vDiffXZ);
	_float fCombinedRadius = fRadiusA + fRadiusB;
	_float fCombinedRadiusSquared = fCombinedRadius * fCombinedRadius;

	if (fDistanceSquaredXZ >= fCombinedRadiusSquared)
		return FALSE; // 반지름 합보다 멀면 충돌 없음

	// Y축 범위 확인 (두 캡슐이 수직으로 겹치는지 체크)
	_float fMinA = vCenterA.y - fHalfHeightA;
	_float fMaxA = vCenterA.y + fHalfHeightA;
	_float fMinB = vCenterB.y - fHalfHeightB;
	_float fMaxB = vCenterB.y + fHalfHeightB;

	if (fMaxA < fMinB || fMaxB < fMinA)
		return FALSE; // Y축에서 겹치지 않으면 충돌 없음

	// 충돌 좌표: XZ 평면에서 겹치는 부분의 중점 + Y축 중간값
	_float fMidY = (max(fMinA, fMinB) + min(fMaxA, fMaxB)) * 0.5f;
	m_vLast_Collision_Pos = _float3((vCenterA.x + vCenterB.x) * 0.5f, fMidY, (vCenterA.z + vCenterB.z) * 0.5f);

	// 충돌 깊이 벡터 (XZ 평면에서만 이동)
	_float fDistanceXZ = sqrt(fDistanceSquaredXZ);
	_float fPenetrationDepth = fCombinedRadius - fDistanceXZ;
	_float3 vCollisionNormalXZ = (fDistanceXZ > 1e-6f) ? (vDiffXZ * (1.0f / fDistanceXZ)) : _float3(1, 0, 0);

	m_vLast_Collision_Depth = vCollisionNormalXZ * fPenetrationDepth;

	return TRUE;
}

_bool CCollider_Capsule::Intersect_With_Rect(const CCollider* pOther)
{
	const auto& pRectInfo = *static_cast<const CCollider_Rect*>(pOther)->Get_Info();

	const _float3& vCapsuleCenter = m_tInfo.vCenter;
	const _float fCapsuleRadius = m_tInfo.fRadius;
	const _float fCapsuleHalfHeight = m_tInfo.fHeight * 0.5f;

	const _float3& vRectCenter = pRectInfo.vCenter;
	const _float3& vRectNormal = pRectInfo.GetNormal();
	const _float3& vRectRight = pRectInfo.vRight;
	const _float3& vRectUp = pRectInfo.vUp;
	const _float fRectHalfWidth = pRectInfo.fWidth * 0.5f;
	const _float fRectHalfHeight = pRectInfo.fHeight * 0.5f;

	// 캡슐의 중심선 상단과 하단 좌표
	_float3 vCapsuleTop = vCapsuleCenter + _float3(0, fCapsuleHalfHeight, 0);
	_float3 vCapsuleBottom = vCapsuleCenter - _float3(0, fCapsuleHalfHeight, 0);

	// 평면의 중심을 기준으로 캡슐의 중심을 평면으로 투영
	_float3 vToCenter = vCapsuleCenter - vRectCenter;
	_float fDistToPlane = vToCenter.Dot(vRectNormal);
	_float3 vProjectedCenter = vCapsuleCenter - fDistToPlane * vRectNormal;

	// 투영된 점을 Rect의 내부로 클램핑
	_float fProjRight = vProjectedCenter.Dot(vRectRight);
	_float fProjUp = vProjectedCenter.Dot(vRectUp);

	fProjRight = max(-fRectHalfWidth, min(fRectHalfWidth, fProjRight));
	fProjUp = max(-fRectHalfHeight, min(fRectHalfHeight, fProjUp));

	_float3 vClosestPoint = vRectCenter + fProjRight * vRectRight + fProjUp * vRectUp;

	// 평면까지의 거리 계산
	_float3 vDiff = vCapsuleCenter - vClosestPoint;
	_float fDistance = vDiff.Dot(vRectNormal);
	_float fPenetrationDepth = fCapsuleRadius - abs(fDistance);

	if (fPenetrationDepth <= 0) // 충돌 없음
		return FALSE;

	// 충돌 좌표 (겹쳐진 평면의 중점)
	m_vLast_Collision_Pos = vClosestPoint;

	// 충돌 깊이 방향 벡터 (Rect의 면으로 밀어내는 방향)
	_float3 vCollisionNormal = (fDistance > 0) ? vRectNormal : -vRectNormal;
	m_vLast_Collision_Depth = vCollisionNormal * fPenetrationDepth;

	return TRUE;
}

_bool CCollider_Capsule::Intersect_With_Line(const CCollider* pOther)
{
	const auto& pLineInfo = *static_cast<const CCollider_Line*>(pOther)->Get_Info();

	const _float3& vCapsuleCenter = m_tInfo.vCenter;
	const _float fCapsuleRadius = m_tInfo.fRadius;
	const _float fCapsuleHalfHeight = m_tInfo.fHeight * 0.5f;

	const _float3& vLineStart = pLineInfo.vStart;
	const _float3& vLineEnd = pLineInfo.vEnd;

	// 캡슐의 중심선 상단과 하단 좌표
	_float3 vCapsuleTop = vCapsuleCenter + _float3(0, fCapsuleHalfHeight, 0);
	_float3 vCapsuleBottom = vCapsuleCenter - _float3(0, fCapsuleHalfHeight, 0);

	// 선분과 캡슐의 중심선의 최근접점 찾기
	_float3 vClosestCapsule, vClosestLine;
	_float3 vLineDir = vLineEnd - vLineStart;
	_float3 vCapsuleDir = vCapsuleTop - vCapsuleBottom;
	_float3 vStartToCapsule = vLineStart - vCapsuleBottom;

	_float fA = vLineDir.Dot(vLineDir);
	_float fB = vLineDir.Dot(vCapsuleDir);
	_float fC = vCapsuleDir.Dot(vCapsuleDir);
	_float fD = vLineDir.Dot(vStartToCapsule);
	_float fE = vCapsuleDir.Dot(vStartToCapsule);

	_float fDenom = fA * fC - fB * fB;
	_float fS, fT;

	if (abs(fDenom) > 1e-6f) 
	{
		fS = (fB * fE - fC * fD) / fDenom;
		fT = (fA * fE - fB * fD) / fDenom;
	}
	else 
	{
		fS = 0.0f;
		fT = fE / fC;
	}

	fS = max(0.0f, min(1.0f, fS));
	fT = max(0.0f, min(1.0f, fT));

	vClosestLine = vLineStart + fS * vLineDir;
	vClosestCapsule = vCapsuleBottom + fT * vCapsuleDir;

	// 최근접점 간 거리 계산
	_float3 vDiff = vClosestCapsule - vClosestLine;
	_float fDistance = sqrt(vDiff.Dot(vDiff));

	if (fDistance >= fCapsuleRadius) // 충돌 없음
		return FALSE;

	// 충돌 좌표: 캡슐의 표면과 선분의 교차점
	_float3 vCollisionNormal = vDiff * (1.0f / fDistance);
	m_vLast_Collision_Pos = vClosestCapsule - vCollisionNormal * fCapsuleRadius;

	// 충돌 깊이 방향 벡터: 캡슐을 선분의 `end -> start` 방향으로 밀어냄
	_float fPenetrationDepth = fCapsuleRadius - fDistance;
	_float3 vLineMoveDir = vLineStart - vLineEnd;

	m_vLast_Collision_Depth = vLineMoveDir.Normalize() * fPenetrationDepth;

	return TRUE;
}

CCollider_Capsule* CCollider_Capsule::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCollider_Capsule* pInstance = new CCollider_Capsule(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCollider_Capsule");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CCollider_Capsule::Clone(void* pArg)
{
	CCollider_Capsule* pInstance = new CCollider_Capsule(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CCollider_Capsule");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollider_Capsule::Free()
{
	__super::Free();
}
