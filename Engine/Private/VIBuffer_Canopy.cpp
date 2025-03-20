#include "VIBuffer_Canopy.h"

CVIBuffer_Canopy::CVIBuffer_Canopy(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
}

CVIBuffer_Canopy::CVIBuffer_Canopy(const CVIBuffer_Canopy& Prototype)
	: CVIBuffer(Prototype)
{
}

HRESULT CVIBuffer_Canopy::Initialize_Prototype()
{
	m_iNumVertices = 54;
	m_iVertexStride = sizeof(VTXPOSNORTEX);
	m_iFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
	m_iNumPritimive = 46;

	m_iIndexStride = 2;
	m_iNumIndices = 144;
	m_eIndexFormat = D3DFMT_INDEX16;

#pragma region VERTEX_BUFFER
	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXPOSNORTEX* pVertices = { nullptr };

	m_pVB->Lock(0, 0, reinterpret_cast<void**>(&pVertices), 0);

	for (int i = 0; i < m_iNumVertices; i++)
		pVertices[i].vNormal = _float3(0.f, 0.f, 0.f);

#pragma region ¾Õ ±âµÕ ¾Õ¸é
	pVertices[0].vPosition = _float3(-0.5f, 0.5f, -0.5f);
	pVertices[0].vTexcoord = _float2(0.f, 1.f);//
	
	pVertices[1].vPosition = _float3(0.5f, 0.5f, -0.5f);
	pVertices[1].vTexcoord = _float2(0.25f, 0.f);//

	pVertices[2].vPosition = _float3(0.5f, -0.5f, -0.5f);
	pVertices[2].vTexcoord = _float2(0.25f, 1.f);//

	pVertices[3].vPosition = _float3(0.375f, -0.5f, -0.5f);
	pVertices[3].vTexcoord = _float2(0.f, 1.f);//

	pVertices[4].vPosition = _float3(0.375f, 0.375f, -0.5f);
	pVertices[4].vTexcoord = _float2(0.25f, 0.f);//

	pVertices[5].vPosition = _float3(-0.5f, 0.375f, -0.5f);
	pVertices[5].vTexcoord = _float2(0.25f, 1.f);//

	pVertices[32].vPosition = _float3(0.375f, 0.5f, -0.5f);
	pVertices[32].vTexcoord = _float2(0.f, 0.f);//
#pragma endregion

#pragma region ¾Õ ±âµÕ µÞ¸é
	pVertices[6].vPosition = _float3(-0.5f, 0.5f, -0.375f);
	pVertices[6].vTexcoord = _float2(0.f, 1.25f);//

	pVertices[7].vPosition = _float3(0.5f, 0.5f, -0.375f);
	pVertices[7].vTexcoord = _float2(0.5f, 0.f);//

	pVertices[8].vPosition = _float3(0.5f, -0.5f, -0.375f);
	pVertices[8].vTexcoord = _float2(0.5f, 1.f);//

	pVertices[9].vPosition = _float3(0.375f, -0.5f, -0.375f);
	pVertices[9].vTexcoord = _float2(0.75f, 1.f);//

	pVertices[10].vPosition = _float3(0.375f, 0.375f, -0.375f);
	pVertices[10].vTexcoord = _float2(1.f, 0.f);//

	pVertices[11].vPosition = _float3(-0.5f, 0.375f, -0.375f);
	pVertices[11].vTexcoord = _float2(0.25f, 1.25f);//

	pVertices[33].vPosition = _float3(0.375f, 0.5f, -0.375f);
	pVertices[33].vTexcoord = _float2(0.75f, 0.f);//
#pragma endregion

#pragma region µÞ ±âµÕ ¾Õ¸é
	pVertices[12].vPosition = _float3(-0.5f, 0.5f, 0.375f);
	pVertices[12].vTexcoord = _float2(0.75f, 1.25f);//

	pVertices[13].vPosition = _float3(0.5f, 0.5f, 0.375f);
	pVertices[13].vTexcoord = _float2(0.25f, 0.f);//

	pVertices[14].vPosition = _float3(0.5f, -0.5f, 0.375f);
	pVertices[14].vTexcoord = _float2(0.25f, 0.f);//

	pVertices[15].vPosition = _float3(0.375f, -0.5f, 0.375f);
	pVertices[15].vTexcoord = _float2(0.f, 1.f);//

	pVertices[16].vPosition = _float3(0.375f, 0.375f, 0.375f);
	pVertices[16].vTexcoord = _float2(0.25f, 0.f);//

	pVertices[17].vPosition = _float3(-0.5f, 0.375f, 0.375f);
	pVertices[17].vTexcoord = _float2(0.5f, 1.25f);//

	pVertices[34].vPosition = _float3(0.375f, 0.5f, 0.375f);
	pVertices[34].vTexcoord = _float2(0.f, 0.f);//
#pragma endregion

#pragma region µÞ ±âµÕ µÞ¸é
	pVertices[18].vPosition = _float3(-0.5f, 0.5f, 0.5f);
	pVertices[18].vTexcoord = _float2(0.75f, 1.f);//

	pVertices[19].vPosition = _float3(0.5f, 0.5f, 0.5f);
	pVertices[19].vTexcoord = _float2(0.5f, 0.f);//

	pVertices[20].vPosition = _float3(0.5f, -0.5f, 0.5f);
	pVertices[20].vTexcoord = _float2(0.5f, 1.f);//

	pVertices[21].vPosition = _float3(0.375f, -0.5f, 0.5f);
	pVertices[21].vTexcoord = _float2(0.75f, 1.f);//

	pVertices[22].vPosition = _float3(0.375f, 0.375f, 0.5f);
	pVertices[22].vTexcoord = _float2(0.5f, 1.f);//

	pVertices[23].vPosition = _float3(-0.5f, 0.375f, 0.5f);
	pVertices[23].vTexcoord = _float2(0.5f, 1.f);//

	pVertices[35].vPosition = _float3(0.375f, 0.5f, 0.5f);
	pVertices[35].vTexcoord = _float2(0.75f, 0.f);//
#pragma endregion

#pragma region Ãµ¸· ¾Õ¸é
	pVertices[24].vPosition = _float3(-0.25f, 0.5f, -0.375f);
	pVertices[24].vTexcoord = _float2(0.f, 1.f);//

	pVertices[25].vPosition = _float3(0.25f, 0.5f, -0.375f);
	pVertices[25].vTexcoord = _float2(1.f, 1.f);//

	pVertices[26].vPosition = _float3(0.25f, 0.375f, -0.375f);
	pVertices[26].vTexcoord = _float2(0.f, 1.f);//

	pVertices[27].vPosition = _float3(-0.25f, 0.375f, -0.375f);
	pVertices[27].vTexcoord = _float2(1.f, 1.f);//
#pragma endregion

#pragma region Ãµ¸· µÞ¸é
	pVertices[28].vPosition = _float3(-0.25f, 0.5f, 0.375f);
	pVertices[28].vTexcoord = _float2(0.f, 0.f);//

	pVertices[29].vPosition = _float3(0.25f, 0.5f, 0.375f);
	pVertices[29].vTexcoord = _float2(1.f, 0.f);//

	pVertices[30].vPosition = _float3(0.25f, 0.375f, 0.375f);
	pVertices[30].vTexcoord = _float2(0.f, 0.f);//

	pVertices[31].vPosition = _float3(-0.25f, 0.375f, 0.375f);
	pVertices[31].vTexcoord = _float2(1.f, 0.f);//
#pragma endregion

#pragma region ÅØ½ºÃ³ ¶§¹®¿¡ Ãß°¡
	pVertices[36].vPosition = _float3(-0.25f, 0.5f, 0.375f);		//28
	pVertices[36].vTexcoord = _float2(0.75f, 2.f);

	pVertices[37].vPosition = _float3(-0.25f, 0.375f, 0.375f);		//31
	pVertices[37].vTexcoord = _float2(0.5f, 2.f);

	pVertices[38].vPosition = _float3(0.25f, 0.5f, 0.375f);			//29
	pVertices[38].vTexcoord = _float2(0.f, 0.25f);

	pVertices[39].vPosition = _float3(0.25f, 0.375f, 0.375f);		//30
	pVertices[39].vTexcoord = _float2(0.25f, 0.25f);

	pVertices[40].vPosition = _float3(0.5f, -0.5f, 0.375f);			//14
	pVertices[40].vTexcoord = _float2(0.25f, 1.f);
		
	pVertices[41].vPosition = _float3(0.25f, 0.5f, -0.375f);		//25
	pVertices[41].vTexcoord = _float2(0.75f, -0.25f);

	pVertices[42].vPosition = _float3(0.25f, 0.375f, -0.375f);		//26
	pVertices[42].vTexcoord = _float2(1.f, -0.25f);

	pVertices[43].vPosition = _float3(-0.5f, 0.375f, 0.375f);		//17
	pVertices[43].vTexcoord = _float2(0.25f, 1.f);

	pVertices[44].vPosition = _float3(0.375f, 0.375f, 0.5f);		//22
	pVertices[44].vTexcoord = _float2(0.5f, 0.f);

	pVertices[45].vPosition = _float3(0.375f, -0.5f, 0.5f);			//21
	pVertices[45].vTexcoord = _float2(0.5f, -1.f);

	pVertices[46].vPosition = _float3(0.375f, -0.5f, 0.375f);		//15
	pVertices[46].vTexcoord = _float2(0.25f, -1.f);

	pVertices[47].vPosition = _float3(-0.5f, 0.375f, -0.5f);		//5
	pVertices[47].vTexcoord = _float2(0.f, 1.25f);

	pVertices[48].vPosition = _float3(0.375f, 0.375f, -0.5f);		//4
	pVertices[48].vTexcoord = _float2(0.f, 0.25f);

	pVertices[49].vPosition = _float3(0.375f, 0.375f, -0.375f);		//10
	pVertices[49].vTexcoord = _float2(0.f, 0.f);

	pVertices[50].vPosition = _float3(0.375f, 0.375f, -0.5f);		//4
	pVertices[50].vTexcoord = _float2(0.25f, 0.f);

	pVertices[51].vPosition = _float3(0.375f, -0.5f, -0.5f);		//3
	pVertices[51].vTexcoord = _float2(0.25f, 1.f);

	pVertices[52].vPosition = _float3(0.375f, -0.5f, -0.375f);		//9
	pVertices[52].vTexcoord = _float2(0.f, 1.f);

	pVertices[53].vPosition = _float3(0.375f, 0.375f, -0.375f);		//10
	pVertices[53].vTexcoord = _float2(0.25f, 0.25f);
#pragma endregion

#pragma endregion

#pragma region INDEX_BUFFER
	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	_ushort* pIndices = { nullptr };

	m_pIB->Lock(0, 0, reinterpret_cast<void**>(&pIndices), 0);
	
#pragma region ¾Õ
	Set_IndexBuffer(pIndices, 0, 0, 32, 4, 5);
	pVertices[4].vNormal += ComputeNormalVector(pVertices, 4, 1, 2);
	pVertices[1].vNormal += ComputeNormalVector(pVertices, 4, 1, 2);
	pVertices[2].vNormal += ComputeNormalVector(pVertices, 4, 1, 2);
	pVertices[3].vNormal += ComputeNormalVector(pVertices, 4, 2, 3);
	
	Set_IndexBuffer(pIndices, 6, 32, 1, 2, 3);
	Set_IndexBuffer(pIndices, 12, 12, 36, 37, 17);
	Set_IndexBuffer(pIndices, 18, 38, 34, 16, 39);
	Set_IndexBuffer(pIndices, 24, 34, 13, 40, 15);
#pragma endregion

#pragma region µÚ
	Set_IndexBuffer(pIndices, 30, 7, 33, 9, 8);
	Set_IndexBuffer(pIndices, 36, 33, 41, 42, 10);
	Set_IndexBuffer(pIndices, 42, 24, 6, 11, 27);
	Set_IndexBuffer(pIndices, 48, 19, 35, 21, 20);
	Set_IndexBuffer(pIndices, 54, 35, 18, 23, 22);
#pragma endregion

#pragma region ¿ÞÂÊ
	Set_IndexBuffer(pIndices, 60, 18, 12, 17, 23);
	Set_IndexBuffer(pIndices, 66, 6, 0, 5, 11);
	Set_IndexBuffer(pIndices, 72, 49, 50, 51, 52);
	Set_IndexBuffer(pIndices, 78, 22, 16, 46, 45);
	Set_IndexBuffer(pIndices, 120, 28, 24, 27, 31);
#pragma endregion

#pragma region ¿À¸¥ÂÊ
	Set_IndexBuffer(pIndices, 84, 1, 7, 8, 2);
	Set_IndexBuffer(pIndices, 90, 13, 19, 20, 14);
	Set_IndexBuffer(pIndices, 126, 25, 29, 30, 26);
#pragma endregion

#pragma region À§
	Set_IndexBuffer(pIndices, 96, 0, 6, 7, 1);
	Set_IndexBuffer(pIndices, 102, 12, 18, 19, 13);
	Set_IndexBuffer(pIndices, 132, 28, 29, 25, 24);
#pragma endregion

#pragma region ¾Æ·¡
	Set_IndexBuffer(pIndices, 108, 53, 11, 47, 48);
	Set_IndexBuffer(pIndices, 114, 44, 23, 43, 16);
	Set_IndexBuffer(pIndices, 138, 30, 31, 27, 26);
#pragma endregion

	m_pVB->Unlock();
	m_pIB->Unlock();
#pragma endregion
	return S_OK;
}

HRESULT CVIBuffer_Canopy::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_Canopy::Render(_uint Surface)
{		
	Surface *= 120;
	return m_pGraphic_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iNumVertices, Surface, m_iNumPritimive);
}

void CVIBuffer_Canopy::Set_IndexBuffer(_ushort* pIndices, _uint StartIndex, _uint LT, _uint RT, _uint RB, _uint LB)
{
	pIndices[StartIndex++] = LT;
	pIndices[StartIndex++] = RT;
	pIndices[StartIndex++] = RB;

	pIndices[StartIndex++] = LT;
	pIndices[StartIndex++] = RB;
	pIndices[StartIndex++] = LB;
}

VECTOR CVIBuffer_Canopy::ComputeNormalVector(VTXPOSNORTEX* pVertices, _uint vertex1, _uint vertex2, _uint vertex3)
{
	return __super::ComputeNormal(&(pVertices[vertex1].vPosition), &(pVertices[vertex2].vPosition), &(pVertices[vertex3].vPosition));
}

CVIBuffer_Canopy* CVIBuffer_Canopy::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_Canopy* pInstance = new CVIBuffer_Canopy(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Canopy");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Canopy::Clone(void* pArg)
{
	CVIBuffer_Canopy* pInstance = new CVIBuffer_Canopy(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CVIBuffer_Canopy");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Canopy::Free()
{
	__super::Free();
}
