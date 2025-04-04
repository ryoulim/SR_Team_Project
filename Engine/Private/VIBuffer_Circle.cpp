#include "VIBuffer_Circle.h"

CVIBuffer_Circle::CVIBuffer_Circle(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer{ pGraphic_Device }
{
}

CVIBuffer_Circle::CVIBuffer_Circle(const CVIBuffer& Prototype)
	: CVIBuffer(Prototype)
{
}

HRESULT CVIBuffer_Circle::Initialize_Prototype()
{
	m_iNumVertices = 20;
	m_iVertexStride = sizeof(VTXPOSTEX);					// 버텍스 크기
	m_iFVF = D3DFVF_XYZ | D3DFVF_TEX1;		// 버텍스 타입
	m_iNumPritimive = m_iNumVertices;

	m_iIndexStride = 2;
	m_iNumIndices = m_iNumVertices+1;
	m_eIndexFormat = D3DFMT_INDEX16;

#pragma region VERTEX_BUFFER
	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXPOSTEX* pVertices = { nullptr };

	m_pVB->Lock(0, /*m_iNumVertices * m_iVertexStride*/0, reinterpret_cast<void**>(&pVertices), 0);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		pVertices[i].vPosition = _float3(0.5f * cosf(RADIAN(-360.f / m_iNumVertices * i)), 
			0.5f * sinf(RADIAN(-360.f / m_iNumVertices * i)), 0.f);
		pVertices[i].vTexcoord = _float2((_float)i, 0.f);
	}

	m_pVB->Unlock();
#pragma endregion


#pragma region INDEX_BUFFER
	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	_ushort* pIndices = { nullptr };

	m_pIB->Lock(0, 0, reinterpret_cast<void**>(&pIndices), 0);

 	//_uint IndexNum{};
	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		pIndices[i] = i;
	}

	pIndices[m_iNumVertices] = 0;

	m_pIB->Unlock();
#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_Circle::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_Circle::Render(_uint Surface)
{
	return m_pGraphic_Device->DrawIndexedPrimitive(D3DPT_LINESTRIP, 0, 0, m_iNumVertices, 0, m_iNumPritimive);
}

CVIBuffer_Circle* CVIBuffer_Circle::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_Circle* pInstance = new CVIBuffer_Circle(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Circle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Circle::Clone(void* pArg)
{
	CVIBuffer_Circle* pInstance = new CVIBuffer_Circle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CVIBuffer_Circle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Circle::Free()
{
	__super::Free();


}
