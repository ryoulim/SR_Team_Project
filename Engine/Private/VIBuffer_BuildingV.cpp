#include "VIBuffer_BuildingV.h"

CVIBuffer_BuildingV::CVIBuffer_BuildingV(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CVIBuffer{pGraphic_Device}
{
}

CVIBuffer_BuildingV::CVIBuffer_BuildingV(const CVIBuffer_BuildingV& Prototype)
    :CVIBuffer(Prototype)
{
}

HRESULT CVIBuffer_BuildingV::Initialize_Prototype()
{
	m_iNumVertices = 25;					// 버텍스 갯수
	m_iVertexStride = sizeof(VTXPOSNORTEX);	// 버텍스 크기
	m_iFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;		// 버텍스 타입
	m_iNumPritimive = 25;					// 삼각형 갯수

	m_iIndexStride = 2;						// 인덱스 크기
	m_iNumIndices = 75;						// 인덱스 갯수
	m_eIndexFormat = D3DFMT_INDEX16;		// 인덱스 포맷

#pragma region VERTEX_BUFFER
	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXPOSNORTEX* pVertices = { nullptr };

	m_pVB->Lock(0, /*m_iNumVertices * m_iVertexStride*/0, reinterpret_cast<void**>(&pVertices), 0);
	
	pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.5f);
	pVertices[0].vTexcoord = _float2(3.f, 0.f);//

	pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.5f);
	pVertices[1].vTexcoord = _float2(5.f, 0.f);//

	pVertices[2].vPosition = _float3(0.5f, 0.5f, 0.125f);
	pVertices[2].vTexcoord = _float2(4.f, 0.f);//

	pVertices[3].vPosition = _float3(0.125f, 0.5f, 0.125f);
	pVertices[3].vTexcoord = _float2(3.f, 0.f);//

	pVertices[4].vPosition = _float3(-0.125f, 0.5f, -0.125f);
	pVertices[4].vTexcoord = _float2(2.f, 0.f);//

	pVertices[5].vPosition = _float3(-0.125f, 0.5f, -0.5f);
	pVertices[5].vTexcoord = _float2(1.f, 0.f);//

	pVertices[6].vPosition = _float3(-0.5f, 0.5f, -0.5f);
	pVertices[6].vTexcoord = _float2(0.f, 0.f);//

	pVertices[7].vPosition = _float3(-0.5f, -0.5f, 0.5f);
	pVertices[7].vTexcoord = _float2(3.f, 1.f);//

	pVertices[8].vPosition = _float3(0.25f, -0.5f, 0.5f);
	pVertices[8].vTexcoord = _float2(5.f, 1.f);//

	pVertices[9].vPosition = _float3(0.25f, -0.5f, 0.125f);
	pVertices[9].vTexcoord = _float2(4.f, 1.f);//

	pVertices[10].vPosition = _float3(0.125f, -0.5f, 0.125f);
	pVertices[10].vTexcoord = _float2(3.f, 1.f);//

	pVertices[11].vPosition = _float3(-0.125f, -0.5f, -0.125f);
	pVertices[11].vTexcoord = _float2(2.f, 1.f);//

	pVertices[12].vPosition = _float3(-0.125f, -0.5f, -0.25f);
	pVertices[12].vTexcoord = _float2(1.f, 1.f);//

	pVertices[13].vPosition = _float3(-0.5f, -0.5f, -0.25f);
	pVertices[13].vTexcoord = _float2(0.f, 1.f);//

	pVertices[14].vPosition = _float3(0.25f, 0.5f, 0.5f);
	pVertices[14].vTexcoord = _float2(1.f, 0.f);//

	pVertices[15].vPosition = _float3(0.25f, 0.5f, 0.125f);
	pVertices[15].vTexcoord = _float2(3.3f, 0.f);//

	pVertices[16].vPosition = _float3(-0.125f, 0.5f, -0.25f);
	pVertices[16].vTexcoord = _float2(1.7f, 0.f);//

	pVertices[17].vPosition = _float3(-0.5f, 0.5f, -0.25f);
	pVertices[17].vTexcoord = _float2(5.f, 0.f);//

	pVertices[18].vPosition = _float3(-0.125f, -0.5f, 0.125f);	//8과 13의 중점
	pVertices[18].vTexcoord = _float2(0.f, 1.f);

#pragma region 텍스쳐
	pVertices[19].vPosition = _float3(-0.125f, -0.5f, -0.25f);	//12
	pVertices[19].vTexcoord = _float2(1.7f, 1.f);//

	pVertices[20].vPosition = _float3(0.25f, -0.5f, 0.125f);	//9
	pVertices[20].vTexcoord = _float2(3.3f, 1.f);

	pVertices[21].vPosition = _float3(0.25f, -0.5f, 0.5f);		//8
	pVertices[21].vTexcoord = _float2(1.f, 1.f);

	pVertices[22].vPosition = _float3(-0.5f, -0.5f, -0.25f);	//13
	pVertices[22].vTexcoord = _float2(5.f, 1.f);

	pVertices[23].vPosition = _float3(0.5f, 0.5f, 0.5f);		//1
	pVertices[23].vTexcoord = _float2(0.3f, 0.f);

	pVertices[24].vPosition = _float3(-0.5f, 0.5f, -0.5f);		//6
	pVertices[24].vTexcoord = _float2(5.7f, 0.f);//
#pragma endregion

#pragma region 법선w
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
	Set_IndexBuffer(pIndices, 0, 6, 5, 12, 13);
	pVertices[6].vNormal += ComputeNormalVector(pVertices, 6, 5, 12);
	pVertices[5].vNormal += ComputeNormalVector(pVertices, 6, 5, 12);
	pVertices[12].vNormal += ComputeNormalVector(pVertices, 6, 5, 12);
	pVertices[13].vNormal += ComputeNormalVector(pVertices, 6, 12, 13);

	Set_IndexBuffer(pIndices, 6, 3, 15, 20, 10);
	pVertices[3].vNormal += ComputeNormalVector(pVertices, 3, 15, 20);
	pVertices[15].vNormal += ComputeNormalVector(pVertices, 3, 15, 20);
	pVertices[20].vNormal += ComputeNormalVector(pVertices, 3, 15, 20);
	pVertices[10].vNormal += ComputeNormalVector(pVertices, 3, 20, 10);

	pIndices[12] = 15; pIndices[13] = 2; pIndices[14] = 20;
	pVertices[15].vNormal += ComputeNormalVector(pVertices, 15, 2, 20);
	pVertices[2].vNormal += ComputeNormalVector(pVertices, 15, 2, 20);
	pVertices[20].vNormal += ComputeNormalVector(pVertices, 15, 2, 20);

	Set_IndexBuffer(pIndices, 15, 4, 3, 10, 11);
	pVertices[4].vNormal += ComputeNormalVector(pVertices, 4, 3, 10);
	pVertices[3].vNormal += ComputeNormalVector(pVertices, 4, 3, 10);
	pVertices[10].vNormal += ComputeNormalVector(pVertices, 4, 3, 10);
	pVertices[11].vNormal += ComputeNormalVector(pVertices, 4, 10, 11);
#pragma endregion

#pragma region 뒤
	Set_IndexBuffer(pIndices, 21, 14, 0, 7, 21);
	pVertices[14].vNormal += ComputeNormalVector(pVertices, 14, 0, 7);
	pVertices[0].vNormal += ComputeNormalVector(pVertices, 14, 0, 7);
	pVertices[7].vNormal += ComputeNormalVector(pVertices, 14, 0, 7);
	pVertices[21].vNormal += ComputeNormalVector(pVertices, 14, 7, 21);

	pIndices[27] = 23; pIndices[28] = 14; pIndices[29] = 21;
	pVertices[23].vNormal += ComputeNormalVector(pVertices, 23, 14, 21);
	pVertices[14].vNormal += ComputeNormalVector(pVertices, 23, 14, 21);
	pVertices[21].vNormal += ComputeNormalVector(pVertices, 23, 14, 21);
#pragma endregion

#pragma region 왼쪽
	Set_IndexBuffer(pIndices, 30, 0, 17, 22, 7);
	pVertices[0].vNormal += ComputeNormalVector(pVertices, 0, 17, 22);
	pVertices[17].vNormal += ComputeNormalVector(pVertices, 0, 17, 22);
	pVertices[22].vNormal += ComputeNormalVector(pVertices, 0, 17, 22);
	pVertices[7].vNormal += ComputeNormalVector(pVertices, 0, 22, 7);

	pIndices[36] = 17; pIndices[37] = 24; pIndices[38] = 22;
	pVertices[17].vNormal += ComputeNormalVector(pVertices, 17, 24, 22);
	pVertices[24].vNormal += ComputeNormalVector(pVertices, 17, 24, 22);
	pVertices[22].vNormal += ComputeNormalVector(pVertices, 17, 24, 22);
#pragma endregion

#pragma region 오른쪽
	pIndices[39] = 5; pIndices[40] = 16; pIndices[41] = 19;
	pVertices[5].vNormal += ComputeNormalVector(pVertices, 5, 16, 19);
	pVertices[16].vNormal += ComputeNormalVector(pVertices, 5, 16, 19);
	pVertices[19].vNormal += ComputeNormalVector(pVertices, 5, 16, 19);

	Set_IndexBuffer(pIndices, 42, 16, 4, 11, 19);
	pVertices[16].vNormal += ComputeNormalVector(pVertices, 16, 4, 11);
	pVertices[4].vNormal += ComputeNormalVector(pVertices, 16, 4, 11);
	pVertices[11].vNormal += ComputeNormalVector(pVertices, 16, 4, 11);
	pVertices[19].vNormal += ComputeNormalVector(pVertices, 16, 11, 19);

	Set_IndexBuffer(pIndices, 48, 2, 1, 8, 9);
	pVertices[2].vNormal += ComputeNormalVector(pVertices, 2, 1, 8);
	pVertices[1].vNormal += ComputeNormalVector(pVertices, 2, 1, 8);
	pVertices[8].vNormal += ComputeNormalVector(pVertices, 2, 1, 8);
	pVertices[9].vNormal += ComputeNormalVector(pVertices, 2, 8, 9);
#pragma endregion

#pragma region 위
	pIndices[54] = 0; pIndices[55] = 1; pIndices[56] = 6;
	pVertices[0].vNormal += ComputeNormalVector(pVertices, 0, 1, 6);
	pVertices[1].vNormal += ComputeNormalVector(pVertices, 0, 1, 6);
	pVertices[6].vNormal += ComputeNormalVector(pVertices, 0, 1, 6);

	pIndices[57] = 6; pIndices[58] = 4; pIndices[59] = 5;
	pVertices[6].vNormal += ComputeNormalVector(pVertices, 6, 4, 5);
	pVertices[4].vNormal += ComputeNormalVector(pVertices, 6, 4, 5);
	pVertices[5].vNormal += ComputeNormalVector(pVertices, 6, 4, 5);

	pIndices[60] = 3; pIndices[61] = 1; pIndices[62] = 2;
	pVertices[3].vNormal += ComputeNormalVector(pVertices, 3, 1, 2);
	pVertices[1].vNormal += ComputeNormalVector(pVertices, 3, 1, 2);
	pVertices[2].vNormal += ComputeNormalVector(pVertices, 3, 1, 2);
#pragma endregion

#pragma region 아래
	pIndices[63] = 8; pIndices[64] = 7; pIndices[65] = 13;
	pVertices[8].vNormal += ComputeNormalVector(pVertices, 8, 7, 13);
	pVertices[7].vNormal += ComputeNormalVector(pVertices, 8, 7, 13);
	pVertices[13].vNormal += ComputeNormalVector(pVertices, 8, 7, 13);

	pIndices[66] = 9; pIndices[67] = 8; pIndices[68] = 18;
	pVertices[9].vNormal += ComputeNormalVector(pVertices, 9, 8, 18);
	pVertices[8].vNormal += ComputeNormalVector(pVertices, 9, 8, 18);
	pVertices[18].vNormal += ComputeNormalVector(pVertices, 9, 8, 18);

	pIndices[69] = 10; pIndices[70] = 18; pIndices[71] = 11;
	pVertices[10].vNormal += ComputeNormalVector(pVertices, 10, 18, 11);
	pVertices[18].vNormal += ComputeNormalVector(pVertices, 10, 18, 11);
	pVertices[11].vNormal += ComputeNormalVector(pVertices, 10, 18, 11);

	pIndices[72] = 12; pIndices[73] = 18; pIndices[74] = 13;
	pVertices[12].vNormal += ComputeNormalVector(pVertices, 12, 18, 13);
	pVertices[18].vNormal += ComputeNormalVector(pVertices, 12, 18, 13);
	pVertices[13].vNormal += ComputeNormalVector(pVertices, 12, 18, 13);
#pragma endregion

	m_pVB->Unlock();
	m_pIB->Unlock();
#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_BuildingV::Initialize(void* pArg)
{
    return S_OK;
}

void CVIBuffer_BuildingV::Set_IndexBuffer(_ushort* pIndices, _uint StartIndex, _uint LT, _uint RT, _uint RB, _uint LB)
{
	pIndices[StartIndex++] = LT;
	pIndices[StartIndex++] = RT;
	pIndices[StartIndex++] = RB;

	pIndices[StartIndex++] = LT;
	pIndices[StartIndex++] = RB;
	pIndices[StartIndex++] = LB;
}

VECTOR CVIBuffer_BuildingV::ComputeNormalVector(VTXPOSNORTEX* pVertices, _uint vertex1, _uint vertex2, _uint vertex3)
{
	return __super::ComputeNormal(&(pVertices[vertex1].vPosition), &(pVertices[vertex2].vPosition), &(pVertices[vertex3].vPosition));
}

CVIBuffer_BuildingV* CVIBuffer_BuildingV::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_BuildingV* pInstance = new CVIBuffer_BuildingV(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_BuildingV");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_BuildingV::Clone(void* pArg)
{
	CVIBuffer_BuildingV* pInstance = new CVIBuffer_BuildingV(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CVIBuffer_BuildingV");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_BuildingV::Free()
{
	__super::Free();
}
