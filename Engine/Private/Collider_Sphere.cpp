#include "Collider_Sphere.h"

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
}

void CCollider_Sphere::Update_Scale(const _float3& vScale)
{
    m_tInfo.fRadius = vScale.x;
}

_bool CCollider_Sphere::Intersect_With_AABB_Cube(const CCollider* pOther)
{
    return _bool();
}

_bool CCollider_Sphere::Intersect_With_OBB_Cube(const CCollider* pOther)
{
    return _bool();
}

_bool CCollider_Sphere::Intersect_With_Sphere(const CCollider* pOther)
{
    auto pOtherInfo = static_cast<const CCollider_Sphere*>(pOther)->Get_Info();

    _float3 diff = m_tInfo.vPosition - pOtherInfo->vPosition;
    float distanceSquared = D3DXVec3LengthSq(&diff); // 제곱 거리 계산
    float radiusSum = m_tInfo.fRadius + pOtherInfo->fRadius;

    return distanceSquared <= (radiusSum * radiusSum);
}

_bool CCollider_Sphere::Intersect_With_Capsule(const CCollider* pOther)
{
    return _bool();
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
