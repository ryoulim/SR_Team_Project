#include "VIBuffer_TriangularPillar.h"

CVIBuffer_TriangularPillar::CVIBuffer_TriangularPillar(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer{ pGraphic_Device }
{
}

CVIBuffer_TriangularPillar::CVIBuffer_TriangularPillar(CVIBuffer_TriangularPillar& Prototype)
	: CVIBuffer(Prototype)
{
}

HRESULT CVIBuffer_TriangularPillar::Initialize_Prototype()
{
	m_iNumVertices = 10;					// ���ؽ� ����
	m_iVertexStride = sizeof(VTXPOSNORTEX);	// ���ؽ� ũ��
	m_iFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;		// ���ؽ� Ÿ��
	m_iNumPritimive = 10;					// �ﰢ�� ����

	m_iIndexStride = 2;						// �ε��� ũ��
	m_iNumIndices = 24;						// �ε��� ����
	m_eIndexFormat = D3DFMT_INDEX16;		// �ε��� ����

#pragma region VERTEX_BUFFER
	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXPOSNORTEX* pVertices = { nullptr };

	m_pVB->Lock(0, /*m_iNumVertices * m_iVertexStride*/0, reinterpret_cast<void**>(&pVertices), 0);

	pVertices[0].vPosition = { -0.5f, 0.5f, -0.5f };
	pVertices[0].vTexcoord = { 1.f, 0.f };

	pVertices[1].vPosition = { 0.5f, 0.5f, -0.5f };
	pVertices[1].vTexcoord = { 2.f, 0.f };

	pVertices[2].vPosition = { 0.5f, -0.5f, -0.5f };
	pVertices[2].vTexcoord = { 2.f, 1.f };

	pVertices[3].vPosition = { -0.5f, -0.5f, -0.5f };
	pVertices[3].vTexcoord = { 1.f, 1.f };

	pVertices[4].vPosition = { -0.5f, 0.5f, 0.5f };
	pVertices[4].vTexcoord = { 0.f, 0.f };

	pVertices[5].vPosition = { -0.5f, -0.5f, 0.5f };
	pVertices[5].vTexcoord = { 0.f, 1.f };

	/* �ؽ��� �� (��)*/
	pVertices[6].vPosition = { -0.5f, 0.5f, -0.5f };
	pVertices[6].vTexcoord = { 0.f, 1.f };

	pVertices[7].vPosition = { 0.5f, 0.5f, -0.5f };
	pVertices[7].vTexcoord = { 1.f, 1.f };
	/* �ؽ��� �� (�Ʒ�) */
	pVertices[8].vPosition = { -0.5f, -0.5f, -0.5f };
	pVertices[8].vTexcoord = { 0.f, 0.f };

	pVertices[9].vPosition = { 0.5f, -0.5f, -0.5f };
	pVertices[9].vTexcoord = { 1.f, 0.f };

	m_pVB->Unlock();

#pragma endregion

#pragma region Index Buffer
	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;


	_ushort* pIndices = { nullptr };

	m_pIB->Lock(0, 0, reinterpret_cast<void**>(&pIndices), 0);
	
	/* �� */
	Set_IndexBuffer(pIndices, 0, 0, 1, 2, 3);

	/* ���� */
	Set_IndexBuffer(pIndices, 6, 4, 0, 3, 5);

	/* ������ */
	Set_IndexBuffer(pIndices, 12, 1, 4, 5, 2);

	/* �� */
	pIndices[18] = 4; pIndices[19] = 7; pIndices[20] = 6;
	/* �Ʒ� */
	pIndices[21] = 9; pIndices[22] = 5; pIndices[23] = 8;

	m_pIB->Unlock();


	return S_OK;
}

HRESULT CVIBuffer_TriangularPillar::Initialize(void* pArg)
{
	return S_OK;
}

void CVIBuffer_TriangularPillar::Set_IndexBuffer(_ushort* pIndices, _uint StartIndex, _uint LT, _uint RT, _uint RB, _uint LB)
{
	pIndices[StartIndex++] = LT;
	pIndices[StartIndex++] = RT;
	pIndices[StartIndex++] = RB;

	pIndices[StartIndex++] = LT;
	pIndices[StartIndex++] = RB;
	pIndices[StartIndex++] = LB;
}

CVIBuffer_TriangularPillar* CVIBuffer_TriangularPillar::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_TriangularPillar* pInstance = new CVIBuffer_TriangularPillar(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_TriangularPillar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_TriangularPillar::Clone(void* pArg)
{
	CVIBuffer_TriangularPillar* pInstance = new CVIBuffer_TriangularPillar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CVIBuffer_TriangularPillar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_TriangularPillar::Free()
{
	__super::Free();
}
