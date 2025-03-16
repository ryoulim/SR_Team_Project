#include "Collider_Capsule.h"
#include "Collider_AABB_Cube.h"
#include "Collider_OBB_Cube.h"
#include "Collider_Sphere.h"

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

HRESULT CCollider_Capsule::Initialize(void* pArg)
{
	if (pArg == nullptr)
	{
		MSG_BOX("콜라이더 pArg에 nullptr을 넣어주면 어떡해...");
		return E_FAIL;
	}

	DESC* pDesc = static_cast<DESC*>(pArg);
	_float3 vScale = pDesc->pTransform->Compute_Scaled();
	m_tInfo.fHeight = vScale.y;
	m_tInfo.fRadius = (vScale.x + vScale.z) * 0.25f;
	Update_Collider(pDesc->pTransform);
	return S_OK;
}

void CCollider_Capsule::Update_Collider(const CTransform* pTransform)
{
	m_tInfo.vCenter = *pTransform->Get_State(CTransform::STATE_POSITION);
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

		// Y축 깊이 계산
		_float depthY = (capsuleCenter.y < pCubeInfo->vCenter.y) ?
			(pCubeInfo->vMinPos.y - capsuleMaxY) :
			(pCubeInfo->vMaxPos.y - capsuleMinY);
		if (fabs(depthY) < minPush)
		{
			minPush = fabs(depthY);
			moveDirection = { 0.0f, depthY, 0.0f };
		}

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
	auto pOBBInfo = dynamic_cast<const CCollider_OBB_Cube*>(pOther)->Get_Info();
	
	_float3 obbCenter = pOBBInfo->vPosition;
	const _float3* obbAxes = pOBBInfo->vAxis;
	_float3 obbHalfSize = pOBBInfo->vHalfScale;

	
	// 캡슐 중심을 OBB 로컬 좌표계로 변환
	_float3 capsuleLocalPos = m_tInfo.vCenter - obbCenter;
	_float3 closestPointOnOBB = obbCenter;

	for (int i = 0; i < 3; i++)
	{
		float distance = capsuleLocalPos.Dot(obbAxes[i]);
		distance = max(-obbHalfSize[i], min(distance, obbHalfSize[i]));
		closestPointOnOBB += obbAxes[i] * distance;
	}

	_float3 diff = m_tInfo.vCenter - closestPointOnOBB;
	_float distanceSquared = diff.Dot(diff);

	if (distanceSquared >= SQUARE(m_tInfo.fRadius))
		return FALSE;

	_float3 moveDirection = { 0.0f, 0.0f, 0.0f };
	_float minPush = FLT_MAX;

	for (int i = 0; i < 3; i++)
	{
		float push = diff.Dot(obbAxes[i]); // 캡슐과 OBB 축 방향 거리 계산

		// OBB 면과 캡슐의 표면 간의 거리 계산
		float penetration = obbHalfSize[i] - fabs(push);

		// 캡슐 반지름을 OBB의 면과 직접 비교하여 정확한 깊이 계산
		float depth = penetration - m_tInfo.fRadius;

		// 🔹 **캡슐 표면이 이미 OBB 면과 접촉 중이면 이동하지 않음**
		if (depth < 0) continue;

		// 🔹 **너무 작은 깊이(거의 0)는 이동 필요 없음**
		if (depth < 0.001f) depth = 0.0f;

		if (depth < minPush)
		{
			minPush = depth;
			moveDirection = obbAxes[i] * ((push < 0) ? -1.0f : 1.0f) * depth;
		}
	}

	// 충돌 해결을 위한 이동값 설정
	m_vLast_Collision_Depth = moveDirection;
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

	// 거리 비교하여 충돌 여부 결정
	return distanceSquared <= SQUARE(fRadiusSum);
}

_bool CCollider_Capsule::Intersect_With_Capsule(const CCollider* pOther)
{
	auto pOtherInfo = dynamic_cast<const CCollider_Capsule*>(pOther)->Get_Info();

	_float3 vDistance = m_tInfo.vCenter - pOtherInfo->vCenter;
	_float fLengthSquared = SQUARE(vDistance.x) + SQUARE(vDistance.z);
	_float fRadiusSum = m_tInfo.fRadius + pOtherInfo->fRadius;


	// Y축 범위 확인 (두 캡슐이 수직으로 겹치는지 체크)
	_float fMin1 = m_tInfo.vCenter.y - m_tInfo.fHeight * 0.5f;
	_float fMax1 = m_tInfo.vCenter.y + m_tInfo.fHeight * 0.5f;
	_float fMin2 = pOtherInfo->vCenter.y - pOtherInfo->fHeight * 0.5f;
	_float fMax2 = pOtherInfo->vCenter.y + pOtherInfo->fHeight * 0.5f;

	_bool bOverlap = !(fMax1 < fMin2 || fMax2 < fMin1);

	return fLengthSquared <= SQUARE(fRadiusSum) && bOverlap;
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
