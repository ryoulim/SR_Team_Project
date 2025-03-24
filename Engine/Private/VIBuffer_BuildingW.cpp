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
	m_iNumVertices = 24;					// 버텍스 갯수
	m_iVertexStride = sizeof(VTXPOSNORTEX);	// 버텍스 크기
	m_iFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;		// 버텍스 타입
	m_iNumPritimive = 36;					// 삼각형 갯수

	m_iIndexStride = 2;						// 인덱스 크기
	m_iNumIndices = 108;						// 인덱스 갯수
	m_eIndexFormat = D3DFMT_INDEX16;		// 인덱스 포맷

#pragma region VERTEX_BUFFER
	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXPOSNORTEX* pVertices = { nullptr };

	m_pVB->Lock(0, /*m_iNumVertices * m_iVertexStride*/0, reinterpret_cast<void**>(&pVertices), 0);
	
	pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.5f);
	pVertices[0].vTexcoord = _float2(0.f, 0.f);//

	pVertices[1].vPosition = _float3(0.25f, 0.5f, 0.5f);
	pVertices[1].vTexcoord = _float2(1.f, 0.f);

	pVertices[2].vPosition = _float3(0.5f, 0.5f, 0.5f);
	pVertices[2].vTexcoord = _float2(7.f, 0.f);//

	pVertices[3].vPosition = _float3(0.5f, 0.5f, 0.25f);
	pVertices[3].vTexcoord = _float2(6.f, 0.f);//

	pVertices[4].vPosition = _float3(0.25f, 0.5f, 0.f);
	pVertices[4].vTexcoord = _float2(5.f, 0.f);//

	pVertices[5].vPosition = _float3(0.f, 0.5f, 0.f);
	pVertices[5].vTexcoord = _float2(4.f, 0.f);//

	pVertices[6].vPosition = _float3(0.f, 0.5f, -0.25f);
	pVertices[6].vTexcoord = _float2(3.f, 0.f);//

	pVertices[7].vPosition = _float3(-0.25f, 0.5f, -0.5f);
	pVertices[7].vTexcoord = _float2(2.f, 0.f);//

	pVertices[8].vPosition = _float3(-0.5f, 0.5f, -0.5f);
	pVertices[8].vTexcoord = _float2(1.f, 0.f);//

	pVertices[9].vPosition = _float3(-0.5f, 0.5f, -0.25f);
	pVertices[9].vTexcoord = _float2(1.f, 0.f);

	pVertices[10].vPosition = _float3(-0.25f, 0.5f, 0.f);
	pVertices[10].vTexcoord = _float2(1.f, 1.f);

	pVertices[11].vPosition = _float3(0.f, 0.5f, 0.25f);
	pVertices[11].vTexcoord = _float2(0.f, 1.f);

	pVertices[12].vPosition = _float3(-0.5f, -0.5f, 0.5f);
	pVertices[12].vTexcoord = _float2(0.f, 1.f);//

	pVertices[13].vPosition = _float3(0.25f, -0.5f, 0.5f);
	pVertices[13].vTexcoord = _float2(1.f, 0.f);

	pVertices[14].vPosition = _float3(0.5f, -0.5f, 0.5f);
	pVertices[14].vTexcoord = _float2(7.f, 1.f);//

	pVertices[15].vPosition = _float3(0.5f, -0.5f, 0.25f);
	pVertices[15].vTexcoord = _float2(6.f, 1.f);//

	pVertices[16].vPosition = _float3(0.25f, -0.5f, 0.f);
	pVertices[16].vTexcoord = _float2(5.f, 1.f);//

	pVertices[17].vPosition = _float3(0.f, -0.5f, 0.f);
	pVertices[17].vTexcoord = _float2(4.f, 1.f);//

	pVertices[18].vPosition = _float3(0.f, -0.5f, -0.25f);
	pVertices[18].vTexcoord = _float2(3.f, 1.f);//

	pVertices[19].vPosition = _float3(-0.25f, -0.5f, -0.5f);
	pVertices[19].vTexcoord = _float2(2.f, 1.f);//

	pVertices[20].vPosition = _float3(-0.5f, -0.5f, -0.5f);
	pVertices[20].vTexcoord = _float2(1.f, 1.f);//

	pVertices[21].vPosition = _float3(-0.5f, -0.5f, -0.25f);
	pVertices[21].vTexcoord = _float2(1.f, 0.f);

	pVertices[22].vPosition = _float3(-0.25f, -0.5f, 0.f);
	pVertices[22].vTexcoord = _float2(1.f, 1.f);

	pVertices[23].vPosition = _float3(0.f, -0.5f, 0.25f);
	pVertices[23].vTexcoord = _float2(0.f, 1.f);

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
	Set_IndexBuffer(pIndices, 0, 8, 7, 19, 20);
	pVertices[8].vNormal += ComputeNormalVector(pVertices, 8, 7, 19);
	pVertices[7].vNormal += ComputeNormalVector(pVertices, 8, 7, 19);
	pVertices[19].vNormal += ComputeNormalVector(pVertices, 8, 7, 19);
	pVertices[20].vNormal += ComputeNormalVector(pVertices, 8, 19, 20);

	Set_IndexBuffer(pIndices, 6, 7, 6, 18, 19);
	pVertices[7].vNormal += ComputeNormalVector(pVertices, 7, 6, 18);
	pVertices[6].vNormal += ComputeNormalVector(pVertices, 7, 6, 18);
	pVertices[18].vNormal += ComputeNormalVector(pVertices, 7, 6, 18);
	pVertices[19].vNormal += ComputeNormalVector(pVertices, 7, 18, 19);

	Set_IndexBuffer(pIndices, 12, 5, 4, 16, 17);
	pVertices[5].vNormal += ComputeNormalVector(pVertices, 5, 4, 16);
	pVertices[4].vNormal += ComputeNormalVector(pVertices, 5, 4, 16);
	pVertices[16].vNormal += ComputeNormalVector(pVertices, 5, 4, 16);
	pVertices[17].vNormal += ComputeNormalVector(pVertices, 5, 16, 17);

	Set_IndexBuffer(pIndices, 18, 4, 3, 15, 16);
	pVertices[4].vNormal += ComputeNormalVector(pVertices, 4, 3, 15);
	pVertices[3].vNormal += ComputeNormalVector(pVertices, 4, 3, 15);
	pVertices[15].vNormal += ComputeNormalVector(pVertices, 4, 3, 15);
	pVertices[16].vNormal += ComputeNormalVector(pVertices, 4, 15, 16);
#pragma endregion

#pragma region 뒤
	Set_IndexBuffer(pIndices, 24, 2, 0, 12, 14);
	pVertices[2].vNormal += ComputeNormalVector(pVertices, 2, 0, 12);
	pVertices[0].vNormal += ComputeNormalVector(pVertices, 2, 0, 12);
	pVertices[12].vNormal += ComputeNormalVector(pVertices, 2, 0, 12);
	pVertices[14].vNormal += ComputeNormalVector(pVertices, 2, 12, 14);
#pragma endregion

#pragma region 왼쪽
	Set_IndexBuffer(pIndices, 30, 0, 8, 20, 12);
	pVertices[0].vNormal += ComputeNormalVector(pVertices, 0, 8, 20);
	pVertices[8].vNormal += ComputeNormalVector(pVertices, 0, 8, 20);
	pVertices[20].vNormal += ComputeNormalVector(pVertices, 0, 8, 20);
	pVertices[12].vNormal += ComputeNormalVector(pVertices, 0, 20, 12);
#pragma endregion

#pragma region 오른쪽
	Set_IndexBuffer(pIndices, 36, 6, 5, 17, 18);
	pVertices[6].vNormal += ComputeNormalVector(pVertices, 6, 5, 17);
	pVertices[5].vNormal += ComputeNormalVector(pVertices, 6, 5, 17);
	pVertices[17].vNormal += ComputeNormalVector(pVertices, 6, 5, 17);
	pVertices[18].vNormal += ComputeNormalVector(pVertices, 6, 17, 18);

	Set_IndexBuffer(pIndices, 42, 3, 2, 14, 15);
	pVertices[3].vNormal += ComputeNormalVector(pVertices, 3, 2, 14);
	pVertices[2].vNormal += ComputeNormalVector(pVertices, 3, 2, 14);
	pVertices[14].vNormal += ComputeNormalVector(pVertices, 3, 2, 14);
	pVertices[15].vNormal += ComputeNormalVector(pVertices, 3, 14, 15);
#pragma endregion

#pragma region 위
	Set_IndexBuffer(pIndices, 48, 10, 6, 7, 9);
	pVertices[10].vNormal += ComputeNormalVector(pVertices, 10, 6, 7);
	pVertices[6].vNormal += ComputeNormalVector(pVertices, 10, 6, 7);
	pVertices[7].vNormal += ComputeNormalVector(pVertices, 10, 6, 7);
	pVertices[9].vNormal += ComputeNormalVector(pVertices, 10, 7, 9);

	Set_IndexBuffer(pIndices, 54, 1, 3, 4, 11);
	pVertices[1].vNormal += ComputeNormalVector(pVertices, 1, 3, 4);
	pVertices[3].vNormal += ComputeNormalVector(pVertices, 1, 3, 4);
	pVertices[4].vNormal += ComputeNormalVector(pVertices, 1, 3, 4);
	pVertices[11].vNormal += ComputeNormalVector(pVertices, 1, 4, 11);

	pIndices[60] = 0; pIndices[61] = 1; pIndices[62] = 9;
	pVertices[0].vNormal += ComputeNormalVector(pVertices, 0, 1, 9);
	pVertices[1].vNormal += ComputeNormalVector(pVertices, 0, 1, 9);
	pVertices[9].vNormal += ComputeNormalVector(pVertices, 0, 1, 9);

	pIndices[63] = 8; pIndices[64] = 9; pIndices[65] = 7;
	pVertices[8].vNormal += ComputeNormalVector(pVertices, 8, 9, 7);
	pVertices[9].vNormal += ComputeNormalVector(pVertices, 8, 9, 7);
	pVertices[7].vNormal += ComputeNormalVector(pVertices, 8, 9, 7);

	pIndices[66] = 10; pIndices[67] = 5; pIndices[68] = 6;
	pVertices[10].vNormal += ComputeNormalVector(pVertices, 10, 5, 6);
	pVertices[5].vNormal += ComputeNormalVector(pVertices, 10, 5, 6);
	pVertices[6].vNormal += ComputeNormalVector(pVertices, 10, 5, 6);

	pIndices[69] = 10; pIndices[70] = 11; pIndices[71] = 5;
	pVertices[10].vNormal += ComputeNormalVector(pVertices, 10, 11, 5);
	pVertices[11].vNormal += ComputeNormalVector(pVertices, 10, 11, 5);
	pVertices[5].vNormal += ComputeNormalVector(pVertices, 10, 11, 5);

	pIndices[72] = 5; pIndices[73] = 11; pIndices[74] = 4;
	pVertices[5].vNormal += ComputeNormalVector(pVertices, 5, 11, 4);
	pVertices[11].vNormal += ComputeNormalVector(pVertices, 5, 11, 4);
	pVertices[4].vNormal += ComputeNormalVector(pVertices, 5, 11, 4);

	pIndices[75] = 1; pIndices[76] = 2; pIndices[77] = 3;
	pVertices[1].vNormal += ComputeNormalVector(pVertices, 1, 2, 3);
	pVertices[2].vNormal += ComputeNormalVector(pVertices, 1, 2, 3);
	pVertices[3].vNormal += ComputeNormalVector(pVertices, 1, 2, 3);
#pragma endregion

#pragma region 아래
	Set_IndexBuffer(pIndices, 78, 22, 21, 19, 18);
	pVertices[22].vNormal += ComputeNormalVector(pVertices, 22, 21, 19);
	pVertices[21].vNormal += ComputeNormalVector(pVertices, 22, 21, 19);
	pVertices[19].vNormal += ComputeNormalVector(pVertices, 22, 21, 19);
	pVertices[18].vNormal += ComputeNormalVector(pVertices, 22, 19, 18);

	Set_IndexBuffer(pIndices, 84, 13, 23, 16, 15);
	pVertices[13].vNormal += ComputeNormalVector(pVertices, 13, 23, 16);
	pVertices[23].vNormal += ComputeNormalVector(pVertices, 13, 23, 16);
	pVertices[16].vNormal += ComputeNormalVector(pVertices, 13, 23, 16);
	pVertices[15].vNormal += ComputeNormalVector(pVertices, 13, 16, 15);

	pIndices[90] = 12; pIndices[91] = 21; pIndices[92] = 13;
	pVertices[12].vNormal += ComputeNormalVector(pVertices, 12, 21, 13);
	pVertices[21].vNormal += ComputeNormalVector(pVertices, 12, 21, 13);
	pVertices[13].vNormal += ComputeNormalVector(pVertices, 12, 21, 13);

	pIndices[93] = 20; pIndices[94] = 19; pIndices[95] = 21;
	pVertices[20].vNormal += ComputeNormalVector(pVertices, 20, 19, 21);
	pVertices[19].vNormal += ComputeNormalVector(pVertices, 20, 19, 21);
	pVertices[21].vNormal += ComputeNormalVector(pVertices, 20, 19, 21);

	pIndices[96] = 22; pIndices[97] = 18; pIndices[98] = 17;
	pVertices[22].vNormal += ComputeNormalVector(pVertices, 22, 18, 17);
	pVertices[18].vNormal += ComputeNormalVector(pVertices, 22, 18, 17);
	pVertices[17].vNormal += ComputeNormalVector(pVertices, 22, 18, 17);

	pIndices[99] = 22; pIndices[100] = 17; pIndices[101] = 23;
	pVertices[22].vNormal += ComputeNormalVector(pVertices, 22, 17, 23);
	pVertices[17].vNormal += ComputeNormalVector(pVertices, 22, 17, 23);
	pVertices[23].vNormal += ComputeNormalVector(pVertices, 22, 17, 23);

	pIndices[102] = 17; pIndices[103] = 16; pIndices[104] = 23;
	pVertices[17].vNormal += ComputeNormalVector(pVertices, 17, 16, 23);
	pVertices[16].vNormal += ComputeNormalVector(pVertices, 17, 16, 23);
	pVertices[23].vNormal += ComputeNormalVector(pVertices, 17, 16, 23);

	pIndices[105] = 13; pIndices[106] = 15; pIndices[107] = 14;
	pVertices[13].vNormal += ComputeNormalVector(pVertices, 13, 15, 14);
	pVertices[15].vNormal += ComputeNormalVector(pVertices, 13, 15, 14);
	pVertices[14].vNormal += ComputeNormalVector(pVertices, 13, 15, 14);
#pragma endregion

	m_pVB->Unlock();
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
