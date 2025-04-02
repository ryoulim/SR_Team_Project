#include "VIBuffer_RaceCylinder.h"

CVIBuffer_RaceCylinder::CVIBuffer_RaceCylinder(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CVIBuffer{pGraphic_Device}
{
}

CVIBuffer_RaceCylinder::CVIBuffer_RaceCylinder(const CVIBuffer_RaceCylinder& Prototype)
    :CVIBuffer(Prototype)
{
}

HRESULT CVIBuffer_RaceCylinder::Initialize_Prototype()
{
	m_iNumVertices = 28;					// ¹öÅØ½º °¹¼ö
	m_iVertexStride = sizeof(VTXPOSNORTEX);	// ¹öÅØ½º Å©±â
	m_iFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;		// ¹öÅØ½º Å¸ÀÔ
	m_iNumPritimive = 32;	//20 + 12		// »ï°¢Çü °¹¼ö

	m_iIndexStride = 2;						// ÀÎµ¦½º Å©±â
	m_iNumIndices = 96;						// ÀÎµ¦½º °¹¼ö
	m_eIndexFormat = D3DFMT_INDEX16;		// ÀÎµ¦½º Æ÷¸Ë

#pragma region VERTEX_BUFFER
	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXPOSNORTEX* pVertices = { nullptr };

	m_pVB->Lock(0, /*m_iNumVertices * m_iVertexStride*/0, reinterpret_cast<void**>(&pVertices), 0);
	
	for (_uint i = 0; i < 10; i++)
	{
		pVertices[i].vPosition = _float3(cosf(D3DXToRadian(i * 36.f)), sinf(D3DXToRadian(i * 36.f)), -0.5f);
		pVertices[i + 10].vPosition = _float3(cosf(D3DXToRadian(i * 36.f)), sinf(D3DXToRadian(i * 36.f)), 0.5f);
	}

	pVertices[0].vTexcoord = _float2(2.f, 0.f);
	pVertices[1].vTexcoord = _float2(3.f, 0.f);
	pVertices[2].vTexcoord = _float2(4.f, 0.f);
	pVertices[3].vTexcoord = _float2(5.f, 0.f);
	pVertices[4].vTexcoord = _float2(6.f, 0.f);
	pVertices[5].vTexcoord = _float2(7.f, 0.f);
	pVertices[6].vTexcoord = _float2(8.f, 0.f);
	pVertices[7].vTexcoord = _float2(9.f, 0.f);
	pVertices[8].vTexcoord = _float2(0.f, 0.f);
	pVertices[9].vTexcoord = _float2(1.f, 0.f);
	pVertices[10].vTexcoord = _float2(2.f, 15.f);
	pVertices[11].vTexcoord = _float2(3.f, 15.f);
	pVertices[12].vTexcoord = _float2(4.f, 15.f);
	pVertices[13].vTexcoord = _float2(5.f, 15.f);
	pVertices[14].vTexcoord = _float2(6.f, 15.f);
	pVertices[15].vTexcoord = _float2(7.f, 15.f);
	pVertices[16].vTexcoord = _float2(8.f, 15.f);
	pVertices[17].vTexcoord = _float2(9.f, 15.f);
	pVertices[18].vTexcoord = _float2(0.f, 15.f);
	pVertices[19].vTexcoord = _float2(1.f, 15.f);

	pVertices[20].vPosition = _float3(-100.f, 1.f, -0.5f);
	pVertices[20].vTexcoord = _float2(0.f, 0.f);
	
	pVertices[21].vPosition = _float3(-1.f, 1.f, -0.5f);
	pVertices[21].vTexcoord = _float2(49.f, 0.f);//

	pVertices[22].vPosition = _float3(1.f, 1.f, -0.5f);
	pVertices[22].vTexcoord = _float2(50.f, 0.f);//

	pVertices[23].vPosition = _float3(100.f, 1.f, -0.5f);
	pVertices[23].vTexcoord = _float2(0.f, 0.f);

	pVertices[24].vPosition = _float3(100.f, -1.f, -0.5f);
	pVertices[24].vTexcoord = _float2(0.f, 1.f);

	pVertices[25].vPosition = _float3(1.f, -1.f, -0.5f);
	pVertices[25].vTexcoord = _float2(50.f, 1.f);//

	pVertices[26].vPosition = _float3(-1.f, -1.f, -0.5f);
	pVertices[26].vTexcoord = _float2(49.f, 1.f);//

	pVertices[27].vPosition = _float3(-100.f, -1.f, -0.5f);
	pVertices[27].vTexcoord = _float2(0.f, 1.f);
#pragma region ¹ý¼±
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

	for (_uint i = 0; i < 9; i++)
	{
		Set_IndexBuffer(pIndices, i * 6, i + 1, i, i + 10, i + 11);
	}

	Set_IndexBuffer(pIndices, 54, 0, 9, 19, 10);

	pIndices[60] = 4; pIndices[61] = 21; pIndices[62] = 3;
	pIndices[63] = 2; pIndices[64] = 22; pIndices[65] = 1;
	pIndices[66] = 1; pIndices[67] = 22; pIndices[68] = 0;
	pIndices[69] = 9; pIndices[70] = 0; pIndices[71] = 25;
	pIndices[72] = 8; pIndices[73] = 9; pIndices[74] = 25;
	pIndices[75] = 26; pIndices[76] = 6; pIndices[77] = 7;
	pIndices[78] = 26; pIndices[79] = 5; pIndices[80] = 6;
	pIndices[81] = 5; pIndices[82] = 21; pIndices[83] = 4;

	Set_IndexBuffer(pIndices, 84, 20, 21, 26, 27);
	Set_IndexBuffer(pIndices, 90, 22, 23, 24, 25);

	m_pIB->Unlock();
#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_RaceCylinder::Initialize(void* pArg)
{
    return S_OK;
}

HRESULT CVIBuffer_RaceCylinder::Render(_uint Surface)
{
	_uint iNumber{ 0 };

	switch (Surface)
	{
	case COMMON:
		iNumber = 0;
		break;

	case MIDDLE:
		iNumber = 60;
		break;

	case SIDE:
		iNumber = 84;
		break;

	default:
		return E_FAIL;
		break;
	}

	return m_pGraphic_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iNumVertices, iNumber, m_iNumPritimive);
}

void CVIBuffer_RaceCylinder::Set_IndexBuffer(_ushort* pIndices, _uint StartIndex, _uint LT, _uint RT, _uint RB, _uint LB)
{
	pIndices[StartIndex++] = LT;
	pIndices[StartIndex++] = RT;
	pIndices[StartIndex++] = RB;

	pIndices[StartIndex++] = LT;
	pIndices[StartIndex++] = RB;
	pIndices[StartIndex++] = LB;
}

VECTOR CVIBuffer_RaceCylinder::ComputeNormalVector(VTXPOSNORTEX* pVertices, _uint vertex1, _uint vertex2, _uint vertex3)
{
	return __super::ComputeNormal(&(pVertices[vertex1].vPosition), &(pVertices[vertex2].vPosition), &(pVertices[vertex3].vPosition));
}

CVIBuffer_RaceCylinder* CVIBuffer_RaceCylinder::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_RaceCylinder* pInstance = new CVIBuffer_RaceCylinder(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_RaceCylinder");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_RaceCylinder::Clone(void* pArg)
{
	CVIBuffer_RaceCylinder* pInstance = new CVIBuffer_RaceCylinder(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CVIBuffer_RaceCylinder");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_RaceCylinder::Free()
{
	__super::Free();
}
