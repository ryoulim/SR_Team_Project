#include "Transform.h"
#include "Collider_Sphere.h"
#include "Collider_AABB_Cube.h"
#include "Collider_OBB_Cube.h"
#include "Collider_Capsule.h"

CCollider_Sphere::CCollider_Sphere(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CCollider{ pGraphic_Device }
{
}

CCollider_Sphere::CCollider_Sphere(const CCollider_Sphere& Prototype)
    : CCollider(Prototype)
{
}

HRESULT CCollider_Sphere::Initialize_Prototype()
{
    return __super::Initialize_Prototype(SPHERE);
}

void CCollider_Sphere::Update_Collider()
{
    m_tInfo.vPosition = *m_pTransform->Get_State(CTransform::STATE_POSITION) + m_vOffSet;
#ifdef _COLLIDERRENDER
    m_pRenderTransform->Set_State(CTransform::STATE_POSITION, m_tInfo.vPosition);
#endif

}

void CCollider_Sphere::Update_Scale(const _float3& vScale)
{
    m_tInfo.fRadius = vScale.x;
#ifdef _COLLIDERRENDER
    m_pRenderTransform->Set_State(CTransform::STATE_RIGHT, _float3{ 1.f,0.f,0.f } * m_tInfo.fRadius * 2.f);
    m_pRenderTransform->Set_State(CTransform::STATE_UP, _float3{ 0.f,1.f,0.f } * m_tInfo.fRadius * 2.f);
    m_pRenderTransform->Set_State(CTransform::STATE_LOOK, _float3{ 0.f,0.f,1.f });
#endif

}

_bool CCollider_Sphere::RayCasting(const _float3& rayOrigin, const _float3& rayDir)
{
    const _float3& center = m_tInfo.vPosition;
    const _float radius = m_tInfo.fRadius;

    // ����: ���� ������ �� �� �߽�
    _float3 m = rayOrigin - center;

    _float b = m.Dot(rayDir);
    _float c = m.Dot(m) - radius * radius;

    // ���� �������� �� �ٱ��̰�, �� ���� ������ ����
    if (c > 0.f && b > 0.f)
        return FALSE;

    _float discr = b * b - c;

    // �Ǻ��� < 0 �� ���� ����
    if (discr < 0.f)
        return FALSE;

    // ���� �������� �Ÿ�
    _float t = -b - sqrtf(discr);

    // ������ ���� �������� �� �ȿ� �ִ� ��� �� ���� ������ ������
    if (t < 0.f)
        t = 0.f;

    // ���� ���� ��ǥ
    _float3 hitPoint = rayOrigin + rayDir * t;

    // �浹 ����: �� �߽� �� ���� ����
    _float3 normal = hitPoint - center;
    _float length = normal.Length();
    _float3 pushDir = (length > 1e-4f) ? normal.Normalize() : _float3(0.f, 1.f, 0.f);

    m_vLast_Collision_Pos = hitPoint;
    m_vLast_Collision_Depth = pushDir;

    return TRUE;
}

#ifdef _COLLIDERRENDER
#include "VIBuffer.h"   
void CCollider_Sphere::Render()
{
    if (!m_bColliderRender)
        return;

    m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
    m_pGraphic_Device->SetTexture(0, nullptr);
    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pRenderTransform->Billboard_Y());
    m_pRenderBuffer->Bind_Buffers();
    m_pRenderBuffer->Render();
    m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

}
#endif

_bool CCollider_Sphere::Intersect_With_AABB_Cube(const CCollider* pOther)
{
    auto pAABBInfo = static_cast<const CCollider_AABB_Cube*>(pOther)->Get_Info();

    // 1. AABB�� min/max ��ǥ
    const _float3& vMin = pAABBInfo->vMinPos;
    const _float3& vMax = pAABBInfo->vMaxPos;

    // 2. ���� �߽� ��ǥ
    const _float3& vCenter = m_tInfo.vPosition;
    const _float fRadius = m_tInfo.fRadius;

    // 3. AABB ���ο��� �� �߽ɿ� ���� ����� �� ã�� (Ŭ����)
    _float3 vClosest = {
        max(vMin.x, min(vCenter.x, vMax.x)),
        max(vMin.y, min(vCenter.y, vMax.y)),
        max(vMin.z, min(vCenter.z, vMax.z))
    };

    // 4. �� �߽ɰ� ���� ����� �� ���� �Ÿ�
    _float3 vDiff = vCenter - vClosest;
    _float fDistSq = D3DXVec3LengthSq(&vDiff);

    if (fDistSq >= fRadius * fRadius)
        return FALSE; // �浹 �ƴ�

    _float fDist = sqrtf(fDistSq);
    _float3 vNormal = {};

    if (fDist > 0.f)
    {
        vNormal = vDiff / fDist;
    }
    else
    {
        // ���� AABB ���ο� ���� �� ���� ����� �� ���� ã��
        _float3 vDelta = vCenter - pAABBInfo->vCenter;
        _float3 vOverlap = {
            (vMax.x - vMin.x) * 0.5f - fabsf(vDelta.x),
            (vMax.y - vMin.y) * 0.5f - fabsf(vDelta.y),
            (vMax.z - vMin.z) * 0.5f - fabsf(vDelta.z)
        };

        if (vOverlap.x <= vOverlap.y && vOverlap.x <= vOverlap.z)
            vNormal = { (vDelta.x >= 0.f) ? 1.f : -1.f, 0.f, 0.f };
        else if (vOverlap.y <= vOverlap.x && vOverlap.y <= vOverlap.z)
            vNormal = { 0.f, (vDelta.y >= 0.f) ? 1.f : -1.f, 0.f };
        else
            vNormal = { 0.f, 0.f, (vDelta.z >= 0.f) ? 1.f : -1.f };
    }

    // 5. �浹 ���� ���� (���� �̵��ؾ� �ϴ� ����)
    const _float fPenetration = fRadius - fDist;
    m_vLast_Collision_Depth = vNormal * fPenetration;

    // 6. �浹 ��ǥ (���� �ܰ� �� AABB �鿡 ���� ���� ����)
    m_vLast_Collision_Pos = vCenter - vNormal * fRadius;

    return TRUE;
}

_bool CCollider_Sphere::Intersect_With_OBB_Cube(const CCollider* pOther)
{
    auto pOBBInfo = static_cast<const CCollider_OBB_Cube*>(pOther)->Get_Info();

    const _float3& vSphereCenter = m_tInfo.vPosition;
    const _float fSphereRadius = m_tInfo.fRadius;

    const _float3& vOBBCenter = pOBBInfo->vPosition;
    const _float3* vAxis = pOBBInfo->vAxis;
    const _float3& vHalf = pOBBInfo->vHalfScale;

    // 1. �� �߽��� OBB ���� �������� ����
    _float3 vLocal = vSphereCenter - vOBBCenter;

    // 2. OBB �ȿ��� �� �߽ɿ� ���� ����� �� ���
    _float3 vClosest = vOBBCenter;

    for (int i = 0; i < 3; ++i)
    {
        _float fDist = D3DXVec3Dot(&vLocal, &vAxis[i]);

        if (fDist > vHalf[i]) fDist = vHalf[i];
        if (fDist < -vHalf[i]) fDist = -vHalf[i];

        vClosest = vClosest + vAxis[i] * fDist;
    }

    // 3. �Ÿ� ���
    _float3 vDiff = vSphereCenter - vClosest;
    _float fDistSq = D3DXVec3LengthSq(&vDiff);

    if (fDistSq >= fSphereRadius * fSphereRadius)
        return FALSE; // �浹 ����

    _float fDist = sqrtf(fDistSq);
    _float3 vNormal = {};

    if (fDist > 0.f)
    {
        vNormal = vDiff / fDist;
    }
    else
    {
        // ���� OBB �߽ɰ� ��Ȯ�� ��ĥ ��� �� ���� ����� �� ���� ����
        _float3 vDelta = vSphereCenter - vOBBCenter;
        _float fMinOverlap = FLT_MAX;
        int iMinAxis = 0;

        for (int i = 0; i < 3; ++i)
        {
            _float fProj = D3DXVec3Dot(&vDelta, &vAxis[i]);
            _float fOverlap = vHalf[i] - fabsf(fProj);

            if (fOverlap < fMinOverlap)
            {
                fMinOverlap = fOverlap;
                iMinAxis = i;
            }
        }

        vNormal = vAxis[iMinAxis] * ((D3DXVec3Dot(&vDelta, &vAxis[iMinAxis]) >= 0.f) ? 1.f : -1.f);
    }

    // 4. ���� ���Ϳ� �浹 ��ǥ
    _float fPenetration = fSphereRadius - fDist;
    m_vLast_Collision_Depth = vNormal * fPenetration;
    m_vLast_Collision_Pos = vSphereCenter - vNormal * fSphereRadius;

    return TRUE;
}

_bool CCollider_Sphere::Intersect_With_Sphere(const CCollider* pOther)
{
    auto pOtherInfo = static_cast<const CCollider_Sphere*>(pOther)->Get_Info();

    _float3 diff = m_tInfo.vPosition - pOtherInfo->vPosition;
    float distanceSquared = D3DXVec3LengthSq(&diff); // ���� �Ÿ� ���
    float radiusSum = m_tInfo.fRadius + pOtherInfo->fRadius;

    return distanceSquared <= (radiusSum * radiusSum);
}

_bool CCollider_Sphere::Intersect_With_Capsule(const CCollider* pOther)
{
    auto pCapsuleInfo = static_cast<const CCollider_Capsule*>(pOther)->Get_Info();

    // 1. ĸ�� ��/�Ʒ� �߽� ���
    const _float fHalfHeight = pCapsuleInfo->fHeight * 0.5f;
    const _float3 vHalf = { 0.f, fHalfHeight, 0.f };
    const _float3 vTop = pCapsuleInfo->vCenter + vHalf;
    const _float3 vBottom = pCapsuleInfo->vCenter - vHalf;

    // 2. ĸ�� ���� ���� �� ����
    _float3 vCapsuleAxis = vTop - vBottom;
    const _float fCapsuleHeightSq = D3DXVec3LengthSq(&vCapsuleAxis);
    _float fCapsuleHeight = sqrtf(fCapsuleHeightSq);

    // Normalize
    _float3 vCapsuleDir = {};
    if (fCapsuleHeight != 0.f)
        vCapsuleDir = vCapsuleAxis / fCapsuleHeight;
    else
        vCapsuleDir = { 0.f, 1.f, 0.f }; // ���������� ���, Y�� �ӽ� ó��

    // 3. �� �߽ɿ��� ĸ�� �Ʒ����� ����
    const _float3 vToSphere = m_tInfo.vPosition - vBottom;
    _float t = D3DXVec3Dot(&vToSphere, &vCapsuleDir);

    // clamp(t, 0.f, fCapsuleHeight)
    if (t < 0.f) t = 0.f;
    else if (t > fCapsuleHeight) t = fCapsuleHeight;

    // 4. ĸ�� ���п��� ���� ����� ��
    const _float3 vClosest = vBottom + vCapsuleDir * t;

    // 5. �Ÿ� �� �浹 �Ǵ�
    const _float3 vDiff = m_tInfo.vPosition - vClosest;
    const _float fDistSq = D3DXVec3LengthSq(&vDiff);
    const _float fTotalRadius = m_tInfo.fRadius + pCapsuleInfo->fRadius;
    const _float fTotalRadiusSq = fTotalRadius * fTotalRadius;

    if (fDistSq >= fTotalRadiusSq)
        return FALSE; // �浹 ����

    // 6. �浹 ���� ���� ����
    _float fDist = sqrtf(fDistSq);
    _float3 vNormal = {};
    if (fDist > 0.f)
        vNormal = vDiff / fDist;
    else
        vNormal = { 1.f, 0.f, 0.f }; // ��ģ ��� ���� ����

    //const _float fPenetration = fTotalRadius - fDist;
    //m_vLast_Collision_Depth = vNormal * fPenetration;
    m_vLast_Collision_Depth = m_tInfo.vPosition - pCapsuleInfo->vCenter;


    // 7. �浹 ��ǥ ��� (������ ����)
    const _float3 vPointOnSphere = m_tInfo.vPosition - vNormal * m_tInfo.fRadius;
    const _float3 vPointOnCapsule = vClosest + vNormal * pCapsuleInfo->fRadius;
    m_vLast_Collision_Pos = (vPointOnSphere + vPointOnCapsule) * 0.5f;

    return TRUE;
}

_bool CCollider_Sphere::Intersect_With_Rect(const CCollider* pOther)
{
    return _bool();
}

_bool CCollider_Sphere::Intersect_With_Line(const CCollider* pOther)
{
    return _bool();
}

CCollider_Sphere* CCollider_Sphere::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CCollider_Sphere* pInstance = new CCollider_Sphere(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CCollider_Sphere");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CCollider_Sphere::Clone(void* pArg)
{
    CCollider_Sphere* pInstance = new CCollider_Sphere(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Clone : CCollider_Sphere");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CCollider_Sphere::Free()
{
    __super::Free();
}
