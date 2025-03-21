#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_RaceTerrain final : public CVIBuffer
{
private:
	CVIBuffer_RaceTerrain(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_RaceTerrain(const CVIBuffer_RaceTerrain& Prototype);
	virtual ~CVIBuffer_RaceTerrain() = default;

public:
	virtual HRESULT Initialize_Prototype(_uint iTileNumVerticesX, _uint iTileNumVerticesZ);
	virtual HRESULT Initialize(void* pArg) override;

private:
	_uint				m_iNumVerticesX = {};
	_uint				m_iNumVerticesZ = {};
	_float3* m_pVertexPos = {};

private:
	void Set_IndexBuffer(_ushort* pIndices, _uint StartIndex, _uint LT, _uint RT, _uint LB, _uint RB);
	VECTOR ComputeNormalVector(VTXPOSNORTEX* pVertices, _uint vertex1, _uint vertex2, _uint vertex3);

public:
	static CVIBuffer_RaceTerrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iTileNumVerticesX, _uint iTileNumVerticesZ);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END