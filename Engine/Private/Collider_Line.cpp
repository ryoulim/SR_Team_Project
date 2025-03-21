#include "Collider_Line.h"
#include "Collider_Capsule.h"
#include "Collider_Sphere.h"
#include "Collider_AABB_Cube.h"
#include "Collider_OBB_Cube.h"
#include "Collider_Rect.h"

CCollider_Line::CCollider_Line(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CCollider {pGraphic_Device}
{
}

CCollider_Line::CCollider_Line(const CCollider_Line& Prototype)
	:CCollider (Prototype)
{
}

HRESULT CCollider_Line::Initialize_Prototype()
{
	return __super::Initialize_Prototype(LINE);
}

void CCollider_Line::Update_Collider()
{
    const _float3 vCenter = *m_pTransform->Get_State(CTransform::STATE_POSITION) + m_vOffSet;
    m_tInfo.vStart = vCenter - m_vDiffFromCenter;
    m_tInfo.vEnd = vCenter + m_vDiffFromCenter;
}

void CCollider_Line::Update_Scale(const _float3& vScale)
{
    m_vDiffFromCenter = m_pTransform->Get_State(CTransform::STATE_LOOK)->Normalize() * (vScale.x * 0.5f);
}

_bool CCollider_Line::Intersect_With_AABB_Cube(const CCollider* pOther)
{
    const auto& aabbInfo = static_cast<const CCollider_AABB_Cube*>(pOther)->Get_Info();

    const _float3& minB = aabbInfo->vMinPos;
    const _float3& maxB = aabbInfo->vMaxPos;

    _float3 d = m_tInfo.vEnd - m_tInfo.vStart; // 선분 방향 벡터

    _float tMin = 0.0f;
    _float tMax = 1.0f;

    int hitAxis = -1;
    _bool hitNegative = FALSE;

    // Liang-Barsky 알고리즘: 각 축에 대해 교차 시점 계산
    for (int i = 0; i < 3; i++)
    {
        _float start = (&m_tInfo.vStart.x)[i];
        _float dir = (&d.x)[i];
        _float boxMin = (&minB.x)[i];
        _float boxMax = (&maxB.x)[i];

        if (fabsf(dir) < 1e-6f)
        {
            // 축 방향으로 평행하면서 박스 바깥에 있다면 교차하지 않음
            if (start < boxMin || start > boxMax)
                return FALSE;
        }
        else
        {
            _float t1 = (boxMin - start) / dir;
            _float t2 = (boxMax - start) / dir;

            if (t1 > t2) std::swap(t1, t2);

            if (t1 > tMin)
            {
                tMin = t1;
                hitAxis = i;
                hitNegative = (dir > 0.f); // 진입 방향이 -면인지
            }

            tMax = min(tMax, t2);

            if (tMin > tMax)
                return FALSE; // 교차 없음
        }
    }

    // 교차 있음: AABB의 실제 면과 선분의 교차 좌표 계산
    _float facePos = hitNegative ? (&minB.x)[hitAxis] : (&maxB.x)[hitAxis];
    _float dirComponent = (&d.x)[hitAxis];
    _float startComponent = (&m_tInfo.vStart.x)[hitAxis];
    _float t = (facePos - startComponent) / dirComponent;

    _float3 hitPos = m_tInfo.vStart + d * t;

    // 충돌 좌표 저장
    m_vLast_Collision_Pos = hitPos;

    // 충돌 깊이 벡터 = 출구점 - 진입점
    _float3 exitPoint = m_tInfo.vStart + d * tMax;
    m_vLast_Collision_Depth = exitPoint - hitPos;

    return TRUE;
}

_bool CCollider_Line::Intersect_With_OBB_Cube(const CCollider* pOther)
{
    const auto& pOBBInfo = *static_cast<const CCollider_OBB_Cube*>(pOther)->Get_Info();

    // OBB 정보 주소 참조 사용
    const _float3& vOBBCenter = pOBBInfo.vPosition;
    const _float3& vOBBAxisX = pOBBInfo.vAxis[0]; // X축 방향 벡터
    const _float3& vOBBAxisY = pOBBInfo.vAxis[1]; // Y축 방향 벡터
    const _float3& vOBBAxisZ = pOBBInfo.vAxis[2]; // Z축 방향 벡터
    const _float3& vOBBHalfScale = pOBBInfo.vHalfScale; // OBB 절반 크기

    // 선분을 OBB의 로컬 공간으로 변환
    _float3 vStartLocal = m_tInfo.vStart - vOBBCenter;
    _float3 vEndLocal = m_tInfo.vEnd - vOBBCenter;

    vStartLocal = { vStartLocal.Dot(vOBBAxisX), vStartLocal.Dot(vOBBAxisY), vStartLocal.Dot(vOBBAxisZ) };
    vEndLocal = { vEndLocal.Dot(vOBBAxisX), vEndLocal.Dot(vOBBAxisY), vEndLocal.Dot(vOBBAxisZ) };

    _float3 vLineLocalDir = vEndLocal - vStartLocal; // 로컬 공간에서 선분 방향 벡터

    _float fTMin = 0.0f, fTMax = 1.0f;
    for (int i = 0; i < 3; i++) {
        _float fMinB = -(&vOBBHalfScale.x)[i]; // OBB 최소 좌표 (로컬)
        _float fMaxB = (&vOBBHalfScale.x)[i];  // OBB 최대 좌표 (로컬)
        _float fStartP = (&vStartLocal.x)[i]; // 선분 시작점 좌표 (로컬)
        _float fDir = (&vLineLocalDir.x)[i];  // 선분 방향 (로컬)

        if (abs(fDir) < 1e-6f) { // 평행한 경우
            if (fStartP < fMinB || fStartP > fMaxB)
                return FALSE; // 충돌 없음
        }
        else {
            _float fT1 = (fMinB - fStartP) / fDir;
            _float fT2 = (fMaxB - fStartP) / fDir;

            if (fT1 > fT2)
                swap(fT1, fT2);
            fTMin = max(fTMin, fT1);
            fTMax = min(fTMax, fT2);

            if (fTMin > fTMax)
                return FALSE; // 충돌 없음
        }
    }

    // 충돌한 경우
    _float3 vIntersectionLocal = vStartLocal + fTMin * vLineLocalDir; // 충돌 좌표 (로컬 공간)

    // 로컬 → 월드 변환 (충돌 좌표)
    m_vLast_Collision_Pos =
        vOBBCenter +
        vIntersectionLocal.x * vOBBAxisX +
        vIntersectionLocal.y * vOBBAxisY +
        vIntersectionLocal.z * vOBBAxisZ;

    // 충돌 깊이 방향 벡터 계산 (진입점 -> 출구점 벡터)
    _float3 vExitLocal = vStartLocal + fTMax * vLineLocalDir;
    _float3 vSeparationLocal = vExitLocal - vIntersectionLocal;

    // 로컬 → 월드 변환 (충돌 깊이 벡터)
    m_vLast_Collision_Depth =
        vSeparationLocal.x * vOBBAxisX +
        vSeparationLocal.y * vOBBAxisY +
        vSeparationLocal.z * vOBBAxisZ;

    return TRUE;
}

_bool CCollider_Line::Intersect_With_Sphere(const CCollider* pOther)
{
    auto pSphereInfo = static_cast<const CCollider_Sphere*>(pOther)->Get_Info();

    _float3 d = m_tInfo.vEnd - m_tInfo.vStart; // 선분 방향 벡터

    // 선분과 구의 최근접점 찾기
    _float3 startToCenter = pSphereInfo->vPosition - m_tInfo.vStart;
    _float proj = startToCenter.Dot(d) / d.Dot(d);

    // 최근접점 계산 (t는 0~1 범위로 클램핑)
    _float t = max(0.0f, min(1.0f, proj));
    _float3 closestPoint = m_tInfo.vStart + t * d;

    // 최근접점과 구의 중심 거리 계산
    _float3 diff = closestPoint - pSphereInfo->vPosition;
    _float distance = diff.Length();

    if (distance > pSphereInfo->fRadius)
        return FALSE; // 충돌 없음

    // 충돌 좌표 저장
    m_vLast_Collision_Pos = closestPoint;

    // 충돌 깊이 방향 벡터 계산 (구 표면에서 최근접점까지의 방향)
    m_vLast_Collision_Depth = diff.Normalize() * (pSphereInfo->fRadius - distance);

    return TRUE;
}

_bool CCollider_Line::Intersect_With_Capsule(const CCollider* pOther)
{
    auto pCapsuleInfo = static_cast<const CCollider_Capsule*>(pOther)->Get_Info();

    // 캡슐의 중심선 두 끝점
    _float3 vCapsuleTop = pCapsuleInfo->vCenter;
    vCapsuleTop.y += pCapsuleInfo->fHeight * 0.5f;
    _float3 vCapsuleBottom = pCapsuleInfo->vCenter;
    vCapsuleBottom.y -= pCapsuleInfo->fHeight * 0.5f;

    // 직선과 캡슐의 중심선의 최근접점 찾기
    _float3 closestP, closestQ;
    _float3 d1 = m_tInfo.vEnd - m_tInfo.vStart;
    _float3 d2 = vCapsuleTop - vCapsuleBottom;
    _float3 r = m_tInfo.vStart - vCapsuleBottom;
    
    float a = d1.Dot(d1);
    float e = d2.Dot(d2);
    float f = d2.Dot(r);

    float s, t;
    if (a <= 1e-6f && e <= 1e-6f) {
        s = t = 0.0f;
    }
    else if (a <= 1e-6f) {
        s = 0.0f;
        t = f / e;
    }
    else {
        float c = d1.Dot(r);
        if (e <= 1e-6f) {
            t = 0.0f;
            s = -c / a;
        }
        else {
            float b = d1.Dot(d2);
            float denom = a * e - b * b;
            if (denom != 0.0f)
                s = (b * f - c * e) / denom;
            else
                s = 0.0f;
            t = (b * s + f) / e;
        }
    }

    s = max(0.0f, min(1.0f, s));
    t = max(0.0f, min(1.0f, t));

    closestP = m_tInfo.vStart + s * d1;
    closestQ = vCapsuleBottom + t * d2;

    // 최근접점 간 벡터 및 거리 계산
    _float3 diff = closestP - closestQ;
    _float distance = diff.Length();

    // 충돌 여부 확인
    _bool bCollision = distance <= pCapsuleInfo->fRadius;

    if (bCollision) {
        // 충돌 해소 방향 벡터 (선분을 이동해야 하는 벡터)
        _float3 collisionNormal = diff;
        collisionNormal.Normalize();
        m_vLast_Collision_Depth = collisionNormal * (pCapsuleInfo->fRadius - distance);
        // 캡슐 표면과 선분이 교차하는 지점
        m_vLast_Collision_Pos = closestQ + collisionNormal * pCapsuleInfo->fRadius;
    }

    return bCollision;
}

_bool CCollider_Line::Intersect_With_Rect(const CCollider* pOther)
{
    auto pRectInfo = static_cast<const CCollider_Rect*>(pOther)->Get_Info();

    _float3 vLineDir = m_tInfo.vEnd - m_tInfo.vStart; // 선분 방향 벡터
    _float3 vRectNormal = pRectInfo->GetNormal();  // 직사각형 법선 벡터
    _float3 vStartToCenter = m_tInfo.vStart - pRectInfo->vCenter; // 선분 시작점 → 직사각형 중심 벡터

    // 평면과 선분의 교차 검사 (N · D)
    _float fDotNormalDir = vRectNormal.Dot(vLineDir);
    _float fDotNormalStart = vRectNormal.Dot(vStartToCenter);

    if (abs(fDotNormalDir) < 1e-6f) // 선분이 평면과 평행하면 충돌 없음
        return FALSE;

    _float fIntersectionTime = -fDotNormalStart / fDotNormalDir; // 선분이 평면과 교차하는 t 값
    if (fIntersectionTime < 0.0f || fIntersectionTime > 1.0f) // 선분의 범위를 벗어나면 충돌 없음
        return FALSE;

    // 교차점 계산
    _float3 vIntersectionPoint = m_tInfo.vStart + fIntersectionTime * vLineDir;

    // 교차점이 Rect 내부에 있는지 확인 (직사각형의 좌표계를 사용)
    _float3 vLocalIntersection = vIntersectionPoint - pRectInfo->vCenter;
    _float fProjOnRight = vLocalIntersection.Dot(pRectInfo->vRight);
    _float fProjOnUp = vLocalIntersection.Dot(pRectInfo->vUp);

    if (abs(fProjOnRight) > pRectInfo->fWidth * 0.5f || abs(fProjOnUp) > pRectInfo->fHeight * 0.5f)
        return FALSE; // 직사각형 바깥이면 충돌 없음

    // 충돌 좌표 저장
    m_vLast_Collision_Pos = vIntersectionPoint;

    // 충돌 깊이 방향 벡터 계산 (법선 방향으로 이동해야 하는 벡터)
    m_vLast_Collision_Depth = vRectNormal * (-fDotNormalStart / fDotNormalDir);

    return TRUE;
}

_bool CCollider_Line::Intersect_With_Line(const CCollider* pOther)
{
    const auto& pOtherLineInfo = *static_cast<const CCollider_Line*>(pOther)->Get_Info();

    // 현재 선분 정보
    const _float3& vStartA = m_tInfo.vStart;
    const _float3& vEndA = m_tInfo.vEnd;
    _float3 vDirA = vEndA - vStartA;

    // 다른 선분 정보
    const _float3& vStartB = pOtherLineInfo.vStart;
    const _float3& vEndB = pOtherLineInfo.vEnd;
    _float3 vDirB = vEndB - vStartB;

    _float3 vDiff = vStartA - vStartB;
    _float fA = vDirA.Dot(vDirA);
    _float fB = vDirA.Dot(vDirB);
    _float fC = vDirB.Dot(vDirB);
    _float fD = vDirA.Dot(vDiff);
    _float fE = vDirB.Dot(vDiff);

    _float fDenom = fA * fC - fB * fB; // 분모

    _float fS, fT;
    if (abs(fDenom) > 1e-6f) // 두 선분이 평행하지 않은 경우
    {
        fS = (fB * fE - fC * fD) / fDenom;
        fT = (fA * fE - fB * fD) / fDenom;
    }
    else // 두 선분이 평행한 경우
    {
        fS = 0.0f;
        fT = fE / fC;
    }

    // 0~1 사이로 클램핑하여 선분의 유효 범위 내에서 확인
    fS = max(0.0f, min(1.0f, fS));
    fT = max(0.0f, min(1.0f, fT));

    // 최근접점 계산
    _float3 vClosestA = vStartA + fS * vDirA;
    _float3 vClosestB = vStartB + fT * vDirB;

    // 두 최근접점 사이 거리 계산
    _float3 vDiffClosest = vClosestA - vClosestB;
    _float fDistance = sqrt(vDiffClosest.Dot(vDiffClosest));

    if (fDistance > 1e-6f) // 충돌하지 않음
        return FALSE;

    // 충돌 좌표 저장 (두 최근접점의 평균점)
    m_vLast_Collision_Pos = (vClosestA + vClosestB) * 0.5f;

    // 충돌 깊이 방향 벡터 (최근접점에서의 보정 벡터)
    _float3 vCollisionNormal = vDiffClosest;
    if (fDistance > 1e-6f) // 작은 거리라도 있다면 정규화
        vCollisionNormal = vCollisionNormal * (1.0f / fDistance);

    m_vLast_Collision_Depth = vCollisionNormal * fDistance;

    return TRUE;
}

CCollider_Line* CCollider_Line::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CCollider_Line* pInstance = new CCollider_Line(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CCollider_Line");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CCollider_Line::Clone(void* pArg)
{
    CCollider_Line* pInstance = new CCollider_Line(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Clone : CCollider_Line");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CCollider_Line::Free()
{
	__super::Free();
}
