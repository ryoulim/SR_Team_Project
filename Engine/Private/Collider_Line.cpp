#include "Collider_Line.h"
#include "Collider_Capsule.h"

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

HRESULT CCollider_Line::Initialize(void* pArg)
{
	if (pArg == nullptr)
	{
		MSG_BOX("콜라이더 pArg에 nullptr을 넣어주면 어떡해...");
		return E_FAIL;
	}

	DESC* pDesc = static_cast<DESC*>(pArg);
	m_vDiffFromCenter = pDesc->pTransform->Get_State(CTransform::STATE_LOOK)->Normalize() * (pDesc->fLength * 0.5f);
	Update_Collider(pDesc->pTransform);
	return S_OK;
}

void CCollider_Line::Update_Collider(const CTransform* pTransform)
{
	const _float3* pPos = pTransform->Get_State(CTransform::STATE_POSITION);
	m_tInfo.vStart = *pPos + m_vDiffFromCenter;
	m_tInfo.vEnd = *pPos - m_vDiffFromCenter;
}

_bool CCollider_Line::Intersect_With_AABB_Cube(const CCollider* pOther)
{
	return _bool();
}

_bool CCollider_Line::Intersect_With_OBB_Cube(const CCollider* pOther)
{
	return _bool();
}

_bool CCollider_Line::Intersect_With_Sphere(const CCollider* pOther)
{
	return _bool();
}

_bool CCollider_Line::Intersect_With_Capsule(const CCollider* pOther)
{
    auto pCapsuleInfo = dynamic_cast<const CCollider_Capsule*>(pOther)->Get_Info();

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

    float a = D3DXVec3Dot(&d1, &d1);
    float e = D3DXVec3Dot(&d2, &d2);
    float f = D3DXVec3Dot(&d2, &r);

    float s, t;
    if (a <= 1e-6f && e <= 1e-6f) {
        s = t = 0.0f;
    }
    else if (a <= 1e-6f) {
        s = 0.0f;
        t = f / e;
    }
    else {
        float c = D3DXVec3Dot(&d1, &r);
        if (e <= 1e-6f) {
            t = 0.0f;
            s = -c / a;
        }
        else {
            float b = D3DXVec3Dot(&d1, &d2);
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

    // 최근접점 간 XZ 평면 거리 계산
    D3DXVECTOR3 diff = closestP - closestQ;
    float distanceXZ = sqrt(diff.x * diff.x + diff.z * diff.z);

    // XZ 평면 거리 비교
    return distanceXZ <= pCapsuleInfo->fRadius;
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
