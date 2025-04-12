#include "VIBuffer_RaceLandscape.h"

CVIBuffer_RaceLandscape::CVIBuffer_RaceLandscape(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CVIBuffer{pGraphic_Device}
{
}

CVIBuffer_RaceLandscape::CVIBuffer_RaceLandscape(const CVIBuffer_RaceLandscape& Prototype)
    :CVIBuffer(Prototype)
{
}

HRESULT CVIBuffer_RaceLandscape::Initialize_Prototype()
{
	m_iNumber = 180;										// ±âµÕ? °¹¼ö
	m_iNumVertices = 4 * (m_iNumber + 1);					// ¹öÅØ½º °¹¼ö
	m_iVertexStride = sizeof(VTXPOSNORTEX);					// ¹öÅØ½º Å©±â
	m_iFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;		// ¹öÅØ½º Å¸ÀÔ
	m_iNumPritimive = 8 * m_iNumber + 4;					// »ï°¢Çü °¹¼ö

	m_iIndexStride = 2;						// ÀÎµ¦½º Å©±â
	m_iNumIndices = m_iNumPritimive * 3;	// ÀÎµ¦½º °¹¼ö
	m_eIndexFormat = D3DFMT_INDEX16;		// ÀÎµ¦½º Æ÷¸Ë

#pragma region VERTEX_BUFFER
	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXPOSNORTEX* pVertices = { nullptr };

	m_pVB->Lock(0, /*m_iNumVertices * m_iVertexStride*/0, reinterpret_cast<void**>(&pVertices), 0);
	
	for (_uint i = 0; i < m_iNumVertices; i++)
	{
		pVertices[i].vTexcoord = _float2(0.f, 0.f);
		pVertices[i].vNormal = _float3(0.f, 0.f, 0.f);
	}

	for (_uint i = 0; i <= m_iNumber; i++)
	{
		pVertices[4 * i].vPosition = _float3(-0.5f, 0.5f, -0.5f + i);
		pVertices[4 * i + 1].vPosition = _float3(0.25f, 0.5f, -0.5f + i);
		pVertices[4 * i + 2].vPosition = _float3(0.5f, -0.5f, -0.5f + i);
		pVertices[4 * i + 3].vPosition = _float3(-0.25f, -0.5f, -0.5f + i);

		pVertices[4 * i + 1].vTexcoord = _float2(static_cast<_float>(i), 0);
		pVertices[4 * i + 2].vTexcoord = _float2(static_cast<_float>(i), 1);
	}
#pragma endregion

#pragma region INDEX_BUFFER
	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	_ushort* pIndices = { nullptr };

	m_pIB->Lock(0, 0, reinterpret_cast<void**>(&pIndices), 0);

	Set_IndexBuffer(pIndices, 0, 0, 1, 2, 3);
	Set_IndexBuffer(pIndices, 6, 4 * m_iNumber + 1, 4 * m_iNumber , 4 * m_iNumber + 3, 4 * m_iNumber + 2);

	_uint BufferIndex = 12;

	for (_uint i = 0; i < m_iNumber; i++)
	{
		_uint Index[8] = {
			4 * i,
			4 * i + 1,
			4 * i + 2,
			4 * i + 3,
			4 * (i + 1),
			4 * (i + 1) + 1,
			4 * (i + 1) + 2,
			4 * (i + 1) + 3
		};

		pIndices[BufferIndex++] = Index[4];
		pIndices[BufferIndex++] = Index[0];
		pIndices[BufferIndex++] = Index[3];

		pIndices[BufferIndex++] = Index[4];
		pIndices[BufferIndex++] = Index[3];
		pIndices[BufferIndex++] = Index[7];

		pIndices[BufferIndex++] = Index[4];
		pIndices[BufferIndex++] = Index[5];
		pIndices[BufferIndex++] = Index[1];

		pIndices[BufferIndex++] = Index[4];
		pIndices[BufferIndex++] = Index[1];
		pIndices[BufferIndex++] = Index[0];

		pIndices[BufferIndex++] = Index[1];
		pIndices[BufferIndex++] = Index[5];
		pIndices[BufferIndex++] = Index[6];

		pIndices[BufferIndex++] = Index[1];
		pIndices[BufferIndex++] = Index[6];
		pIndices[BufferIndex++] = Index[2];

		pIndices[BufferIndex++] = Index[6];
		pIndices[BufferIndex++] = Index[7];
		pIndices[BufferIndex++] = Index[3];

		pIndices[BufferIndex++] = Index[6];
		pIndices[BufferIndex++] = Index[3];
		pIndices[BufferIndex++] = Index[2];

		pVertices[Index[0]].vNormal += ComputeNormalVector(pVertices, Index[0], Index[1], Index[2]);
		pVertices[Index[1]].vNormal += ComputeNormalVector(pVertices, Index[0], Index[1], Index[2]);
		pVertices[Index[2]].vNormal += ComputeNormalVector(pVertices, Index[0], Index[1], Index[2]);
		pVertices[Index[3]].vNormal += ComputeNormalVector(pVertices, Index[0], Index[2], Index[3]);

		pVertices[Index[5]].vNormal += ComputeNormalVector(pVertices, Index[5], Index[4], Index[7]);
		pVertices[Index[4]].vNormal += ComputeNormalVector(pVertices, Index[5], Index[4], Index[7]);
		pVertices[Index[7]].vNormal += ComputeNormalVector(pVertices, Index[5], Index[4], Index[7]);
		pVertices[Index[6]].vNormal += ComputeNormalVector(pVertices, Index[5], Index[7], Index[6]);

		pVertices[Index[4]].vNormal += ComputeNormalVector(pVertices, Index[4], Index[0], Index[3]);
		pVertices[Index[0]].vNormal += ComputeNormalVector(pVertices, Index[4], Index[0], Index[3]);
		pVertices[Index[3]].vNormal += ComputeNormalVector(pVertices, Index[4], Index[0], Index[3]);
		pVertices[Index[7]].vNormal += ComputeNormalVector(pVertices, Index[4], Index[3], Index[7]);

		pVertices[Index[1]].vNormal += ComputeNormalVector(pVertices, Index[1], Index[5], Index[6]);
		pVertices[Index[5]].vNormal += ComputeNormalVector(pVertices, Index[1], Index[5], Index[6]);
		pVertices[Index[6]].vNormal += ComputeNormalVector(pVertices, Index[1], Index[5], Index[6]);
		pVertices[Index[2]].vNormal += ComputeNormalVector(pVertices, Index[1], Index[6], Index[2]);

		pVertices[Index[4]].vNormal += ComputeNormalVector(pVertices, Index[4], Index[5], Index[1]);
		pVertices[Index[5]].vNormal += ComputeNormalVector(pVertices, Index[4], Index[5], Index[1]);
		pVertices[Index[1]].vNormal += ComputeNormalVector(pVertices, Index[4], Index[5], Index[1]);
		pVertices[Index[0]].vNormal += ComputeNormalVector(pVertices, Index[4], Index[1], Index[0]);

		pVertices[Index[6]].vNormal += ComputeNormalVector(pVertices, Index[6], Index[7], Index[3]);
		pVertices[Index[7]].vNormal += ComputeNormalVector(pVertices, Index[6], Index[7], Index[3]);
		pVertices[Index[3]].vNormal += ComputeNormalVector(pVertices, Index[6], Index[7], Index[3]);
		pVertices[Index[2]].vNormal += ComputeNormalVector(pVertices, Index[6], Index[3], Index[2]);
	}
	m_pVB->Unlock();
	m_pIB->Unlock();
#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_RaceLandscape::Initialize(void* pArg)
{
    return S_OK;
}

void CVIBuffer_RaceLandscape::Set_IndexBuffer(_ushort* pIndices, _uint StartIndex, _uint LT, _uint RT, _uint RB, _uint LB)
{
	pIndices[StartIndex++] = LT;
	pIndices[StartIndex++] = RT;
	pIndices[StartIndex++] = RB;

	pIndices[StartIndex++] = LT;
	pIndices[StartIndex++] = RB;
	pIndices[StartIndex++] = LB;
}

VECTOR CVIBuffer_RaceLandscape::ComputeNormalVector(VTXPOSNORTEX* pVertices, _uint vertex1, _uint vertex2, _uint vertex3)
{
	return __super::ComputeNormal(&(pVertices[vertex1].vPosition), &(pVertices[vertex2].vPosition), &(pVertices[vertex3].vPosition));
}

CVIBuffer_RaceLandscape* CVIBuffer_RaceLandscape::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_RaceLandscape* pInstance = new CVIBuffer_RaceLandscape(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_RaceLandscape");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_RaceLandscape::Clone(void* pArg)
{
	CVIBuffer_RaceLandscape* pInstance = new CVIBuffer_RaceLandscape(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CVIBuffer_RaceLandscape");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_RaceLandscape::Free()
{
	__super::Free();
}
