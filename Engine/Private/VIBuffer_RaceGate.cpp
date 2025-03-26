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
	m_iNumVertices = 30;					// 버텍스 갯수
	m_iVertexStride = sizeof(VTXPOSNORTEX);	// 버텍스 크기
	m_iFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;		// 버텍스 타입
	m_iNumPritimive = 50;					// 삼각형 갯수

	m_iIndexStride = 2;						// 인덱스 크기
	m_iNumIndices = 144;						// 인덱스 갯수
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
	pVertices[2].vTexcoord = _float2(1.f, 1.f);

	pVertices[3].vPosition = _float3(0.5f, 0.5f, -0.5f);
	pVertices[3].vTexcoord = _float2(0.f, 1.f);

	pVertices[4].vPosition = _float3(0.5f, 0.5f, -0.375f);
	pVertices[4].vTexcoord = _float2(1.f, 0.f);

	pVertices[5].vPosition = _float3(0.375f, 0.5f, -0.375f);
	pVertices[5].vTexcoord = _float2(0.f, 0.f);

	pVertices[6].vPosition = _float3(0.125f, 0.5f, -0.125f);
	pVertices[6].vTexcoord = _float2(0.f, 1.f);

	pVertices[7].vPosition = _float3(-0.125f, 0.5f, -0.125f);
	pVertices[7].vTexcoord = _float2(1.f, 1.f);

	pVertices[8].vPosition = _float3(-0.375f, 0.5f, -0.375f);
	pVertices[8].vTexcoord = _float2(0.f, 0.f);

	pVertices[9].vPosition = _float3(-0.5f, 0.5f, -0.375f);
	pVertices[9].vTexcoord = _float2(1.f, 0.f);

	pVertices[10].vPosition = _float3(-0.5f, 0.5f, -0.5f);
	pVertices[10].vTexcoord = _float2(0.f, 1.f);

	pVertices[11].vPosition = _float3(-3.0f, 0.5f, -0.5f);
	pVertices[11].vTexcoord = _float2(1.f, 1.f);

	pVertices[12].vPosition = _float3(-3.0f, -0.5f, 0.5f);
	pVertices[12].vTexcoord = _float2(0.f, 0.f);

	pVertices[13].vPosition = _float3(3.0f, -0.5f, 0.5f);
	pVertices[13].vTexcoord = _float2(1.f, 0.f);

	pVertices[14].vPosition = _float3(3.0f, -0.5f, -0.5f);
	pVertices[14].vTexcoord = _float2(1.f, 1.f);

	pVertices[15].vPosition = _float3(0.5f, -0.5f, -0.5f);
	pVertices[15].vTexcoord = _float2(0.f, 1.f);

	pVertices[16].vPosition = _float3(0.5f, -0.5f, -0.375f);
	pVertices[16].vTexcoord = _float2(1.f, 0.f);

	pVertices[17].vPosition = _float3(0.375f, -0.5f, -0.375f);
	pVertices[17].vTexcoord = _float2(0.f, 0.f);

	pVertices[18].vPosition = _float3(0.125f, -0.5f, -0.125f);
	pVertices[18].vTexcoord = _float2(0.f, 1.f);

	pVertices[19].vPosition = _float3(-0.125f, -0.5f, -0.125f);
	pVertices[19].vTexcoord = _float2(1.f, 1.f);

	pVertices[20].vPosition = _float3(-0.375f, -0.5f, -0.375f);
	pVertices[20].vTexcoord = _float2(0.f, 0.f);

	pVertices[21].vPosition = _float3(-0.5f, -0.5f, -0.375f);
	pVertices[21].vTexcoord = _float2(1.f, 0.f);

	pVertices[22].vPosition = _float3(-0.5f, -0.5f, -0.5f);
	pVertices[22].vTexcoord = _float2(0.f, 1.f);

	pVertices[23].vPosition = _float3(-3.0f, -0.5f, -0.5f);
	pVertices[23].vTexcoord = _float2(1.f, 1.f);

	pVertices[24].vPosition = _float3(0.125f, 0.f, -0.125f);
	pVertices[24].vTexcoord = _float2(0.f, 1.f);

	pVertices[25].vPosition = _float3(-0.125f, 0.f, -0.125f);
	pVertices[25].vTexcoord = _float2(1.f, 1.f);

	pVertices[26].vPosition = _float3(-0.125f, 0.5f, 0.5f);
	pVertices[26].vTexcoord = _float2(1.f, 1.f);

	pVertices[27].vPosition = _float3(0.125f, 0.5f, 0.5f);
	pVertices[27].vTexcoord = _float2(0.f, 1.f);

	pVertices[28].vPosition = _float3(-0.125f, -0.5f, 0.5f);
	pVertices[28].vTexcoord = _float2(1.f, 1.f);

	pVertices[29].vPosition = _float3(0.125f, -0.5f, 0.5f);
	pVertices[29].vTexcoord = _float2(0.f, 1.f);
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
	Set_IndexBuffer(pIndices, 6, 9, 8, 20, 21);
	Set_IndexBuffer(pIndices, 12, 8, 7, 19, 20);
	Set_IndexBuffer(pIndices, 18, 6, 5, 17, 18);
	Set_IndexBuffer(pIndices, 24, 5, 4, 11, 17);
	Set_IndexBuffer(pIndices, 30, 3, 2, 14, 15);
	Set_IndexBuffer(pIndices, 132, 7, 6, 24, 25);
	Set_IndexBuffer(pIndices, 138, 25, 24, 18, 19);
#pragma endregion

#pragma region 뒤
	Set_IndexBuffer(pIndices, 36, 1, 0, 12, 13);
#pragma endregion

#pragma region 왼쪽
	Set_IndexBuffer(pIndices, 42, 0, 11, 23, 12);
	Set_IndexBuffer(pIndices, 48, 4, 3, 15, 11);
#pragma endregion

#pragma region 오른쪽
	Set_IndexBuffer(pIndices, 54, 10, 9, 21, 22);
	Set_IndexBuffer(pIndices, 60, 2, 1, 13, 14);
#pragma endregion

#pragma region 위
	Set_IndexBuffer(pIndices, 60, 0, 9, 10, 11);
	Set_IndexBuffer(pIndices, 66, 0, 26, 7, 9);
	Set_IndexBuffer(pIndices, 72, 26, 27, 6, 7);
	Set_IndexBuffer(pIndices, 78, 27, 1, 4, 6);
	Set_IndexBuffer(pIndices, 84, 4, 1, 2, 3);
	pIndices[90] = 9; pIndices[91] = 7; pIndices[92] = 8;
	pIndices[93] = 6; pIndices[94] = 4; pIndices[95] = 5;
#pragma endregion

#pragma region 아래
	Set_IndexBuffer(pIndices, 96, 21, 12, 23, 22);
	Set_IndexBuffer(pIndices, 102, 28, 12, 21, 19);
	Set_IndexBuffer(pIndices, 108, 29, 28, 19, 18);
	Set_IndexBuffer(pIndices, 114, 13, 29, 18, 11);
	Set_IndexBuffer(pIndices, 120, 13, 11, 15, 14);
	pIndices[126] = 11; pIndices[127] = 18; pIndices[128] = 17;
	pIndices[129] = 19; pIndices[130] = 21; pIndices[131] = 20;
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
