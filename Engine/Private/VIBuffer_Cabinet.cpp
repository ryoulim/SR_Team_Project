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
	m_iNumVertices = 48;
	m_iVertexStride = sizeof(VTXPOSTEX);
	m_iFVF = D3DFVF_XYZ | D3DFVF_TEX1;
	m_iNumPritimive = 50; //40 + 10

	m_iIndexStride = 2;
	m_iNumIndices = 150;
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

	/*pVertices[0].vNormal = ComputeNormalVector(pVertices, 0, 1, 2);
	pVertices[1].vNormal = ComputeNormalVector(pVertices, 0, 1, 2);
	pVertices[2].vNormal = ComputeNormalVector(pVertices, 0, 1, 2);
	pVertices[3].vNormal = ComputeNormalVector(pVertices, 0, 2, 3);*/
#pragma endregion

#pragma region 외부 뒷면
	pVertices[4].vPosition = _float3(-0.5f, 1.f, 0.5f);
	pVertices[4].vTexcoord = _float2(1.f, 0.f);

	pVertices[5].vPosition = _float3(0.5f, 1.f, 0.5f);
	pVertices[5].vTexcoord = _float2(0.f, 0.f);

	pVertices[6].vPosition = _float3(0.5f, -1.f, 0.5f);
	pVertices[6].vTexcoord = _float2(0.f, 1.f);

	pVertices[7].vPosition = _float3(-0.5f, -1.f, 0.5f);
	pVertices[7].vTexcoord = _float2(1.f, 1.f);

	/*pVertices[4].vNormal = ComputeNormalVector(pVertices, 5, 4, 7);
	pVertices[5].vNormal = ComputeNormalVector(pVertices, 5, 4, 7);
	pVertices[6].vNormal = ComputeNormalVector(pVertices, 5, 7, 6);
	pVertices[7].vNormal = ComputeNormalVector(pVertices, 5, 4, 7);*/
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
	pVertices[12].vPosition = _float3(-0.4f, 0.9f, 0.4f);
	pVertices[12].vTexcoord = _float2(1.f, 0.f);

	pVertices[13].vPosition = _float3(0.4f, 0.9f, 0.4f);
	pVertices[13].vTexcoord = _float2(1.f, 1.f);

	pVertices[14].vPosition = _float3(0.4f, -0.9f, 0.4f);
	pVertices[14].vTexcoord = _float2(1.f, 0.f);

	pVertices[15].vPosition = _float3(-0.4f, -0.9f, 0.4f);
	pVertices[15].vTexcoord = _float2(1.f, 1.f);
#pragma endregion

#pragma region 정면 좌측 상단
	pVertices[16].vPosition = _float3(-0.4f, 1.f, -0.5f);
	pVertices[16].vTexcoord = _float2(1.f, 0.f);
#pragma endregion

#pragma region 정면 우측 상단
	pVertices[17].vPosition = _float3(0.4f, 1.f, -0.5f);
	pVertices[17].vTexcoord = _float2(1.f, 1.f);
#pragma endregion

#pragma region 정면 우측 하단
	pVertices[18].vPosition = _float3(0.4f, -1.f, -0.5f);
	pVertices[18].vTexcoord = _float2(1.f, 0.f);
#pragma endregion

#pragma region 정면 좌측 하단
	pVertices[19].vPosition = _float3(-0.4f, -1.f, -0.5f);
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
	pVertices[24].vPosition = _float3(-0.4f, 0.5f, 0.4f);
	pVertices[24].vTexcoord = _float2(1.f, 1.f);

	pVertices[25].vPosition = _float3(0.4f, 0.5f, 0.4f);
	pVertices[25].vTexcoord = _float2(1.f, 0.f);

	pVertices[26].vPosition = _float3(0.4f, 0.4f, 0.4f);
	pVertices[26].vTexcoord = _float2(1.f, 1.f);

	pVertices[27].vPosition = _float3(-0.4f, 0.4f, 0.4f);
	pVertices[27].vTexcoord = _float2(1.f, 0.f);
#pragma endregion





#pragma region 윗면
	pVertices[28].vPosition = _float3(-0.5f, 1.f, 0.5f);
	pVertices[28].vTexcoord = _float2(1.f, 0.f);

	pVertices[29].vPosition = _float3(0.5f, 1.f, 0.5f);
	pVertices[29].vTexcoord = _float2(0.f, 0.f);

	pVertices[30].vPosition = _float3(0.5f, 1.f, -0.5f);
	pVertices[30].vTexcoord = _float2(1.f, 1.f);

	pVertices[31].vPosition = _float3(-0.5f, 1.f, -0.5f);
	pVertices[31].vTexcoord = _float2(0.f, 1.f);
#pragma endregion

#pragma region 정면 상부
	pVertices[32].vPosition = _float3(-0.4f, 0.9f, 0.4f);
	pVertices[32].vTexcoord = _float2(1.f, 0.f);

	pVertices[33].vPosition = _float3(0.4f, 0.9f, 0.4f);
	pVertices[33].vTexcoord = _float2(0.f, 0.f);

	pVertices[34].vPosition = _float3(0.4f, 0.5f, 0.4f);
	pVertices[34].vTexcoord = _float2(0.f, 1.f);

	pVertices[35].vPosition = _float3(-0.4f, 0.5f, 0.4f);
	pVertices[35].vTexcoord = _float2(1.f, 1.f);
#pragma endregion

#pragma region 정면 중간
	pVertices[36].vPosition = _float3(-0.4f, 0.5f, -0.5f);
	pVertices[36].vTexcoord = _float2(0.f, 0.f);

	pVertices[37].vPosition = _float3(0.4f, 0.5f, -0.5f);
	pVertices[37].vTexcoord = _float2(1.f, 0.f);

	pVertices[38].vPosition = _float3(0.4f, 0.4f, -0.5f);
	pVertices[38].vTexcoord = _float2(1.f, 1.f);

	pVertices[39].vPosition = _float3(-0.4f, 0.4f, -0.5f);
	pVertices[39].vTexcoord = _float2(0.f, 1.f);
#pragma endregion

#pragma region 정면 하부
	pVertices[40].vPosition = _float3(-0.4f, 0.4f, 0.4f);
	pVertices[40].vTexcoord = _float2(1.f, 0.f);

	pVertices[41].vPosition = _float3(0.4f, 0.4f, 0.4f);
	pVertices[41].vTexcoord = _float2(0.f, 0.f);

	pVertices[42].vPosition = _float3(0.4f, -0.9f, 0.4f);
	pVertices[42].vTexcoord = _float2(0.f, 1.f);

	pVertices[43].vPosition = _float3(-0.4f, -0.9f, 0.4f);
	pVertices[43].vTexcoord = _float2(1.f, 1.f);
#pragma endregion

#pragma region 정면 오른쪽
	pVertices[44].vPosition = _float3(0.4f, 1.f, -0.5f);
	pVertices[44].vTexcoord = _float2(0.f, 0.f);

	pVertices[45].vPosition = _float3(0.5f, 1.f, -0.5f);
	pVertices[45].vTexcoord = _float2(1.f, 0.f);

	pVertices[46].vPosition = _float3(0.5f, -1.f, -0.5f);
	pVertices[46].vTexcoord = _float2(1.f, 1.f);

	pVertices[47].vPosition = _float3(0.4f, -1.f, -0.5f);
	pVertices[47].vTexcoord = _float2(0.f, 1.f);
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
	Set_IndexBuffer(pIndices, 18, 12, 13, 25, 24);
	Set_IndexBuffer(pIndices, 24, 20, 21, 22, 23);
	Set_IndexBuffer(pIndices, 30, 27, 26, 14, 15);
	Set_IndexBuffer(pIndices, 36, 11, 10, 18, 19);
#pragma endregion

	///////////////////////////////////////////
#pragma region 뒤
	Set_IndexBuffer(pIndices, 132, 5, 4, 7, 6);
#pragma endregion

#pragma region 왼쪽
	Set_IndexBuffer(pIndices, 138, 4, 0, 3, 7);
#pragma endregion

#pragma region 오른쪽
	Set_IndexBuffer(pIndices, 144, 1, 5, 6, 2);
#pragma endregion
	///////////////////////////////////////////

#pragma region 내부 왼쪽
	Set_IndexBuffer(pIndices, 108, 13, 9, 21, 25);
	Set_IndexBuffer(pIndices, 114, 26, 22, 10, 14);
#pragma endregion

#pragma region 내부 오른쪽
	Set_IndexBuffer(pIndices, 120, 8, 12, 24, 20);
	Set_IndexBuffer(pIndices, 126, 23, 27, 15, 11);
#pragma endregion

#pragma region 위
	Set_IndexBuffer(pIndices, 42, 4, 5, 1, 0);
	Set_IndexBuffer(pIndices, 48, 24, 25, 21, 20);
	Set_IndexBuffer(pIndices, 54, 15, 14, 10, 11);
#pragma endregion

#pragma region 아래
	Set_IndexBuffer(pIndices, 60, 8, 9, 13, 12);
	Set_IndexBuffer(pIndices, 66, 23, 22, 26, 27);
	Set_IndexBuffer(pIndices, 72, 3, 2, 6, 7);
#pragma endregion



#pragma region 윗면
	Set_IndexBuffer(pIndices, 78, 28, 29, 30, 31);
#pragma endregion

#pragma region 정면 상부
	Set_IndexBuffer(pIndices, 84, 32, 33, 34, 35);
#pragma endregion

#pragma region 정면 중간
	Set_IndexBuffer(pIndices, 90, 36, 37, 38, 39);
#pragma endregion

#pragma region 정면 하부
	Set_IndexBuffer(pIndices, 96, 40, 41, 42, 43);
#pragma endregion

#pragma region 정면 오른쪽
	Set_IndexBuffer(pIndices, 102, 44, 45, 46, 47);
#pragma endregion

	m_pIB->Unlock();
#pragma endregion
	return S_OK;
}

HRESULT CVIBuffer_Cabinet::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_Cabinet::Render(_uint Surface)
{
	_uint iNumber{ 0 };
	
	switch (Surface)
	{
	case COMMON:
		iNumber = 0;
		break;

	case BACK:
		iNumber = 132;
		break;

	case SIDE:
		iNumber = 138;
		break;

	default:
		return E_FAIL;
		break;
	}
	
	return m_pGraphic_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iNumVertices, iNumber, m_iNumPritimive);
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

VECTOR CVIBuffer_Cabinet::ComputeNormalVector(VTXPOSNORTEX* pVertices, _uint vertex1, _uint vertex2, _uint vertex3)
{
	return __super::ComputeNormal(&(pVertices[vertex1].vPosition), &(pVertices[vertex2].vPosition), &(pVertices[vertex3].vPosition));
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
