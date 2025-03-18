#include "VIBuffer_Computer.h"

CVIBuffer_Computer::CVIBuffer_Computer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer { pGraphic_Device }
{
}

CVIBuffer_Computer::CVIBuffer_Computer(const CVIBuffer& Prototype)
	: CVIBuffer ( Prototype )
{
}

HRESULT CVIBuffer_Computer::Initialize_Prototype()
{
	m_iNumVertices = 28;
	m_iVertexStride = sizeof(VTXPOSTEX);
	m_iFVF = D3DFVF_XYZ | D3DFVF_TEX1/* | D3DFVF_TEXCOORDSIZE2(0)*/;
	m_iNumPritimive = 23;

	m_iIndexStride = 2;
	m_iNumIndices = 84;
	m_eIndexFormat = D3DFMT_INDEX16;

#pragma region VERTEX_BUFFER
	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXPOSTEX* pVertices = { nullptr };


	m_pVB->Lock(0, /*m_iNumVertices * m_iVertexStride*/0, reinterpret_cast<void**>(&pVertices), 0);

#pragma region 왼쪽면
	pVertices[0].vPosition = _float3(-0.5f, 0.25f, -0.375f); 
	pVertices[0].vTexcoord = _float2(1.f, 0.f);

	pVertices[1].vPosition = _float3(0.375f, 0.5f, -0.5f);
	pVertices[1].vTexcoord = _float2(1.f, 0.f);

	pVertices[2].vPosition = _float3(0.5f, 0.5f, -0.5f);	
	pVertices[2].vTexcoord = _float2(0.f, 0.f);		

	pVertices[3].vPosition = _float3(0.5f, -0.5f, -0.5f);	
	pVertices[3].vTexcoord = _float2(0.f, 1.f);

	pVertices[4].vPosition = _float3(0.375f, -0.5f, -0.5f);
	pVertices[4].vTexcoord = _float2(1.f, 1.f);

	pVertices[5].vPosition = _float3(-0.5f, -0.5f, -0.375f); 
	pVertices[5].vTexcoord = _float2(1.f, 1.f);

	pVertices[6].vPosition = _float3(0.375f, 0.25f, -0.5f); 
	pVertices[6].vTexcoord = _float2(1.f, 0.f);
#pragma endregion

#pragma region 오른쪽면
	pVertices[7].vPosition = _float3(-0.5f, 0.25f, 0.375f); 
	pVertices[7].vTexcoord = _float2(0.f, 0.f);

	pVertices[8].vPosition = _float3(0.375f, 0.5f, 0.5f);
	pVertices[8].vTexcoord = _float2(0.f, 0.f);

	pVertices[9].vPosition = _float3(0.5f, 0.5f, 0.5f);	
	pVertices[9].vTexcoord = _float2(1.f, 0.f);

	pVertices[10].vPosition = _float3(0.5f, -0.5f, 0.5f);	
	pVertices[10].vTexcoord = _float2(1.f, 1.f);

	pVertices[11].vPosition = _float3(0.375f, -0.5f, 0.5f);
	pVertices[11].vTexcoord = _float2(0.f, 1.f);

	pVertices[12].vPosition = _float3(-0.5f, -0.5f, 0.375f); 
	pVertices[12].vTexcoord = _float2(0.f, 1.f);

	pVertices[13].vPosition = _float3(0.375f, 0.25f, 0.5f);	
	pVertices[13].vTexcoord = _float2(0.f, 0.f);
#pragma endregion

#pragma region 위쪽 추가
	pVertices[14].vPosition = _float3(0.375f, 0.5f, 0.375f); 
	pVertices[14].vTexcoord = _float2(1.f, 1.f);

	pVertices[15].vPosition = _float3(0.375f, 0.5f, -0.375f); 
	pVertices[15].vTexcoord = _float2(0.f, 1.f);
#pragma endregion

#pragma region 아래쪽 추가
	pVertices[16].vPosition = _float3(0.375f, -0.5f, 0.375f);
	pVertices[16].vTexcoord = _float2(1.f, 0.f);

	pVertices[17].vPosition = _float3(0.375f, -0.5f, -0.375f);
	pVertices[17].vTexcoord = _float2(0.f, 1.f);
#pragma endregion

#pragma region 텍스쳐 때문에 추가
	pVertices[18].vPosition = _float3(-0.5f, 0.25f, 0.375f);		//7
	pVertices[18].vTexcoord = _float2(1.f, 0.f);

	pVertices[19].vPosition = _float3(-0.5f, -0.5f, 0.375f);		//12
	pVertices[19].vTexcoord = _float2(1.f, 1.f);

	pVertices[20].vPosition = _float3(-0.5f, 0.25f, -0.375f);		//0
	pVertices[20].vTexcoord = _float2(0.f, 0.f);

	pVertices[21].vPosition = _float3(-0.5f, -0.5f, -0.375f);		//5
	pVertices[21].vTexcoord = _float2(0.f, 1.f);

	pVertices[22].vPosition = _float3(-0.5f, 0.25f, -0.375f);		//0
	pVertices[22].vTexcoord = _float2(0.f, 0.5f);

	pVertices[23].vPosition = _float3(0.375f, 0.25f, -0.5f);		//6
	pVertices[23].vTexcoord = _float2(1.f, 0.5f);

	pVertices[24].vPosition = _float3(-0.5f, 0.25f, 0.375f);		//7
	pVertices[24].vTexcoord = _float2(1.f, 0.5f);

	pVertices[25].vPosition = _float3(0.375f, 0.25f, 0.5f);			//13
	pVertices[25].vTexcoord = _float2(0.f, 0.5f);

	pVertices[26].vPosition = _float3(0.375f, 0.5f, -0.5f);			//1
	pVertices[26].vTexcoord = _float2(0.f, 1.f);

	pVertices[27].vPosition = _float3(0.375f, 0.5f, 0.5f);			//8
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
	Set_IndexBuffer(pIndices, 0, 1, 2, 3, 4);
	pIndices[18] = 22, pIndices[19] = 1; pIndices[20] = 23;
	Set_IndexBuffer(pIndices, 21, 20, 6, 4, 21);
#pragma endregion
	
#pragma region 뒤
	Set_IndexBuffer(pIndices, 6, 9, 8, 11, 10);
	pIndices[27] = 8, pIndices[28] = 24; pIndices[29] = 25;
	Set_IndexBuffer(pIndices, 30, 13, 18, 19, 11);
#pragma endregion

#pragma region 왼쪽
	Set_IndexBuffer(pIndices, 36, 7, 0, 5, 12);
#pragma endregion

#pragma region 오른쪽
	Set_IndexBuffer(pIndices, 60, 2, 9, 10, 3);
#pragma endregion

#pragma region 위쪽
	Set_IndexBuffer(pIndices, 12, 26, 27, 9, 2);
	pIndices[42] = 7, pIndices[43] = 8; pIndices[44] = 14;
	Set_IndexBuffer(pIndices, 54, 14, 15, 20, 18);
	pIndices[51] = 0, pIndices[52] = 15; pIndices[53] = 1;
#pragma endregion

#pragma region 아래쪽
	Set_IndexBuffer(pIndices, 66, 3, 10, 11, 4);
	pIndices[72] = 4, pIndices[73] = 17; pIndices[74] = 5;
	Set_IndexBuffer(pIndices, 75, 17, 16, 12, 5);
	pIndices[81] = 16, pIndices[82] = 11; pIndices[83] = 12;
#pragma endregion



	m_pIB->Unlock();
#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_Computer::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_Computer::Render(_uint Surface)
{
	_uint iNumber{ 0 };

	switch (Surface)
	{
	case FRONT:
		iNumber = 0;
		break;

	case BACK:
		iNumber = 18;
		break;

	case MONITOR:
		iNumber = 60;
		break;

	default:
		return E_FAIL;
		break;
	}

	return m_pGraphic_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iNumVertices, iNumber, m_iNumPritimive);
}

void CVIBuffer_Computer::Set_IndexBuffer(_ushort* pIndices, _uint StartIndex, _uint LT, _uint RT, _uint RB, _uint LB)
{
	pIndices[StartIndex++] = LT;
	pIndices[StartIndex++] = RT;
	pIndices[StartIndex++] = RB;

	pIndices[StartIndex++] = LT;
	pIndices[StartIndex++] = RB;
	pIndices[StartIndex++] = LB;
}

CVIBuffer_Computer* CVIBuffer_Computer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_Computer* pInstance = new CVIBuffer_Computer(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Computer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Computer::Clone(void* pArg)
{
	CVIBuffer_Computer* pInstance = new CVIBuffer_Computer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CVIBuffer_Computer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Computer::Free()
{
	__super::Free();


}
