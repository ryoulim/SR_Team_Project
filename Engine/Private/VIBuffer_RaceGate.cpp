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
	m_iNumPritimive = 52;					// 삼각형 갯수

	m_iIndexStride = 2;						// 인덱스 크기
	m_iNumIndices = 156;						// 인덱스 갯수
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
	pVertices[22].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[23].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[11].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[10].vNormal += ComputeNormalVector(pVertices, 22, 11, 10);

	Set_IndexBuffer(pIndices, 12, 8, 7, 19, 20);
	pVertices[22].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[23].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[11].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[10].vNormal += ComputeNormalVector(pVertices, 22, 11, 10);

	Set_IndexBuffer(pIndices, 18, 6, 5, 17, 18);
	pVertices[22].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[23].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[11].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[10].vNormal += ComputeNormalVector(pVertices, 22, 11, 10);

	Set_IndexBuffer(pIndices, 24, 5, 4, 16, 17);
	pVertices[22].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[23].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[11].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[10].vNormal += ComputeNormalVector(pVertices, 22, 11, 10);

	Set_IndexBuffer(pIndices, 30, 3, 2, 14, 15);
	pVertices[22].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[23].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[11].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[10].vNormal += ComputeNormalVector(pVertices, 22, 11, 10);

	Set_IndexBuffer(pIndices, 138, 7, 6, 24, 25);
	pVertices[22].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[23].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[11].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[10].vNormal += ComputeNormalVector(pVertices, 22, 11, 10);

	Set_IndexBuffer(pIndices, 144, 33, 32, 30, 31);
	pVertices[22].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[23].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[11].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[10].vNormal += ComputeNormalVector(pVertices, 22, 11, 10);

	Set_IndexBuffer(pIndices, 150, 31, 30, 18, 19);
	pVertices[22].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[23].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[11].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[10].vNormal += ComputeNormalVector(pVertices, 22, 11, 10);
#pragma endregion

#pragma region 뒤
	Set_IndexBuffer(pIndices, 36, 1, 0, 12, 13);
	pVertices[22].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[23].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[11].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[10].vNormal += ComputeNormalVector(pVertices, 22, 11, 10);
#pragma endregion

#pragma region 왼쪽
	Set_IndexBuffer(pIndices, 42, 0, 11, 23, 12);
	pVertices[22].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[23].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[11].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[10].vNormal += ComputeNormalVector(pVertices, 22, 11, 10);

	Set_IndexBuffer(pIndices, 48, 4, 3, 15, 16);
	pVertices[22].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[23].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[11].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[10].vNormal += ComputeNormalVector(pVertices, 22, 11, 10);
#pragma endregion

#pragma region 오른쪽
	Set_IndexBuffer(pIndices, 54, 10, 9, 21, 22);
	pVertices[22].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[23].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[11].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[10].vNormal += ComputeNormalVector(pVertices, 22, 11, 10);

	Set_IndexBuffer(pIndices, 60, 2, 1, 13, 14);
	pVertices[22].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[23].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[11].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[10].vNormal += ComputeNormalVector(pVertices, 22, 11, 10);
#pragma endregion

#pragma region 위
	Set_IndexBuffer(pIndices, 66, 0, 9, 10, 11);
	pVertices[22].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[23].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[11].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[10].vNormal += ComputeNormalVector(pVertices, 22, 11, 10);

	Set_IndexBuffer(pIndices, 72, 0, 26, 7, 9);
	pVertices[22].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[23].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[11].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[10].vNormal += ComputeNormalVector(pVertices, 22, 11, 10);

	Set_IndexBuffer(pIndices, 78, 26, 27, 6, 7);
	pVertices[22].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[23].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[11].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[10].vNormal += ComputeNormalVector(pVertices, 22, 11, 10);

	Set_IndexBuffer(pIndices, 84, 27, 1, 4, 6);
	pVertices[22].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[23].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[11].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[10].vNormal += ComputeNormalVector(pVertices, 22, 11, 10);

	Set_IndexBuffer(pIndices, 90, 4, 1, 2, 3);
	pVertices[22].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[23].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[11].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[10].vNormal += ComputeNormalVector(pVertices, 22, 11, 10);

	pIndices[96] = 9; pIndices[97] = 7; pIndices[98] = 8;
	pVertices[22].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[23].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[11].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);

	pIndices[99] = 6; pIndices[100] = 4; pIndices[101] = 5;
	pVertices[22].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[23].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[11].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
#pragma endregion

#pragma region 아래
	Set_IndexBuffer(pIndices, 102, 21, 12, 23, 22);
	pVertices[22].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[23].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[11].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[10].vNormal += ComputeNormalVector(pVertices, 22, 11, 10);

	Set_IndexBuffer(pIndices, 108, 28, 12, 21, 19);
	pVertices[22].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[23].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[11].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[10].vNormal += ComputeNormalVector(pVertices, 22, 11, 10);

	Set_IndexBuffer(pIndices, 114, 29, 28, 19, 18);
	pVertices[22].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[23].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[11].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[10].vNormal += ComputeNormalVector(pVertices, 22, 11, 10);

	Set_IndexBuffer(pIndices, 120, 13, 29, 18, 16);
	pVertices[22].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[23].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[11].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[10].vNormal += ComputeNormalVector(pVertices, 22, 11, 10);

	Set_IndexBuffer(pIndices, 126, 13, 16, 15, 14);
	pVertices[22].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[23].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[11].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[10].vNormal += ComputeNormalVector(pVertices, 22, 11, 10);

	pIndices[132] = 16; pIndices[133] = 18; pIndices[134] = 17;
	pVertices[22].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[23].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[11].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);

	pIndices[135] = 19; pIndices[136] = 21; pIndices[137] = 20;
	pVertices[22].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[23].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
	pVertices[11].vNormal += ComputeNormalVector(pVertices, 22, 23, 11);
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
		iNumber = 138;
		break;

	case DOWN:
		iNumber = 144;
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
