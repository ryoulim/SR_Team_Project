#include "VIBuffer_Cabinet.h"

CVIBuffer_Cabinet::CVIBuffer_Cabinet(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
}

CVIBuffer_Cabinet::CVIBuffer_Cabinet(const CVIBuffer_Cabinet& Prototype)
	: CVIBuffer(Prototype)
{
}

HRESULT CVIBuffer_Cabinet::Initialize_Prototype()
{
	m_iNumVertices = 28;
	m_iVertexStride = sizeof(VTXPOSTEX);
	m_iFVF = D3DFVF_XYZ | D3DFVF_TEX1;
	m_iNumPritimive = 40;

	m_iIndexStride = 2;
	m_iNumIndices = 120;
	m_eIndexFormat = D3DFMT_INDEX16;

#pragma region VERTEX_BUFFER
	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXPOSTEX* pVertices = { nullptr };

	m_pVB->Lock(0, 0, reinterpret_cast<void**>(&pVertices), 0);

#pragma region 외부 앞면
	pVertices[0].vPosition = _float3(-0.5f, 1.f, -0.5f);
	pVertices[0].vTexcoord = _float2(0.f, 0.f);

	pVertices[1].vPosition = _float3(0.5f, 1.f, -0.5f);
	pVertices[1].vTexcoord = _float2(1.f, 0.f);

	pVertices[2].vPosition = _float3(0.5f, -1.f, -0.5f);
	pVertices[2].vTexcoord = _float2(1.f, 1.f);

	pVertices[3].vPosition = _float3(-0.5f, -1.f, -0.5f);
	pVertices[3].vTexcoord = _float2(0.f, 1.f);
#pragma endregion

#pragma region 외부 뒷면
	pVertices[4].vPosition = _float3(0.5f, 1.f, 0.5f);
	pVertices[4].vTexcoord = _float2(0.f, 0.f);

	pVertices[5].vPosition = _float3(-0.5f, 1.f, 0.5f);
	pVertices[5].vTexcoord = _float2(1.f, 0.f);

	pVertices[6].vPosition = _float3(-0.5f, -1.f, 0.5f);
	pVertices[6].vTexcoord = _float2(1.f, 1.f);

	pVertices[7].vPosition = _float3(0.5f, -1.f, 0.5f);
	pVertices[7].vTexcoord = _float2(0.f, 1.f);
#pragma endregion

#pragma region 내부 앞면
	pVertices[8].vPosition = _float3(-0.4f, 0.9f, -0.5f);
	pVertices[8].vTexcoord = _float2(0.f, 0.f);

	pVertices[9].vPosition = _float3(0.4f, 0.9f, -0.5f);
	pVertices[9].vTexcoord = _float2(0.f, 1.f);

	pVertices[10].vPosition = _float3(0.4f, -0.9f, -0.5f);
	pVertices[10].vTexcoord = _float2(0.f, 0.f);

	pVertices[11].vPosition = _float3(-0.4f, -0.9f, -0.5f);
	pVertices[11].vTexcoord = _float2(0.f, 1.f);
#pragma endregion

#pragma region 내부 뒷면
	pVertices[12].vPosition = _float3(0.4f, 0.9f, 0.4f);
	pVertices[12].vTexcoord = _float2(1.f, 1.f);

	pVertices[13].vPosition = _float3(-0.4f, 0.9f, 0.4f);
	pVertices[13].vTexcoord = _float2(1.f, 0.f);

	pVertices[14].vPosition = _float3(-0.4f, -0.9f, 0.4f);
	pVertices[14].vTexcoord = _float2(1.f, 1.f);

	pVertices[15].vPosition = _float3(0.4f, -0.9f, 0.4f);
	pVertices[15].vTexcoord = _float2(1.f, 0.f);
#pragma endregion

#pragma region 정면 좌측 상단
	pVertices[16].vPosition = pVertices[0].vPosition + _float3(0.1f, 0.f, 0.f);
	pVertices[16].vTexcoord = _float2(1.f, 0.f);
#pragma endregion

#pragma region 정면 우측 상단
	pVertices[17].vPosition = pVertices[1].vPosition + _float3(-0.1f, 0.f, 0.f);
	pVertices[17].vTexcoord = _float2(1.f, 1.f);
#pragma endregion

#pragma region 정면 우측 하단
	pVertices[18].vPosition = pVertices[2].vPosition + _float3(-0.1f, 0.f, 0.f);
	pVertices[18].vTexcoord = _float2(1.f, 0.f);
#pragma endregion

#pragma region 정면 좌측 하단
	pVertices[19].vPosition = pVertices[3].vPosition + _float3(0.1f, 0.f, 0.f);
	pVertices[19].vTexcoord = _float2(1.f, 1.f);
#pragma endregion

#pragma region 층 나누기 앞면
	pVertices[20].vPosition = _float3(-0.4f, 0.5f, -0.5f);
	pVertices[20].vTexcoord = _float2(0.f, 1.f);

	pVertices[21].vPosition = _float3(0.4f, 0.5f, -0.5f);
	pVertices[21].vTexcoord = _float2(0.f, 0.f);

	pVertices[22].vPosition = _float3(0.4f, 0.4f, -0.5f);
	pVertices[22].vTexcoord = _float2(0.f, 1.f);

	pVertices[23].vPosition = _float3(-0.4f, 0.4f, -0.5f);
	pVertices[23].vTexcoord = _float2(0.f, 0.f);
#pragma endregion

#pragma region 층 나누기 뒷면
	pVertices[24].vPosition = _float3(0.4f, 0.5f, 0.4f);
	pVertices[24].vTexcoord = _float2(1.f, 0.f);

	pVertices[25].vPosition = _float3(-0.4f, 0.5f, 0.4f);
	pVertices[25].vTexcoord = _float2(1.f, 1.f);

	pVertices[26].vPosition = _float3(-0.4f, 0.4f, 0.4f);
	pVertices[26].vTexcoord = _float2(1.f, 0.f);

	pVertices[27].vPosition = _float3(0.4f, 0.4f, 0.4f);
	pVertices[27].vTexcoord = _float2(1.f, 1.f);
#pragma endregion
	m_pVB->Unlock();
#pragma endregion

#pragma region INDEX_BUFFER
	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	_ushort* pIndices = { nullptr };

	m_pIB->Lock(0, 0, reinterpret_cast<void**>(&pIndices), 0);
	
#pragma region 앞
	Set_IndexBuffer(pIndices, 0, 0, 16, 19, 3);
	Set_IndexBuffer(pIndices, 6, 17, 1, 2, 18);

	Set_IndexBuffer(pIndices, 12, 16, 17, 9, 8);
	Set_IndexBuffer(pIndices, 18, 13, 12, 24, 25);
	Set_IndexBuffer(pIndices, 24, 20, 21, 22, 23);
	Set_IndexBuffer(pIndices, 30, 26, 27, 15, 14);
	Set_IndexBuffer(pIndices, 36, 11, 10, 18, 19);
#pragma endregion

#pragma region 뒤
	Set_IndexBuffer(pIndices, 42, 4, 5, 6, 7);
#pragma endregion

#pragma region 왼쪽
	Set_IndexBuffer(pIndices, 48, 5, 0, 3, 6);
	Set_IndexBuffer(pIndices, 54, 12, 9, 21, 24);
	Set_IndexBuffer(pIndices, 60, 27, 22, 10, 15);
#pragma endregion

#pragma region 오른쪽
	Set_IndexBuffer(pIndices, 66, 8, 13, 25, 20);
	Set_IndexBuffer(pIndices, 72, 23, 26, 14, 11);
	Set_IndexBuffer(pIndices, 78, 1, 4, 7, 2);
#pragma endregion
	
#pragma region 위
	Set_IndexBuffer(pIndices, 84, 5, 4, 1, 0);
	Set_IndexBuffer(pIndices, 90, 25, 24, 21, 20);
	Set_IndexBuffer(pIndices, 96, 14, 15, 10, 11);
#pragma endregion

#pragma region 아래
	Set_IndexBuffer(pIndices, 102, 8, 9, 12, 13);
	Set_IndexBuffer(pIndices, 108, 23, 22, 27, 26);
	Set_IndexBuffer(pIndices, 114, 3, 2, 7, 6);
#pragma endregion

	m_pIB->Unlock();
#pragma endregion
	return S_OK;
}

HRESULT CVIBuffer_Cabinet::Initialize(void* pArg)
{
	return S_OK;
}

void CVIBuffer_Cabinet::Set_IndexBuffer(_ushort* pIndices, _uint StartIndex, _uint LT, _uint RT, _uint RB, _uint LB)
{
	pIndices[StartIndex++] = LT;
	pIndices[StartIndex++] = RT;
	pIndices[StartIndex++] = RB;

	pIndices[StartIndex++] = LT;
	pIndices[StartIndex++] = RB;
	pIndices[StartIndex++] = LB;
}

CVIBuffer_Cabinet* CVIBuffer_Cabinet::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_Cabinet* pInstance = new CVIBuffer_Cabinet(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Cabinet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Cabinet::Clone(void* pArg)
{
	CVIBuffer_Cabinet* pInstance = new CVIBuffer_Cabinet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CVIBuffer_Cabinet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Cabinet::Free()
{
	__super::Free();
}
