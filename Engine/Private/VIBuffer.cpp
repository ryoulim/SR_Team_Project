#include "VIBuffer.h"

CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CComponent { pGraphic_Device }
{
}

CVIBuffer::CVIBuffer(const CVIBuffer& Prototype)
    : CComponent(Prototype)
    , m_pVB { Prototype.m_pVB }
    , m_pIB { Prototype.m_pIB }
    , m_iNumVertices { Prototype.m_iNumVertices }
    , m_iVertexStride { Prototype.m_iVertexStride }
    , m_iNumIndices { Prototype.m_iNumIndices }
    , m_iIndexStride { Prototype.m_iIndexStride }
    , m_iFVF { Prototype.m_iFVF }
    , m_iNumPritimive { Prototype.m_iNumPritimive }
{
    Safe_AddRef(m_pVB);
    Safe_AddRef(m_pIB);
}

HRESULT CVIBuffer::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CVIBuffer::Initialize(void* pArg)
{
    return S_OK;
}

HRESULT CVIBuffer::Render(_uint Surface)
{  
    /* �������� �̿��ؼ� ���س��� ��Ĵ�� �������� �׷���.  */
    /*m_pGraphic_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_iNumPritimive);*/

    return m_pGraphic_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iNumVertices, 0, m_iNumPritimive);
}

HRESULT CVIBuffer::Bind_Buffers()
{
    /* �������۸� ��ġ�� ���ε��Ѵ�. */
    m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, m_iVertexStride);    

    m_pGraphic_Device->SetIndices(m_pIB);

    /* ��ġ�� �˾Ƽ� �� ������ �̿��Ͽ� Ư�� ������ ������ �� �ֵ��� */
    /* �� ������ ������ �˷��ش�. */
    m_pGraphic_Device->SetFVF(m_iFVF);

    return S_OK;
}

LPDIRECT3DVERTEXBUFFER9 CVIBuffer::DeepCopyVertexBuffer(LPDIRECT3DVERTEXBUFFER9 pOriginVB)
{
    if (nullptr == pOriginVB)
        return nullptr;

    D3DVERTEXBUFFER_DESC VBDesc = {};
    pOriginVB->GetDesc(&VBDesc);

    LPDIRECT3DVERTEXBUFFER9 pCloneVB = { nullptr };
    if (FAILED(m_pGraphic_Device->CreateVertexBuffer(VBDesc.Size, VBDesc.Usage, VBDesc.FVF, VBDesc.Pool,
        &pCloneVB, nullptr)))
        return nullptr;

    void* pTmp{ nullptr };
    void* pSrc{ nullptr };

    if (FAILED(pOriginVB->Lock(0, 0, &pTmp, D3DLOCK_READONLY)))
    {
        pCloneVB->Release();
        return nullptr;
    }

    if (FAILED(pCloneVB->Lock(0, 0, &pSrc, 0)))
    {
        pOriginVB->Unlock();
        pCloneVB->Release();
        return nullptr;
    }

    memcpy(pSrc, pTmp, VBDesc.Size);

    pOriginVB->Unlock();
    pCloneVB->Unlock();

    return pCloneVB;
}

void CVIBuffer::Set_VertexBuffer(LPDIRECT3DVERTEXBUFFER9 pNewVB)
{
    Safe_Release(m_pVB);
    m_pVB = pNewVB;
    Safe_AddRef(m_pVB);
}


HRESULT CVIBuffer::Create_VertexBuffer()
{
    if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iNumVertices * m_iVertexStride,
        0/*D3DUSAGE_DYNAMIC*/, m_iFVF, D3DPOOL_MANAGED, &m_pVB, 0)))
        return E_FAIL;

    return S_OK;
}

HRESULT CVIBuffer::Create_IndexBuffer()
{
 
     if (FAILED(m_pGraphic_Device->CreateIndexBuffer(m_iNumIndices * m_iIndexStride,
        0, m_eIndexFormat, D3DPOOL_MANAGED, &m_pIB, 0)))
        return E_FAIL;

    return S_OK;
}

VECTOR CVIBuffer::ComputeNormal(_float3* p0, _float3* p1, _float3* p2)
{
    VECTOR u = *p1 - *p0;
    VECTOR v = *p2 - *p0;
    
    return (u.Cross(v)).Normalize();
}

void CVIBuffer::Free()
{
    __super::Free();

    Safe_Release(m_pIB);
    Safe_Release(m_pVB);
}

void CVIBuffer::Test()
{
    //ID3DXMesh*              m_pMeshOcc = { nullptr };   //��� �޽�
    //D3DMATERIAL9            m_matOcc{};
    //LPDIRECT3DTEXTURE9      m_ptexocc = { nullptr };

    //ID3DXMesh*              m_pMeshFloor = { nullptr }; //�׸��ڰ� �帮���� ��� �޽�
    //D3DMATERIAL9            m_matOcc{};
    //LPDIRECT3DTEXTURE9      m_ptexFloor = { nullptr };

    ////D3DXVECTOR4 m_vLightPos; //�׸��ڸ� ���� ������ ��ġ
    ////D3DXPLANE   m_planeFloor; //�׸��ڰ� �׷��� ��� ������
    ////D3DXMATRIXA16  m_matWorld; //���� ��� ������ �ϱ� ���ؼ�

   


    //
    ///*D3DXMATRIX* WINAPI D3DXMatrixShadow
    //(D3DXMATRIX * pOut, CONST D3DXVECTOR4 * pLight,
    //    CONST D3DXPLANE * pPlane);
    //pPlane�� ������ ����� �������� �ְ� 
    //pLight�� ������ ��ġ�� ������
    //��������� pOut�� ��ȯ���ش�.
    //�� �����
    //���� ��ȯ -> �׸���(shadow) ��ȯ -> ī�޶�(view)��ȯ -> ���� ��ȯ*/
    //
    ////�޽� : �������� ���ΰ�

    ////1. �޽ø� �о���δ�. (�ٴ� �޽� ����, ��ü �޽� ����)
    ////2. ������ ��ġ�� ��� �������� �����Ѵ�.
    ////3. 
    //

    //D3DXMATRIXA16 matShadow;

    //D3DXVECTOR4 m_vLightPos; //�׸��ڸ� ���� ������ ��ġ
    //D3DXPLANE   m_planeFloor; //�׸��ڰ� �׷��� ��� ������
    //D3DXMATRIXA16  m_matWorld; //���� ��� ������ �ϱ� ���ؼ�

    //D3DXMatrixShadow(&matShadow, &m_vLightPos, &m_planeFloor);

    //m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, FALSE);

    
    
}
