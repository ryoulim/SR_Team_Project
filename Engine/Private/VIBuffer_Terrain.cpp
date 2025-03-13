#include "VIBuffer_Terrain.h"

CVIBuffer_Terrain::CVIBuffer_Terrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer { pGraphic_Device }
{
}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain& Prototype)
	: CVIBuffer(Prototype)
	, m_iNumVerticesX { Prototype.m_iNumVerticesX }
	, m_iNumVerticesZ { Prototype.m_iNumVerticesZ }
	, m_pVertexPos{ Prototype.m_pVertexPos}
{
	m_iCloneCount++;
}

HRESULT CVIBuffer_Terrain::Initialize_Prototype(_uint iNumVerticesX, _uint iNumVerticesZ, const _tchar* szHeightImagePath)
{
	m_iNumVerticesX = iNumVerticesX;
	m_iNumVerticesZ = iNumVerticesZ;
	m_iNumVertices = iNumVerticesX * iNumVerticesZ;
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

	// 높이맵 적용

	HANDLE hFile = CreateFile(szHeightImagePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX("높이맵 텍스처 개방 실패");
	}

	_ulong	dwByte(0);

	BITMAPFILEHEADER fH{};
	BITMAPINFOHEADER iH{};
	BOOL dwResult = ReadFile(hFile, &fH, sizeof(BITMAPFILEHEADER), &dwByte, NULL);
	dwResult = ReadFile(hFile, &iH, sizeof(BITMAPINFOHEADER), &dwByte, NULL);

	_ulong* pPixel = new _ulong[iH.biWidth * iH.biHeight];

	dwResult = ReadFile(hFile, pPixel, sizeof(_ulong) * iH.biWidth * iH.biHeight, &dwByte, NULL);

	CloseHandle(hFile);

	_uint	iIndex(0);

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	m_pVertexPos = new _float3[m_iNumVertices];

	for (_ulong i = 0; i < m_iNumVerticesZ; ++i)
	{
		for (_ulong j = 0; j < m_iNumVerticesX; ++j)
		{
			 iIndex = i * m_iNumVerticesX + j;

			pVertices[iIndex].vPosition =
				_float3(_float(j),
					_float(pPixel[iIndex] & 0x000000ff) / 20.f,
					_float(i));

			pVertices[iIndex].vNormal = { 0.f, 0.f, 0.f };

			pVertices[iIndex].vTexcoord = _float2((_float(j) / (m_iNumVerticesX - 1)) * m_iNumVerticesX,
				(_float(i) / (m_iNumVerticesZ - 1)) * m_iNumVerticesZ);

			m_pVertexPos[iIndex] = pVertices[iIndex].vPosition;
		}
	}

	Safe_Delete_Array(pPixel);

#pragma endregion

#pragma region INDEX_BUFFER
	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	iIndex = 0;

	_uint BufferIndex{};
	_uint* pIndices = { nullptr };
	_float3		vDst, vSrc, vNormal;

	m_pIB->Lock(0, 0, reinterpret_cast<void**>(&pIndices), 0);
	
	for (_ulong i = 0; i < m_iNumVerticesZ - 1; ++i)
	{
		for (_ulong j = 0; j < m_iNumVerticesX - 1; ++j)
		{
			iIndex = i * m_iNumVerticesX + j;

			// 오른쪽 위
			pIndices[BufferIndex++] = iIndex + m_iNumVerticesX;
			pIndices[BufferIndex++] = iIndex + m_iNumVerticesX + 1;
			pIndices[BufferIndex++] = iIndex + 1;

			vDst = pVertices[pIndices[BufferIndex - 2]].vPosition - pVertices[pIndices[BufferIndex - 3]].vPosition;
			vSrc = pVertices[pIndices[BufferIndex - 1]].vPosition - pVertices[pIndices[BufferIndex - 2]].vPosition;
			vNormal = vDst.Cross(vSrc);

			pVertices[pIndices[BufferIndex - 3]].vNormal += vNormal;
			pVertices[pIndices[BufferIndex - 2]].vNormal += vNormal;
			pVertices[pIndices[BufferIndex - 1]].vNormal += vNormal;

			// 왼쪽 아래
			pIndices[BufferIndex++] = iIndex + m_iNumVerticesX;
			pIndices[BufferIndex++] = iIndex + 1;
			pIndices[BufferIndex++] = iIndex;

			vDst = pVertices[pIndices[BufferIndex - 2]].vPosition - pVertices[pIndices[BufferIndex - 3]].vPosition;
			vSrc = pVertices[pIndices[BufferIndex - 1]].vPosition - pVertices[pIndices[BufferIndex - 2]].vPosition;
			vNormal = vDst.Cross(vSrc);

			pVertices[pIndices[BufferIndex - 3]].vNormal += vNormal;
			pVertices[pIndices[BufferIndex - 2]].vNormal += vNormal;
			pVertices[pIndices[BufferIndex - 1]].vNormal += vNormal;
		}
	}

#pragma endregion

	m_pVB->Unlock();
	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_Terrain::Initialize(void* pArg)
{
	return S_OK;
}

CVIBuffer_Terrain* CVIBuffer_Terrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iNumVerticesX, _uint iNumVerticesZ, const _tchar* szHeightImagePath)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(iNumVerticesX, iNumVerticesZ, szHeightImagePath)))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Terrain::Clone(void* pArg)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Terrain::Free()
{
	__super::Free();
	if (m_iCloneCount)
		m_iCloneCount--;
	else
		Safe_Delete_Array(m_pVertexPos);
}
