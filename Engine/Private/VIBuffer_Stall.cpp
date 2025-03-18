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
	m_iVertexStride = sizeof(VTXPOSTEX);
	m_iFVF = D3DFVF_XYZ | D3DFVF_TEX1;
	m_iNumPritimive = 38;

	m_iIndexStride = 2;
	m_iNumIndices = 120;
	m_eIndexFormat = D3DFMT_INDEX16;

#pragma region VERTEX_BUFFER
	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXPOSTEX* pVertices = { nullptr };

	m_pVB->Lock(0, 0, reinterpret_cast<void**>(&pVertices), 0);

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
	m_pVB->Unlock();
#pragma endregion

#pragma region INDEX_BUFFER
	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	_ushort* pIndices = { nullptr };

	m_pIB->Lock(0, 0, reinterpret_cast<void**>(&pIndices), 0);
	
#pragma region 정면 둘레
	Set_IndexBuffer(pIndices, 0, 0, 18, 19, 1);
	Set_IndexBuffer(pIndices, 6, 18, 20, 14, 10);
	Set_IndexBuffer(pIndices, 12, 20, 5, 6, 21);
	Set_IndexBuffer(pIndices, 18, 11, 15, 21, 19);
#pragma endregion

#pragma region 정면 안쪽
	Set_IndexBuffer(pIndices, 24, 10, 14, 17, 13);
	Set_IndexBuffer(pIndices, 30, 12, 16, 15, 11);
	Set_IndexBuffer(pIndices, 36, 12, 16, 15, 11);
	Set_IndexBuffer(pIndices, 42, 12, 16, 15, 11);
	
	pIndices[48] = 12; pIndices[49] = 11; pIndices[50] = 22;
	pIndices[51] = 12; pIndices[52] = 22; pIndices[53] = 13;
	pIndices[54] = 13; pIndices[55] = 22; pIndices[56] = 10;

	pIndices[57] = 17; pIndices[58] = 14; pIndices[59] = 23;
	pIndices[60] = 17; pIndices[61] = 23; pIndices[62] = 16;
	pIndices[63] = 16; pIndices[64] = 23; pIndices[65] = 15;
#pragma endregion

#pragma region 왼쪽
	pIndices[66] = 0; pIndices[67] = 1; pIndices[68] = 4;
	Set_IndexBuffer(pIndices, 69, 4, 1, 2, 3);
#pragma endregion

#pragma region 오른쪽
	pIndices[75] = 6; pIndices[76] = 5; pIndices[77] = 9;
	Set_IndexBuffer(pIndices, 78, 6, 9, 8, 7);
#pragma endregion

#pragma region 앞
	Set_IndexBuffer(pIndices, 84, 1, 6, 7, 2);
#pragma endregion

#pragma region 뒤
	Set_IndexBuffer(pIndices, 90, 26, 24, 25, 27);
#pragma endregion

#pragma region 정면 이미지
	Set_IndexBuffer(pIndices, 114, 13, 17, 16, 12);
#pragma endregion

#pragma region 두께 추가
	Set_IndexBuffer(pIndices, 96, 24, 28, 29, 25);
	Set_IndexBuffer(pIndices, 102, 30, 26, 27, 31);
	Set_IndexBuffer(pIndices, 108, 26, 32, 28, 24);
#pragma endregion


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
	Surface *= 114;
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
