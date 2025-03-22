#include "VIBuffer_BuildingH.h"

CVIBuffer_BuildingH::CVIBuffer_BuildingH(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CVIBuffer{pGraphic_Device}
{
}

CVIBuffer_BuildingH::CVIBuffer_BuildingH(const CVIBuffer_BuildingH& Prototype)
    :CVIBuffer(Prototype)
{
}

HRESULT CVIBuffer_BuildingH::Initialize_Prototype()
{
	m_iNumVertices = 40;					// 버텍스 갯수
	m_iVertexStride = sizeof(VTXPOSNORTEX);	// 버텍스 크기
	m_iFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;		// 버텍스 타입
	m_iNumPritimive = 44;					// 삼각형 갯수

	m_iIndexStride = 2;						// 인덱스 크기
	m_iNumIndices = 132;					// 인덱스 갯수
	m_eIndexFormat = D3DFMT_INDEX16;		// 인덱스 포맷

#pragma region VERTEX_BUFFER
	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXPOSNORTEX* pVertices = { nullptr };

	m_pVB->Lock(0, /*m_iNumVertices * m_iVertexStride*/0, reinterpret_cast<void**>(&pVertices), 0);
	
	pVertices[0].vPosition = _float3(-0.5f, 0.5f, -0.5f);
	pVertices[0].vTexcoord = _float2(0.f, 0.f);//

	pVertices[1].vPosition = _float3(-0.25f, 0.5f, -0.5f);
	pVertices[1].vTexcoord = _float2(1.f, 0.f);//

	pVertices[2].vPosition = _float3(0.25f, 0.5f, -0.5f);
	pVertices[2].vTexcoord = _float2(0.f, 0.f);//

	pVertices[3].vPosition = _float3(0.5f, 0.5f, -0.5f);
	pVertices[3].vTexcoord = _float2(1.f, 0.f);//

	pVertices[4].vPosition = _float3(0.5f, -0.5f, -0.5f);
	pVertices[4].vTexcoord = _float2(1.f, 1.f);//

	pVertices[5].vPosition = _float3(0.25f, -0.5f, -0.5f);
	pVertices[5].vTexcoord = _float2(0.f, 1.f);//

	pVertices[6].vPosition = _float3(-0.25f, -0.5f, -0.5f);
	pVertices[6].vTexcoord = _float2(1.f, 1.f);//

	pVertices[7].vPosition = _float3(-0.5f, -0.5f, -0.5f);
	pVertices[7].vTexcoord = _float2(0.f, 1.f);//

	pVertices[8].vPosition = _float3(-0.25f, 0.375f, -0.25f);
	pVertices[8].vTexcoord = _float2(0.f, 0.f);//

	pVertices[9].vPosition = _float3(0.25f, 0.375f, -0.25f);
	pVertices[9].vTexcoord = _float2(2.f, 0.f);//

	pVertices[10].vPosition = _float3(0.25f, -0.125f, -0.25f);
	pVertices[10].vTexcoord = _float2(2.f, 1.f);//

	pVertices[11].vPosition = _float3(-0.25f, -0.125f, -0.25f);
	pVertices[11].vTexcoord = _float2(0.f, 1.f);//


	pVertices[12].vPosition = _float3(-0.5f, 0.5f, 0.5f);
	pVertices[12].vTexcoord = _float2(1.f, 0.f);//

	pVertices[13].vPosition = _float3(-0.25f, 0.5f, 0.5f);
	pVertices[13].vTexcoord = _float2(0.f, 0.f);//

	pVertices[14].vPosition = _float3(0.25f, 0.5f, 0.5f);
	pVertices[14].vTexcoord = _float2(1.f, 0.f);//

	pVertices[15].vPosition = _float3(0.5f, 0.5f, 0.5f);
	pVertices[15].vTexcoord = _float2(0.f, 0.f);//

	pVertices[16].vPosition = _float3(0.5f, -0.5f, 0.5f);
	pVertices[16].vTexcoord = _float2(0.f, 1.f);//

	pVertices[17].vPosition = _float3(0.25f, -0.5f, 0.5f);
	pVertices[17].vTexcoord = _float2(1.f, 1.f);//

	pVertices[18].vPosition = _float3(-0.25f, -0.5f, 0.5f);
	pVertices[18].vTexcoord = _float2(0.f, 1.f);//

	pVertices[19].vPosition = _float3(-0.5f, -0.5f, 0.5f);
	pVertices[19].vTexcoord = _float2(1.f, 1.f);//

	pVertices[20].vPosition = _float3(-0.25f, 0.375f, 0.25f);
	pVertices[20].vTexcoord = _float2(1.f, 0.f);//

	pVertices[21].vPosition = _float3(0.25f, 0.375f, 0.25f);
	pVertices[21].vTexcoord = _float2(0.f, 0.f);//

	pVertices[22].vPosition = _float3(0.25f, -0.125f, 0.25f);
	pVertices[22].vTexcoord = _float2(0.f, 1.f);//

	pVertices[23].vPosition = _float3(-0.25f, -0.125f, 0.25f);
	pVertices[23].vTexcoord = _float2(1.f, 1.f);//


	pVertices[24].vPosition = _float3(-0.25f, 0.5f, -0.25f);
	pVertices[24].vTexcoord = _float2(2.f, 0.f);//

	pVertices[25].vPosition = _float3(0.25f, 0.5f, -0.25f);
	pVertices[25].vTexcoord = _float2(1.f, 2.f);//

	pVertices[26].vPosition = _float3(0.25f, -0.5f, -0.25f);
	pVertices[26].vTexcoord = _float2(1.f, 1.f);//

	pVertices[27].vPosition = _float3(-0.25f, -0.5f, -0.25f);
	pVertices[27].vTexcoord = _float2(2.f, 1.f);//

	pVertices[28].vPosition = _float3(-0.25f, 0.5f, 0.25f);
	pVertices[28].vTexcoord = _float2(2.f, 1.f);

	pVertices[29].vPosition = _float3(0.25f, 0.5f, 0.25f);
	pVertices[29].vTexcoord = _float2(2.f, 0.f);//

	pVertices[30].vPosition = _float3(0.25f, -0.5f, 0.25f);
	pVertices[30].vTexcoord = _float2(2.f, 1.f);//

	pVertices[31].vPosition = _float3(-0.25f, -0.5f, 0.25f);
	pVertices[31].vTexcoord = _float2(1.f, 1.4f);//

#pragma region 텍스쳐
	pVertices[32].vPosition = _float3(0.25f, -0.125f, 0.25f);	//22
	pVertices[32].vTexcoord = _float2(2.f, 1.4f);

	pVertices[33].vPosition = _float3(-0.25f, -0.125f, 0.25f);	//23
	pVertices[33].vTexcoord = _float2(0.f, 2.f);

	pVertices[34].vPosition = _float3(-0.25f, -0.5f, -0.25f);	//27
	pVertices[34].vTexcoord = _float2(0.f, 1.4f);

	pVertices[35].vPosition = _float3(-0.25f, 0.5f, 0.5f);		//13
	pVertices[35].vTexcoord = _float2(3.f, 1.f);

	pVertices[36].vPosition = _float3(-0.25f, -0.5f, 0.5f);		//18
	pVertices[36].vTexcoord = _float2(3.f, 2.f);

	pVertices[37].vPosition = _float3(-0.25f, -0.5f, 0.25f);	//31
	pVertices[37].vTexcoord = _float2(2.f, 2.f);

	pVertices[38].vPosition = _float3(0.25f, 0.5f, -0.5f);		//2
	pVertices[38].vTexcoord = _float2(0.f, 2.f);

	pVertices[39].vPosition = _float3(0.25f, -0.125f, -0.25f);	//10
	pVertices[39].vTexcoord = _float2(1.f, 1.4f);
#pragma endregion

	for (int i = 0; i < m_iNumVertices; i++)
	{
		pVertices[i].vNormal = _float3(0.f, 0.f, 0.f);
	}

	

#pragma endregion

#pragma region INDEX_BUFFER
	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	_ushort* pIndices = { nullptr };

	m_pIB->Lock(0, 0, reinterpret_cast<void**>(&pIndices), 0);

#pragma region 정면
 	Set_IndexBuffer(pIndices, 114, 0, 1, 6, 7);
	pVertices[0].vNormal += ComputeNormalVector(pVertices, 0, 1, 6);
	pVertices[1].vNormal += ComputeNormalVector(pVertices, 0, 1, 6);
	pVertices[6].vNormal += ComputeNormalVector(pVertices, 0, 1, 6);
	pVertices[7].vNormal += ComputeNormalVector(pVertices, 0, 6, 7);

	Set_IndexBuffer(pIndices, 120, 8, 9, 10, 11);
	pVertices[8].vNormal += ComputeNormalVector(pVertices, 8, 9, 10);
	pVertices[9].vNormal += ComputeNormalVector(pVertices, 8, 9, 10);
	pVertices[10].vNormal += ComputeNormalVector(pVertices, 8, 9, 10);
	pVertices[11].vNormal += ComputeNormalVector(pVertices, 8, 10, 11);

	Set_IndexBuffer(pIndices, 126, 2, 3, 4, 5);
	pVertices[2].vNormal += ComputeNormalVector(pVertices, 2, 3, 4);
	pVertices[3].vNormal += ComputeNormalVector(pVertices, 2, 3, 4);
	pVertices[4].vNormal += ComputeNormalVector(pVertices, 2, 3, 4);
	pVertices[5].vNormal += ComputeNormalVector(pVertices, 2, 4, 5);
#pragma endregion

#pragma region 후면
	Set_IndexBuffer(pIndices, 0, 13, 12, 19, 18);
	pVertices[13].vNormal += ComputeNormalVector(pVertices, 13, 12, 19);
	pVertices[12].vNormal += ComputeNormalVector(pVertices, 13, 12, 19);
	pVertices[19].vNormal += ComputeNormalVector(pVertices, 13, 12, 19);
	pVertices[18].vNormal += ComputeNormalVector(pVertices, 13, 19, 18);

	Set_IndexBuffer(pIndices, 6, 21, 20, 23, 22);
	pVertices[21].vNormal += ComputeNormalVector(pVertices, 21, 20, 23);
	pVertices[20].vNormal += ComputeNormalVector(pVertices, 21, 20, 23);
	pVertices[23].vNormal += ComputeNormalVector(pVertices, 21, 20, 23);
	pVertices[22].vNormal += ComputeNormalVector(pVertices, 21, 23, 22);

	Set_IndexBuffer(pIndices, 12, 15, 14, 17, 16);
	pVertices[15].vNormal += ComputeNormalVector(pVertices, 15, 14, 17);
	pVertices[14].vNormal += ComputeNormalVector(pVertices, 15, 14, 17);
	pVertices[17].vNormal += ComputeNormalVector(pVertices, 15, 14, 17);
	pVertices[16].vNormal += ComputeNormalVector(pVertices, 15, 17, 16);
#pragma endregion

#pragma region 왼쪽
	Set_IndexBuffer(pIndices, 18, 12, 0, 7, 19);
	pVertices[12].vNormal += ComputeNormalVector(pVertices, 12, 0, 7);
	pVertices[0].vNormal += ComputeNormalVector(pVertices, 12, 0, 7);
	pVertices[7].vNormal += ComputeNormalVector(pVertices, 12, 0, 7);
	pVertices[19].vNormal += ComputeNormalVector(pVertices, 12, 7, 19);

	Set_IndexBuffer(pIndices, 24, 14, 29, 30, 17);
	pVertices[14].vNormal += ComputeNormalVector(pVertices, 14, 29, 30);
	pVertices[29].vNormal += ComputeNormalVector(pVertices, 14, 29, 30);
	pVertices[30].vNormal += ComputeNormalVector(pVertices, 14, 29, 30);
	pVertices[17].vNormal += ComputeNormalVector(pVertices, 14, 30, 17);

	Set_IndexBuffer(pIndices, 30, 29, 25, 9, 21);
	pVertices[29].vNormal += ComputeNormalVector(pVertices, 29, 25, 9);
	pVertices[25].vNormal += ComputeNormalVector(pVertices, 29, 25, 9);
	pVertices[9].vNormal += ComputeNormalVector(pVertices, 29, 25, 9);
	pVertices[21].vNormal += ComputeNormalVector(pVertices, 29, 9, 21);

	Set_IndexBuffer(pIndices, 36, 25, 38, 5, 26);
	pVertices[25].vNormal += ComputeNormalVector(pVertices, 25, 2, 5);
	pVertices[2].vNormal += ComputeNormalVector(pVertices, 25, 2, 5);
	pVertices[5].vNormal += ComputeNormalVector(pVertices, 25, 2, 5);
	pVertices[26].vNormal += ComputeNormalVector(pVertices, 25, 5, 26);
	
	Set_IndexBuffer(pIndices, 42, 32, 39, 26, 30);
	pVertices[22].vNormal += ComputeNormalVector(pVertices, 22, 10, 26);
	pVertices[10].vNormal += ComputeNormalVector(pVertices, 22, 10, 26);
	pVertices[26].vNormal += ComputeNormalVector(pVertices, 22, 10, 26);
	pVertices[30].vNormal += ComputeNormalVector(pVertices, 22, 26, 30);
#pragma endregion

#pragma region 오른쪽
	Set_IndexBuffer(pIndices, 48, 3, 15, 16, 4);
	pVertices[3].vNormal += ComputeNormalVector(pVertices, 3, 15, 16);
	pVertices[15].vNormal += ComputeNormalVector(pVertices, 3, 15, 16);
	pVertices[16].vNormal += ComputeNormalVector(pVertices, 3, 15, 16);
	pVertices[4].vNormal += ComputeNormalVector(pVertices, 3, 16, 4);

	Set_IndexBuffer(pIndices, 54, 1, 24, 27, 6);
	pVertices[1].vNormal += ComputeNormalVector(pVertices, 1, 24, 27);
	pVertices[24].vNormal += ComputeNormalVector(pVertices, 1, 24, 27);
	pVertices[27].vNormal += ComputeNormalVector(pVertices, 1, 24, 27);
	pVertices[6].vNormal += ComputeNormalVector(pVertices, 1, 27, 26);

	Set_IndexBuffer(pIndices, 60, 24, 28, 20, 8);
	pVertices[24].vNormal += ComputeNormalVector(pVertices, 24, 28, 20);
	pVertices[28].vNormal += ComputeNormalVector(pVertices, 24, 28, 20);
	pVertices[20].vNormal += ComputeNormalVector(pVertices, 24, 28, 20);
	pVertices[8].vNormal += ComputeNormalVector(pVertices, 24, 20, 8);

	Set_IndexBuffer(pIndices, 66, 28, 35, 36, 37);
	pVertices[28].vNormal += ComputeNormalVector(pVertices, 28, 13, 18);
	pVertices[13].vNormal += ComputeNormalVector(pVertices, 28, 13, 18);
	pVertices[18].vNormal += ComputeNormalVector(pVertices, 28, 13, 18);
	pVertices[31].vNormal += ComputeNormalVector(pVertices, 28, 18, 31);

	Set_IndexBuffer(pIndices, 72, 11, 23, 31, 34);
	pVertices[11].vNormal += ComputeNormalVector(pVertices, 11, 23, 31);
	pVertices[23].vNormal += ComputeNormalVector(pVertices, 11, 23, 31);
	pVertices[31].vNormal += ComputeNormalVector(pVertices, 11, 23, 31);
	pVertices[27].vNormal += ComputeNormalVector(pVertices, 11, 31, 27);
#pragma endregion

#pragma region 위
	Set_IndexBuffer(pIndices, 78, 13, 12, 0, 1);
	pVertices[13].vNormal += ComputeNormalVector(pVertices, 13, 12, 0);
	pVertices[12].vNormal += ComputeNormalVector(pVertices, 13, 12, 0);
	pVertices[0].vNormal += ComputeNormalVector(pVertices, 13, 12, 0);
	pVertices[1].vNormal += ComputeNormalVector(pVertices, 13, 0, 1);

	Set_IndexBuffer(pIndices, 84, 21, 20, 8, 9);
	pVertices[21].vNormal += ComputeNormalVector(pVertices, 21, 20, 8);
	pVertices[20].vNormal += ComputeNormalVector(pVertices, 21, 20, 8);
	pVertices[8].vNormal += ComputeNormalVector(pVertices, 21, 20, 8);
	pVertices[9].vNormal += ComputeNormalVector(pVertices, 21, 8, 9);

	Set_IndexBuffer(pIndices, 90, 15, 14, 2, 3);
	pVertices[15].vNormal += ComputeNormalVector(pVertices, 15, 14, 2);
	pVertices[14].vNormal += ComputeNormalVector(pVertices, 15, 14, 2);
	pVertices[2].vNormal += ComputeNormalVector(pVertices, 15, 14, 2);
	pVertices[3].vNormal += ComputeNormalVector(pVertices, 15, 2, 3);
#pragma endregion

#pragma region 아래
	Set_IndexBuffer(pIndices, 96, 16, 17, 5, 4);
	pVertices[16].vNormal += ComputeNormalVector(pVertices, 16, 17, 5);
	pVertices[17].vNormal += ComputeNormalVector(pVertices, 16, 17, 5);
	pVertices[5].vNormal += ComputeNormalVector(pVertices, 16, 17, 5);
	pVertices[4].vNormal += ComputeNormalVector(pVertices, 16, 5, 4);

	Set_IndexBuffer(pIndices, 102, 32, 33, 11, 10);
	pVertices[22].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[23].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[11].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[10].vNormal += ComputeNormalVector(pVertices, 22, 11, 10);

	Set_IndexBuffer(pIndices, 108, 18, 19, 7, 6);
	pVertices[18].vNormal += ComputeNormalVector(pVertices, 18, 19, 7);
	pVertices[19].vNormal += ComputeNormalVector(pVertices, 18, 19, 7);
	pVertices[7].vNormal += ComputeNormalVector(pVertices, 18, 19, 7);
	pVertices[6].vNormal += ComputeNormalVector(pVertices, 18, 7, 6);
#pragma endregion

	m_pVB->Unlock();
	m_pIB->Unlock();
#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_BuildingH::Initialize(void* pArg)
{
    return S_OK;
}

HRESULT CVIBuffer_BuildingH::Render(_uint Surface)
{
	_uint iNumber{ 0 };

	switch (Surface)
	{
	case COMMON:
		iNumber = 0;
		break;

	case LEFT:
		iNumber = 114;
		break;

	case MIDDLE:
		iNumber = 120;
		break;

	case RIGHT:
		iNumber = 126;
		break;

	default:
		return E_FAIL;
		break;
	}

	return m_pGraphic_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iNumVertices, iNumber, m_iNumPritimive);
}

void CVIBuffer_BuildingH::Set_IndexBuffer(_ushort* pIndices, _uint StartIndex, _uint LT, _uint RT, _uint RB, _uint LB)
{
	pIndices[StartIndex++] = LT;
	pIndices[StartIndex++] = RT;
	pIndices[StartIndex++] = RB;

	pIndices[StartIndex++] = LT;
	pIndices[StartIndex++] = RB;
	pIndices[StartIndex++] = LB;
}

VECTOR CVIBuffer_BuildingH::ComputeNormalVector(VTXPOSNORTEX* pVertices, _uint vertex1, _uint vertex2, _uint vertex3)
{
	return __super::ComputeNormal(&(pVertices[vertex1].vPosition), &(pVertices[vertex2].vPosition), &(pVertices[vertex3].vPosition));
}

CVIBuffer_BuildingH* CVIBuffer_BuildingH::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_BuildingH* pInstance = new CVIBuffer_BuildingH(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_BuildingH");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_BuildingH::Clone(void* pArg)
{
	CVIBuffer_BuildingH* pInstance = new CVIBuffer_BuildingH(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CVIBuffer_BuildingH");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_BuildingH::Free()
{
	__super::Free();
}
