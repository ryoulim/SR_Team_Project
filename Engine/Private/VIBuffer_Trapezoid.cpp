#include "VIBuffer_Trapezoid.h"

CVIBuffer_Trapezoid::CVIBuffer_Trapezoid(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
}

CVIBuffer_Trapezoid::CVIBuffer_Trapezoid(const CVIBuffer_Trapezoid& Prototype)
	: CVIBuffer(Prototype)
{
}

HRESULT CVIBuffer_Trapezoid::Initialize_Prototype()
{
	m_iNumVertices = 10;
	m_iVertexStride = sizeof(VTXPOSNORTEX);
	m_iFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
	m_iNumPritimive = 14;

	m_iIndexStride = 2;
	m_iNumIndices = 42;
	m_eIndexFormat = D3DFMT_INDEX16;

#pragma region VERTEX_BUFFER
	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXPOSNORTEX* pVertices = { nullptr };

	m_pVB->Lock(0, 0, reinterpret_cast<void**>(&pVertices), 0);

	for (_uint i = 0; i < m_iNumVertices; i++)
		pVertices[i].vNormal = _float3(0.f, 0.f, 0.f);

#pragma region 앞면
	pVertices[0].vPosition = _float3(-0.5f, 1.f, -0.5f);
	pVertices[0].vTexcoord = _float2(2.f, 0.f);
	
	pVertices[1].vPosition = _float3(0.5f, 0.f, -0.5f);
	pVertices[1].vTexcoord = _float2(1.f, 1.f);

	pVertices[2].vPosition = _float3(0.5f, -1.f, -0.5f);
	pVertices[2].vTexcoord = _float2(0.f, 1.f);

	pVertices[3].vPosition = _float3(-0.5f, -1.f, -0.5f);
	pVertices[3].vTexcoord = _float2(0.f, 0.f);

	pVertices[4].vPosition = _float3(-0.5f, 0.f, -0.5f);
	pVertices[4].vTexcoord = _float2(1.f, 0.f);
#pragma endregion

#pragma region 뒷면
	pVertices[5].vPosition = _float3(-0.5f, 1.f, 0.5f);
	pVertices[5].vTexcoord = _float2(2.f, 1.f);

	pVertices[6].vPosition = _float3(0.5f, 0.f, 0.5f);
	pVertices[6].vTexcoord = _float2(1.f, 0.f);

	pVertices[7].vPosition = _float3(0.5f, -1.f, 0.5f);
	pVertices[7].vTexcoord = _float2(0.f, 0.f);

	pVertices[8].vPosition = _float3(-0.5f, -1.f, 0.5f);
	pVertices[8].vTexcoord = _float2(0.f, 1.f);

	pVertices[9].vPosition = _float3(-0.5f, 0.f, 0.5f);
	pVertices[9].vTexcoord = _float2(1.f, 1.f);
#pragma endregion

#pragma endregion

#pragma region INDEX_BUFFER
	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	_ushort* pIndices = { nullptr };

	m_pIB->Lock(0, 0, reinterpret_cast<void**>(&pIndices), 0);
	
	Set_IndexBuffer(pIndices, 0, 4, 1, 2, 3); // 좌
	pVertices[4].vNormal += ComputeNormalVector(pVertices, 4, 1, 2);
	pVertices[1].vNormal += ComputeNormalVector(pVertices, 4, 1, 2);
	pVertices[2].vNormal += ComputeNormalVector(pVertices, 4, 1, 2);
	pVertices[3].vNormal += ComputeNormalVector(pVertices, 4, 2, 3);

	pIndices[6] = 0; pIndices[7] = 1; pIndices[8] = 4;
	pVertices[0].vNormal += ComputeNormalVector(pVertices, 4, 0, 1);
	pVertices[1].vNormal += ComputeNormalVector(pVertices, 4, 0, 1);
	pVertices[4].vNormal += ComputeNormalVector(pVertices, 4, 0, 1);

	Set_IndexBuffer(pIndices, 9, 6, 9, 8, 7); // 우
	pVertices[6].vNormal += ComputeNormalVector(pVertices, 6, 9, 8);
	pVertices[9].vNormal += ComputeNormalVector(pVertices, 6, 9, 8);
	pVertices[8].vNormal += ComputeNormalVector(pVertices, 6, 9, 8);
	pVertices[7].vNormal += ComputeNormalVector(pVertices, 6, 8, 7);

	pIndices[15] = 6; pIndices[16] = 5; pIndices[17] = 9;
	pVertices[6].vNormal += ComputeNormalVector(pVertices, 6, 5, 9);
	pVertices[5].vNormal += ComputeNormalVector(pVertices, 6, 5, 9);
	pVertices[9].vNormal += ComputeNormalVector(pVertices, 6, 5, 9);

	Set_IndexBuffer(pIndices, 18, 5, 0, 3, 8); // 뒤
	pVertices[5].vNormal += ComputeNormalVector(pVertices, 5, 0, 3);
	pVertices[0].vNormal += ComputeNormalVector(pVertices, 5, 0, 3);
	pVertices[3].vNormal += ComputeNormalVector(pVertices, 5, 0, 3);
	pVertices[8].vNormal += ComputeNormalVector(pVertices, 5, 3, 8);

	Set_IndexBuffer(pIndices, 24, 1, 6, 7, 2); // 앞
	pVertices[1].vNormal += ComputeNormalVector(pVertices, 1, 6, 7);
	pVertices[6].vNormal += ComputeNormalVector(pVertices, 1, 6, 7);
	pVertices[7].vNormal += ComputeNormalVector(pVertices, 1, 6, 7);
	pVertices[2].vNormal += ComputeNormalVector(pVertices, 1, 7, 2);

	Set_IndexBuffer(pIndices, 30, 2, 7, 8, 3); // 아래
	pVertices[2].vNormal += ComputeNormalVector(pVertices, 2, 7, 8);
	pVertices[7].vNormal += ComputeNormalVector(pVertices, 2, 7, 8);
	pVertices[8].vNormal += ComputeNormalVector(pVertices, 2, 7, 8);
	pVertices[3].vNormal += ComputeNormalVector(pVertices, 2, 8, 3);

	Set_IndexBuffer(pIndices, 36, 0, 5, 6, 1); // 위
	pVertices[0].vNormal += ComputeNormalVector(pVertices, 0, 5, 6);
	pVertices[5].vNormal += ComputeNormalVector(pVertices, 0, 5, 6);
	pVertices[6].vNormal += ComputeNormalVector(pVertices, 0, 5, 6);
	pVertices[1].vNormal += ComputeNormalVector(pVertices, 0, 6, 1);

	m_pVB->Unlock();
	m_pIB->Unlock();
#pragma endregion
	return S_OK;
}

HRESULT CVIBuffer_Trapezoid::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_Trapezoid::Render(_uint Surface)
{		
	Surface *= 36;
	return m_pGraphic_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iNumVertices, Surface, m_iNumPritimive);
}

void CVIBuffer_Trapezoid::Set_IndexBuffer(_ushort* pIndices, _uint StartIndex, _uint LT, _uint RT, _uint RB, _uint LB)
{
	pIndices[StartIndex++] = LT;
	pIndices[StartIndex++] = RT;
	pIndices[StartIndex++] = RB;

	pIndices[StartIndex++] = LT;
	pIndices[StartIndex++] = RB;
	pIndices[StartIndex++] = LB;
}

VECTOR CVIBuffer_Trapezoid::ComputeNormalVector(VTXPOSNORTEX* pVertices, _uint vertex1, _uint vertex2, _uint vertex3)
{
	return __super::ComputeNormal(&(pVertices[vertex1].vPosition), &(pVertices[vertex2].vPosition), &(pVertices[vertex3].vPosition));
}

CVIBuffer_Trapezoid* CVIBuffer_Trapezoid::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_Trapezoid* pInstance = new CVIBuffer_Trapezoid(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Trapezoid");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Trapezoid::Clone(void* pArg)
{
	CVIBuffer_Trapezoid* pInstance = new CVIBuffer_Trapezoid(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CVIBuffer_Trapezoid");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Trapezoid::Free()
{
	__super::Free();
}
