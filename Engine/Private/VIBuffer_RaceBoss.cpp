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
	m_iNumVertices = 86; //70 + 16			// 버텍스 갯수
	m_iVertexStride = sizeof(VTXPOSNORTEX);	// 버텍스 크기
	m_iFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;		// 버텍스 타입
	m_iNumPritimive = 138; 				// 삼각형 갯수
	
	m_iIndexStride = 2;						// 인덱스 크기
	m_iNumIndices = 336; 					// 인덱스 갯수
	m_eIndexFormat = D3DFMT_INDEX16;		// 인덱스 포맷

#pragma region VERTEX_BUFFER
	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXPOSNORTEX* pVertices = { nullptr };

	m_pVB->Lock(0, /*m_iNumVertices * m_iVertexStride*/0, reinterpret_cast<void**>(&pVertices), 0);
	
	pVertices[0].vPosition = _float3(-2.f, 0.25f, -2.f);
	pVertices[0].vTexcoord = _float2(0.f, 1.f);//

	pVertices[1].vPosition = _float3(-1.75f, 0.5f, -2.f);
	pVertices[1].vTexcoord = _float2(1.f, 1.f);//

	pVertices[2].vPosition = _float3(-1.5f, 0.5f, -2.f);
	pVertices[2].vTexcoord = _float2(2.f, 1.f);//

	pVertices[3].vPosition = _float3(-1.f, 0.25f, -2.f);
	pVertices[3].vTexcoord = _float2(3.f, 1.f);//

	pVertices[4].vPosition = _float3(-0.75f, 0.25f, -2.f);
	pVertices[4].vTexcoord = _float2(4.f, 1.f);//

	pVertices[5].vPosition = _float3(-0.25f, 0.5f, -2.f);
	pVertices[5].vTexcoord = _float2(5.f, 1.f);//

	pVertices[6].vPosition = _float3(0.25f, 0.5f, -2.f);
	pVertices[6].vTexcoord = _float2(6.f, 1.f);//

	pVertices[7].vPosition = _float3(0.75f, 0.25f, -2.f);
	pVertices[7].vTexcoord = _float2(7.f, 1.f);//

	pVertices[8].vPosition = _float3(1.f, 0.25f, -2.f);
	pVertices[8].vTexcoord = _float2(8.f, 1.f);//

	pVertices[9].vPosition = _float3(1.5f, 0.5f, -2.f);
	pVertices[9].vTexcoord = _float2(9.f, 1.f);//

	pVertices[10].vPosition = _float3(1.75f, 0.5f, -2.f);
	pVertices[10].vTexcoord = _float2(10.f, 1.f);//

	pVertices[11].vPosition = _float3(2.f, 0.25f, -2.f);
	pVertices[11].vTexcoord = _float2(11.f, 1.f);//

	pVertices[12].vPosition = _float3(1.75f, 0.f, -2.f);
	pVertices[12].vTexcoord = _float2(10.f, 2.f);//

	pVertices[13].vPosition = _float3(1.5f, 0.f, -2.f);
	pVertices[13].vTexcoord = _float2(9.f, 2.f);//

	pVertices[14].vPosition = _float3(1.f, -0.25f, -2.f);
	pVertices[14].vTexcoord = _float2(8.f, 2.f);//

	pVertices[15].vPosition = _float3(0.75f, -0.25f, -2.f);
	pVertices[15].vTexcoord = _float2(7.f, 2.f);//

	pVertices[16].vPosition = _float3(0.25f, 0.f, -2.f);
	pVertices[16].vTexcoord = _float2(6.f, 2.f);//

	pVertices[17].vPosition = _float3(-0.25f, 0.f, -2.f);
	pVertices[17].vTexcoord = _float2(5.f, 2.f);//

	pVertices[18].vPosition = _float3(-0.75f, -0.25f, -2.f);
	pVertices[18].vTexcoord = _float2(4.f, 2.f);//
	
	pVertices[19].vPosition = _float3(-1.f, -0.25f, -2.f);
	pVertices[19].vTexcoord = _float2(3.f, 2.f);//

	pVertices[20].vPosition = _float3(-1.5f, 0.f, -2.f);
	pVertices[20].vTexcoord = _float2(2.f, 2.f);//

	pVertices[21].vPosition = _float3(-1.75f, 0.f, -2.f);
	pVertices[21].vTexcoord = _float2(1.f, 2.f);//

	pVertices[22].vPosition = _float3(-2.f, 0.25f, 2.f);
	pVertices[22].vTexcoord = _float2(0.f, 5.f);//

	pVertices[23].vPosition = _float3(-1.75f, 0.5f, 2.f);
	pVertices[23].vTexcoord = _float2(1.f, 5.f);//

	pVertices[24].vPosition = _float3(-1.5f, 0.5f, 2.f);
	pVertices[24].vTexcoord = _float2(2.f, 5.f);//

	pVertices[25].vPosition = _float3(-1.f, 0.25f, 2.f);
	pVertices[25].vTexcoord = _float2(3.f, 5.f);//

	pVertices[26].vPosition = _float3(-0.75f, 0.25f, 2.f);
	pVertices[26].vTexcoord = _float2(4.f, 5.f);//

	pVertices[27].vPosition = _float3(-0.25f, 0.5f, 2.f);
	pVertices[27].vTexcoord = _float2(5.f, 5.f);//

	pVertices[28].vPosition = _float3(0.25f, 0.5f, 2.f);
	pVertices[28].vTexcoord = _float2(6.f, 5.f);//

	pVertices[29].vPosition = _float3(0.75f, 0.25f, 2.f);
	pVertices[29].vTexcoord = _float2(7.f, 5.f);//

	pVertices[30].vPosition = _float3(1.f, 0.25f, 2.f);
	pVertices[30].vTexcoord = _float2(8.f, 5.f);//

	pVertices[31].vPosition = _float3(1.5f, 0.5f, 2.f);
	pVertices[31].vTexcoord = _float2(9.f, 5.f);//

	pVertices[32].vPosition = _float3(1.75f, 0.5f, 2.f);
	pVertices[32].vTexcoord = _float2(10.f, 5.f);//

	pVertices[33].vPosition = _float3(2.f, 0.25f, 2.f);
	pVertices[33].vTexcoord = _float2(11.f, 5.f);//

	pVertices[34].vPosition = _float3(1.75f, 0.f, 2.f);
	pVertices[34].vTexcoord = _float2(10.f, 6.f);//

	pVertices[35].vPosition = _float3(1.5f, 0.f, 2.f);
	pVertices[35].vTexcoord = _float2(9.f, 6.f);//

	pVertices[36].vPosition = _float3(1.f, -0.25f, 2.f);
	pVertices[36].vTexcoord = _float2(8.f, 6.f);//

	pVertices[37].vPosition = _float3(0.75f, -0.25f, 2.f);
	pVertices[37].vTexcoord = _float2(7.f, 6.f);//

	pVertices[38].vPosition = _float3(0.25f, 0.f, 2.f);
	pVertices[38].vTexcoord = _float2(6.f, 6.f);//

	pVertices[39].vPosition = _float3(-0.25f, 0.f, 2.f);
	pVertices[39].vTexcoord = _float2(5.f, 6.f);//

	pVertices[40].vPosition = _float3(-0.75f, -0.25f, 2.f);
	pVertices[40].vTexcoord = _float2(4.f, 6.f);//

	pVertices[41].vPosition = _float3(-1.f, -0.25f, 2.f);
	pVertices[41].vTexcoord = _float2(3.f, 6.f);//

	pVertices[42].vPosition = _float3(-1.5f, 0.f, 2.f);
	pVertices[42].vTexcoord = _float2(2.f, 6.f);//

	pVertices[43].vPosition = _float3(-1.75f, 0.f, 2.f);
	pVertices[43].vTexcoord = _float2(1.f, 6.f);//

	pVertices[44].vPosition = _float3(0.f, 0.75f, -2.f);
	pVertices[44].vTexcoord = _float2(5.5f, 0.f);//

	pVertices[45].vPosition = _float3(0.f, 0.75f, 2.f);
	pVertices[45].vTexcoord = _float2(5.5f, 4.f);//

	pVertices[46].vPosition = _float3(-1.9375f, 0.1875f, -2.f);
	pVertices[46].vTexcoord = _float2(0.f, 1.25f);//

	pVertices[47].vPosition = _float3(-1.75f, 0.375f, -2.f);
	pVertices[47].vTexcoord = _float2(1.f, 1.125f);//

	pVertices[48].vPosition = _float3(-1.5f, 0.375f, -2.f);
	pVertices[48].vTexcoord = _float2(2.f, 1.125f);//

	pVertices[49].vPosition = _float3(-1.f, 0.125f, -2.f);
	pVertices[49].vTexcoord = _float2(3.f, 1.125f);//

	pVertices[50].vPosition = _float3(-0.75f, 0.125f, -2.f);
	pVertices[50].vTexcoord = _float2(4.f, 1.125f);//

	pVertices[51].vPosition = _float3(-0.25f, 0.375f, -2.f);
	pVertices[51].vTexcoord = _float2(5.f, 1.125f);//

	pVertices[52].vPosition = _float3(0.25f, 0.375f, -2.f);
	pVertices[52].vTexcoord = _float2(6.f, 1.125f);//

	pVertices[53].vPosition = _float3(0.75f, 0.125f, -2.f);
	pVertices[53].vTexcoord = _float2(7.f, 1.125f);//

	pVertices[54].vPosition = _float3(1.f, 0.125f, -2.f);
	pVertices[54].vTexcoord = _float2(8.f, 1.125f);//

	pVertices[55].vPosition = _float3(1.5f, 0.375f, -2.f);
	pVertices[55].vTexcoord = _float2(9.f, 1.125f);//

	pVertices[56].vPosition = _float3(1.75f, 0.375f, -2.f);
	pVertices[56].vTexcoord = _float2(10.f, 1.125f);//

	pVertices[57].vPosition = _float3(1.9375f, 0.1875f, -2.f);
	pVertices[57].vTexcoord = _float2(11.f, 1.25f);//

	pVertices[58].vPosition = _float3(-1.9375f, 0.1875f, 2.f);
	pVertices[58].vTexcoord = _float2(0.f, 5.25f);//

	pVertices[59].vPosition = _float3(-1.75f, 0.375f, 2.f);
	pVertices[59].vTexcoord = _float2(1.f, 5.25f);//

	pVertices[60].vPosition = _float3(-1.5f, 0.375f, 2.f);
	pVertices[60].vTexcoord = _float2(2.f, 5.25f);//

	pVertices[61].vPosition = _float3(-1.f, 0.125f, 2.f);
	pVertices[61].vTexcoord = _float2(3.f, 5.25f);//

	pVertices[62].vPosition = _float3(-0.75f, 0.125f, 2.f);
	pVertices[62].vTexcoord = _float2(4.f, 5.25f);//

	pVertices[63].vPosition = _float3(-0.25f, 0.375f, 2.f);
	pVertices[63].vTexcoord = _float2(5.f, 5.25f);//

	pVertices[64].vPosition = _float3(0.25f, 0.375f, 2.f);
	pVertices[64].vTexcoord = _float2(6.f, 5.25f);//

	pVertices[65].vPosition = _float3(0.75f, 0.125f, 2.f);
	pVertices[65].vTexcoord = _float2(7.f, 5.25f);//

	pVertices[66].vPosition = _float3(1.f, 0.125f, 2.f);
	pVertices[66].vTexcoord = _float2(8.f, 5.25f);//

	pVertices[67].vPosition = _float3(1.5f, 0.375f, 2.f);
	pVertices[67].vTexcoord = _float2(9.f, 5.25f);//

	pVertices[68].vPosition = _float3(1.75f, 0.375f, 2.f);
	pVertices[68].vTexcoord = _float2(10.f, 5.25f);//

	pVertices[69].vPosition = _float3(1.9375f, 0.1875f, 2.f);
	pVertices[69].vTexcoord = _float2(11.f, 5.25f);//

#pragma region 텍스쳐
	pVertices[70].vPosition = _float3(-1.75f, 0.f, -2.f); //21
	pVertices[70].vTexcoord = _float2(0.f, 2.f);//

	pVertices[71].vPosition = _float3(1.75f, 0.f, -2.f); //12
	pVertices[71].vTexcoord = _float2(11.f, 2.f);//

	pVertices[72].vPosition = _float3(-1.75f, 0.f, 2.f); //43
	pVertices[72].vTexcoord = _float2(0.f, 6.f);//

	pVertices[73].vPosition = _float3(1.75f, 0.f, 2.f); //34
	pVertices[73].vTexcoord = _float2(11.f, 6.f);//

	pVertices[74].vPosition = _float3(-1.75f, 0.375f, -2.f); //47
	pVertices[74].vTexcoord = _float2(0.75f, 1.25f);//

	pVertices[75].vPosition = _float3(1.75f, 0.375f, -2.f); //56
	pVertices[75].vTexcoord = _float2(10.25f, 1.25f);//

	pVertices[76].vPosition = _float3(-1.75f, 0.375f, 2.f); //59
	pVertices[76].vTexcoord = _float2(1.25f, 5.25f);//

	pVertices[77].vPosition = _float3(1.75f, 0.375f, 2.f); //68
	pVertices[77].vTexcoord = _float2(10.25f, 5.25f);//

	pVertices[78].vPosition = _float3(-2.f, 0.25f, -2.f); //0
	pVertices[78].vTexcoord = _float2(0.f, 2.f);//

	pVertices[79].vPosition = _float3(-2.f, 0.25f, 2.f); //22
	pVertices[79].vTexcoord = _float2(0.f, 6.f);//

	pVertices[80].vPosition = _float3(2.f, 0.25f, -2.f); //11
	pVertices[80].vTexcoord = _float2(11.f, 2.f);//

	pVertices[81].vPosition = _float3(2.f, 0.25f, 2.f); //33
	pVertices[81].vTexcoord = _float2(11.f, 6.f);//

	pVertices[82].vPosition = _float3(0.f, 0.75f, 2.f); //45
	pVertices[82].vTexcoord = _float2(6.f, 5.f);//

	pVertices[83].vPosition = _float3(0.f, 0.75f, -2.f); //44
	pVertices[83].vTexcoord = _float2(6.f, 1.f);//

	pVertices[84].vPosition = _float3(0.f, 0.75f, 2.f); //45
	pVertices[84].vTexcoord = _float2(5.f, 5.f);//

	pVertices[85].vPosition = _float3(0.f, 0.75f, -2.f); //44
	pVertices[85].vTexcoord = _float2(5.f, 1.f);//
#pragma endregion

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
	pIndices[54] = 46; pIndices[55] = 74; pIndices[56] = 70;///정면
	pVertices[46].vNormal += ComputeNormalVector(pVertices, 46, 74, 70);
	pVertices[74].vNormal += ComputeNormalVector(pVertices, 46, 74, 70);
	pVertices[70].vNormal += ComputeNormalVector(pVertices, 46, 74, 70);

	Set_IndexBuffer(pIndices, 57, 48, 49, 19, 20);///정면
	pVertices[48].vNormal += ComputeNormalVector(pVertices, 48, 49, 19);
	pVertices[49].vNormal += ComputeNormalVector(pVertices, 48, 49, 19);
	pVertices[19].vNormal += ComputeNormalVector(pVertices, 48, 49, 19);
	pVertices[20].vNormal += ComputeNormalVector(pVertices, 48, 19, 20);

	pIndices[63] = 50; pIndices[64] = 17; pIndices[65] = 18;///정면
	pVertices[50].vNormal += ComputeNormalVector(pVertices, 50, 17, 18);
	pVertices[17].vNormal += ComputeNormalVector(pVertices, 50, 17, 18);
	pVertices[18].vNormal += ComputeNormalVector(pVertices, 50, 17, 18);

	pIndices[66] = 16; pIndices[67] = 53; pIndices[68] = 15;///정면
	pVertices[16].vNormal += ComputeNormalVector(pVertices, 16, 53, 15);
	pVertices[53].vNormal += ComputeNormalVector(pVertices, 16, 53, 15);
	pVertices[15].vNormal += ComputeNormalVector(pVertices, 16, 53, 15);

	Set_IndexBuffer(pIndices, 69, 54, 55, 13, 14);///정면
	pVertices[54].vNormal += ComputeNormalVector(pVertices, 54, 55, 13);
	pVertices[55].vNormal += ComputeNormalVector(pVertices, 54, 55, 13);
	pVertices[13].vNormal += ComputeNormalVector(pVertices, 54, 55, 13);
	pVertices[14].vNormal += ComputeNormalVector(pVertices, 54, 13, 14);

	pIndices[75] = 75; pIndices[76] = 57; pIndices[77] = 71;///정면
	pVertices[75].vNormal += ComputeNormalVector(pVertices, 75, 57, 71);
	pVertices[57].vNormal += ComputeNormalVector(pVertices, 75, 57, 71);
	pVertices[71].vNormal += ComputeNormalVector(pVertices, 75, 57, 71);

	Set_IndexBuffer(pIndices, 186, 1, 2, 20, 21);
	pVertices[1].vNormal += ComputeNormalVector(pVertices, 1, 2, 20);
	pVertices[2].vNormal += ComputeNormalVector(pVertices, 1, 2, 20);
	pVertices[20].vNormal += ComputeNormalVector(pVertices, 1, 2, 20);
	pVertices[21].vNormal += ComputeNormalVector(pVertices, 1, 20, 21);

	Set_IndexBuffer(pIndices, 192, 3, 4, 18, 19);
	pVertices[3].vNormal += ComputeNormalVector(pVertices, 3, 4, 18);
	pVertices[4].vNormal += ComputeNormalVector(pVertices, 3, 4, 18);
	pVertices[18].vNormal += ComputeNormalVector(pVertices, 3, 4, 18);
	pVertices[19].vNormal += ComputeNormalVector(pVertices, 3, 18, 19);

	Set_IndexBuffer(pIndices, 198, 7, 8, 14, 15);
	pVertices[7].vNormal += ComputeNormalVector(pVertices, 7, 8, 14);
	pVertices[8].vNormal += ComputeNormalVector(pVertices, 7, 8, 14);
	pVertices[14].vNormal += ComputeNormalVector(pVertices, 7, 8, 14);
	pVertices[15].vNormal += ComputeNormalVector(pVertices, 7, 14, 15);

	Set_IndexBuffer(pIndices, 204, 9, 10, 12, 13);
	pVertices[9].vNormal += ComputeNormalVector(pVertices, 9, 10, 12);
	pVertices[10].vNormal += ComputeNormalVector(pVertices, 9, 10, 12);
	pVertices[12].vNormal += ComputeNormalVector(pVertices, 9, 10, 12);
	pVertices[13].vNormal += ComputeNormalVector(pVertices, 9, 12, 13);

	Set_IndexBuffer(pIndices, 234, 5, 6, 16, 17);
	pVertices[5].vNormal += ComputeNormalVector(pVertices, 5, 6, 16);
	pVertices[6].vNormal += ComputeNormalVector(pVertices, 5, 6, 16);
	pVertices[16].vNormal += ComputeNormalVector(pVertices, 5, 6, 16);
	pVertices[17].vNormal += ComputeNormalVector(pVertices, 5, 16, 17);

	Set_IndexBuffer(pIndices, 246, 0, 1, 74, 46);
	pVertices[0].vNormal += ComputeNormalVector(pVertices, 0, 1, 74);
	pVertices[1].vNormal += ComputeNormalVector(pVertices, 0, 1, 74);
	pVertices[74].vNormal += ComputeNormalVector(pVertices, 0, 1, 74);
	pVertices[46].vNormal += ComputeNormalVector(pVertices, 0, 74, 46);

	Set_IndexBuffer(pIndices, 252, 2, 3, 49, 48);
	pVertices[2].vNormal += ComputeNormalVector(pVertices, 2, 3, 49);
	pVertices[3].vNormal += ComputeNormalVector(pVertices, 2, 3, 49);
	pVertices[49].vNormal += ComputeNormalVector(pVertices, 2, 3, 49);
	pVertices[48].vNormal += ComputeNormalVector(pVertices, 2, 49, 48);

	Set_IndexBuffer(pIndices, 258, 4, 5, 51, 50);
	pVertices[4].vNormal += ComputeNormalVector(pVertices, 4, 5, 51);
	pVertices[5].vNormal += ComputeNormalVector(pVertices, 4, 5, 51);
	pVertices[51].vNormal += ComputeNormalVector(pVertices, 4, 5, 51);
	pVertices[50].vNormal += ComputeNormalVector(pVertices, 4, 51, 50);

	Set_IndexBuffer(pIndices, 264, 6, 7, 53, 52);
	pVertices[6].vNormal += ComputeNormalVector(pVertices, 6, 7, 53);
	pVertices[7].vNormal += ComputeNormalVector(pVertices, 6, 7, 53);
	pVertices[53].vNormal += ComputeNormalVector(pVertices, 6, 7, 53);
	pVertices[52].vNormal += ComputeNormalVector(pVertices, 6, 53, 52);

	Set_IndexBuffer(pIndices, 270, 8, 9, 55, 54);
	pVertices[8].vNormal += ComputeNormalVector(pVertices, 8, 9, 55);
	pVertices[9].vNormal += ComputeNormalVector(pVertices, 8, 9, 55);
	pVertices[55].vNormal += ComputeNormalVector(pVertices, 8, 9, 55);
	pVertices[54].vNormal += ComputeNormalVector(pVertices, 8, 55, 54);

	Set_IndexBuffer(pIndices, 276, 10, 11, 57, 75);
	pVertices[10].vNormal += ComputeNormalVector(pVertices, 10, 11, 57);
	pVertices[11].vNormal += ComputeNormalVector(pVertices, 10, 11, 57);
	pVertices[57].vNormal += ComputeNormalVector(pVertices, 10, 11, 57);
	pVertices[75].vNormal += ComputeNormalVector(pVertices, 10, 57, 75);

	pIndices[318] = 50; pIndices[319] = 51; pIndices[320] = 17;
	pVertices[50].vNormal += ComputeNormalVector(pVertices, 50, 51, 17);
	pVertices[51].vNormal += ComputeNormalVector(pVertices, 50, 51, 17);
	pVertices[17].vNormal += ComputeNormalVector(pVertices, 50, 51, 17);

	pIndices[321] = 5; pIndices[322] = 44; pIndices[323] = 6;
	pVertices[5].vNormal += ComputeNormalVector(pVertices, 5, 44, 6);
	pVertices[44].vNormal += ComputeNormalVector(pVertices, 5, 44, 6);
	pVertices[6].vNormal += ComputeNormalVector(pVertices, 5, 44, 6);

	pIndices[324] = 52; pIndices[325] = 53; pIndices[326] = 16;
	pVertices[52].vNormal += ComputeNormalVector(pVertices, 52, 53, 16);
	pVertices[53].vNormal += ComputeNormalVector(pVertices, 52, 53, 16);
	pVertices[16].vNormal += ComputeNormalVector(pVertices, 52, 53, 16);

#pragma endregion

#pragma region 뒤
	pIndices[78] = 76; pIndices[79] = 58; pIndices[80] = 72;//정면
	pVertices[76].vNormal += ComputeNormalVector(pVertices, 76, 58, 72);
	pVertices[58].vNormal += ComputeNormalVector(pVertices, 76, 58, 72);
	pVertices[72].vNormal += ComputeNormalVector(pVertices, 76, 58, 72);

	Set_IndexBuffer(pIndices, 81, 61, 60, 42, 41);//정면
	pVertices[61].vNormal += ComputeNormalVector(pVertices, 61, 60, 42);
	pVertices[60].vNormal += ComputeNormalVector(pVertices, 61, 60, 42);
	pVertices[42].vNormal += ComputeNormalVector(pVertices, 61, 60, 42);
	pVertices[41].vNormal += ComputeNormalVector(pVertices, 61, 42, 41);

	pIndices[87] = 39; pIndices[88] = 62; pIndices[89] = 40;//정면
	pVertices[39].vNormal += ComputeNormalVector(pVertices, 39, 62, 40);
	pVertices[62].vNormal += ComputeNormalVector(pVertices, 39, 62, 40);
	pVertices[40].vNormal += ComputeNormalVector(pVertices, 39, 62, 40);

	pIndices[90] = 65; pIndices[91] = 38; pIndices[92] = 37;//정면
	pVertices[65].vNormal += ComputeNormalVector(pVertices, 65, 38, 37);
	pVertices[38].vNormal += ComputeNormalVector(pVertices, 65, 38, 37);
	pVertices[37].vNormal += ComputeNormalVector(pVertices, 65, 38, 37);

	Set_IndexBuffer(pIndices, 93, 67, 66, 36, 35);//정면
	pVertices[67].vNormal += ComputeNormalVector(pVertices, 67, 66, 36);
	pVertices[66].vNormal += ComputeNormalVector(pVertices, 67, 66, 36);
	pVertices[36].vNormal += ComputeNormalVector(pVertices, 67, 66, 36);
	pVertices[35].vNormal += ComputeNormalVector(pVertices, 67, 36, 35);

	pIndices[99] = 69; pIndices[100] = 77; pIndices[101] = 73;//정면
	pVertices[69].vNormal += ComputeNormalVector(pVertices, 69, 77, 73);
	pVertices[77].vNormal += ComputeNormalVector(pVertices, 69, 77, 73);
	pVertices[73].vNormal += ComputeNormalVector(pVertices, 69, 77, 73);

	Set_IndexBuffer(pIndices, 210, 24, 23, 43, 42);
	pVertices[24].vNormal += ComputeNormalVector(pVertices, 24, 23, 43);
	pVertices[23].vNormal += ComputeNormalVector(pVertices, 24, 23, 43);
	pVertices[43].vNormal += ComputeNormalVector(pVertices, 24, 23, 43);
	pVertices[42].vNormal += ComputeNormalVector(pVertices, 24, 43, 42);

	Set_IndexBuffer(pIndices, 216, 26, 25, 41, 40);
	pVertices[26].vNormal += ComputeNormalVector(pVertices, 26, 25, 41);
	pVertices[25].vNormal += ComputeNormalVector(pVertices, 26, 25, 41);
	pVertices[41].vNormal += ComputeNormalVector(pVertices, 26, 25, 41);
	pVertices[40].vNormal += ComputeNormalVector(pVertices, 26, 41, 40);

	Set_IndexBuffer(pIndices, 222, 30, 29, 37, 36);
	pVertices[30].vNormal += ComputeNormalVector(pVertices, 30, 29, 37);
	pVertices[29].vNormal += ComputeNormalVector(pVertices, 30, 29, 37);
	pVertices[37].vNormal += ComputeNormalVector(pVertices, 30, 29, 37);
	pVertices[36].vNormal += ComputeNormalVector(pVertices, 30, 37, 36);

	Set_IndexBuffer(pIndices, 228, 32, 31, 35, 34);
	pVertices[32].vNormal += ComputeNormalVector(pVertices, 32, 31, 35);
	pVertices[31].vNormal += ComputeNormalVector(pVertices, 32, 31, 35);
	pVertices[35].vNormal += ComputeNormalVector(pVertices, 32, 31, 35);
	pVertices[34].vNormal += ComputeNormalVector(pVertices, 32, 35, 34);

	Set_IndexBuffer(pIndices, 240, 28, 27, 39, 38);
	pVertices[28].vNormal += ComputeNormalVector(pVertices, 28, 27, 39);
	pVertices[27].vNormal += ComputeNormalVector(pVertices, 28, 27, 39);
	pVertices[39].vNormal += ComputeNormalVector(pVertices, 28, 27, 39);
	pVertices[38].vNormal += ComputeNormalVector(pVertices, 28, 39, 38);

	Set_IndexBuffer(pIndices, 282, 23, 22, 58, 76);
	pVertices[23].vNormal += ComputeNormalVector(pVertices, 23, 22, 58);
	pVertices[22].vNormal += ComputeNormalVector(pVertices, 23, 22, 58);
	pVertices[58].vNormal += ComputeNormalVector(pVertices, 23, 22, 58);
	pVertices[76].vNormal += ComputeNormalVector(pVertices, 23, 58, 76);

	Set_IndexBuffer(pIndices, 288, 25, 24, 60, 61);
	pVertices[25].vNormal += ComputeNormalVector(pVertices, 25, 24, 60);
	pVertices[24].vNormal += ComputeNormalVector(pVertices, 25, 24, 60);
	pVertices[60].vNormal += ComputeNormalVector(pVertices, 25, 24, 60);
	pVertices[61].vNormal += ComputeNormalVector(pVertices, 25, 60, 61);

	Set_IndexBuffer(pIndices, 294, 27, 26, 62, 63);
	pVertices[27].vNormal += ComputeNormalVector(pVertices, 27, 26, 62);
	pVertices[26].vNormal += ComputeNormalVector(pVertices, 27, 26, 62);
	pVertices[62].vNormal += ComputeNormalVector(pVertices, 27, 26, 62);
	pVertices[63].vNormal += ComputeNormalVector(pVertices, 27, 62, 63);

	Set_IndexBuffer(pIndices, 300, 29, 28, 64, 65);
	pVertices[29].vNormal += ComputeNormalVector(pVertices, 29, 28, 64);
	pVertices[28].vNormal += ComputeNormalVector(pVertices, 29, 28, 64);
	pVertices[64].vNormal += ComputeNormalVector(pVertices, 29, 28, 64);
	pVertices[65].vNormal += ComputeNormalVector(pVertices, 29, 64, 65);

	Set_IndexBuffer(pIndices, 306, 31, 30, 66, 67);
	pVertices[31].vNormal += ComputeNormalVector(pVertices, 31, 30, 66);
	pVertices[30].vNormal += ComputeNormalVector(pVertices, 31, 30, 66);
	pVertices[66].vNormal += ComputeNormalVector(pVertices, 31, 30, 66);
	pVertices[67].vNormal += ComputeNormalVector(pVertices, 31, 66, 67);

	Set_IndexBuffer(pIndices, 312, 33, 32, 77, 69);
	pVertices[33].vNormal += ComputeNormalVector(pVertices, 33, 32, 77);
	pVertices[32].vNormal += ComputeNormalVector(pVertices, 33, 32, 77);
	pVertices[77].vNormal += ComputeNormalVector(pVertices, 33, 32, 77);
	pVertices[69].vNormal += ComputeNormalVector(pVertices, 33, 77, 69);

	pIndices[327] = 63; pIndices[328] = 62; pIndices[329] = 39;
	pVertices[63].vNormal += ComputeNormalVector(pVertices, 63, 62, 39);
	pVertices[62].vNormal += ComputeNormalVector(pVertices, 63, 62, 39);
	pVertices[39].vNormal += ComputeNormalVector(pVertices, 63, 62, 39);

	pIndices[330] = 28; pIndices[331] = 45; pIndices[332] = 27;
	pVertices[28].vNormal += ComputeNormalVector(pVertices, 28, 45, 27);
	pVertices[45].vNormal += ComputeNormalVector(pVertices, 28, 45, 27);
	pVertices[27].vNormal += ComputeNormalVector(pVertices, 28, 45, 27);

	pIndices[333] = 65; pIndices[334] = 64; pIndices[335] = 38;
	pVertices[65].vNormal += ComputeNormalVector(pVertices, 65, 64, 38);
	pVertices[64].vNormal += ComputeNormalVector(pVertices, 65, 64, 38);
	pVertices[38].vNormal += ComputeNormalVector(pVertices, 65, 64, 38);
#pragma endregion

#pragma region 위
	Set_IndexBuffer(pIndices, 162, 22, 23, 1, 0);///위(옆)
	pVertices[22].vNormal += ComputeNormalVector(pVertices, 22, 23, 1);
	pVertices[23].vNormal += ComputeNormalVector(pVertices, 22, 23, 1);
	pVertices[1].vNormal += ComputeNormalVector(pVertices, 22, 23, 1);
	pVertices[0].vNormal += ComputeNormalVector(pVertices, 22, 1, 0);

	Set_IndexBuffer(pIndices, 102, 23, 24, 2, 1);///위
	pVertices[23].vNormal += ComputeNormalVector(pVertices, 23, 24, 2);
	pVertices[24].vNormal += ComputeNormalVector(pVertices, 23, 24, 2);
	pVertices[2].vNormal += ComputeNormalVector(pVertices, 23, 24, 2);
	pVertices[1].vNormal += ComputeNormalVector(pVertices, 23, 2, 1);

	Set_IndexBuffer(pIndices, 108, 24, 25, 3, 2);///위
	pVertices[24].vNormal += ComputeNormalVector(pVertices, 24, 25, 3);
	pVertices[25].vNormal += ComputeNormalVector(pVertices, 24, 25, 3);
	pVertices[3].vNormal += ComputeNormalVector(pVertices, 24, 25, 3);
	pVertices[2].vNormal += ComputeNormalVector(pVertices, 24, 3, 2);

	Set_IndexBuffer(pIndices, 114, 25, 26, 4, 3);///위
	pVertices[25].vNormal += ComputeNormalVector(pVertices, 25, 26, 4);
	pVertices[26].vNormal += ComputeNormalVector(pVertices, 25, 26, 4);
	pVertices[4].vNormal += ComputeNormalVector(pVertices, 25, 26, 4);
	pVertices[3].vNormal += ComputeNormalVector(pVertices, 25, 4, 3);

	Set_IndexBuffer(pIndices, 120, 26, 27, 5, 4);///위
	pVertices[26].vNormal += ComputeNormalVector(pVertices, 26, 27, 5);
	pVertices[27].vNormal += ComputeNormalVector(pVertices, 26, 27, 5);
	pVertices[5].vNormal += ComputeNormalVector(pVertices, 26, 27, 5);
	pVertices[4].vNormal += ComputeNormalVector(pVertices, 26, 5, 4);

	Set_IndexBuffer(pIndices, 126, 27, 82, 83, 5);///위
	pVertices[27].vNormal += ComputeNormalVector(pVertices, 27, 82, 83);
	pVertices[82].vNormal += ComputeNormalVector(pVertices, 27, 82, 83);
	pVertices[83].vNormal += ComputeNormalVector(pVertices, 27, 82, 83);
	pVertices[5].vNormal += ComputeNormalVector(pVertices, 27, 83, 5);

	Set_IndexBuffer(pIndices, 132, 84, 28, 6, 85);///위
	pVertices[84].vNormal += ComputeNormalVector(pVertices, 84, 28, 6);
	pVertices[28].vNormal += ComputeNormalVector(pVertices, 84, 28, 6);
	pVertices[6].vNormal += ComputeNormalVector(pVertices, 84, 28, 6);
	pVertices[85].vNormal += ComputeNormalVector(pVertices, 84, 6, 85);

	Set_IndexBuffer(pIndices, 138, 28, 29, 7, 6);///위
	pVertices[28].vNormal += ComputeNormalVector(pVertices, 28, 29, 7);
	pVertices[29].vNormal += ComputeNormalVector(pVertices, 28, 29, 7);
	pVertices[7].vNormal += ComputeNormalVector(pVertices, 28, 29, 7);
	pVertices[6].vNormal += ComputeNormalVector(pVertices, 28, 7, 6);

	Set_IndexBuffer(pIndices, 144, 29, 30, 8, 7);///위
	pVertices[29].vNormal += ComputeNormalVector(pVertices, 29, 30, 8);
	pVertices[30].vNormal += ComputeNormalVector(pVertices, 29, 30, 8);
	pVertices[8].vNormal += ComputeNormalVector(pVertices, 29, 30, 8);
	pVertices[7].vNormal += ComputeNormalVector(pVertices, 29, 8, 7);

	Set_IndexBuffer(pIndices, 150, 30, 31, 9, 8);///위
	pVertices[30].vNormal += ComputeNormalVector(pVertices, 30, 31, 9);
	pVertices[31].vNormal += ComputeNormalVector(pVertices, 30, 31, 9);
	pVertices[9].vNormal += ComputeNormalVector(pVertices, 30, 31, 9);
	pVertices[8].vNormal += ComputeNormalVector(pVertices, 30, 9, 8);

	Set_IndexBuffer(pIndices, 156, 31, 32, 10, 9);///위
	pVertices[31].vNormal += ComputeNormalVector(pVertices, 31, 32, 10);
	pVertices[32].vNormal += ComputeNormalVector(pVertices, 31, 32, 10);
	pVertices[10].vNormal += ComputeNormalVector(pVertices, 31, 32, 10);
	pVertices[9].vNormal += ComputeNormalVector(pVertices, 31, 10, 9);

	Set_IndexBuffer(pIndices, 168, 32, 33, 11, 10);///위(옆)
	pVertices[32].vNormal += ComputeNormalVector(pVertices, 32, 33, 11);
	pVertices[33].vNormal += ComputeNormalVector(pVertices, 32, 33, 11);
	pVertices[11].vNormal += ComputeNormalVector(pVertices, 32, 33, 11);
	pVertices[10].vNormal += ComputeNormalVector(pVertices, 32, 11, 10);
#pragma endregion

#pragma region 아래
	Set_IndexBuffer(pIndices, 174, 78, 21, 43, 79);///아래(옆)
	pVertices[78].vNormal += ComputeNormalVector(pVertices, 78, 21, 43);
	pVertices[21].vNormal += ComputeNormalVector(pVertices, 78, 21, 43);
	pVertices[43].vNormal += ComputeNormalVector(pVertices, 78, 21, 43);
	pVertices[79].vNormal += ComputeNormalVector(pVertices, 78, 43, 79);

	Set_IndexBuffer(pIndices, 30, 21, 20, 42, 43);///아래 줄무늬
	pVertices[21].vNormal += ComputeNormalVector(pVertices, 21, 20, 42);
	pVertices[20].vNormal += ComputeNormalVector(pVertices, 21, 20, 42);
	pVertices[42].vNormal += ComputeNormalVector(pVertices, 21, 20, 42);
	pVertices[43].vNormal += ComputeNormalVector(pVertices, 21, 42, 43);

	Set_IndexBuffer(pIndices, 0, 20, 19, 41, 42);///아래
	pVertices[20].vNormal += ComputeNormalVector(pVertices, 20, 19, 41);
	pVertices[19].vNormal += ComputeNormalVector(pVertices, 20, 19, 41);
	pVertices[41].vNormal += ComputeNormalVector(pVertices, 20, 19, 41);
	pVertices[42].vNormal += ComputeNormalVector(pVertices, 20, 41, 42);

	Set_IndexBuffer(pIndices, 36, 19, 18, 40, 41);///아래 줄무늬
	pVertices[19].vNormal += ComputeNormalVector(pVertices, 19, 18, 40);
	pVertices[18].vNormal += ComputeNormalVector(pVertices, 19, 18, 40);
	pVertices[40].vNormal += ComputeNormalVector(pVertices, 19, 18, 40);
	pVertices[41].vNormal += ComputeNormalVector(pVertices, 19, 40, 41);

	Set_IndexBuffer(pIndices, 6, 18, 17, 39, 40);///아래
	pVertices[18].vNormal += ComputeNormalVector(pVertices, 18, 17, 39);
	pVertices[17].vNormal += ComputeNormalVector(pVertices, 18, 17, 39);
	pVertices[39].vNormal += ComputeNormalVector(pVertices, 18, 17, 39);
	pVertices[40].vNormal += ComputeNormalVector(pVertices, 18, 39, 40);

	Set_IndexBuffer(pIndices, 12, 17, 16, 38, 39);///아래
	pVertices[17].vNormal += ComputeNormalVector(pVertices, 17, 16, 38);
	pVertices[16].vNormal += ComputeNormalVector(pVertices, 17, 16, 38);
	pVertices[38].vNormal += ComputeNormalVector(pVertices, 17, 16, 38);
	pVertices[39].vNormal += ComputeNormalVector(pVertices, 17, 38, 39);

	Set_IndexBuffer(pIndices, 18, 16, 15, 37, 38);///아래
	pVertices[16].vNormal += ComputeNormalVector(pVertices, 16, 15, 37);
	pVertices[15].vNormal += ComputeNormalVector(pVertices, 16, 15, 37);
	pVertices[37].vNormal += ComputeNormalVector(pVertices, 16, 15, 37);
	pVertices[38].vNormal += ComputeNormalVector(pVertices, 16, 37, 38);

	Set_IndexBuffer(pIndices, 42, 15, 14, 36, 37);///아래 줄무늬
	pVertices[15].vNormal += ComputeNormalVector(pVertices, 15, 14, 36);
	pVertices[14].vNormal += ComputeNormalVector(pVertices, 15, 14, 36);
	pVertices[36].vNormal += ComputeNormalVector(pVertices, 15, 14, 36);
	pVertices[37].vNormal += ComputeNormalVector(pVertices, 15, 36, 37);

	Set_IndexBuffer(pIndices, 24, 14, 13, 35, 36);///아래
	pVertices[14].vNormal += ComputeNormalVector(pVertices, 14, 13, 35);
	pVertices[13].vNormal += ComputeNormalVector(pVertices, 14, 13, 35);
	pVertices[35].vNormal += ComputeNormalVector(pVertices, 14, 13, 35);
	pVertices[36].vNormal += ComputeNormalVector(pVertices, 14, 35, 36);

	Set_IndexBuffer(pIndices, 48, 13, 12, 34, 35);///아래 줄무늬
	pVertices[13].vNormal += ComputeNormalVector(pVertices, 13, 12, 34);
	pVertices[12].vNormal += ComputeNormalVector(pVertices, 13, 12, 34);
	pVertices[34].vNormal += ComputeNormalVector(pVertices, 13, 12, 34);
	pVertices[35].vNormal += ComputeNormalVector(pVertices, 13, 34, 35);

	Set_IndexBuffer(pIndices, 180, 12, 80, 81, 34);///아래(옆)
	pVertices[12].vNormal += ComputeNormalVector(pVertices, 12, 80, 81);
	pVertices[80].vNormal += ComputeNormalVector(pVertices, 12, 80, 81);
	pVertices[81].vNormal += ComputeNormalVector(pVertices, 12, 80, 81);
	pVertices[34].vNormal += ComputeNormalVector(pVertices, 12, 81, 34);
#pragma endregion

	m_pVB->Unlock();
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
	case BOTTOM:
		iNumber = 0;
		break;

	case BOTTOMLINE:
		iNumber = 30;
		break;

	case FRONTFACE:
		iNumber = 54;
		break;

	case TOP:
		iNumber = 102;
		break;

	case TOPSIDE:
		iNumber = 162;
		break;

	case BOTTOMSIDE:
		iNumber = 174;
		break;

	case MUZZLE1:
		iNumber = 186;
		break;

	case MUZZLE2:
		iNumber = 192;
		break;

	case MUZZLE3:
		iNumber = 198;
		break;

	case MUZZLE4:
		iNumber = 204;
		break;

	case MIDDLE:
		iNumber = 234;
		break;

	case OUTLINE:
		iNumber = 246;
		break;

	case TRIANGLE:
		iNumber = 318;
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
