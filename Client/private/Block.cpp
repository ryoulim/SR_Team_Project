#include "Block.h"

CBlock::CBlock(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CMap{pGraphic_Device}
{
}

CBlock::CBlock(const CBlock& Prototype)
    : CMap(Prototype)
{
}

HRESULT CBlock::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBlock::Initialize(void* pArg)
{
    m_eLevelID = LEVEL_GAMEPLAY;
    m_szTextureID = TEXT("TTest");
    m_szBufferType = TEXT("Cube");

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CBlock::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

EVENT CBlock::Update(_float fTimeDelta)
{
    return __super::Update(fTimeDelta);
}

void CBlock::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CBlock::Render()
{
    return __super::Render();
}

HRESULT CBlock::Ready_Components(void* pArg)
{
    __super::Ready_Components(pArg);

    CCollider_AABB_Cube::DESC ColliderDesc{};
    ColliderDesc.pTransform = m_pTransformCom;
    ColliderDesc.vScale = m_pTransformCom->Compute_Scaled();

    /* For.Com_Collider */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB_Cube"),
        TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
        return E_FAIL;
}

CBlock* CBlock::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CBlock* pInstance = new CBlock(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CBlock");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBlock::Clone(void* pArg)
{
    CBlock* pInstance = new CBlock(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Clone : CBlock");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBlock::Free()
{
    __super::Free();
}
