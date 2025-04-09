#include "VIBuffer_RaceBoss.h"

CVIBuffer_RaceBoss::CVIBuffer_RaceBoss(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CVIBuffer{pGraphic_Device}
{
}

CVIBuffer_RaceBoss::CVIBuffer_RaceBoss(const CVIBuffer_RaceBoss& Prototype)
    :CVIBuffer(Prototype)
{
}

HRESULT CVIBuffer_RaceBoss::Initialize_Prototype()
{
	m_iNumVertices = 44;					// 버텍스 갯수
	m_iVertexStride = sizeof(VTXPOSNORTEX);	// 버텍스 크기
	m_iFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;		// 버텍스 타입
	m_iNumPritimive = 84;					// 삼각형 갯수

	m_iIndexStride = 2;						// 인덱스 크기
	m_iNumIndices = 252;					// 인덱스 갯수
	m_eIndexFormat = D3DFMT_INDEX16;		// 인덱스 포맷

#pragma region VERTEX_BUFFER
	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXPOSNORTEX* pVertices = { nullptr };

	m_pVB->Lock(0, /*m_iNumVertices * m_iVertexStride*/0, reinterpret_cast<void**>(&pVertices), 0);
	
	pVertices[0].vPosition = _float3(-2.f, 0.25f, -2.f);
	pVertices[0].vTexcoord = _float2(0.f, 0.5f);

	pVertices[1].vPosition = _float3(-1.75f, 0.5f, -2.f);
	pVertices[1].vTexcoord = _float2(1.f, 0.f);//

	pVertices[2].vPosition = _float3(-1.5f, 0.5f, -2.f);
	pVertices[2].vTexcoord = _float2(2.f, 0.f);//

	pVertices[3].vPosition = _float3(-1.f, 0.25f, -2.f);
	pVertices[3].vTexcoord = _float2(3.f, 0.f);//

	pVertices[4].vPosition = _float3(-0.75f, 0.25f, -2.f);
	pVertices[4].vTexcoord = _float2(4.f, 0.f);//

	pVertices[5].vPosition = _float3(-0.25f, 0.5f, -2.f);
	pVertices[5].vTexcoord = _float2(5.f, 0.f);//

	pVertices[6].vPosition = _float3(0.25f, 0.5f, -2.f);
	pVertices[6].vTexcoord = _float2(6.f, 0.f);//

	pVertices[7].vPosition = _float3(0.75f, 0.25f, -2.f);
	pVertices[7].vTexcoord = _float2(7.f, 0.f);//

	pVertices[8].vPosition = _float3(1.f, 0.25f, -2.f);
	pVertices[8].vTexcoord = _float2(8.f, 0.f);//

	pVertices[9].vPosition = _float3(1.5f, 0.5f, -2.f);
	pVertices[9].vTexcoord = _float2(9.f, 0.f);//

	pVertices[10].vPosition = _float3(1.75f, 0.5f, -2.f);
	pVertices[10].vTexcoord = _float2(10.f, 0.f);//

	pVertices[11].vPosition = _float3(2.f, 0.25f, -2.f);
	pVertices[11].vTexcoord = _float2(11.f, 0.5f);

	pVertices[12].vPosition = _float3(1.75f, 0.f, -2.f);
	pVertices[12].vTexcoord = _float2(10.f, 1.f);//

	pVertices[13].vPosition = _float3(1.5f, 0.f, -2.f);
	pVertices[13].vTexcoord = _float2(9.f, 1.f);//

	pVertices[14].vPosition = _float3(1.f, -0.25f, -2.f);
	pVertices[14].vTexcoord = _float2(8.f, 1.f);//

	pVertices[15].vPosition = _float3(0.75f, -0.25f, -2.f);
	pVertices[15].vTexcoord = _float2(7.f, 1.f);//

	pVertices[16].vPosition = _float3(0.25f, 0.f, -2.f);
	pVertices[16].vTexcoord = _float2(6.f, 1.f);//

	pVertices[17].vPosition = _float3(-0.25f, 0.f, -2.f);
	pVertices[17].vTexcoord = _float2(5.f, 1.f);//

	pVertices[18].vPosition = _float3(-0.75f, -0.25f, -2.f);
	pVertices[18].vTexcoord = _float2(4.f, 1.f);//

	pVertices[19].vPosition = _float3(-1.f, -0.25f, -2.f);
	pVertices[19].vTexcoord = _float2(3.f, 1.f);//

	pVertices[20].vPosition = _float3(-1.5f, 0.f, -2.f);
	pVertices[20].vTexcoord = _float2(2.f, 1.f);//

	pVertices[21].vPosition = _float3(-1.75f, 0.f, -2.f);
	pVertices[21].vTexcoord = _float2(1.f, 1.f);//

	pVertices[22].vPosition = _float3(-2.f, 0.25f, 2.f);
	pVertices[22].vTexcoord = _float2(0.f, 3.5f);

	pVertices[23].vPosition = _float3(-1.75f, 0.5f, 2.f);
	pVertices[23].vTexcoord = _float2(1.f, 3.f);//

	pVertices[24].vPosition = _float3(-1.5f, 0.5f, 2.f);
	pVertices[24].vTexcoord = _float2(2.f, 3.f);//

	pVertices[25].vPosition = _float3(-1.f, 0.25f, 2.f);
	pVertices[25].vTexcoord = _float2(3.f, 3.f);//

	pVertices[26].vPosition = _float3(-0.75f, 0.25f, 2.f);
	pVertices[26].vTexcoord = _float2(4.f, 3.f);//

	pVertices[27].vPosition = _float3(-0.25f, 0.5f, 2.f);
	pVertices[27].vTexcoord = _float2(5.f, 3.f);//

	pVertices[28].vPosition = _float3(0.25f, 0.5f, 2.f);
	pVertices[28].vTexcoord = _float2(6.f, 3.f);//

	pVertices[29].vPosition = _float3(0.75f, 0.25f, 2.f);
	pVertices[29].vTexcoord = _float2(7.f, 3.f);//

	pVertices[30].vPosition = _float3(1.f, 0.25f, 2.f);
	pVertices[30].vTexcoord = _float2(8.f, 3.f);//

	pVertices[31].vPosition = _float3(1.5f, 0.5f, 2.f);
	pVertices[31].vTexcoord = _float2(9.f, 3.f);//

	pVertices[32].vPosition = _float3(1.75f, 0.5f, 2.f);
	pVertices[32].vTexcoord = _float2(10.f, 3.f);//

	pVertices[33].vPosition = _float3(2.f, 0.25f, 2.f);
	pVertices[33].vTexcoord = _float2(11.f, 3.5f);

	pVertices[34].vPosition = _float3(1.75f, 0.f, 2.f);
	pVertices[34].vTexcoord = _float2(10.f, 4.f);//

	pVertices[35].vPosition = _float3(1.5f, 0.f, 2.f);
	pVertices[35].vTexcoord = _float2(9.f, 4.f);//

	pVertices[36].vPosition = _float3(1.f, -0.25f, 2.f);
	pVertices[36].vTexcoord = _float2(8.f, 4.f);//

	pVertices[37].vPosition = _float3(0.75f, -0.25f, 2.f);
	pVertices[37].vTexcoord = _float2(7.f, 4.f);//

	pVertices[38].vPosition = _float3(0.25f, 0.f, 2.f);
	pVertices[38].vTexcoord = _float2(6.f, 4.f);//

	pVertices[39].vPosition = _float3(-0.25f, 0.f, 2.f);
	pVertices[39].vTexcoord = _float2(5.f, 4.f);//

	pVertices[40].vPosition = _float3(-0.75f, -0.25f, 2.f);
	pVertices[40].vTexcoord = _float2(4.f, 4.f);//

	pVertices[41].vPosition = _float3(-1.f, -0.25f, 2.f);
	pVertices[41].vTexcoord = _float2(3.f, 4.f);//

	pVertices[42].vPosition = _float3(-1.5f, 0.f, 2.f);
	pVertices[42].vTexcoord = _float2(2.f, 4.f);//

	pVertices[43].vPosition = _float3(-1.75f, 0.f, 2.f);
	pVertices[43].vTexcoord = _float2(1.f, 4.f);//

#pragma region 법선
	for (_uint i = 0; i < m_iNumVertices; i++)
	{
		pVertices[i].vNormal = _float3(0.f, 0.f, 0.f);
	}
#pragma endregion

	m_pVB->Unlock();

#pragma endregion

#pragma region INDEX_BUFFER
	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	_ushort* pIndices = { nullptr };

	m_pIB->Lock(0, 0, reinterpret_cast<void**>(&pIndices), 0);

#pragma region 앞
	pIndices[0] = 0; pIndices[1] = 1; pIndices[2] = 21;
	Set_IndexBuffer(pIndices, 3, 2, 3, 19, 20);
	Set_IndexBuffer(pIndices, 9, 4, 5, 17, 18);
	Set_IndexBuffer(pIndices, 15, 6, 7, 15, 16);
	Set_IndexBuffer(pIndices, 21, 8, 9, 13, 14);
	pIndices[27] = 10; pIndices[28] = 11; pIndices[29] = 12;

	Set_IndexBuffer(pIndices, 192, 1, 2, 20, 21);
	Set_IndexBuffer(pIndices, 198, 3, 4, 18, 19);
	Set_IndexBuffer(pIndices, 204, 7, 8, 14, 15);
	Set_IndexBuffer(pIndices, 210, 9, 10, 12, 13);

	Set_IndexBuffer(pIndices, 240, 5, 6, 16, 17);
#pragma endregion

#pragma region 뒤
	pIndices[30] = 33; pIndices[31] = 32; pIndices[32] = 34;
	Set_IndexBuffer(pIndices, 33, 31, 30, 36, 35);
	Set_IndexBuffer(pIndices, 39, 29, 28, 38, 37);
	Set_IndexBuffer(pIndices, 45, 27, 26, 40, 39);
	Set_IndexBuffer(pIndices, 51, 25, 24, 42, 41);
	pIndices[57] = 23; pIndices[58] = 22; pIndices[59] = 43;

	Set_IndexBuffer(pIndices, 216, 24, 23, 43, 42);
	Set_IndexBuffer(pIndices, 222, 26, 25, 41, 40);
	Set_IndexBuffer(pIndices, 228, 30, 29, 37, 36);
	Set_IndexBuffer(pIndices, 234, 32, 31, 35, 34);

	Set_IndexBuffer(pIndices, 246, 28, 27, 39, 38);
#pragma endregion

#pragma region 위
	Set_IndexBuffer(pIndices, 60, 22, 23, 1, 0);
	Set_IndexBuffer(pIndices, 66, 23, 24, 2, 1);
	Set_IndexBuffer(pIndices, 72, 24, 25, 3, 2);
	Set_IndexBuffer(pIndices, 78, 25, 26, 4, 3);
	Set_IndexBuffer(pIndices, 84, 26, 27, 5, 4);
	Set_IndexBuffer(pIndices, 90, 27, 28, 6, 5);
	Set_IndexBuffer(pIndices, 96, 28, 29, 7, 6);
	Set_IndexBuffer(pIndices, 102, 29, 30, 8, 7);
	Set_IndexBuffer(pIndices, 108, 30, 31, 9, 8);
	Set_IndexBuffer(pIndices, 114, 31, 32, 10, 9);
	Set_IndexBuffer(pIndices, 120, 32, 33, 11, 10);
#pragma endregion

#pragma region 아래
	Set_IndexBuffer(pIndices, 126, 0, 21, 43, 22);
	Set_IndexBuffer(pIndices, 132, 21, 20, 42, 43);
	Set_IndexBuffer(pIndices, 138, 20, 19, 41, 42);
	Set_IndexBuffer(pIndices, 144, 19, 18, 40, 41);
	Set_IndexBuffer(pIndices, 150, 18, 17, 39, 40);
	Set_IndexBuffer(pIndices, 156, 17, 16, 38, 39);
	Set_IndexBuffer(pIndices, 162, 16, 15, 37, 38);
	Set_IndexBuffer(pIndices, 168, 15, 14, 36, 37);
	Set_IndexBuffer(pIndices, 174, 14, 13, 35, 36);
	Set_IndexBuffer(pIndices, 180, 13, 12, 34, 35);
	Set_IndexBuffer(pIndices, 186, 12, 11, 33, 34);
#pragma endregion
	m_pIB->Unlock();
#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_RaceBoss::Initialize(void* pArg)
{
    return S_OK;
}

HRESULT CVIBuffer_RaceBoss::Render(_uint Surface)
{
	_uint iNumber{ 0 };

	switch (Surface)
	{
	case COMMON:
		iNumber = 0;
		break;

	case MUZZLE1:
		iNumber = 192;
		break;

	case MUZZLE2:
		iNumber = 198;
		break;

	case MUZZLE3:
		iNumber = 204;
		break;

	case MUZZLE4:
		iNumber = 210;
		break;

	case MIDDLE:
		iNumber = 240;
		break;

	default:
		return E_FAIL;
		break;
	}

	return m_pGraphic_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iNumVertices, iNumber, m_iNumPritimive);
}

void CVIBuffer_RaceBoss::Set_IndexBuffer(_ushort* pIndices, _uint StartIndex, _uint LT, _uint RT, _uint RB, _uint LB)
{
	pIndices[StartIndex++] = LT;
	pIndices[StartIndex++] = RT;
	pIndices[StartIndex++] = RB;

	pIndices[StartIndex++] = LT;
	pIndices[StartIndex++] = RB;
	pIndices[StartIndex++] = LB;
}

VECTOR CVIBuffer_RaceBoss::ComputeNormalVector(VTXPOSNORTEX* pVertices, _uint vertex1, _uint vertex2, _uint vertex3)
{
	return __super::ComputeNormal(&(pVertices[vertex1].vPosition), &(pVertices[vertex2].vPosition), &(pVertices[vertex3].vPosition));
}

CVIBuffer_RaceBoss* CVIBuffer_RaceBoss::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_RaceBoss* pInstance = new CVIBuffer_RaceBoss(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_RaceBoss");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_RaceBoss::Clone(void* pArg)
{
	CVIBuffer_RaceBoss* pInstance = new CVIBuffer_RaceBoss(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CVIBuffer_RaceBoss");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_RaceBoss::Free()
{
	__super::Free();
}
