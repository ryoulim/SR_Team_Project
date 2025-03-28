#include "VIBuffer_RaceGate.h"

CVIBuffer_RaceGate::CVIBuffer_RaceGate(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CVIBuffer{pGraphic_Device}
{
}

CVIBuffer_RaceGate::CVIBuffer_RaceGate(const CVIBuffer_RaceGate& Prototype)
    :CVIBuffer(Prototype)
{
}

HRESULT CVIBuffer_RaceGate::Initialize_Prototype()
{
	m_iNumVertices = 34;					// 버텍스 갯수
	m_iVertexStride = sizeof(VTXPOSNORTEX);	// 버텍스 크기
	m_iFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;		// 버텍스 타입
	m_iNumPritimive = 48;					// 삼각형 갯수

	m_iIndexStride = 2;						// 인덱스 크기
	m_iNumIndices = 144;					// 인덱스 갯수
	m_eIndexFormat = D3DFMT_INDEX16;		// 인덱스 포맷

#pragma region VERTEX_BUFFER
	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXPOSNORTEX* pVertices = { nullptr };

	m_pVB->Lock(0, /*m_iNumVertices * m_iVertexStride*/0, reinterpret_cast<void**>(&pVertices), 0);
	
	pVertices[0].vPosition = _float3(-3.0f, 0.5f, 0.5f);
	pVertices[0].vTexcoord = _float2(0.f, 0.f);

	pVertices[1].vPosition = _float3(3.0f, 0.5f, 0.5f);
	pVertices[1].vTexcoord = _float2(1.f, 0.f);

	pVertices[2].vPosition = _float3(3.0f, 0.5f, -0.5f);
	pVertices[2].vTexcoord = _float2(15.f, 0.f);//

	pVertices[3].vPosition = _float3(0.5f, 0.5f, -0.5f);
	pVertices[3].vTexcoord = _float2(9.f, 0.f);//

	pVertices[4].vPosition = _float3(0.5f, 0.5f, -0.375f);
	pVertices[4].vTexcoord = _float2(8.75f, 0.f);//

	pVertices[5].vPosition = _float3(0.375f, 0.5f, -0.375f);
	pVertices[5].vTexcoord = _float2(8.5f, 0.f);//

	pVertices[6].vPosition = _float3(0.25f, 0.5f, -0.125f);
	pVertices[6].vTexcoord = _float2(8.f, 0.f);//

	pVertices[7].vPosition = _float3(-0.25f, 0.5f, -0.125f);
	pVertices[7].vTexcoord = _float2(7.f, 0.f);//

	pVertices[8].vPosition = _float3(-0.375f, 0.5f, -0.375f);
	pVertices[8].vTexcoord = _float2(6.5f, 0.f);//

	pVertices[9].vPosition = _float3(-0.5f, 0.5f, -0.375f);
	pVertices[9].vTexcoord = _float2(6.25f, 0.f);//

	pVertices[10].vPosition = _float3(-0.5f, 0.5f, -0.5f);
	pVertices[10].vTexcoord = _float2(6.f, 0.f);//

	pVertices[11].vPosition = _float3(-3.0f, 0.5f, -0.5f);
	pVertices[11].vTexcoord = _float2(0.f, 0.f);//

	pVertices[12].vPosition = _float3(-3.0f, -0.5f, 0.5f);
	pVertices[12].vTexcoord = _float2(0.f, 0.f);

	pVertices[13].vPosition = _float3(3.0f, -0.5f, 0.5f);
	pVertices[13].vTexcoord = _float2(1.f, 0.f);

	pVertices[14].vPosition = _float3(3.0f, -0.5f, -0.5f);
	pVertices[14].vTexcoord = _float2(15.f, 1.f);//

	pVertices[15].vPosition = _float3(0.5f, -0.5f, -0.5f);
	pVertices[15].vTexcoord = _float2(9.f, 1.f);//

	pVertices[16].vPosition = _float3(0.5f, -0.5f, -0.375f);
	pVertices[16].vTexcoord = _float2(8.75f, 1.f);//

	pVertices[17].vPosition = _float3(0.375f, -0.5f, -0.375f);
	pVertices[17].vTexcoord = _float2(8.5f, 1.f);//

	pVertices[18].vPosition = _float3(0.25f, -0.5f, -0.125f);
	pVertices[18].vTexcoord = _float2(8.f, 1.f);//

	pVertices[19].vPosition = _float3(-0.25f, -0.5f, -0.125f);
	pVertices[19].vTexcoord = _float2(7.f, 1.f);//

	pVertices[20].vPosition = _float3(-0.375f, -0.5f, -0.375f);
	pVertices[20].vTexcoord = _float2(6.5f, 1.f);//

	pVertices[21].vPosition = _float3(-0.5f, -0.5f, -0.375f);
	pVertices[21].vTexcoord = _float2(6.25f, 1.f);//

	pVertices[22].vPosition = _float3(-0.5f, -0.5f, -0.5f);
	pVertices[22].vTexcoord = _float2(6.f, 1.f);//

	pVertices[23].vPosition = _float3(-3.0f, -0.5f, -0.5f);
	pVertices[23].vTexcoord = _float2(0.f, 1.f);//

	pVertices[24].vPosition = _float3(0.25f, 0.f, -0.125f);
	pVertices[24].vTexcoord = _float2(8.f, 1.f);//

	pVertices[25].vPosition = _float3(-0.25f, 0.f, -0.125f);
	pVertices[25].vTexcoord = _float2(7.f, 1.f);//

	pVertices[26].vPosition = _float3(-0.25f, 0.5f, 0.5f);
	pVertices[26].vTexcoord = _float2(1.f, 1.f);

	pVertices[27].vPosition = _float3(0.25f, 0.5f, 0.5f);
	pVertices[27].vTexcoord = _float2(0.f, 1.f);

	pVertices[28].vPosition = _float3(-0.25f, -0.5f, 0.5f);
	pVertices[28].vTexcoord = _float2(1.f, 1.f);

	pVertices[29].vPosition = _float3(0.25f, -0.5f, 0.5f);
	pVertices[29].vTexcoord = _float2(0.f, 1.f);

	pVertices[30].vPosition = _float3(0.25f, -0.25f, -0.125f);
	pVertices[30].vTexcoord = _float2(1.f, 1.f);

	pVertices[31].vPosition = _float3(-0.25f, -0.25f, -0.125f);
	pVertices[31].vTexcoord = _float2(0.f, 1.f);
	 
	pVertices[32].vPosition = _float3(0.25f, 0.f, -0.125f);		//24
	pVertices[32].vTexcoord = _float2(1.f, 0.f);

	pVertices[33].vPosition = _float3(-0.25f, 0.f, -0.125f);	//25
	pVertices[33].vTexcoord = _float2(0.f, 0.f);
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
	Set_IndexBuffer(pIndices, 0, 11, 10, 22, 23);
	pVertices[22].vNormal += ComputeNormalVector(pVertices, 11, 10, 22);
	pVertices[23].vNormal += ComputeNormalVector(pVertices, 11, 10, 22);
	pVertices[11].vNormal += ComputeNormalVector(pVertices, 11, 10, 22);
	pVertices[10].vNormal += ComputeNormalVector(pVertices, 11, 22, 23);

	Set_IndexBuffer(pIndices, 6, 9, 8, 20, 21);
	pVertices[9].vNormal += ComputeNormalVector(pVertices, 9, 8, 20);
	pVertices[8].vNormal += ComputeNormalVector(pVertices, 9, 8, 20);
	pVertices[20].vNormal += ComputeNormalVector(pVertices, 9, 8, 20);
	pVertices[21].vNormal += ComputeNormalVector(pVertices, 9, 20, 21);

	Set_IndexBuffer(pIndices, 12, 8, 7, 19, 20);
	pVertices[8].vNormal += ComputeNormalVector(pVertices, 8, 7, 19);
	pVertices[7].vNormal += ComputeNormalVector(pVertices, 8, 7, 19);
	pVertices[19].vNormal += ComputeNormalVector(pVertices, 8, 7, 19);
	pVertices[20].vNormal += ComputeNormalVector(pVertices, 8, 19, 20);

	Set_IndexBuffer(pIndices, 18, 6, 5, 17, 18);
	pVertices[6].vNormal += ComputeNormalVector(pVertices, 6, 5, 17);
	pVertices[5].vNormal += ComputeNormalVector(pVertices, 6, 5, 17);
	pVertices[17].vNormal += ComputeNormalVector(pVertices, 6, 5, 17);
	pVertices[18].vNormal += ComputeNormalVector(pVertices, 6, 17, 18);

	Set_IndexBuffer(pIndices, 24, 5, 4, 16, 17);
	pVertices[5].vNormal += ComputeNormalVector(pVertices, 5, 4, 16);
	pVertices[4].vNormal += ComputeNormalVector(pVertices, 5, 4, 16);
	pVertices[16].vNormal += ComputeNormalVector(pVertices, 5, 4, 16);
	pVertices[17].vNormal += ComputeNormalVector(pVertices, 5, 16, 17);

	Set_IndexBuffer(pIndices, 30, 3, 2, 14, 15);
	pVertices[3].vNormal += ComputeNormalVector(pVertices, 3, 2, 14);
	pVertices[2].vNormal += ComputeNormalVector(pVertices, 3, 2, 14);
	pVertices[14].vNormal += ComputeNormalVector(pVertices, 3, 2, 14);
	pVertices[15].vNormal += ComputeNormalVector(pVertices, 3, 14, 15);

	Set_IndexBuffer(pIndices, 132, 7, 6, 24, 25);
	pVertices[7].vNormal += ComputeNormalVector(pVertices, 7, 6, 24);
	pVertices[6].vNormal += ComputeNormalVector(pVertices, 7, 6, 24);
	pVertices[24].vNormal += ComputeNormalVector(pVertices, 7, 6, 24);
	pVertices[25].vNormal += ComputeNormalVector(pVertices, 7, 24, 25);

	Set_IndexBuffer(pIndices, 138, 33, 32, 30, 31);
	pVertices[25].vNormal += ComputeNormalVector(pVertices, 25, 24, 30);
	pVertices[24].vNormal += ComputeNormalVector(pVertices, 25, 24, 30);
	pVertices[30].vNormal += ComputeNormalVector(pVertices, 25, 24, 30);
	pVertices[31].vNormal += ComputeNormalVector(pVertices, 25, 30, 31);
#pragma endregion

#pragma region 뒤
	
#pragma endregion

#pragma region 왼쪽
	Set_IndexBuffer(pIndices, 36, 0, 11, 23, 12);
	pVertices[0].vNormal += ComputeNormalVector(pVertices, 0, 11, 23);
	pVertices[11].vNormal += ComputeNormalVector(pVertices, 0, 11, 23);
	pVertices[23].vNormal += ComputeNormalVector(pVertices, 0, 11, 23);
	pVertices[12].vNormal += ComputeNormalVector(pVertices, 0, 23, 12);

	Set_IndexBuffer(pIndices, 42, 4, 3, 15, 16);
	pVertices[4].vNormal += ComputeNormalVector(pVertices, 4, 3, 15);
	pVertices[3].vNormal += ComputeNormalVector(pVertices, 4, 3, 15);
	pVertices[15].vNormal += ComputeNormalVector(pVertices, 4, 3, 15);
	pVertices[16].vNormal += ComputeNormalVector(pVertices, 4, 15, 16);
#pragma endregion

#pragma region 오른쪽
	Set_IndexBuffer(pIndices, 48, 10, 9, 21, 22);
	pVertices[10].vNormal += ComputeNormalVector(pVertices, 10, 9, 21);
	pVertices[9].vNormal += ComputeNormalVector(pVertices, 10, 9, 21);
	pVertices[21].vNormal += ComputeNormalVector(pVertices, 10, 9, 21);
	pVertices[22].vNormal += ComputeNormalVector(pVertices, 10, 21, 22);

	Set_IndexBuffer(pIndices, 54, 2, 1, 13, 14);
	pVertices[2].vNormal += ComputeNormalVector(pVertices, 2, 1, 13);
	pVertices[1].vNormal += ComputeNormalVector(pVertices, 2, 1, 13);
	pVertices[13].vNormal += ComputeNormalVector(pVertices, 2, 1, 13);
	pVertices[14].vNormal += ComputeNormalVector(pVertices, 2, 13, 14);
#pragma endregion

#pragma region 위
	Set_IndexBuffer(pIndices, 60, 0, 9, 10, 11);
	pVertices[0].vNormal += ComputeNormalVector(pVertices, 0, 9, 10);
	pVertices[9].vNormal += ComputeNormalVector(pVertices, 0, 9, 10);
	pVertices[10].vNormal += ComputeNormalVector(pVertices, 0, 9, 10);
	pVertices[11].vNormal += ComputeNormalVector(pVertices, 0, 10, 11);

	Set_IndexBuffer(pIndices, 66, 0, 26, 7, 9);
	pVertices[0].vNormal += ComputeNormalVector(pVertices, 0, 26, 7);
	pVertices[26].vNormal += ComputeNormalVector(pVertices, 0, 26, 7);
	pVertices[7].vNormal += ComputeNormalVector(pVertices, 0, 26, 7);
	pVertices[9].vNormal += ComputeNormalVector(pVertices, 0, 7, 9);

	Set_IndexBuffer(pIndices, 72, 26, 27, 6, 7);
	pVertices[26].vNormal += ComputeNormalVector(pVertices, 26, 27, 6);
	pVertices[27].vNormal += ComputeNormalVector(pVertices, 26, 27, 6);
	pVertices[6].vNormal += ComputeNormalVector(pVertices, 26, 27, 6);
	pVertices[7].vNormal += ComputeNormalVector(pVertices, 26, 6, 7);

	Set_IndexBuffer(pIndices, 78, 27, 1, 4, 6);
	pVertices[27].vNormal += ComputeNormalVector(pVertices, 27, 1, 4);
	pVertices[1].vNormal += ComputeNormalVector(pVertices, 27, 1, 4);
	pVertices[4].vNormal += ComputeNormalVector(pVertices, 27, 1, 4);
	pVertices[6].vNormal += ComputeNormalVector(pVertices, 27, 4, 6);

	Set_IndexBuffer(pIndices, 84, 4, 1, 2, 3);
	pVertices[4].vNormal += ComputeNormalVector(pVertices, 4, 1, 2);
	pVertices[1].vNormal += ComputeNormalVector(pVertices, 4, 1, 2);
	pVertices[2].vNormal += ComputeNormalVector(pVertices, 4, 1, 2);
	pVertices[3].vNormal += ComputeNormalVector(pVertices, 4, 2, 3);

	pIndices[90] = 9; pIndices[91] = 7; pIndices[92] = 8;
	pVertices[9].vNormal += ComputeNormalVector(pVertices, 9, 7, 8);
	pVertices[7].vNormal += ComputeNormalVector(pVertices, 9, 7, 8);
	pVertices[8].vNormal += ComputeNormalVector(pVertices, 9, 7, 8);

	pIndices[93] = 6; pIndices[94] = 4; pIndices[95] = 5;
	pVertices[6].vNormal += ComputeNormalVector(pVertices, 6, 4, 5);
	pVertices[4].vNormal += ComputeNormalVector(pVertices, 6, 4, 5);
	pVertices[5].vNormal += ComputeNormalVector(pVertices, 6, 4, 5);
#pragma endregion

#pragma region 아래
	Set_IndexBuffer(pIndices, 96, 21, 12, 23, 22);
	pVertices[21].vNormal += ComputeNormalVector(pVertices, 21, 12, 23);
	pVertices[12].vNormal += ComputeNormalVector(pVertices, 21, 12, 23);
	pVertices[23].vNormal += ComputeNormalVector(pVertices, 21, 12, 23);
	pVertices[22].vNormal += ComputeNormalVector(pVertices, 21, 23, 22);

	Set_IndexBuffer(pIndices, 102, 28, 12, 21, 19);
	pVertices[28].vNormal += ComputeNormalVector(pVertices, 28, 12, 21);
	pVertices[12].vNormal += ComputeNormalVector(pVertices, 28, 12, 21);
	pVertices[21].vNormal += ComputeNormalVector(pVertices, 28, 12, 21);
	pVertices[19].vNormal += ComputeNormalVector(pVertices, 28, 21, 19);

	Set_IndexBuffer(pIndices, 108, 29, 28, 19, 18);
	pVertices[29].vNormal += ComputeNormalVector(pVertices, 29, 28, 19);
	pVertices[28].vNormal += ComputeNormalVector(pVertices, 29, 28, 19);
	pVertices[19].vNormal += ComputeNormalVector(pVertices, 29, 28, 19);
	pVertices[18].vNormal += ComputeNormalVector(pVertices, 29, 19, 18);

	Set_IndexBuffer(pIndices, 114, 13, 29, 18, 16);
	pVertices[13].vNormal += ComputeNormalVector(pVertices, 13, 29, 18);
	pVertices[29].vNormal += ComputeNormalVector(pVertices, 13, 29, 18);
	pVertices[18].vNormal += ComputeNormalVector(pVertices, 13, 29, 18);
	pVertices[16].vNormal += ComputeNormalVector(pVertices, 13, 18, 16);

	Set_IndexBuffer(pIndices, 120, 13, 16, 15, 14);
	pVertices[13].vNormal += ComputeNormalVector(pVertices, 13, 16, 15);
	pVertices[16].vNormal += ComputeNormalVector(pVertices, 13, 16, 15);
	pVertices[15].vNormal += ComputeNormalVector(pVertices, 13, 16, 15);
	pVertices[14].vNormal += ComputeNormalVector(pVertices, 13, 15, 14);

	pIndices[126] = 16; pIndices[127] = 18; pIndices[128] = 17;
	pVertices[16].vNormal += ComputeNormalVector(pVertices, 16, 18, 17);
	pVertices[18].vNormal += ComputeNormalVector(pVertices, 16, 18, 17);
	pVertices[17].vNormal += ComputeNormalVector(pVertices, 16, 18, 17);

	pIndices[129] = 19; pIndices[130] = 21; pIndices[131] = 20;
	pVertices[19].vNormal += ComputeNormalVector(pVertices, 19, 21, 20);
	pVertices[21].vNormal += ComputeNormalVector(pVertices, 19, 21, 20);
	pVertices[20].vNormal += ComputeNormalVector(pVertices, 19, 21, 20);
#pragma endregion

	m_pVB->Unlock();
	m_pIB->Unlock();
#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_RaceGate::Initialize(void* pArg)
{
    return S_OK;
}

HRESULT CVIBuffer_RaceGate::Render(_uint Surface)
{
	_uint iNumber{ 0 };

	switch (Surface)
	{
	case COMMON:
		iNumber = 0;
		break;

	case MIDDLE:
		iNumber = 132;
		break;

	case DOWN:
		iNumber = 138;
		break;

	default:
		return E_FAIL;
		break;
	}

	return m_pGraphic_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iNumVertices, iNumber, m_iNumPritimive);
}

void CVIBuffer_RaceGate::Set_IndexBuffer(_ushort* pIndices, _uint StartIndex, _uint LT, _uint RT, _uint RB, _uint LB)
{
	pIndices[StartIndex++] = LT;
	pIndices[StartIndex++] = RT;
	pIndices[StartIndex++] = RB;

	pIndices[StartIndex++] = LT;
	pIndices[StartIndex++] = RB;
	pIndices[StartIndex++] = LB;
}

VECTOR CVIBuffer_RaceGate::ComputeNormalVector(VTXPOSNORTEX* pVertices, _uint vertex1, _uint vertex2, _uint vertex3)
{
	return __super::ComputeNormal(&(pVertices[vertex1].vPosition), &(pVertices[vertex2].vPosition), &(pVertices[vertex3].vPosition));
}

CVIBuffer_RaceGate* CVIBuffer_RaceGate::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_RaceGate* pInstance = new CVIBuffer_RaceGate(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_RaceGate");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_RaceGate::Clone(void* pArg)
{
	CVIBuffer_RaceGate* pInstance = new CVIBuffer_RaceGate(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CVIBuffer_RaceGate");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_RaceGate::Free()
{
	__super::Free();
}
