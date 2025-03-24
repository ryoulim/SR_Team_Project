#include "TriangularPillar.h"

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
    m_eLevelID = LEVEL_GAMEPLAY;
    m_szTextureID = TEXT("Test");
    m_szBufferType = TEXT("TriangularPillar");

    //if (FAILED(__super::Initialize(pArg)))
    //    return E_FAIL;

        /* For.Com_Texture */
    if (FAILED(__super::Add_Component(m_eLevelID, _wstring(TEXT("Prototype_Component_Texture_")) + m_szTextureID,
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, _wstring(TEXT("Prototype_Component_VIBuffer_")) + m_szBufferType,
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* For.Com_Transform */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), pArg)))
        return E_FAIL;

    m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(100, 70, 100));
    m_pTransformCom->Scaling(_float3(50.f, 50.f, 50.f));

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
