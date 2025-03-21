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

    _float3 d = m_tInfo.vEnd - m_tInfo.vStart; // ���� ���� ����

    _float tMin = 0.0f;
    _float tMax = 1.0f;

    int hitAxis = -1;
    _bool hitNegative = FALSE;

    // Liang-Barsky �˰���: �� �࿡ ���� ���� ���� ���
    for (int i = 0; i < 3; i++)
    {
        _float start = (&m_tInfo.vStart.x)[i];
        _float dir = (&d.x)[i];
        _float boxMin = (&minB.x)[i];
        _float boxMax = (&maxB.x)[i];

        if (fabsf(dir) < 1e-6f)
        {
            // �� �������� �����ϸ鼭 �ڽ� �ٱ��� �ִٸ� �������� ����
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
                hitNegative = (dir > 0.f); // ���� ������ -������
            }

            tMax = min(tMax, t2);

            if (tMin > tMax)
                return FALSE; // ���� ����
        }
    }

    // ���� ����: AABB�� ���� ��� ������ ���� ��ǥ ���
    _float facePos = hitNegative ? (&minB.x)[hitAxis] : (&maxB.x)[hitAxis];
    _float dirComponent = (&d.x)[hitAxis];
    _float startComponent = (&m_tInfo.vStart.x)[hitAxis];
    _float t = (facePos - startComponent) / dirComponent;

    _float3 hitPos = m_tInfo.vStart + d * t;

    // �浹 ��ǥ ����
    m_vLast_Collision_Pos = hitPos;

    // �浹 ���� ���� = �ⱸ�� - ������
    _float3 exitPoint = m_tInfo.vStart + d * tMax;
    m_vLast_Collision_Depth = exitPoint - hitPos;

    return TRUE;
}

_bool CCollider_Line::Intersect_With_OBB_Cube(const CCollider* pOther)
{
    const auto& pOBBInfo = *static_cast<const CCollider_OBB_Cube*>(pOther)->Get_Info();

    // OBB ���� �ּ� ���� ���
    const _float3& vOBBCenter = pOBBInfo.vPosition;
    const _float3& vOBBAxisX = pOBBInfo.vAxis[0]; // X�� ���� ����
    const _float3& vOBBAxisY = pOBBInfo.vAxis[1]; // Y�� ���� ����
    const _float3& vOBBAxisZ = pOBBInfo.vAxis[2]; // Z�� ���� ����
    const _float3& vOBBHalfScale = pOBBInfo.vHalfScale; // OBB ���� ũ��

    // ������ OBB�� ���� �������� ��ȯ
    _float3 vStartLocal = m_tInfo.vStart - vOBBCenter;
    _float3 vEndLocal = m_tInfo.vEnd - vOBBCenter;

    vStartLocal = { vStartLocal.Dot(vOBBAxisX), vStartLocal.Dot(vOBBAxisY), vStartLocal.Dot(vOBBAxisZ) };
    vEndLocal = { vEndLocal.Dot(vOBBAxisX), vEndLocal.Dot(vOBBAxisY), vEndLocal.Dot(vOBBAxisZ) };

    _float3 vLineLocalDir = vEndLocal - vStartLocal; // ���� �������� ���� ���� ����

    _float fTMin = 0.0f, fTMax = 1.0f;
    for (int i = 0; i < 3; i++) {
        _float fMinB = -(&vOBBHalfScale.x)[i]; // OBB �ּ� ��ǥ (����)
        _float fMaxB = (&vOBBHalfScale.x)[i];  // OBB �ִ� ��ǥ (����)
        _float fStartP = (&vStartLocal.x)[i]; // ���� ������ ��ǥ (����)
        _float fDir = (&vLineLocalDir.x)[i];  // ���� ���� (����)

        if (abs(fDir) < 1e-6f) { // ������ ���
            if (fStartP < fMinB || fStartP > fMaxB)
                return FALSE; // �浹 ����
        }
        else {
            _float fT1 = (fMinB - fStartP) / fDir;
            _float fT2 = (fMaxB - fStartP) / fDir;

            if (fT1 > fT2)
                swap(fT1, fT2);
            fTMin = max(fTMin, fT1);
            fTMax = min(fTMax, fT2);

            if (fTMin > fTMax)
                return FALSE; // �浹 ����
        }
    }

    // �浹�� ���
    _float3 vIntersectionLocal = vStartLocal + fTMin * vLineLocalDir; // �浹 ��ǥ (���� ����)

    // ���� �� ���� ��ȯ (�浹 ��ǥ)
    m_vLast_Collision_Pos =
        vOBBCenter +
        vIntersectionLocal.x * vOBBAxisX +
        vIntersectionLocal.y * vOBBAxisY +
        vIntersectionLocal.z * vOBBAxisZ;

    // �浹 ���� ���� ���� ��� (������ -> �ⱸ�� ����)
    _float3 vExitLocal = vStartLocal + fTMax * vLineLocalDir;
    _float3 vSeparationLocal = vExitLocal - vIntersectionLocal;

    // ���� �� ���� ��ȯ (�浹 ���� ����)
    m_vLast_Collision_Depth =
        vSeparationLocal.x * vOBBAxisX +
        vSeparationLocal.y * vOBBAxisY +
        vSeparationLocal.z * vOBBAxisZ;

    return TRUE;
}

_bool CCollider_Line::Intersect_With_Sphere(const CCollider* pOther)
{
    auto pSphereInfo = static_cast<const CCollider_Sphere*>(pOther)->Get_Info();

    _float3 d = m_tInfo.vEnd - m_tInfo.vStart; // ���� ���� ����

    // ���а� ���� �ֱ����� ã��
    _float3 startToCenter = pSphereInfo->vPosition - m_tInfo.vStart;
    _float proj = startToCenter.Dot(d) / d.Dot(d);

    // �ֱ����� ��� (t�� 0~1 ������ Ŭ����)
    _float t = max(0.0f, min(1.0f, proj));
    _float3 closestPoint = m_tInfo.vStart + t * d;

    // �ֱ������� ���� �߽� �Ÿ� ���
    _float3 diff = closestPoint - pSphereInfo->vPosition;
    _float distance = diff.Length();

    if (distance > pSphereInfo->fRadius)
        return FALSE; // �浹 ����

    // �浹 ��ǥ ����
    m_vLast_Collision_Pos = closestPoint;

    // �浹 ���� ���� ���� ��� (�� ǥ�鿡�� �ֱ����������� ����)
    m_vLast_Collision_Depth = diff.Normalize() * (pSphereInfo->fRadius - distance);

    return TRUE;
}

_bool CCollider_Line::Intersect_With_Capsule(const CCollider* pOther)
{
    auto pCapsuleInfo = static_cast<const CCollider_Capsule*>(pOther)->Get_Info();

    // ĸ���� �߽ɼ� �� ����
    _float3 vCapsuleTop = pCapsuleInfo->vCenter;
    vCapsuleTop.y += pCapsuleInfo->fHeight * 0.5f;
    _float3 vCapsuleBottom = pCapsuleInfo->vCenter;
    vCapsuleBottom.y -= pCapsuleInfo->fHeight * 0.5f;

    // ������ ĸ���� �߽ɼ��� �ֱ����� ã��
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

    // �ֱ����� �� ���� �� �Ÿ� ���
    _float3 diff = closestP - closestQ;
    _float distance = diff.Length();

    // �浹 ���� Ȯ��
    _bool bCollision = distance <= pCapsuleInfo->fRadius;

    if (bCollision) {
        // �浹 �ؼ� ���� ���� (������ �̵��ؾ� �ϴ� ����)
        _float3 collisionNormal = diff;
        collisionNormal.Normalize();
        m_vLast_Collision_Depth = collisionNormal * (pCapsuleInfo->fRadius - distance);
        // ĸ�� ǥ��� ������ �����ϴ� ����
        m_vLast_Collision_Pos = closestQ + collisionNormal * pCapsuleInfo->fRadius;
    }

    return bCollision;
}

_bool CCollider_Line::Intersect_With_Rect(const CCollider* pOther)
{
    auto pRectInfo = static_cast<const CCollider_Rect*>(pOther)->Get_Info();

    _float3 vLineDir = m_tInfo.vEnd - m_tInfo.vStart; // ���� ���� ����
    _float3 vRectNormal = pRectInfo->GetNormal();  // ���簢�� ���� ����
    _float3 vStartToCenter = m_tInfo.vStart - pRectInfo->vCenter; // ���� ������ �� ���簢�� �߽� ����

    // ���� ������ ���� �˻� (N �� D)
    _float fDotNormalDir = vRectNormal.Dot(vLineDir);
    _float fDotNormalStart = vRectNormal.Dot(vStartToCenter);

    if (abs(fDotNormalDir) < 1e-6f) // ������ ���� �����ϸ� �浹 ����
        return FALSE;

    _float fIntersectionTime = -fDotNormalStart / fDotNormalDir; // ������ ���� �����ϴ� t ��
    if (fIntersectionTime < 0.0f || fIntersectionTime > 1.0f) // ������ ������ ����� �浹 ����
        return FALSE;

    // ������ ���
    _float3 vIntersectionPoint = m_tInfo.vStart + fIntersectionTime * vLineDir;

    // �������� Rect ���ο� �ִ��� Ȯ�� (���簢���� ��ǥ�踦 ���)
    _float3 vLocalIntersection = vIntersectionPoint - pRectInfo->vCenter;
    _float fProjOnRight = vLocalIntersection.Dot(pRectInfo->vRight);
    _float fProjOnUp = vLocalIntersection.Dot(pRectInfo->vUp);

    if (abs(fProjOnRight) > pRectInfo->fWidth * 0.5f || abs(fProjOnUp) > pRectInfo->fHeight * 0.5f)
        return FALSE; // ���簢�� �ٱ��̸� �浹 ����

    // �浹 ��ǥ ����
    m_vLast_Collision_Pos = vIntersectionPoint;

    // �浹 ���� ���� ���� ��� (���� �������� �̵��ؾ� �ϴ� ����)
    m_vLast_Collision_Depth = vRectNormal * (-fDotNormalStart / fDotNormalDir);

    return TRUE;
}

_bool CCollider_Line::Intersect_With_Line(const CCollider* pOther)
{
    const auto& pOtherLineInfo = *static_cast<const CCollider_Line*>(pOther)->Get_Info();

    // ���� ���� ����
    const _float3& vStartA = m_tInfo.vStart;
    const _float3& vEndA = m_tInfo.vEnd;
    _float3 vDirA = vEndA - vStartA;

    // �ٸ� ���� ����
    const _float3& vStartB = pOtherLineInfo.vStart;
    const _float3& vEndB = pOtherLineInfo.vEnd;
    _float3 vDirB = vEndB - vStartB;

    _float3 vDiff = vStartA - vStartB;
    _float fA = vDirA.Dot(vDirA);
    _float fB = vDirA.Dot(vDirB);
    _float fC = vDirB.Dot(vDirB);
    _float fD = vDirA.Dot(vDiff);
    _float fE = vDirB.Dot(vDiff);

    _float fDenom = fA * fC - fB * fB; // �и�

    _float fS, fT;
    if (abs(fDenom) > 1e-6f) // �� ������ �������� ���� ���
    {
        fS = (fB * fE - fC * fD) / fDenom;
        fT = (fA * fE - fB * fD) / fDenom;
    }
    else // �� ������ ������ ���
    {
        fS = 0.0f;
        fT = fE / fC;
    }

    // 0~1 ���̷� Ŭ�����Ͽ� ������ ��ȿ ���� ������ Ȯ��
    fS = max(0.0f, min(1.0f, fS));
    fT = max(0.0f, min(1.0f, fT));

    // �ֱ����� ���
    _float3 vClosestA = vStartA + fS * vDirA;
    _float3 vClosestB = vStartB + fT * vDirB;

    // �� �ֱ����� ���� �Ÿ� ���
    _float3 vDiffClosest = vClosestA - vClosestB;
    _float fDistance = sqrt(vDiffClosest.Dot(vDiffClosest));

    if (fDistance > 1e-6f) // �浹���� ����
        return FALSE;

    // �浹 ��ǥ ���� (�� �ֱ������� �����)
    m_vLast_Collision_Pos = (vClosestA + vClosestB) * 0.5f;

    // �浹 ���� ���� ���� (�ֱ����������� ���� ����)
    _float3 vCollisionNormal = vDiffClosest;
    if (fDistance > 1e-6f) // ���� �Ÿ��� �ִٸ� ����ȭ
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
