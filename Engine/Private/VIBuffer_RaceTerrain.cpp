#include "VIBuffer_RaceTerrain.h"

_uint CVIBuffer_RaceTerrain::m_iTexChangeNumber = 0;

CVIBuffer_RaceTerrain::CVIBuffer_RaceTerrain(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CVIBuffer{pGraphic_Device}
{
}

CVIBuffer_RaceTerrain::CVIBuffer_RaceTerrain(const CVIBuffer_RaceTerrain& Prototype)
    : CVIBuffer(Prototype)
	, m_iNumVerticesX{ Prototype.m_iNumVerticesX }
	, m_iNumVerticesZ{ Prototype.m_iNumVerticesZ }
	, m_iRoadWidth{Prototype.m_iRoadWidth}
{
}

HRESULT CVIBuffer_RaceTerrain::Initialize_Prototype(_uint iTileNumVerticesX, _uint iTileNumVerticesZ)
{
	m_iRoadWidth = 2;

	m_iNumVerticesX = iTileNumVerticesX;
	m_iNumVerticesZ = iTileNumVerticesZ;
	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;

	m_iVertexStride = sizeof(VTXPOSNORTEX);
	m_iFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
	m_iNumPritimive = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2;

	m_iNumIndices = m_iNumPritimive * 3;
	m_iIndexStride = 4;
	m_eIndexFormat = D3DFMT_INDEX32;

#pragma region VERTEX_BUFFER
	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXPOSNORTEX* pVertices = { nullptr };

	m_pVB->Lock(0, /*m_iNumVertices * m_iVertexStride*/0, reinterpret_cast<void**>(&pVertices), 0);
	
#pragma region ¹Ù´Ú Å¸ÀÏ
	for (_uint i = 0; i < m_iNumVerticesZ; i++)
	{
		for (_uint j = 0; j < m_iNumVerticesX; j++)
		{
			_uint			iIndex = i * m_iNumVerticesX + j;

			pVertices[iIndex].vPosition = _float3((_float)j * 100.f, 0.f, (_float)i * 100.f);
			pVertices[iIndex].vTexcoord = _float2(m_iNumVerticesX * j / (m_iNumVerticesX - 1.f), m_iNumVerticesZ * i / (m_iNumVerticesZ - 1.f));
			pVertices[iIndex].vNormal = { 0.f, 0.f, 0.f };
		}
	}
#pragma endregion

	m_pVB->Unlock();

#pragma endregion

#pragma region INDEX_BUFFER
	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	_uint* pIndices = { nullptr };

	m_pIB->Lock(0, 0, reinterpret_cast<void**>(&pIndices), 0);

	_uint		iNumIndices = { 0 };

	/*for (_uint i = 0; i < m_iNumVerticesZ - 1; i++)
	{
		for (_uint j = 0; j < m_iNumVerticesX - 1; j++)
		{
			_uint			iIndex = i * m_iNumVerticesX + j;

			_uint			iIndices[4] = {
				iIndex + m_iNumVerticesX,
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[1];
			pIndices[iNumIndices++] = iIndices[2];

			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[2];
			pIndices[iNumIndices++] = iIndices[3];
		}
	}*/

	for (_uint i = 0; i < m_iNumVerticesZ - 1; i++)
	{
		for (_uint j = 0; j < ((m_iNumVerticesX - m_iRoadWidth) / 2) - 1; j++)
		{
			_uint			iIndex = i * m_iNumVerticesX + j;

			_uint			iIndices[4] = {
				iIndex + m_iNumVerticesX,
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[1];
			pIndices[iNumIndices++] = iIndices[2];

			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[2];
			pIndices[iNumIndices++] = iIndices[3];
		}
	}

	for (_uint i = 0; i < m_iNumVerticesZ - 1; i++)
	{
		for (_uint j = m_iNumVerticesX - ((m_iNumVerticesX - m_iRoadWidth) / 2); j < m_iNumVerticesX - 1; j++)
		{
			_uint			iIndex = i * m_iNumVerticesX + j;

			_uint			iIndices[4] = {
				iIndex + m_iNumVerticesX,
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[1];
			pIndices[iNumIndices++] = iIndices[2];

			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[2];
			pIndices[iNumIndices++] = iIndices[3];
		}
	}

	m_iTexChangeNumber = iNumIndices;

	for (_uint i = 0; i < m_iNumVerticesZ - 1; i++)
	{
		for (_uint j = ((m_iNumVerticesX - m_iRoadWidth) / 2) - 1; j < m_iNumVerticesX - ((m_iNumVerticesX - m_iRoadWidth) / 2); j++)
		{
			_uint			iIndex = i * m_iNumVerticesX + j;

			_uint			iIndices[4] = {
				iIndex + m_iNumVerticesX,
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[1];
			pIndices[iNumIndices++] = iIndices[2];

			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[2];
			pIndices[iNumIndices++] = iIndices[3];
		}
	}

	m_pIB->Unlock();
#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_RaceTerrain::Initialize(void* pArg)
{
    return S_OK;
}

HRESULT CVIBuffer_RaceTerrain::Render(_uint Surface)
{
	Surface *= m_iTexChangeNumber;
	return m_pGraphic_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iNumVertices, Surface, m_iNumPritimive);
}

void CVIBuffer_RaceTerrain::Set_IndexBuffer(_ushort* pIndices, _uint StartIndex, _uint LT, _uint RT, _uint RB, _uint LB)
{
	pIndices[StartIndex++] = LT;
	pIndices[StartIndex++] = RT;
	pIndices[StartIndex++] = RB;

	pIndices[StartIndex++] = LT;
	pIndices[StartIndex++] = RB;
	pIndices[StartIndex++] = LB;
}

VECTOR CVIBuffer_RaceTerrain::ComputeNormalVector(VTXPOSNORTEX* pVertices, _uint vertex1, _uint vertex2, _uint vertex3)
{
	return __super::ComputeNormal(&(pVertices[vertex1].vPosition), &(pVertices[vertex2].vPosition), &(pVertices[vertex3].vPosition));
}

CVIBuffer_RaceTerrain* CVIBuffer_RaceTerrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iTileNumVerticesX, _uint iTileNumVerticesZ)
{
	CVIBuffer_RaceTerrain* pInstance = new CVIBuffer_RaceTerrain(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(iTileNumVerticesX, iTileNumVerticesZ)))
	{
		MSG_BOX("Failed to Created : CVIBuffer_RaceTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_RaceTerrain::Clone(void* pArg)
{
	CVIBuffer_RaceTerrain* pInstance = new CVIBuffer_RaceTerrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CVIBuffer_RaceTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_RaceTerrain::Free()
{
	__super::Free();
}
