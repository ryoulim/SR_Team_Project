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

#ifdef _BUFFERRENDER

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

    //m_pBuffer->Initialize(nullptr);
    return S_OK;
}

void CCollider_OBB_Cube::Render()
{
    m_pRenderTransform->Bind_Resource();
    m_pRenderBuffer->Render();
}
#endif

void CCollider_OBB_Cube::Update_Collider()
{
    m_tInfo.vPosition = *m_pTransform->Get_State(CTransform::STATE_POSITION) + m_vOffSet;

    // 원래 로컬 축 가져오기
    _float3 right = m_pTransform->Get_State(CTransform::STATE_RIGHT)->Normalize();
    _float3 up = m_pTransform->Get_State(CTransform::STATE_UP)->Normalize();
    _float3 look = m_pTransform->Get_State(CTransform::STATE_LOOK)->Normalize();

    // Y축 후보 중에서 {0,1,0}과 가장 가까운 축 찾기
    _float3 upDir = { 0.f, 1.f, 0.f };

    _float dotRight = fabsf(right.Dot(upDir));
    _float dotUp = fabsf(up.Dot(upDir));
    _float dotLook = fabsf(look.Dot(upDir));

    // 정렬된 축
    _float3 axisY, axisX, axisZ;

    if (dotRight >= dotUp && dotRight >= dotLook)
    {
        axisY = right;
        axisX = up;
        axisZ = look;
    }
    else if (dotUp >= dotRight && dotUp >= dotLook)
    {
        axisY = up;
        axisX = look;
        axisZ = right;
    }
    else
    {
        axisY = look;
        axisX = right;
        axisZ = up;
    }

    // Y축 재정렬 후, 나머지 두 축은 직교 보장
    axisX = axisY.Cross(axisZ).Normalize();
    axisZ = axisX.Cross(axisY).Normalize();

    m_tInfo.vAxis[AXIS_X] = axisX;
    m_tInfo.vAxis[AXIS_Y] = axisY;
    m_tInfo.vAxis[AXIS_Z] = axisZ;
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

    // OBB 기준 로컬 좌표로 변환
    _float3 localOrigin = rayOrigin - obbCenter;

    _float3 localPos = {
        localOrigin.Dot(obbAxes[0]),
        localOrigin.Dot(obbAxes[1]),
        localOrigin.Dot(obbAxes[2])
    };

    // 1. XZ 범위 안에 있는지 확인 (로컬 기준)
    if (fabs(localPos.x) > obbHalfSize.x) return FALSE;
    if (fabs(localPos.z) > obbHalfSize.z) return FALSE;

    // 2. Ray가 OBB 윗면보다 위에서 시작하는지 확인
    if (localPos.y < obbHalfSize.y)
        return FALSE;

    // 3. 교차 지점 = 윗면 Y 위치 (local Y = +half)
    float t = localPos.y - obbHalfSize.y;
    _float3 hitLocal = localPos + _float3{ 0.f, -t, 0.f };

    // 4. 로컬 → 월드 변환
    _float3 hitWorld =
        obbCenter +
        obbAxes[0] * hitLocal.x +
        obbAxes[1] * hitLocal.y +
        obbAxes[2] * hitLocal.z;

    m_vLast_Collision_Pos = hitWorld;
    m_vLast_Collision_Depth = +obbAxes[1]; // 윗면 충돌

    return TRUE;
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

#ifdef _BUFFERRENDER
    Safe_Release(m_pRenderTransform);
    Safe_Release(m_pRenderBuffer);
#endif
}
