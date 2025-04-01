#include "TriangularPillar.h"
#include "FXMgr.h"

CTriangularPillar::CTriangularPillar(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CMap{pGraphic_Device}
{
}

CTriangularPillar::CTriangularPillar(const CTriangularPillar& Prototype)
    : CMap(Prototype)
{
}

HRESULT CTriangularPillar::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CTriangularPillar::Initialize(void* pArg)
{
    m_szTextureID = TEXT("Test");
    m_szBufferType = TEXT("TriangularPillar");

    if (FAILED(__super::Initialize(pArg)))  
        return E_FAIL;

    return S_OK;
}

void CTriangularPillar::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

EVENT CTriangularPillar::Update(_float fTimeDelta)
{
    return  __super::Update(fTimeDelta);;
}

void CTriangularPillar::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CTriangularPillar::Render()
{
    return __super::Render();
}

HRESULT CTriangularPillar::Ready_Components(void* pArg)
{
    __super::Ready_Components(pArg);

    DESC* pDesc = static_cast<DESC*>(pArg);

    // 콜라이더 설정 구조체
    CCollider::DESC ColliderDesc{};
    ColliderDesc.pTransform = m_pTransformCom;
    ColliderDesc.pOwner = this;
    ColliderDesc.iColliderGroupID = CG_BLOCK;
    ColliderDesc.iColliderID = CI_BLOCK_COMMON;

    // 1. 실제 적용된 스케일을 기준으로 OBB 크기 계산
    _float3 vTriScale = m_pTransformCom->Compute_Scaled();
    _float3& vOBBScale = ColliderDesc.vScale;
    vOBBScale.x = sqrt(SQUARE(vTriScale.x) + SQUARE(vTriScale.z));     // 빗면 길이
    vOBBScale.y = vTriScale.y;                                          // 높이
    vOBBScale.z = (vTriScale.x * vTriScale.z) / vOBBScale.x;            // 얇은 면 깊이

    // 1. 회전된 메시의 현재 Z-방향
    _float3 vStart = -m_pTransformCom->Get_State(CTransform::STATE_LOOK)->Normalize();
    _float3 vUp = m_pTransformCom->Get_State(CTransform::STATE_UP)->Normalize();

    // 4. vUp 축을 기준으로 회전
    _float angle_rad = atan2f(vTriScale.z, vTriScale.x);
    _float4x4 matRot = _float4x4{ vUp, angle_rad };
    vStart.TransformNormal(matRot);

    ColliderDesc.vOffSet = vStart * (vOBBScale.z * 0.5f);

    m_pTransformCom->Turn_Immediately(vUp, angle_rad);

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB_Cube"),
        TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
        return E_FAIL;

    // 9. 회전 복구
    m_pTransformCom->Turn_Immediately(vUp, -angle_rad);

    return S_OK;
}

CTriangularPillar* CTriangularPillar::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CTriangularPillar* pInstance = new CTriangularPillar(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CTriangularPillar");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CTriangularPillar::Clone(void* pArg)
{
    CTriangularPillar* pInstance = new CTriangularPillar(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CTriangularPillar");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CTriangularPillar::Free()
{
    __super::Free();

}
