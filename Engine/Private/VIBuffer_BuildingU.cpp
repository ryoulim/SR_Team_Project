#include "VIBuffer_BuildingU.h"

CVIBuffer_BuildingU::CVIBuffer_BuildingU(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CVIBuffer{pGraphic_Device}
{
}

CVIBuffer_BuildingU::CVIBuffer_BuildingU(const CVIBuffer_BuildingU& Prototype)
    :CVIBuffer(Prototype)
{
}

HRESULT CVIBuffer_BuildingU::Initialize_Prototype()
{
	m_iNumVertices = 16;					// 버텍스 갯수
	m_iVertexStride = sizeof(VTXPOSNORTEX);	// 버텍스 크기
	m_iFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;		// 버텍스 타입
	m_iNumPritimive = 28;					// 삼각형 갯수

	m_iIndexStride = 2;						// 인덱스 크기
	m_iNumIndices = 84;						// 인덱스 갯수
	m_eIndexFormat = D3DFMT_INDEX16;		// 인덱스 포맷

#pragma region VERTEX_BUFFER
	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXPOSNORTEX* pVertices = { nullptr };

	m_pVB->Lock(0, /*m_iNumVertices * m_iVertexStride*/0, reinterpret_cast<void**>(&pVertices), 0);
	
	pVertices[0].vPosition = _float3(-0.5f, 0.5f, -0.25f);
	pVertices[0].vTexcoord = _float2(0.f, 0.f);//

	pVertices[1].vPosition = _float3(0.f, 0.5f, -0.25f);
	pVertices[1].vTexcoord = _float2(7.f, 0.f);//

	pVertices[2].vPosition = _float3(0.25f, 0.5f, 0.f);
	pVertices[2].vTexcoord = _float2(6.f, 0.f);//

	pVertices[3].vPosition = _float3(0.25f, 0.5f, 0.5f);
	pVertices[3].vTexcoord = _float2(5.f, 0.f);//

	pVertices[4].vPosition = _float3(0.5f, 0.5f, 0.5f);
	pVertices[4].vTexcoord = _float2(4.f, 0.f);//

	pVertices[5].vPosition = _float3(0.5f, 0.5f, -0.25f);
	pVertices[5].vTexcoord = _float2(3.f, 0.f);//

	pVertices[6].vPosition = _float3(0.25f, 0.5f, -0.5f);
	pVertices[6].vTexcoord = _float2(2.f, 0.f);//

	pVertices[7].vPosition = _float3(-0.5f, 0.5f, -0.5f);
	pVertices[7].vTexcoord = _float2(1.f, 0.f);//

	pVertices[8].vPosition = _float3(-0.5f, -0.5f, -0.25f);
	pVertices[8].vTexcoord = _float2(0.f, 1.f);//

	pVertices[9].vPosition = _float3(0.f, -0.5f, -0.25f);
	pVertices[9].vTexcoord = _float2(7.f, 1.f);//

	pVertices[10].vPosition = _float3(0.25f, -0.5f, 0.f);
	pVertices[10].vTexcoord = _float2(6.f, 1.f);//

	pVertices[11].vPosition = _float3(0.25f, -0.5f, 0.5f);
	pVertices[11].vTexcoord = _float2(5.f, 1.f);//

	pVertices[12].vPosition = _float3(0.5f, -0.5f, 0.5f);
	pVertices[12].vTexcoord = _float2(4.f, 1.f);//

	pVertices[13].vPosition = _float3(0.5f, -0.5f, -0.25f);
	pVertices[13].vTexcoord = _float2(3.f, 1.f);//

	pVertices[14].vPosition = _float3(0.25f, -0.5f, -0.5f);
	pVertices[14].vTexcoord = _float2(2.f, 1.f);//

	pVertices[15].vPosition = _float3(-0.5f, -0.5f, -0.5f);
	pVertices[15].vTexcoord = _float2(1.f, 1.f);//

#pragma region 법선
	for (_uint i = 0; i < m_iNumVertices; i++)
	{
		pVertices[i].vNormal = _float3(0.f, 0.f, 0.f);
	}
#pragma endregion
	

#pragma endregion

#pragma region INDEX_BUFFER
	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	_ushort* pIndices = { nullptr };

	m_pIB->Lock(0, 0, reinterpret_cast<void**>(&pIndices), 0);

#pragma region 앞
	Set_IndexBuffer(pIndices, 0, 7, 6, 14, 15);
	pVertices[7].vNormal += ComputeNormalVector(pVertices, 7, 6, 14);
	pVertices[6].vNormal += ComputeNormalVector(pVertices, 7, 6, 14);
	pVertices[14].vNormal += ComputeNormalVector(pVertices, 7, 6, 14);
	pVertices[15].vNormal += ComputeNormalVector(pVertices, 7, 14, 15);

	Set_IndexBuffer(pIndices, 6, 6, 5, 13, 14);
	pVertices[6].vNormal += ComputeNormalVector(pVertices, 6, 5, 13);
	pVertices[5].vNormal += ComputeNormalVector(pVertices, 6, 5, 13);
	pVertices[13].vNormal += ComputeNormalVector(pVertices, 6, 5, 13);
	pVertices[14].vNormal += ComputeNormalVector(pVertices, 6, 13, 14);
#pragma endregion

#pragma region 뒤
	Set_IndexBuffer(pIndices, 12, 1, 0, 8, 9);
	pVertices[1].vNormal += ComputeNormalVector(pVertices, 1, 0, 8);
	pVertices[0].vNormal += ComputeNormalVector(pVertices, 1, 0, 8);
	pVertices[8].vNormal += ComputeNormalVector(pVertices, 1, 0, 8);
	pVertices[9].vNormal += ComputeNormalVector(pVertices, 1, 8, 9);

	Set_IndexBuffer(pIndices, 18, 2, 1, 9, 10);
	pVertices[2].vNormal += ComputeNormalVector(pVertices, 2, 1, 9);
	pVertices[1].vNormal += ComputeNormalVector(pVertices, 2, 1, 9);
	pVertices[9].vNormal += ComputeNormalVector(pVertices, 2, 1, 9);
	pVertices[10].vNormal += ComputeNormalVector(pVertices, 2, 9, 10);

	Set_IndexBuffer(pIndices, 24, 4, 3, 11, 12);
	pVertices[4].vNormal += ComputeNormalVector(pVertices, 4, 3, 11);
	pVertices[3].vNormal += ComputeNormalVector(pVertices, 4, 3, 11);
	pVertices[11].vNormal += ComputeNormalVector(pVertices, 4, 3, 11);
	pVertices[12].vNormal += ComputeNormalVector(pVertices, 4, 11, 12);
#pragma endregion

#pragma region 왼쪽
	Set_IndexBuffer(pIndices, 30, 0, 7, 15, 8);
	pVertices[0].vNormal += ComputeNormalVector(pVertices, 0, 7, 15);
	pVertices[7].vNormal += ComputeNormalVector(pVertices, 0, 7, 15);
	pVertices[15].vNormal += ComputeNormalVector(pVertices, 0, 7, 15);
	pVertices[8].vNormal += ComputeNormalVector(pVertices, 0, 15, 8);

	Set_IndexBuffer(pIndices, 36, 3, 2, 10, 11);
	pVertices[3].vNormal += ComputeNormalVector(pVertices, 3, 2, 10);
	pVertices[2].vNormal += ComputeNormalVector(pVertices, 3, 2, 10);
	pVertices[10].vNormal += ComputeNormalVector(pVertices, 3, 2, 10);
	pVertices[11].vNormal += ComputeNormalVector(pVertices, 3, 10, 11);
#pragma endregion

#pragma region 오른쪽
	Set_IndexBuffer(pIndices, 42, 5, 4, 12, 13);
	pVertices[5].vNormal += ComputeNormalVector(pVertices, 5, 4, 12);
	pVertices[4].vNormal += ComputeNormalVector(pVertices, 5, 4, 12);
	pVertices[12].vNormal += ComputeNormalVector(pVertices, 5, 4, 12);
	pVertices[13].vNormal += ComputeNormalVector(pVertices, 5, 12, 13);
#pragma endregion

#pragma region 위
	Set_IndexBuffer(pIndices, 48, 0, 1, 6, 7);
	pVertices[0].vNormal += ComputeNormalVector(pVertices, 0, 1, 6);
	pVertices[1].vNormal += ComputeNormalVector(pVertices, 0, 1, 6);
	pVertices[6].vNormal += ComputeNormalVector(pVertices, 0, 1, 6);
	pVertices[7].vNormal += ComputeNormalVector(pVertices, 0, 6, 7);

	Set_IndexBuffer(pIndices, 54, 1, 2, 5, 6);
	pVertices[1].vNormal += ComputeNormalVector(pVertices, 1, 2, 5);
	pVertices[2].vNormal += ComputeNormalVector(pVertices, 1, 2, 5);
	pVertices[5].vNormal += ComputeNormalVector(pVertices, 1, 2, 5);
	pVertices[6].vNormal += ComputeNormalVector(pVertices, 1, 5, 6);

	Set_IndexBuffer(pIndices, 60, 3, 4, 5, 2);
	pVertices[3].vNormal += ComputeNormalVector(pVertices, 3, 4, 5);
	pVertices[4].vNormal += ComputeNormalVector(pVertices, 3, 4, 5);
	pVertices[5].vNormal += ComputeNormalVector(pVertices, 3, 4, 5);
	pVertices[2].vNormal += ComputeNormalVector(pVertices, 3, 5, 2);
#pragma endregion

#pragma region 아래
	Set_IndexBuffer(pIndices, 66, 9, 8, 15, 14);
	pVertices[9].vNormal += ComputeNormalVector(pVertices, 9, 8, 15);
	pVertices[8].vNormal += ComputeNormalVector(pVertices, 9, 8, 15);
	pVertices[15].vNormal += ComputeNormalVector(pVertices, 9, 8, 15);
	pVertices[14].vNormal += ComputeNormalVector(pVertices, 9, 15, 14);

	Set_IndexBuffer(pIndices, 72, 10, 9, 14, 13);
	pVertices[10].vNormal += ComputeNormalVector(pVertices, 10, 9, 14);
	pVertices[9].vNormal += ComputeNormalVector(pVertices, 10, 9, 14);
	pVertices[14].vNormal += ComputeNormalVector(pVertices, 10, 9, 14);
	pVertices[13].vNormal += ComputeNormalVector(pVertices, 10, 14, 13);

	Set_IndexBuffer(pIndices, 78, 12, 11, 10, 13);
	pVertices[12].vNormal += ComputeNormalVector(pVertices, 12, 11, 10);
	pVertices[11].vNormal += ComputeNormalVector(pVertices, 12, 11, 10);
	pVertices[10].vNormal += ComputeNormalVector(pVertices, 12, 11, 10);
	pVertices[13].vNormal += ComputeNormalVector(pVertices, 12, 10, 13);
#pragma endregion

	m_pVB->Unlock();
	m_pIB->Unlock();
#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_BuildingU::Initialize(void* pArg)
{
    return S_OK;
}

void CVIBuffer_BuildingU::Set_IndexBuffer(_ushort* pIndices, _uint StartIndex, _uint LT, _uint RT, _uint RB, _uint LB)
{
	pIndices[StartIndex++] = LT;
	pIndices[StartIndex++] = RT;
	pIndices[StartIndex++] = RB;

	pIndices[StartIndex++] = LT;
	pIndices[StartIndex++] = RB;
	pIndices[StartIndex++] = LB;
}

VECTOR CVIBuffer_BuildingU::ComputeNormalVector(VTXPOSNORTEX* pVertices, _uint vertex1, _uint vertex2, _uint vertex3)
{
	return __super::ComputeNormal(&(pVertices[vertex1].vPosition), &(pVertices[vertex2].vPosition), &(pVertices[vertex3].vPosition));
}

CVIBuffer_BuildingU* CVIBuffer_BuildingU::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_BuildingU* pInstance = new CVIBuffer_BuildingU(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_BuildingU");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_BuildingU::Clone(void* pArg)
{
	CVIBuffer_BuildingU* pInstance = new CVIBuffer_BuildingU(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CVIBuffer_BuildingU");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_BuildingU::Free()
{
	__super::Free();
}
