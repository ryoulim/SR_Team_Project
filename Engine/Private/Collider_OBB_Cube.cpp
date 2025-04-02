#include "Transform.h"
#include "Collider_OBB_Cube.h"

#define AXIS_X 0
#define AXIS_Y 1
#define AXIS_Z 2

CCollider_OBB_Cube::CCollider_OBB_Cube(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCollider{ pGraphic_Device }
{
}

CCollider_OBB_Cube::CCollider_OBB_Cube(const CCollider_OBB_Cube& Prototype)
	: CCollider(Prototype)
{
}

HRESULT CCollider_OBB_Cube::Initialize_Prototype()
{
    return __super::Initialize_Prototype(OBB_CUBE);
}

#ifdef _COLLIDERRENDER

#include "VIBuffer_Cube.h"
#include "Transform.h"

HRESULT CCollider_OBB_Cube::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    DESC* pDesc = static_cast<DESC*>(pArg);

    m_pRenderTransform = CTransform::Create(m_pGraphic_Device);
    m_pRenderBuffer = CVIBuffer_Cube::Create(m_pGraphic_Device);

    m_pRenderTransform->Set_WorldMatrix(pDesc->pTransform->Get_WorldMatrix());
    m_pRenderTransform->Scaling(pDesc->vScale);
    m_pRenderTransform->Move(pDesc->vOffSet);

    return S_OK;
}

void CCollider_OBB_Cube::Render()
{
 //   m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);  // 와이어프레임

    // 텍스처 제거
    m_pGraphic_Device->SetTexture(0, nullptr);
    m_pRenderTransform->Bind_Resource();
    m_pRenderBuffer->Bind_Buffers();
    m_pRenderBuffer->Render();

}
#endif

void CCollider_OBB_Cube::Update_Collider()
{
    // 위치 + 오프셋
    m_tInfo.vPosition = *m_pTransform->Get_State(CTransform::STATE_POSITION) + m_vOffSet;

    m_tInfo.vAxis[AXIS_X] = m_pTransform->Get_State(CTransform::STATE_RIGHT)->Normalize();
    m_tInfo.vAxis[AXIS_Y] = m_pTransform->Get_State(CTransform::STATE_UP)->Normalize();
    m_tInfo.vAxis[AXIS_Z] = m_pTransform->Get_State(CTransform::STATE_LOOK)->Normalize();
}

void CCollider_OBB_Cube::Update_Scale(const _float3& vScale)
{
    m_tInfo.vHalfScale = vScale * 0.5f;
    m_fMaxLength = sqrtf(
        m_tInfo.vHalfScale.x * m_tInfo.vHalfScale.x +
        m_tInfo.vHalfScale.y * m_tInfo.vHalfScale.y +
        m_tInfo.vHalfScale.z * m_tInfo.vHalfScale.z
    );
}

_bool CCollider_OBB_Cube::RayCasting(const _float3& rayOrigin, const _float3& rayDir)
{
    const _float3& obbCenter = m_tInfo.vPosition;
    const _float3* obbAxes = m_tInfo.vAxis;
    const _float3& obbHalfSize = m_tInfo.vHalfScale;

    // Ray를 OBB의 로컬 공간으로 변환
    _float3 localOrigin = rayOrigin - obbCenter;
    _float3 rayLocalOrigin = {
        localOrigin.Dot(obbAxes[0]),
        localOrigin.Dot(obbAxes[1]),
        localOrigin.Dot(obbAxes[2])
    };

    _float3 rayLocalDir = {
        rayDir.Dot(obbAxes[0]),
        rayDir.Dot(obbAxes[1]),
        rayDir.Dot(obbAxes[2])
    };

    // AABB 방식 그대로 적용 (OBB의 로컬 공간 기준)
    _float3 invDir = {
        1.0f / rayLocalDir.x,
        1.0f / rayLocalDir.y,
        1.0f / rayLocalDir.z
    };

    _float3 t1 = _float3(_float3{ -obbHalfSize.x, -obbHalfSize.y, -obbHalfSize.z } - rayLocalOrigin) * invDir;
    _float3 t2 = _float3(_float3{ +obbHalfSize.x, +obbHalfSize.y, +obbHalfSize.z } - rayLocalOrigin) * invDir;

    if (rayLocalDir.x < 0.0f) std::swap(t1.x, t2.x);
    if (rayLocalDir.y < 0.0f) std::swap(t1.y, t2.y);
    if (rayLocalDir.z < 0.0f) std::swap(t1.z, t2.z);

    _float3 tMin = min(t1, t2);
    _float3 tMax = max(t1, t2);

    _float tNear = max(max(tMin.x, tMin.y), tMin.z);
    _float tFar = min(min(tMax.x, tMax.y), tMax.z);

    if (tNear > tFar || tFar < 0.0f)
        return FALSE;

    // 충돌 지점 (월드 공간으로 다시 변환)
    _float3 localHitPoint = rayLocalOrigin + rayLocalDir * tNear;
    _float3 worldHitPoint = obbCenter
        + obbAxes[0] * localHitPoint.x
        + obbAxes[1] * localHitPoint.y
        + obbAxes[2] * localHitPoint.z;

    m_vLast_Collision_Pos = worldHitPoint;

    // 충돌 면 방향 (로컬 축 기준으로 결정 → 다시 월드축으로 변환)
    if (tNear == tMin.x) m_vLast_Collision_Depth = -obbAxes[0];
    else if (tNear == tMax.x) m_vLast_Collision_Depth = +obbAxes[0];
    else if (tNear == tMin.y) m_vLast_Collision_Depth = -obbAxes[1];
    else if (tNear == tMax.y) m_vLast_Collision_Depth = +obbAxes[1];
    else if (tNear == tMin.z) m_vLast_Collision_Depth = -obbAxes[2];
    else if (tNear == tMax.z) m_vLast_Collision_Depth = +obbAxes[2];

    return TRUE;
}

_bool CCollider_OBB_Cube::RayCast_Downward(const _float3& rayOrigin)
{
    const _float3& obbCenter = m_tInfo.vPosition;
    const _float3* obbAxes = m_tInfo.vAxis;
    const _float3& obbHalfSize = m_tInfo.vHalfScale;

    const _float3 rayDir = _float3(0.f, -1.f, 0.f);
    _float minT = FLT_MAX;
    _bool bHit = FALSE;

    _float3 bestHitPos, bestNormal;

    // OBB 3축에 대해 각 ± 방향(= 총 6면) 검사
    for (int i = 0; i < 3; ++i)
    {
        const _float3& axis = obbAxes[i];
        const _float half = obbHalfSize[i];

        for (int sign = -1; sign <= 1; sign += 2)
        {
            _float3 normal = axis * (_float)sign;
            _float3 planePos = obbCenter + normal * half;

            _float denom = normal.Dot(rayDir);
            if (fabs(denom) < 1e-6f) continue; // 평면과 평행

            _float t = (planePos - rayOrigin).Dot(normal) / denom;
            if (t < 0.f || t > minT) continue; // ray보다 뒤에 있거나, 이전보다 멀면 무시

            _float3 hitPoint = rayOrigin + rayDir * t;

            // hitPoint가 OBB 안쪽인지 검사
            _float3 toHit = hitPoint - obbCenter;
            _bool inside = TRUE;

            for (int j = 0; j < 3; ++j)
            {
                _float dist = toHit.Dot(obbAxes[j]);
                if (fabs(dist) > obbHalfSize[j] + 0.001f) // 살짝 여유
                {
                    inside = FALSE;
                    break;
                }
            }

            if (inside)
            {
                bHit = TRUE;
                minT = t;
                bestHitPos = hitPoint;
                bestNormal = normal;
            }
        }
    }

    if (bHit)
    {
        m_vLast_Collision_Pos = bestHitPos;
        m_vLast_Collision_Depth = bestNormal; // 법선은 면 방향 그대로
    }

    return bHit;
}

_bool CCollider_OBB_Cube::Intersect_With_AABB_Cube(const CCollider* pOther)
{
	return _bool();
}

_bool CCollider_OBB_Cube::Intersect_With_OBB_Cube(const CCollider* pOther)
{
    auto pOtherInfo = static_cast<const CCollider_OBB_Cube*>(pOther)->Get_Info();

    float ra, rb;
    _float3 vDot[3], vAbsDot[3];
    _float3 vDistance = pOtherInfo->vPosition - m_tInfo.vPosition;

    // OBB1의 로컬 축을 기준으로 OBB2의 축을 변환
    for (_uint i = 0; i < 3; i++)
    {
        for (_uint j = 0; j < 3; j++)
        {
            vDot[i][j] = m_tInfo.vAxis[i].Dot(pOtherInfo->vAxis[j]);
        }
    }

    // 절댓값 행렬 생성 (오차 방지)
    for (_uint i = 0; i < 3; i++)
    {
        for (_uint j = 0; j < 3; j++)
        {
            vAbsDot[i][j] = fabs(vDot[i][j]) + 1e-6f;
        }
    }

    // 변환된 중심 벡터를 OBB의 로컬 좌표로 변환
    _float3 localT;
    localT.x = vDistance.Dot(m_tInfo.vAxis[AXIS_X]);
    localT.y = vDistance.Dot(m_tInfo.vAxis[AXIS_Y]);
    localT.z = vDistance.Dot(m_tInfo.vAxis[AXIS_Z]);

    // 15개의 충돌 검사 축 (6 + 9 = 15)
    for (_uint i = 0; i < 3; i++)
    {
        ra = m_tInfo.vHalfScale[i];
        rb = pOtherInfo->vHalfScale.x * vAbsDot[i][0] 
            + pOtherInfo->vHalfScale.y * vAbsDot[i][1] 
            + pOtherInfo->vHalfScale.z * vAbsDot[i][2];

        if (fabs(localT[i]) > ra + rb)
            return FALSE;
    }

    for (_uint i = 0; i < 3; i++)
    {
        ra = m_tInfo.vHalfScale.x * vAbsDot[0][i] + m_tInfo.vHalfScale.y * vAbsDot[1][i] + m_tInfo.vHalfScale.z * vAbsDot[2][i];
        rb = pOtherInfo->vHalfScale[i];

        if (fabs(localT.x * vDot[0][i] + localT.y * vDot[1][i] + localT.z * vDot[2][i]) > ra + rb)
            return FALSE;
    }

    // 9개의 교차 축 검사
    for (_uint i = 0; i < 3; i++)
    {
        for (_uint j = 0; j < 3; j++)
        {
            ra = m_tInfo.vHalfScale[(i + 1) % 3] * vAbsDot[(i + 2) % 3][j] + m_tInfo.vHalfScale[(i + 2) % 3] * vAbsDot[(i + 1) % 3][j];
            rb = pOtherInfo->vHalfScale[(j + 1) % 3] * vAbsDot[i][(j + 2) % 3] + pOtherInfo->vHalfScale[(j + 2) % 3] * vAbsDot[i][(j + 1) % 3];

            if (fabs(localT[(i + 1) % 3] * vDot[(i + 2) % 3][j] - localT[(i + 2) % 3] * vDot[(i + 1) % 3][j]) > ra + rb)
                return FALSE;
        }
    }

    return TRUE; // 모든 분리축 검사를 통과하면 충돌!}
}

_bool CCollider_OBB_Cube::Intersect_With_Sphere(const CCollider* pOther)
{
	return _bool();
}

_bool CCollider_OBB_Cube::Intersect_With_Capsule(const CCollider* pOther)
{
	return _bool();
}

_bool CCollider_OBB_Cube::Intersect_With_Rect(const CCollider* pOther)
{
    return _bool();
}

_bool CCollider_OBB_Cube::Intersect_With_Line(const CCollider* pOther)
{
    return _bool();
}

CCollider_OBB_Cube* CCollider_OBB_Cube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CCollider_OBB_Cube* pInstance = new CCollider_OBB_Cube(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CCollider_OBB_Cube");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CCollider_OBB_Cube::Clone(void* pArg)
{
    CCollider_OBB_Cube* pInstance = new CCollider_OBB_Cube(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Clone : CCollider_OBB_Cube");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CCollider_OBB_Cube::Free()
{
    __super::Free();

#ifdef _COLLIDERRENDER
    Safe_Release(m_pRenderTransform);
    Safe_Release(m_pRenderBuffer);
#endif
}
