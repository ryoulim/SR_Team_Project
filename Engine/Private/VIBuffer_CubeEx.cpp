#include "VIBuffer_CubeEx.h"

CVIBuffer_CubeEx::CVIBuffer_CubeEx(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer {pGraphic_Device}
{
}

CVIBuffer_CubeEx::CVIBuffer_CubeEx(const CVIBuffer_CubeEx& Prototype)
	: CVIBuffer(Prototype)
{
}

HRESULT CVIBuffer_CubeEx::Initialize_Prototype()
{
	m_iNumVertices = 8;
	m_iVertexStride = sizeof(VTXCUBE);
	m_iFVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);
	m_iNumPritimive = 12;

	m_iIndexStride = 2;
	m_iNumIndices = 36;
	m_eIndexFormat = D3DFMT_INDEX16;

#pragma region VERTEX_BUFFER
	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXCUBE* pVertices = { nullptr };

	m_pVB->Lock(0, /*m_iNumVertices * m_iVertexStride*/0, reinterpret_cast<void**>(&pVertices), 0);

	pVertices[0].vPosition = _float3(-0.5f, 0.5f, -0.5f);
	pVertices[0].vTexcoord = pVertices[0].vPosition;

	pVertices[1].vPosition = _float3(0.5f, 0.5f, -0.5f);
	pVertices[1].vTexcoord = pVertices[1].vPosition;

	pVertices[2].vPosition = _float3(0.5f, -0.5f, -0.5f);
	pVertices[2].vTexcoord = pVertices[2].vPosition;

	pVertices[3].vPosition = _float3(-0.5f, -0.5f, -0.5f);
	pVertices[3].vTexcoord = pVertices[3].vPosition;

	pVertices[4].vPosition = _float3(-0.5f, 0.5f, 0.5f);
	pVertices[4].vTexcoord = pVertices[4].vPosition;

	pVertices[5].vPosition = _float3(0.5f, 0.5f, 0.5f);
	pVertices[5].vTexcoord = pVertices[5].vPosition;

	pVertices[6].vPosition = _float3(0.5f, -0.5f, 0.5f);
	pVertices[6].vTexcoord = pVertices[6].vPosition;

	pVertices[7].vPosition = _float3(-0.5f, -0.5f, 0.5f);
	pVertices[7].vTexcoord = pVertices[7].vPosition;

#pragma endregion

#pragma region ¹ý¼±
	for (_uint i = 0; i < m_iNumPritimive; i++)
	{
		pVertices[i].vNormal = _float3(0.f, 0.f, 0.f);
	}

	//¸é1 (Á¤¸é), ÀÎµ¦½º : 0,1,2,3
	pVertices[0].vNormal += ComputeNormalVector(pVertices, 0, 1, 2);
	pVertices[1].vNormal += ComputeNormalVector(pVertices, 0, 1, 2);
	pVertices[2].vNormal += ComputeNormalVector(pVertices, 0, 1, 2);
	pVertices[3].vNormal += ComputeNormalVector(pVertices, 0, 2, 3);

	//¸é2 (ÈÄ¸é), ÀÎµ¦½º : 5,4,7,6
	pVertices[5].vNormal += ComputeNormalVector(pVertices, 5, 4, 7);
	pVertices[4].vNormal += ComputeNormalVector(pVertices, 5, 4, 7);
	pVertices[7].vNormal += ComputeNormalVector(pVertices, 5, 4, 7);
	pVertices[6].vNormal += ComputeNormalVector(pVertices, 5, 7, 6);

	//¸é3 (¿ÞÂÊ), ÀÎµ¦½º : 4,0,3,7
	pVertices[4].vNormal += ComputeNormalVector(pVertices, 4, 0, 3);
	pVertices[0].vNormal += ComputeNormalVector(pVertices, 4, 0, 3);
	pVertices[3].vNormal += ComputeNormalVector(pVertices, 4, 0, 3);
	pVertices[7].vNormal += ComputeNormalVector(pVertices, 4, 3, 7);

	//¸é4 (¿À¸¥ÂÊ), ÀÎµ¦½º : 1,5,6,2
	pVertices[1].vNormal += ComputeNormalVector(pVertices, 1, 5, 6);
	pVertices[5].vNormal += ComputeNormalVector(pVertices, 1, 5, 6);
	pVertices[6].vNormal += ComputeNormalVector(pVertices, 1, 5, 6);
	pVertices[2].vNormal += ComputeNormalVector(pVertices, 1, 6, 2);

	//¸é5 (À§), ÀÎµ¦½º : 4,5,1,0
	pVertices[4].vNormal += ComputeNormalVector(pVertices, 4, 5, 1);
	pVertices[5].vNormal += ComputeNormalVector(pVertices, 4, 5, 1);
	pVertices[1].vNormal += ComputeNormalVector(pVertices, 4, 5, 1);
	pVertices[0].vNormal += ComputeNormalVector(pVertices, 4, 1, 0);

	//¸é6 (¾Æ·¡), ÀÎµ¦½º : 6,7,3,2
	pVertices[6].vNormal += ComputeNormalVector(pVertices, 6, 7, 3);
	pVertices[7].vNormal += ComputeNormalVector(pVertices, 6, 7, 3);
	pVertices[3].vNormal += ComputeNormalVector(pVertices, 6, 7, 3);
	pVertices[2].vNormal += ComputeNormalVector(pVertices, 6, 3, 2);
#pragma endregion


#pragma region INDEX_BUFFER
	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	_ushort* pIndices = { nullptr };

	m_pIB->Lock(0, 0, reinterpret_cast<void**>(&pIndices), 0);

	/* +x */
	pIndices[0] = 1; pIndices[1] = 5; pIndices[2] = 6;
	pIndices[3] = 1; pIndices[4] = 6; pIndices[5] = 2;

	/* -x */
	pIndices[6] = 4; pIndices[7] = 0; pIndices[8] = 3;
	pIndices[9] = 4; pIndices[10] = 3; pIndices[11] = 7;


	/* +y */
	pIndices[12] = 4; pIndices[13] = 5; pIndices[14] = 1;
	pIndices[15] = 4; pIndices[16] = 1; pIndices[17] = 0;

	/* -y */
	pIndices[18] = 3; pIndices[19] = 2; pIndices[20] = 6;
	pIndices[21] = 3; pIndices[22] = 6; pIndices[23] = 7;

	/* +z */
	pIndices[24] = 5; pIndices[25] = 4; pIndices[26] = 7;
	pIndices[27] = 5; pIndices[28] = 7; pIndices[29] = 6;

	/* -z */
	pIndices[30] = 0; pIndices[31] = 1; pIndices[32] = 2;
	pIndices[33] = 0; pIndices[34] = 2; pIndices[35] = 3;

	m_pVB->Unlock();
	m_pIB->Unlock();
#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_CubeEx::Initialize(void* pArg)
{
	return S_OK;
}

_float3 CVIBuffer_CubeEx::ComputeNormalVector(VTXCUBE* pVertices, _uint vertex1, _uint vertex2, _uint vertex3)
{
	return __super::ComputeNormal(&(pVertices[vertex1].vPosition), &(pVertices[vertex2].vPosition), &(pVertices[vertex3].vPosition));
}

CVIBuffer_CubeEx* CVIBuffer_CubeEx::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_CubeEx* pInstance = new CVIBuffer_CubeEx(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_CubeEx");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_CubeEx::Clone(void* pArg)
{
	CVIBuffer_CubeEx* pInstance = new CVIBuffer_CubeEx(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVIBuffer_CubeEx");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_CubeEx::Free()
{
	__super::Free();
}
