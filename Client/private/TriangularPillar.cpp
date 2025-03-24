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
