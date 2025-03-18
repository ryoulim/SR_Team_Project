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

HRESULT CCollider_OBB_Cube::Initialize(void* pArg)
{
	if (pArg == nullptr)
	{
		MSG_BOX("콜라이더 pArg에 nullptr을 넣어주면 어떡해...");
		return E_FAIL;
	}

	const DESC* pDesc = static_cast<DESC*>(pArg);
	m_tInfo.vHalfScale = pDesc->vScale * 0.5f;
	Update_Collider(pDesc->pTransform);

	return S_OK;
}

void CCollider_OBB_Cube::Update_Collider(const CTransform* pTransform)
{
	m_tInfo.vPosition = *pTransform->Get_State(CTransform::STATE_POSITION);
	m_tInfo.vAxis[AXIS_X] = pTransform->Get_State(CTransform::STATE_RIGHT)->Normalize();
	m_tInfo.vAxis[AXIS_Y] = pTransform->Get_State(CTransform::STATE_UP)->Normalize();
	m_tInfo.vAxis[AXIS_Z] = pTransform->Get_State(CTransform::STATE_LOOK)->Normalize();
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
}
