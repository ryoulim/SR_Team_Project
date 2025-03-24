#include "VIBuffer_BuildingW.h"

CVIBuffer_BuildingW::CVIBuffer_BuildingW(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CVIBuffer{pGraphic_Device}
{
}

CVIBuffer_BuildingW::CVIBuffer_BuildingW(const CVIBuffer_BuildingW& Prototype)
    :CVIBuffer(Prototype)
{
}

HRESULT CVIBuffer_BuildingW::Initialize_Prototype()
{
	m_iNumVertices = 16;					// 버텍스 갯수
	m_iVertexStride = sizeof(VTXPOSNORTEX);	// 버텍스 크기
	m_iFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;		// 버텍스 타입
	m_iNumPritimive = 12;					// 삼각형 갯수

	m_iIndexStride = 2;						// 인덱스 크기
	m_iNumIndices = 36;						// 인덱스 갯수
	m_eIndexFormat = D3DFMT_INDEX16;		// 인덱스 포맷

#pragma region VERTEX_BUFFER
	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXPOSNORTEX* pVertices = { nullptr };

	m_pVB->Lock(0, /*m_iNumVertices * m_iVertexStride*/0, reinterpret_cast<void**>(&pVertices), 0);
	
	pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.5f);
	pVertices[0].vTexcoord = _float2(0.f, 0.f);

	pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.5f);
	pVertices[1].vTexcoord = _float2(1.f, 0.f);

	pVertices[2].vPosition = _float3(0.5f, 0.5f, 0.25f);
	pVertices[2].vTexcoord = _float2(1.f, 1.f);

	pVertices[3].vPosition = _float3(0.25f, 0.5f, 0.f);
	pVertices[3].vTexcoord = _float2(0.f, 1.f);

	pVertices[4].vPosition = _float3(0.f, 0.5f, 0.f);
	pVertices[4].vTexcoord = _float2(1.f, 0.f);

	pVertices[5].vPosition = _float3(0.f, 0.5f, -0.25f);
	pVertices[5].vTexcoord = _float2(0.f, 0.f);

	pVertices[6].vPosition = _float3(-0.25f, 0.5f, -0.5f);
	pVertices[6].vTexcoord = _float2(0.f, 1.f);

	pVertices[7].vPosition = _float3(-0.5f, 0.5f, -0.5f);
	pVertices[7].vTexcoord = _float2(1.f, 1.f);


	pVertices[8].vPosition = _float3(-0.5f, -0.5f, 0.5f);
	pVertices[8].vTexcoord = _float2(0.f, 0.f);

	pVertices[9].vPosition = _float3(0.5f, -0.5f, 0.5f);
	pVertices[9].vTexcoord = _float2(1.f, 0.f);

	pVertices[10].vPosition = _float3(0.5f, -0.5f, 0.25f);
	pVertices[10].vTexcoord = _float2(1.f, 1.f);

	pVertices[11].vPosition = _float3(0.25f, -0.5f, 0.f);
	pVertices[11].vTexcoord = _float2(0.f, 1.f);

	pVertices[12].vPosition = _float3(0.f, -0.5f, 0.f);
	pVertices[12].vTexcoord = _float2(1.f, 0.f);

	pVertices[13].vPosition = _float3(0.f, -0.5f, -0.25f);
	pVertices[13].vTexcoord = _float2(0.f, 0.f);

	pVertices[14].vPosition = _float3(-0.25f, -0.5f, -0.5f);
	pVertices[14].vTexcoord = _float2(0.f, 1.f);

	pVertices[15].vPosition = _float3(-0.5f, -0.5f, -0.5f);
	pVertices[15].vTexcoord = _float2(1.f, 1.f);

#pragma region 법선
	for (_uint i = 0; i < m_iNumVertices; i++)
	{
		pVertices[i].vNormal = _float3(0.f, 0.f, 0.f);
	}

	//면1 (정면), 인덱스 : 0,1,2,3
	pVertices[0].vNormal += ComputeNormalVector(pVertices, 0, 1, 2);
	pVertices[1].vNormal += ComputeNormalVector(pVertices, 0, 1, 2);
	pVertices[2].vNormal += ComputeNormalVector(pVertices, 0, 1, 2);
	pVertices[3].vNormal += ComputeNormalVector(pVertices, 0, 2, 3);

	//면2 (후면), 인덱스 : 5,4,7,6
	pVertices[5].vNormal += ComputeNormalVector(pVertices, 5, 4, 7);
	pVertices[4].vNormal += ComputeNormalVector(pVertices, 5, 4, 7);
	pVertices[7].vNormal += ComputeNormalVector(pVertices, 5, 4, 7);
	pVertices[6].vNormal += ComputeNormalVector(pVertices, 5, 7, 6);
	
	//면3 (왼쪽), 인덱스 : 4,0,3,7
	pVertices[4].vNormal += ComputeNormalVector(pVertices, 4, 0, 3);
	pVertices[0].vNormal += ComputeNormalVector(pVertices, 4, 0, 3);
	pVertices[3].vNormal += ComputeNormalVector(pVertices, 4, 0, 3);
	pVertices[7].vNormal += ComputeNormalVector(pVertices, 4, 3, 7);

	//면4 (오른쪽), 인덱스 : 1,5,6,2
	pVertices[1].vNormal += ComputeNormalVector(pVertices, 1, 5, 6);
	pVertices[5].vNormal += ComputeNormalVector(pVertices, 1, 5, 6);
	pVertices[6].vNormal += ComputeNormalVector(pVertices, 1, 5, 6);
	pVertices[2].vNormal += ComputeNormalVector(pVertices, 1, 6, 2);
	
	//면5 (위), 인덱스 : 4,5,1,0
	pVertices[4].vNormal += ComputeNormalVector(pVertices, 4, 5, 1);
	pVertices[5].vNormal += ComputeNormalVector(pVertices, 4, 5, 1);
	pVertices[1].vNormal += ComputeNormalVector(pVertices, 4, 5, 1);
	pVertices[0].vNormal += ComputeNormalVector(pVertices, 4, 1, 0);

	//면6 (아래), 인덱스 : 6,7,3,2
	pVertices[6].vNormal += ComputeNormalVector(pVertices, 6, 7, 3);
	pVertices[7].vNormal += ComputeNormalVector(pVertices, 6, 7, 3);
	pVertices[3].vNormal += ComputeNormalVector(pVertices, 6, 7, 3);
	pVertices[2].vNormal += ComputeNormalVector(pVertices, 6, 3, 2);
#pragma endregion

	// 텍스처용 추가
	// (4)
	pVertices[8].vPosition = _float3(-0.5f, 0.5f, 0.5f);
	pVertices[8].vTexcoord = _float2(0.f, 1.f);

	// (5)
	pVertices[9].vPosition = _float3(0.5f, 0.5f, 0.5f);
	pVertices[9].vTexcoord = _float2(1.f, 1.f);

	// (6)
	pVertices[10].vPosition = _float3(0.5f, -0.5f, 0.5f);
	pVertices[10].vTexcoord = _float2(1.f, 0.f);

	// (7)
	pVertices[11].vPosition = _float3(-0.5f, -0.5f, 0.5f);
	pVertices[11].vTexcoord = _float2(0.f, 0.f);

	pVertices[8].vNormal += pVertices[4].vNormal;
	pVertices[9].vNormal += pVertices[5].vNormal;
	pVertices[10].vNormal += pVertices[6].vNormal;
	pVertices[11].vNormal += pVertices[7].vNormal;

	m_pVB->Unlock();

#pragma endregion

#pragma region INDEX_BUFFER
	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	_ushort* pIndices = { nullptr };

	m_pIB->Lock(0, 0, reinterpret_cast<void**>(&pIndices), 0);

 	Set_IndexBuffer(pIndices, 0, 0, 1, 2, 3);
	Set_IndexBuffer(pIndices, 6, 1, 5, 6, 2);
	Set_IndexBuffer(pIndices, 12, 5, 4, 7, 6);
	Set_IndexBuffer(pIndices, 18, 4, 0, 3, 7);
	Set_IndexBuffer(pIndices, 24, 8, 9, 1, 0);
	Set_IndexBuffer(pIndices, 30, 3, 2, 10, 11);

	m_pIB->Unlock();
#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_BuildingW::Initialize(void* pArg)
{
    return S_OK;
}

void CVIBuffer_BuildingW::Set_IndexBuffer(_ushort* pIndices, _uint StartIndex, _uint LT, _uint RT, _uint RB, _uint LB)
{
	pIndices[StartIndex++] = LT;
	pIndices[StartIndex++] = RT;
	pIndices[StartIndex++] = RB;

	pIndices[StartIndex++] = LT;
	pIndices[StartIndex++] = RB;
	pIndices[StartIndex++] = LB;
}

VECTOR CVIBuffer_BuildingW::ComputeNormalVector(VTXPOSNORTEX* pVertices, _uint vertex1, _uint vertex2, _uint vertex3)
{
	return __super::ComputeNormal(&(pVertices[vertex1].vPosition), &(pVertices[vertex2].vPosition), &(pVertices[vertex3].vPosition));
}

CVIBuffer_BuildingW* CVIBuffer_BuildingW::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_BuildingW* pInstance = new CVIBuffer_BuildingW(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_BuildingW");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_BuildingW::Clone(void* pArg)
{
	CVIBuffer_BuildingW* pInstance = new CVIBuffer_BuildingW(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CVIBuffer_BuildingW");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_BuildingW::Free()
{
	__super::Free();
}
