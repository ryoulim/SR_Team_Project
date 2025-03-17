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
	m_iNumVertices = 80;					// 버텍스 갯수
	m_iVertexStride = sizeof(VTXPOSTEX);	// 버텍스 크기
	m_iFVF = D3DFVF_XYZ | D3DFVF_TEX1;		// 버텍스 타입
	m_iNumPritimive = 100; 					// 삼각형 갯수

	m_iIndexStride = 2;						// 인덱스 크기
	m_iNumIndices = 252;					// 인덱스 갯수
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
	pVertices[2].vTexcoord = _float2(1.f, 2.f);

	pVertices[3].vPosition = _float3(-0.875f, -0.5f, -0.125f);
	pVertices[3].vTexcoord = _float2(0.f, 2.f);
#pragma endregion

#pragma region 대형 판넬 정면 둘레
	pVertices[8].vPosition = _float3(-0.625f, 0.75f, -0.125f);
	pVertices[8].vTexcoord = _float2(0.f, 1.f);

	pVertices[9].vPosition = _float3(0.625f, 0.75f, -0.125f);
	pVertices[9].vTexcoord = _float2(1.f, 1.f);

	pVertices[10].vPosition = _float3(0.625f, 0.f, -0.125f);
	pVertices[10].vTexcoord = _float2(1.f, 0.f);

	pVertices[11].vPosition = _float3(-0.625f, 0.f, -0.125f);
	pVertices[11].vTexcoord = _float2(0.f, 0.f);
#pragma endregion

#pragma region 대형 판넬 정면 중간
	pVertices[48].vPosition = _float3(-0.625f, 0.5f, -0.125f);
	pVertices[48].vTexcoord = _float2(0.f, 0.f);

	pVertices[49].vPosition = _float3(0.625f, 0.5f, -0.125f);
	pVertices[49].vTexcoord = _float2(1.f, 0.f);

	pVertices[50].vPosition = _float3(0.625f, 0.25f, -0.125f);
	pVertices[50].vTexcoord = _float2(1.f, 1.f);

	pVertices[51].vPosition = _float3(-0.625f, 0.25f, -0.125f);
	pVertices[51].vTexcoord = _float2(0.f, 1.f);
#pragma endregion

#pragma region 오른쪽 기둥 정면
	pVertices[16].vPosition = _float3(0.625f, 1.f, -0.125f);
	pVertices[16].vTexcoord = _float2(0.f, 0.f);

	pVertices[17].vPosition = _float3(0.875f, 1.f, -0.125f);
	pVertices[17].vTexcoord = _float2(1.f, 0.f);

	pVertices[18].vPosition = _float3(0.875f, -0.5f, -0.125f);
	pVertices[18].vTexcoord = _float2(1.f, 2.f);

	pVertices[19].vPosition = _float3(0.625f, -0.5f, -0.125f);
	pVertices[19].vTexcoord = _float2(0.f, 2.f);
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
	pVertices[6].vTexcoord = _float2(0.f, 2.f);

	pVertices[7].vPosition = _float3(-0.875f, -0.5f, 0.125f);
	pVertices[7].vTexcoord = _float2(1.f, 2.f);
#pragma endregion

#pragma region 대형 판넬 후면 둘레
	pVertices[12].vPosition = _float3(-0.625f, 0.75f, 0.125f);
	pVertices[12].vTexcoord = _float2(1.f, 1.f);
	
	pVertices[13].vPosition = _float3(0.625f, 0.75f, 0.125f);
	pVertices[13].vTexcoord = _float2(0.f, 1.f);

	pVertices[14].vPosition = _float3(0.625f, 0.f, 0.125f);
	pVertices[14].vTexcoord = _float2(0.f, 0.f);

	pVertices[15].vPosition = _float3(-0.625f, 0.f, 0.125f);
	pVertices[15].vTexcoord = _float2(1.f, 0.f);
#pragma endregion

#pragma region 대형 판넬 후면 중간
	pVertices[52].vPosition = _float3(-0.625f, 0.5f, 0.125f);
	pVertices[52].vTexcoord = _float2(1.f, 0.f);
	
	pVertices[53].vPosition = _float3(0.625f, 0.5f, 0.125f);
	pVertices[53].vTexcoord = _float2(0.f, 0.f);

	pVertices[54].vPosition = _float3(0.625f, 0.25f, 0.125f);
	pVertices[54].vTexcoord = _float2(0.f, 1.f);

	pVertices[55].vPosition = _float3(-0.625f, 0.25f, 0.125f);
	pVertices[55].vTexcoord = _float2(1.f, 1.f);
#pragma endregion

#pragma region 오른쪽 기둥 후면
	pVertices[20].vPosition = _float3(0.625f, 1.f, 0.125f);
	pVertices[20].vTexcoord = _float2(1.f, 0.f);

	pVertices[21].vPosition = _float3(0.875f, 1.f, 0.125f);
	pVertices[21].vTexcoord = _float2(0.f, 0.f);

	pVertices[22].vPosition = _float3(0.875f, -0.5f, 0.125f);
	pVertices[22].vTexcoord = _float2(0.f, 2.f);

	pVertices[23].vPosition = _float3(0.625f, -0.5f, 0.125f);
	pVertices[23].vTexcoord = _float2(1.f, 2.f);
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
	pVertices[40].vTexcoord = _float2(2.f, 0.f);

	pVertices[41].vPosition = _float3(-1.f, -0.625f, 0.f);
	pVertices[41].vTexcoord = _float2(0.5f, 0.f);

	pVertices[42].vPosition = _float3(-0.75f, -0.625f, 0.25f);
	pVertices[42].vTexcoord = _float2(2.f, 0.f);

	pVertices[43].vPosition = _float3(-0.5f, -0.625f, 0.f);
	pVertices[43].vTexcoord = _float2(0.5f, 0.f);

	pVertices[44].vPosition = _float3(0.75f, -0.625f, -0.25f);
	pVertices[44].vTexcoord = _float2(2.f, 0.f);

	pVertices[45].vPosition = _float3(0.5f, -0.625f, 0.f);
	pVertices[45].vTexcoord = _float2(0.5f, 0.f);

	pVertices[46].vPosition = _float3(0.75f, -0.625f, 0.25f);
	pVertices[46].vTexcoord = _float2(2.f, 0.f);

	pVertices[47].vPosition = _float3(1.f, -0.625f, 0.f);
	pVertices[47].vTexcoord = _float2(0.5f, 0.f);
#pragma endregion

#pragma region 텍스쳐 때문에 추가

#pragma region 간판 하단
	pVertices[56].vPosition = _float3(0.625f, 0.f, 0.125f);		//14
	pVertices[56].vTexcoord = _float2(1.f, 1.f);

	pVertices[57].vPosition = _float3(-0.625f, 0.f, 0.125f);	//15
	pVertices[57].vTexcoord = _float2(0.f, 1.f);
#pragma endregion

#pragma region 왼쪽 기둥 우측
	pVertices[58].vPosition = _float3(-0.625f, 1.f, 0.125f);	//5
	pVertices[58].vTexcoord = _float2(1.f, 0.f);

	pVertices[59].vPosition = _float3(-0.625f, 1.f, -0.125f);	//1
	pVertices[59].vTexcoord = _float2(0.f, 0.f);

	pVertices[60].vPosition = _float3(-0.625f, 0.75f, -0.125f);	//8
	pVertices[60].vTexcoord = _float2(0.f, 0.25f);

	pVertices[61].vPosition = _float3(-0.625f, 0.75f, 0.125f);	//12
	pVertices[61].vTexcoord = _float2(1.f, 0.25f);
	
	pVertices[62].vPosition = _float3(-0.625f, 0.f, 0.125f);	//15
	pVertices[62].vTexcoord = _float2(1.f, 0.f);
	
	pVertices[63].vPosition = _float3(-0.625f, 0.f, -0.125f);	//11
	pVertices[63].vTexcoord = _float2(0.f, 0.f);

	pVertices[64].vPosition = _float3(-0.625f, -0.5f, -0.125f);	//2
	pVertices[64].vTexcoord = _float2(0.f, 0.75f);
	
	pVertices[65].vPosition = _float3(-0.625f, -0.5f, 0.125f);	//6
	pVertices[65].vTexcoord = _float2(1.f, 0.75f);
#pragma endregion

#pragma region 오른쪽 기둥 좌측
	pVertices[66].vPosition = _float3(0.625f, 1.f, 0.125f);		//20
	pVertices[66].vTexcoord = _float2(0.f, 0.f);

	pVertices[67].vPosition = _float3(0.625f, 1.f, -0.125f);	//16
	pVertices[67].vTexcoord = _float2(1.f, 0.f);

	pVertices[68].vPosition = _float3(0.625f, 0.75f, -0.125f);	//9
	pVertices[68].vTexcoord = _float2(1.f, 0.25f);

	pVertices[69].vPosition = _float3(0.625f, 0.75f, 0.125f);	//13
	pVertices[69].vTexcoord = _float2(0.f, 0.25f);

	pVertices[70].vPosition = _float3(0.625f, 0.f, 0.125f);		//14
	pVertices[70].vTexcoord = _float2(0.f, 0.f);

	pVertices[71].vPosition = _float3(0.625f, 0.f, -0.125f);	//10
	pVertices[71].vTexcoord = _float2(1.f, 0.f);

	pVertices[72].vPosition = _float3(0.625f, -0.5f, -0.125f);	//19
	pVertices[72].vTexcoord = _float2(1.f, 0.75f);

	pVertices[73].vPosition = _float3(0.625f, -0.5f, 0.125f);	//23
	pVertices[73].vTexcoord = _float2(0.f, 0.75f);
#pragma endregion

#pragma region 윗면들
	pVertices[74].vPosition = _float3(-0.875f, 1.f, 0.125f);	//4
	pVertices[74].vTexcoord = _float2(0.f, 0.25f);

	pVertices[75].vPosition = _float3(-0.625f, 1.f, 0.125f);	//5
	pVertices[75].vTexcoord = _float2(1.f, 0.25f);

	pVertices[76].vPosition = _float3(0.625f, 1.f, 0.125f);		//20
	pVertices[76].vTexcoord = _float2(0.f, 0.25f);

	pVertices[77].vPosition = _float3(0.875f, 1.f, 0.125f);		//21
	pVertices[77].vTexcoord = _float2(1.f, 0.25f);

	pVertices[78].vPosition = _float3(-0.625f, 0.75f, 0.125f);	//12
	pVertices[78].vTexcoord = _float2(0.f, 0.f);

	pVertices[79].vPosition = _float3(0.625f, 0.75f, 0.125f);	//13
	pVertices[79].vTexcoord = _float2(1.f, 0.f);
#pragma endregion

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
	Set_IndexBuffer(pIndices, 6, 16, 17, 18, 19);
	Set_IndexBuffer(pIndices, 72, 8, 9, 49, 48);
	Set_IndexBuffer(pIndices, 78, 51, 50, 10, 11);
	Set_IndexBuffer(pIndices, 108, 48, 49, 50, 51);
	Set_IndexBuffer(pIndices, 120, 24, 25, 26, 27);
	Set_IndexBuffer(pIndices, 126, 32, 33, 34, 35);
#pragma endregion
	
#pragma region 후면
	Set_IndexBuffer(pIndices, 12, 5, 4, 7, 6);
	Set_IndexBuffer(pIndices, 18, 21, 20, 23, 22);
	Set_IndexBuffer(pIndices, 84, 13, 12, 52, 53);
	Set_IndexBuffer(pIndices, 90, 54, 55, 15, 14);
	Set_IndexBuffer(pIndices, 114, 53, 52, 55, 54);
	Set_IndexBuffer(pIndices, 132, 29, 28, 31, 30);
	Set_IndexBuffer(pIndices, 138, 37, 36, 39, 38);
#pragma endregion
	
#pragma region 왼쪽
	Set_IndexBuffer(pIndices, 24, 4, 0, 3, 7);
	Set_IndexBuffer(pIndices, 30, 66, 67, 68, 69);
	Set_IndexBuffer(pIndices, 36, 70, 71, 72, 73);
	Set_IndexBuffer(pIndices, 144, 28, 24, 27, 31);
	Set_IndexBuffer(pIndices, 150, 36, 32, 35, 39);
#pragma endregion

#pragma region 오른쪽
	Set_IndexBuffer(pIndices, 42, 59, 58, 61, 60);
	Set_IndexBuffer(pIndices, 48, 63, 62, 65, 64);
	Set_IndexBuffer(pIndices, 54, 17, 21, 22, 18);
	Set_IndexBuffer(pIndices, 156, 25, 29, 30, 26);
	Set_IndexBuffer(pIndices, 162, 33, 37, 38, 34);
#pragma endregion

#pragma region 위
	Set_IndexBuffer(pIndices, 60, 74, 75, 1, 0);
	Set_IndexBuffer(pIndices, 66, 76, 77, 17, 16);
	Set_IndexBuffer(pIndices, 96, 78, 79, 9, 8);
#pragma endregion

#pragma region 아래
	Set_IndexBuffer(pIndices, 168, 27, 26, 30, 31);
	Set_IndexBuffer(pIndices, 174, 35, 34, 38, 39);
	Set_IndexBuffer(pIndices, 102, 11, 10, 56, 57);
#pragma endregion

#pragma region 왼쪽 발판 삼각형
	Set_IndexBuffer2(pIndices, 180, 24, 3, 40, 2, 25);
	//pIndices[156] = 24; pIndices[157] = 3;	pIndices[158] = 40;
	//pIndices[159] = 40; pIndices[160] = 3;	pIndices[161] = 2;
	//pIndices[162] = 40;	pIndices[163] = 2;	pIndices[164] = 25;
	Set_IndexBuffer2(pIndices, 189, 28, 7, 41, 3, 24);
	Set_IndexBuffer2(pIndices, 198, 29, 6, 42, 7, 28);
	Set_IndexBuffer2(pIndices, 207, 25, 2, 43, 6, 29);
#pragma endregion

#pragma region 오른쪽 발판 삼각형
	Set_IndexBuffer2(pIndices, 216, 32, 19, 44, 18, 33);
	Set_IndexBuffer2(pIndices, 225, 36, 23, 45, 19, 32);
	Set_IndexBuffer2(pIndices, 234, 37, 22, 46, 23, 36);
	Set_IndexBuffer2(pIndices, 243, 33, 18, 47, 22, 37);
#pragma endregion


	m_pIB->Unlock();
#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_Signboard::Initialize(void* pArg)
{
    return S_OK;
}

HRESULT CVIBuffer_Signboard::Render(_uint Surface)
{
	_uint iNumber{ 0 };

	switch (Surface)
	{
	case PILLAR:
		iNumber = 0;
		break;

	case PANELSIDE:
		iNumber = 72;
		break;

	case PANEL:
		iNumber = 108;
		break;

	case FOOTSTONE:
		iNumber = 120;
		break;

	case CONNECTION:
		iNumber = 180;
		break;

	default:
		return E_FAIL;
		break;
	}

	return m_pGraphic_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iNumVertices, iNumber, m_iNumPritimive);
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
