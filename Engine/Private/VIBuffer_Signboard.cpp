#include "VIBuffer_Signboard.h"

CVIBuffer_Signboard::CVIBuffer_Signboard(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CVIBuffer{pGraphic_Device}
{
}

CVIBuffer_Signboard::CVIBuffer_Signboard(const CVIBuffer_Signboard& Prototype)
    :CVIBuffer(Prototype)
{
}

HRESULT CVIBuffer_Signboard::Initialize_Prototype()
{
	m_iNumVertices = 48;					// 버텍스 갯수
	m_iVertexStride = sizeof(VTXPOSTEX);	// 버텍스 크기
	m_iFVF = D3DFVF_XYZ | D3DFVF_TEX1;		// 버텍스 타입
	m_iNumPritimive = 84; // 60 + 24		// 삼각형 갯수

	m_iIndexStride = 2;						// 인덱스 크기
	m_iNumIndices = 228;					// 인덱스 갯수
	m_eIndexFormat = D3DFMT_INDEX16;		// 인덱스 포맷

#pragma region VERTEX_BUFFER
	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXPOSTEX* pVertices = { nullptr };

	m_pVB->Lock(0, /*m_iNumVertices * m_iVertexStride*/0, reinterpret_cast<void**>(&pVertices), 0);
	
#pragma region 왼쪽 기둥 정면
	pVertices[0].vPosition = _float3(-0.875f, 1.f, -0.125f);
	pVertices[0].vTexcoord = _float2(0.f, 0.f);

	pVertices[1].vPosition = _float3(-0.625f, 1.f, -0.125f);
	pVertices[1].vTexcoord = _float2(1.f, 0.f);

	pVertices[2].vPosition = _float3(-0.625f, -0.5f, -0.125f);
	pVertices[2].vTexcoord = _float2(1.f, 1.f);

	pVertices[3].vPosition = _float3(-0.875f, -0.5f, -0.125f);
	pVertices[3].vTexcoord = _float2(0.f, 1.f);
#pragma endregion

#pragma region 대형 판넬 정면
	pVertices[8].vPosition = _float3(-0.625f, 0.75f, -0.125f);
	pVertices[8].vTexcoord = _float2(0.f, 0.f);

	pVertices[9].vPosition = _float3(0.625f, 0.75f, -0.125f);
	pVertices[9].vTexcoord = _float2(1.f, 0.f);

	pVertices[10].vPosition = _float3(0.625f, 0.f, -0.125f);
	pVertices[10].vTexcoord = _float2(1.f, 1.f);

	pVertices[11].vPosition = _float3(-0.625f, 0.f, -0.125f);
	pVertices[11].vTexcoord = _float2(0.f, 1.f);
#pragma endregion

#pragma region 오른쪽 기둥 정면
	pVertices[16].vPosition = _float3(0.625f, 1.f, -0.125f);
	pVertices[16].vTexcoord = _float2(0.f, 0.f);

	pVertices[17].vPosition = _float3(0.875f, 1.f, -0.125f);
	pVertices[17].vTexcoord = _float2(1.f, 0.f);

	pVertices[18].vPosition = _float3(0.875f, -0.5f, -0.125f);
	pVertices[18].vTexcoord = _float2(1.f, 1.f);

	pVertices[19].vPosition = _float3(0.625f, -0.5f, -0.125f);
	pVertices[19].vTexcoord = _float2(0.f, 1.f);
#pragma endregion

#pragma region 왼쪽 아래 받침대 정면
	pVertices[24].vPosition = _float3(-1.f, -0.625f, -0.25f);
	pVertices[24].vTexcoord = _float2(0.f, 0.f);

	pVertices[25].vPosition = _float3(-0.5f, -0.625f, -0.25f);
	pVertices[25].vTexcoord = _float2(1.f, 0.f);

	pVertices[26].vPosition = _float3(-0.5f, -1.f, -0.25f);
	pVertices[26].vTexcoord = _float2(1.f, 1.f);

	pVertices[27].vPosition = _float3(-1.f, -1.f, -0.25f);
	pVertices[27].vTexcoord = _float2(0.f, 1.f);
#pragma endregion

#pragma region 오른쪽 아래 받침대 정면
	pVertices[32].vPosition = _float3(0.5f, -0.625f, -0.25f);
	pVertices[32].vTexcoord = _float2(0.f, 0.f);

	pVertices[33].vPosition = _float3(1.f, -0.625f, -0.25f);
	pVertices[33].vTexcoord = _float2(1.f, 0.f);

	pVertices[34].vPosition = _float3(1.f, -1.f, -0.25f);
	pVertices[34].vTexcoord = _float2(1.f, 1.f);

	pVertices[35].vPosition = _float3(0.5f, -1.f, -0.25f);
	pVertices[35].vTexcoord = _float2(0.f, 1.f);
#pragma endregion

#pragma region 왼쪽 기둥 후면
	pVertices[4].vPosition = _float3(-0.875f, 1.f, 0.125f);
	pVertices[4].vTexcoord = _float2(1.f, 0.f);

	pVertices[5].vPosition = _float3(-0.625f, 1.f, 0.125f);
	pVertices[5].vTexcoord = _float2(0.f, 0.f);

	pVertices[6].vPosition = _float3(-0.625f, -0.5f, 0.125f);
	pVertices[6].vTexcoord = _float2(0.f, 1.f);

	pVertices[7].vPosition = _float3(-0.875f, -0.5f, 0.125f);
	pVertices[7].vTexcoord = _float2(1.f, 1.f);
#pragma endregion

#pragma region 대형 판넬 후면
	pVertices[12].vPosition = _float3(-0.625f, 0.75f, 0.125f);
	pVertices[12].vTexcoord = _float2(1.f, 0.f);

	pVertices[13].vPosition = _float3(0.625f, 0.75f, 0.125f);
	pVertices[13].vTexcoord = _float2(0.f, 0.f);

	pVertices[14].vPosition = _float3(0.625f, 0.f, 0.125f);
	pVertices[14].vTexcoord = _float2(0.f, 1.f);

	pVertices[15].vPosition = _float3(-0.625f, 0.f, 0.125f);
	pVertices[15].vTexcoord = _float2(1.f, 1.f);
#pragma endregion

#pragma region 오른쪽 기둥 후면
	pVertices[20].vPosition = _float3(0.625f, 1.f, 0.125f);
	pVertices[20].vTexcoord = _float2(1.f, 0.f);

	pVertices[21].vPosition = _float3(0.875f, 1.f, 0.125f);
	pVertices[21].vTexcoord = _float2(0.f, 0.f);

	pVertices[22].vPosition = _float3(0.875f, -0.5f, 0.125f);
	pVertices[22].vTexcoord = _float2(0.f, 1.f);

	pVertices[23].vPosition = _float3(0.625f, -0.5f, 0.125f);
	pVertices[23].vTexcoord = _float2(1.f, 1.f);
#pragma endregion

#pragma region 왼쪽 아래 받침대 후면
	pVertices[28].vPosition = _float3(-1.f, -0.625f, 0.25f);
	pVertices[28].vTexcoord = _float2(1.f, 0.f);

	pVertices[29].vPosition = _float3(-0.5f, -0.625f, 0.25f);
	pVertices[29].vTexcoord = _float2(0.f, 0.f);

	pVertices[30].vPosition = _float3(-0.5f, -1.f, 0.25f);
	pVertices[30].vTexcoord = _float2(0.f, 1.f);

	pVertices[31].vPosition = _float3(-1.f, -1.f, 0.25f);
	pVertices[31].vTexcoord = _float2(1.f, 1.f);
#pragma endregion

#pragma region 오른쪽 아래 받침대 후면
	pVertices[36].vPosition = _float3(0.5f, -0.625f, 0.25f);
	pVertices[36].vTexcoord = _float2(1.f, 0.f);

	pVertices[37].vPosition = _float3(1.f, -0.625f, 0.25f);
	pVertices[37].vTexcoord = _float2(0.f, 0.f);

	pVertices[38].vPosition = _float3(1.f, -1.f, 0.25f);
	pVertices[38].vTexcoord = _float2(0.f, 1.f);

	pVertices[39].vPosition = _float3(0.5f, -1.f, 0.25f);
	pVertices[39].vTexcoord = _float2(1.f, 1.f);
#pragma endregion

#pragma region 발판 중점
	pVertices[40].vPosition = _float3(-0.75f, -0.625f, -0.25f);
	pVertices[40].vTexcoord = _float2(0.f, 0.f);

	pVertices[41].vPosition = _float3(-1.f, -0.625f, 0.f);
	pVertices[41].vTexcoord = _float2(1.f, 0.f);

	pVertices[42].vPosition = _float3(-0.75f, -0.625f, 0.25f);
	pVertices[42].vTexcoord = _float2(1.f, 1.f);

	pVertices[43].vPosition = _float3(-0.5f, -0.625f, 0.f);
	pVertices[43].vTexcoord = _float2(0.f, 1.f);

	pVertices[44].vPosition = _float3(0.75f, -0.625f, -0.25f);
	pVertices[44].vTexcoord = _float2(0.f, 0.f);

	pVertices[45].vPosition = _float3(0.5f, -0.625f, 0.f);
	pVertices[45].vTexcoord = _float2(1.f, 0.f);

	pVertices[46].vPosition = _float3(0.75f, -0.625f, 0.25f);
	pVertices[46].vTexcoord = _float2(1.f, 1.f);

	pVertices[47].vPosition = _float3(1.f, -0.625f, 0.f);
	pVertices[47].vTexcoord = _float2(0.f, 1.f);
#pragma endregion

	m_pVB->Unlock();

#pragma endregion

#pragma region INDEX_BUFFER
	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	_ushort* pIndices = { nullptr };

	m_pIB->Lock(0, 0, reinterpret_cast<void**>(&pIndices), 0);

#pragma region 정면
	Set_IndexBuffer(pIndices, 0, 0, 1, 2, 3);
	Set_IndexBuffer(pIndices, 6, 24, 25, 26, 27);
	Set_IndexBuffer(pIndices, 12, 8, 9, 10, 11);
	Set_IndexBuffer(pIndices, 18, 16, 17, 18, 19);
	Set_IndexBuffer(pIndices, 24, 32, 33, 34, 35);
#pragma endregion

#pragma region 후면
	Set_IndexBuffer(pIndices, 30, 5, 4, 7, 6);
	Set_IndexBuffer(pIndices, 36, 29, 28, 31, 30);
	Set_IndexBuffer(pIndices, 42, 13, 12, 15, 14);
	Set_IndexBuffer(pIndices, 48, 21, 20, 23, 22);
	Set_IndexBuffer(pIndices, 54, 37, 36, 39, 38);
#pragma endregion

#pragma region 왼쪽
	Set_IndexBuffer(pIndices, 60, 4, 0, 3, 7);
	Set_IndexBuffer(pIndices, 66, 28, 24, 27, 31);
	Set_IndexBuffer(pIndices, 72, 20, 16, 9, 13);
	Set_IndexBuffer(pIndices, 78, 14, 10, 19, 23);
	Set_IndexBuffer(pIndices, 84, 36, 32, 35, 39);
#pragma endregion

#pragma region 오른쪽
	Set_IndexBuffer(pIndices, 90, 1, 5, 12, 8);
	Set_IndexBuffer(pIndices, 96, 11, 15, 6, 2);
	Set_IndexBuffer(pIndices, 102, 25, 29, 30, 26);
	Set_IndexBuffer(pIndices, 108, 17, 21, 22, 18);
	Set_IndexBuffer(pIndices, 114, 33, 37, 38, 34);
#pragma endregion

#pragma region 위
	Set_IndexBuffer(pIndices, 120, 4, 5, 1, 0);
	Set_IndexBuffer(pIndices, 126, 12, 13, 9, 8);
	Set_IndexBuffer(pIndices, 132, 20, 21, 17, 16);
#pragma endregion

#pragma region 아래
	Set_IndexBuffer(pIndices, 138, 27, 26, 30, 31);
	Set_IndexBuffer(pIndices, 144, 11, 10, 14, 15);
	Set_IndexBuffer(pIndices, 150, 35, 34, 38, 39);
#pragma endregion

#pragma region 왼쪽 발판 삼각형
	Set_IndexBuffer2(pIndices, 156, 24, 3, 40, 2, 25);
	//pIndices[156] = 24; pIndices[157] = 3;	pIndices[158] = 40;
	//pIndices[159] = 40; pIndices[160] = 3;	pIndices[161] = 2;
	//pIndices[162] = 40;	pIndices[163] = 2;	pIndices[164] = 25;
	Set_IndexBuffer2(pIndices, 165, 28, 7, 41, 3, 24);
	Set_IndexBuffer2(pIndices, 174, 29, 6, 42, 7, 28);
	Set_IndexBuffer2(pIndices, 183, 25, 2, 43, 6, 29);
#pragma endregion

#pragma region 오른쪽 발판 삼각형
	Set_IndexBuffer2(pIndices, 192, 32, 19, 44, 18, 33);
	Set_IndexBuffer2(pIndices, 201, 36, 23, 45, 19, 32);
	Set_IndexBuffer2(pIndices, 210, 37, 22, 46, 23, 36);
	Set_IndexBuffer2(pIndices, 219, 33, 18, 47, 22, 37);
#pragma endregion


	m_pIB->Unlock();
#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_Signboard::Initialize(void* pArg)
{
    return S_OK;
}

void CVIBuffer_Signboard::Set_IndexBuffer(_ushort* pIndices, _uint StartIndex, _uint LT, _uint RT, _uint RB, _uint LB)
{
	pIndices[StartIndex++] = LT;
	pIndices[StartIndex++] = RT;
	pIndices[StartIndex++] = RB;

	pIndices[StartIndex++] = LT;
	pIndices[StartIndex++] = RB;
	pIndices[StartIndex++] = LB;
}

void CVIBuffer_Signboard::Set_IndexBuffer2(_ushort* pIndices, _uint StartIndex, _uint LB, _uint LT, _uint MB, _uint RT, _uint RB)
{
	pIndices[StartIndex++] = LB;
	pIndices[StartIndex++] = LT;
	pIndices[StartIndex++] = MB;

	pIndices[StartIndex++] = MB;
	pIndices[StartIndex++] = LT;
	pIndices[StartIndex++] = RT;

	pIndices[StartIndex++] = MB;
	pIndices[StartIndex++] = RT;
	pIndices[StartIndex++] = RB;
}

CVIBuffer_Signboard* CVIBuffer_Signboard::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_Signboard* pInstance = new CVIBuffer_Signboard(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Signboard");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Signboard::Clone(void* pArg)
{
	CVIBuffer_Signboard* pInstance = new CVIBuffer_Signboard(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CVIBuffer_Signboard");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Signboard::Free()
{
	__super::Free();
}
