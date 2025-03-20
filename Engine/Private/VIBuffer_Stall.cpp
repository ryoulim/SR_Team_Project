#include "VIBuffer_Stall.h"

CVIBuffer_Stall::CVIBuffer_Stall(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
}

CVIBuffer_Stall::CVIBuffer_Stall(const CVIBuffer_Stall& Prototype)
	: CVIBuffer(Prototype)
{
}

HRESULT CVIBuffer_Stall::Initialize_Prototype()
{
	m_iNumVertices = 33;
	m_iVertexStride = sizeof(VTXPOSNORTEX);
	m_iFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
	m_iNumPritimive = 38;

	m_iIndexStride = 2;
	m_iNumIndices = 108;
	m_eIndexFormat = D3DFMT_INDEX16;

#pragma region VERTEX_BUFFER
	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXPOSNORTEX* pVertices = { nullptr };

	m_pVB->Lock(0, 0, reinterpret_cast<void**>(&pVertices), 0);

	for (int i = 0; i < m_iNumVertices; i++)
		pVertices[i].vNormal = _float3(0.f, 0.f, 0.f);

#pragma region 앞면
	pVertices[0].vPosition = _float3(-0.5f, 1.f, -0.5f);
	pVertices[0].vTexcoord = _float2(0.f, 0.f);
	
	pVertices[1].vPosition = _float3(0.5f, 0.f, -0.5f);
	pVertices[1].vTexcoord = _float2(1.f, 1.f);

	pVertices[2].vPosition = _float3(0.5f, -1.f, -0.5f);
	pVertices[2].vTexcoord = _float2(1.f, 2.f);

	pVertices[3].vPosition = _float3(-0.5f, -1.f, -0.5f);
	pVertices[3].vTexcoord = _float2(0.f, 2.f);

	pVertices[4].vPosition = _float3(-0.5f, 0.f, -0.5f);
	pVertices[4].vTexcoord = _float2(0.f, 1.f);
#pragma endregion

#pragma region 뒷면
	pVertices[5].vPosition = _float3(-0.5f, 1.f, 0.5f);
	pVertices[5].vTexcoord = _float2(1.f, 0.f);

	pVertices[6].vPosition = _float3(0.5f, 0.f, 0.5f);
	pVertices[6].vTexcoord = _float2(0.f, 1.f);

	pVertices[7].vPosition = _float3(0.5f, -1.f, 0.5f);
	pVertices[7].vTexcoord = _float2(0.f, 2.f);

	pVertices[8].vPosition = _float3(-0.5f, -1.f, 0.5f);
	pVertices[8].vTexcoord = _float2(1.f, 2.f);

	pVertices[9].vPosition = _float3(-0.5f, 0.f, 0.5f);
	pVertices[9].vTexcoord = _float2(1.f, 1.f);
#pragma endregion

#pragma region 정면 사각형
	pVertices[10].vPosition = _float3(-0.375f , 0.875f, -0.4375f);
	pVertices[10].vTexcoord = _float2(0.f, 1.f);

	pVertices[11].vPosition = _float3(0.375f, 0.125f, -0.4375f);
	pVertices[11].vTexcoord = _float2(0.f, 0.f);

	pVertices[12].vPosition = _float3(0.25f, 0.125f, -0.4375f);
	pVertices[12].vTexcoord = _float2(0.f, 1.f);

	pVertices[13].vPosition = _float3(-0.375f, 0.75f, -0.4375f);
	pVertices[13].vTexcoord = _float2(0.f, 0.f);
#pragma endregion

#pragma region 후면 사각형
	pVertices[14].vPosition = _float3(-0.375f, 0.875f, 0.4375f);
	pVertices[14].vTexcoord = _float2(1.f, 1.f);

	pVertices[15].vPosition = _float3(0.375f, 0.125f, 0.4375f);
	pVertices[15].vTexcoord = _float2(1.f, 0.f);

	pVertices[16].vPosition = _float3(0.25f, 0.125f, 0.4375f);
	pVertices[16].vTexcoord = _float2(1.f, 1.f);

	pVertices[17].vPosition = _float3(-0.375f, 0.75f, 0.4375f);
	pVertices[17].vTexcoord = _float2(1.f, 0.f);
#pragma endregion

#pragma region 정면 추가
	pVertices[18].vPosition = _float3(-0.5f, 1.f, -0.4375f);
	pVertices[18].vTexcoord = _float2(0.f, 0.f);

	pVertices[19].vPosition = _float3(0.5f, 0.f, -0.4375f);
	pVertices[19].vTexcoord = _float2(2.f, 1.f);

	pVertices[20].vPosition = _float3(-0.5f, 1.f, 0.4375f);
	pVertices[20].vTexcoord = _float2(1.f, 0.f);

	pVertices[21].vPosition = _float3(0.5f, 0.f, 0.4375f);
	pVertices[21].vTexcoord = _float2(0.f, 1.f);
#pragma endregion

#pragma region 10과 11의 중점
	pVertices[22].vPosition = _float3(0.f, 0.5f, -0.4375f);
	pVertices[22].vTexcoord = _float2(0.f, 1.f);
#pragma endregion

#pragma region 14와 15의 중점
	pVertices[23].vPosition = _float3(0.f, 0.5f, 0.4375f);
	pVertices[23].vTexcoord = _float2(0.f, 1.f);
#pragma endregion

#pragma region 두께 추가
	pVertices[24].vPosition = _float3(-0.625f, 1.f, -0.5f);
	pVertices[24].vTexcoord = _float2(0.f, 0.f);

	pVertices[25].vPosition = _float3(-0.625f, -1.f, -0.5f);
	pVertices[25].vTexcoord = _float2(0.f, 2.f);

	pVertices[26].vPosition = _float3(-0.625f, 1.f, 0.5f);
	pVertices[26].vTexcoord = _float2(1.f, 0.f);

	pVertices[27].vPosition = _float3(-0.625f, -1.f, 0.5f);
	pVertices[27].vTexcoord = _float2(1.f, 2.f);
#pragma endregion

#pragma region 텍스쳐 때문에 추가
	pVertices[28].vPosition = _float3(-0.5f, 1.f, -0.5f);		//0
	pVertices[28].vTexcoord = _float2(1.f, 0.f);	

	pVertices[29].vPosition = _float3(-0.5f, -1.f, -0.5f);		//3
	pVertices[29].vTexcoord = _float2(1.f, 2.f);

	pVertices[30].vPosition = _float3(-0.5f, 1.f, 0.5f);		//5
	pVertices[30].vTexcoord = _float2(0.f, 0.f);

	pVertices[31].vPosition = _float3(-0.5f, -1.f, 0.5f);		//8
	pVertices[31].vTexcoord = _float2(0.f, 2.f);

	pVertices[32].vPosition = _float3(-0.5f, 1.f, 0.5f);		//5
	pVertices[32].vTexcoord = _float2(1.f, 1.f);
#pragma endregion
	
#pragma endregion

#pragma region INDEX_BUFFER
	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	_ushort* pIndices = { nullptr };

	m_pIB->Lock(0, 0, reinterpret_cast<void**>(&pIndices), 0);
	
#pragma region 정면 둘레
	Set_IndexBuffer(pIndices, 0, 0, 18, 19, 1);
	pVertices[0].vNormal += ComputeNormalVector(pVertices, 0, 18, 19);
	pVertices[18].vNormal += ComputeNormalVector(pVertices, 0, 18, 19);
	pVertices[19].vNormal += ComputeNormalVector(pVertices, 0, 18, 19);
	pVertices[1].vNormal += ComputeNormalVector(pVertices, 0, 19, 1);

	Set_IndexBuffer(pIndices, 6, 18, 20, 14, 10);
	pVertices[18].vNormal += ComputeNormalVector(pVertices, 18, 20, 14);
	pVertices[20].vNormal += ComputeNormalVector(pVertices, 18, 20, 14);
	pVertices[14].vNormal += ComputeNormalVector(pVertices, 18, 20, 14);
	pVertices[10].vNormal += ComputeNormalVector(pVertices, 18, 14, 10);

	Set_IndexBuffer(pIndices, 12, 20, 5, 6, 21);
	pVertices[20].vNormal += ComputeNormalVector(pVertices, 20, 5, 6);
	pVertices[5].vNormal += ComputeNormalVector(pVertices, 20, 5, 6);
	pVertices[6].vNormal += ComputeNormalVector(pVertices, 20, 5, 6);
	pVertices[21].vNormal += ComputeNormalVector(pVertices, 20, 6, 21);

	Set_IndexBuffer(pIndices, 18, 11, 15, 21, 19);
	pVertices[11].vNormal += ComputeNormalVector(pVertices, 11, 15, 21);
	pVertices[15].vNormal += ComputeNormalVector(pVertices, 11, 15, 21);
	pVertices[21].vNormal += ComputeNormalVector(pVertices, 11, 15, 21);
	pVertices[19].vNormal += ComputeNormalVector(pVertices, 11, 21, 19);
#pragma endregion

#pragma region 정면 안쪽
	Set_IndexBuffer(pIndices, 24, 10, 14, 17, 13);
	pVertices[10].vNormal += ComputeNormalVector(pVertices, 10, 14, 17);
	pVertices[14].vNormal += ComputeNormalVector(pVertices, 10, 14, 17);
	pVertices[17].vNormal += ComputeNormalVector(pVertices, 10, 14, 17);
	pVertices[13].vNormal += ComputeNormalVector(pVertices, 10, 17, 13);

	Set_IndexBuffer(pIndices, 30, 12, 16, 15, 11);
	pVertices[12].vNormal += ComputeNormalVector(pVertices, 12, 16, 15);
	pVertices[16].vNormal += ComputeNormalVector(pVertices, 12, 16, 15);
	pVertices[15].vNormal += ComputeNormalVector(pVertices, 12, 16, 15);
	pVertices[11].vNormal += ComputeNormalVector(pVertices, 12, 15, 11);

	/*Set_IndexBuffer(pIndices, 36, 12, 16, 15, 11);
	pVertices[12].vNormal += ComputeNormalVector(pVertices, 33, 18, 47);
	pVertices[16].vNormal += ComputeNormalVector(pVertices, 33, 18, 47);
	pVertices[15].vNormal += ComputeNormalVector(pVertices, 33, 18, 47);
	pVertices[11].vNormal += ComputeNormalVector(pVertices, 47, 22, 37);

	Set_IndexBuffer(pIndices, 42, 12, 16, 15, 11);
	pVertices[33].vNormal += ComputeNormalVector(pVertices, 33, 18, 47);
	pVertices[18].vNormal += ComputeNormalVector(pVertices, 33, 18, 47);
	pVertices[47].vNormal += ComputeNormalVector(pVertices, 33, 18, 47);
	pVertices[22].vNormal += ComputeNormalVector(pVertices, 47, 22, 37);*/
	
	pIndices[36] = 12; pIndices[37] = 11; pIndices[38] = 22;
	pIndices[39] = 12; pIndices[40] = 22; pIndices[41] = 13;
	pIndices[42] = 13; pIndices[43] = 22; pIndices[44] = 10;
	pVertices[11].vNormal += ComputeNormalVector(pVertices, 11, 22, 12);
	pVertices[12].vNormal += ComputeNormalVector(pVertices, 11, 22, 12);
	pVertices[22].vNormal += ComputeNormalVector(pVertices, 11, 22, 12);
	pVertices[13].vNormal += ComputeNormalVector(pVertices, 22, 10, 13);
	pVertices[10].vNormal += ComputeNormalVector(pVertices, 22, 10, 13);

	pIndices[45] = 17; pIndices[46] = 14; pIndices[47] = 23;
	pIndices[48] = 17; pIndices[49] = 23; pIndices[50] = 16;
	pIndices[51] = 16; pIndices[52] = 23; pIndices[53] = 15;
	pVertices[14].vNormal += ComputeNormalVector(pVertices, 14, 23, 17);
	pVertices[17].vNormal += ComputeNormalVector(pVertices, 14, 23, 17);
	pVertices[23].vNormal += ComputeNormalVector(pVertices, 14, 23, 17);
	pVertices[16].vNormal += ComputeNormalVector(pVertices, 16, 23, 15);
	pVertices[15].vNormal += ComputeNormalVector(pVertices, 16, 23, 15);
#pragma endregion

#pragma region 왼쪽
	pIndices[54] = 0; pIndices[55] = 1; pIndices[56] = 4;
	pVertices[0].vNormal += ComputeNormalVector(pVertices, 4, 0, 1);
	pVertices[1].vNormal += ComputeNormalVector(pVertices, 4, 0, 1);
	pVertices[4].vNormal += ComputeNormalVector(pVertices, 4, 0, 1);

	Set_IndexBuffer(pIndices, 57, 4, 1, 2, 3);
	pVertices[4].vNormal += ComputeNormalVector(pVertices, 4, 1, 2);
	pVertices[1].vNormal += ComputeNormalVector(pVertices, 4, 1, 2);
	pVertices[2].vNormal += ComputeNormalVector(pVertices, 4, 1, 2);
	pVertices[3].vNormal += ComputeNormalVector(pVertices, 4, 2, 3);
#pragma endregion

#pragma region 오른쪽
	pIndices[63] = 6; pIndices[64] = 5; pIndices[65] = 9;
	pVertices[6].vNormal += ComputeNormalVector(pVertices, 6, 5, 9);
	pVertices[5].vNormal += ComputeNormalVector(pVertices, 6, 5, 9);
	pVertices[9].vNormal += ComputeNormalVector(pVertices, 6, 5, 9);
	
	Set_IndexBuffer(pIndices, 66, 6, 9, 8, 7);
	pVertices[6].vNormal += ComputeNormalVector(pVertices, 6, 9, 8);
	pVertices[9].vNormal += ComputeNormalVector(pVertices, 6, 9, 8);
	pVertices[8].vNormal += ComputeNormalVector(pVertices, 6, 9, 8);
	pVertices[7].vNormal += ComputeNormalVector(pVertices, 6, 8, 7);
#pragma endregion

#pragma region 앞
	Set_IndexBuffer(pIndices, 72, 1, 6, 7, 2);
	pVertices[1].vNormal += ComputeNormalVector(pVertices, 1, 6, 7);
	pVertices[6].vNormal += ComputeNormalVector(pVertices, 1, 6, 7);
	pVertices[7].vNormal += ComputeNormalVector(pVertices, 1, 6, 7);
	pVertices[2].vNormal += ComputeNormalVector(pVertices, 1, 7, 2);
#pragma endregion

#pragma region 뒤
	Set_IndexBuffer(pIndices, 78, 26, 24, 25, 27);
	pVertices[26].vNormal += ComputeNormalVector(pVertices, 26, 24, 25);
	pVertices[24].vNormal += ComputeNormalVector(pVertices, 26, 24, 25);
	pVertices[25].vNormal += ComputeNormalVector(pVertices, 26, 24, 25);
	pVertices[27].vNormal += ComputeNormalVector(pVertices, 26, 25, 27);
#pragma endregion

#pragma region 정면 이미지
	Set_IndexBuffer(pIndices, 102, 13, 17, 16, 12);
	pVertices[13].vNormal += ComputeNormalVector(pVertices, 13, 17, 16);
	pVertices[17].vNormal += ComputeNormalVector(pVertices, 13, 17, 16);
	pVertices[16].vNormal += ComputeNormalVector(pVertices, 13, 17, 16);
	pVertices[12].vNormal += ComputeNormalVector(pVertices, 13, 16, 12);
#pragma endregion

#pragma region 두께 추가
	Set_IndexBuffer(pIndices, 84, 24, 28, 29, 25);
	pVertices[24].vNormal += ComputeNormalVector(pVertices, 24, 0, 3);
	pVertices[0].vNormal += ComputeNormalVector(pVertices, 24, 0, 3);
	pVertices[3].vNormal += ComputeNormalVector(pVertices, 24, 0, 3);
	pVertices[25].vNormal += ComputeNormalVector(pVertices, 24, 3, 25);

	Set_IndexBuffer(pIndices, 90, 30, 26, 27, 31);
	pVertices[5].vNormal += ComputeNormalVector(pVertices, 5, 26, 27);
	pVertices[26].vNormal += ComputeNormalVector(pVertices, 5, 26, 27);
	pVertices[27].vNormal += ComputeNormalVector(pVertices, 5, 26, 27);
	pVertices[8].vNormal += ComputeNormalVector(pVertices, 5, 27, 8);

	Set_IndexBuffer(pIndices, 96, 26, 32, 28, 24);
	pVertices[26].vNormal += ComputeNormalVector(pVertices, 26, 5, 28);
	pVertices[5].vNormal += ComputeNormalVector(pVertices, 26, 5, 28);
	pVertices[28].vNormal += ComputeNormalVector(pVertices, 26, 5, 28);
	pVertices[24].vNormal += ComputeNormalVector(pVertices, 26, 28, 24);
#pragma endregion

	m_pVB->Unlock();
	m_pIB->Unlock();
#pragma endregion
	return S_OK;
}

HRESULT CVIBuffer_Stall::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_Stall::Render(_uint Surface)
{		
	Surface *= 102;
	return m_pGraphic_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iNumVertices, Surface, m_iNumPritimive);
}

void CVIBuffer_Stall::Set_IndexBuffer(_ushort* pIndices, _uint StartIndex, _uint LT, _uint RT, _uint RB, _uint LB)
{
	pIndices[StartIndex++] = LT;
	pIndices[StartIndex++] = RT;
	pIndices[StartIndex++] = RB;

	pIndices[StartIndex++] = LT;
	pIndices[StartIndex++] = RB;
	pIndices[StartIndex++] = LB;
}

VECTOR CVIBuffer_Stall::ComputeNormalVector(VTXPOSNORTEX* pVertices, _uint vertex1, _uint vertex2, _uint vertex3)
{
	return __super::ComputeNormal(&(pVertices[vertex1].vPosition), &(pVertices[vertex2].vPosition), &(pVertices[vertex3].vPosition));
}

CVIBuffer_Stall* CVIBuffer_Stall::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_Stall* pInstance = new CVIBuffer_Stall(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Stall");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Stall::Clone(void* pArg)
{
	CVIBuffer_Stall* pInstance = new CVIBuffer_Stall(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CVIBuffer_Stall");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Stall::Free()
{
	__super::Free();
}
