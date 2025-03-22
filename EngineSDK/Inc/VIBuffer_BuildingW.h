#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_BuildingW final : public CVIBuffer
{
private:
	CVIBuffer_BuildingW(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_BuildingW(const CVIBuffer_BuildingW& Prototype);
	virtual ~CVIBuffer_BuildingW() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

private:
	void Set_IndexBuffer(_ushort* pIndices, _uint StartIndex, _uint LT, _uint RT, _uint LB, _uint RB);
	VECTOR ComputeNormalVector(VTXPOSNORTEX* pVertices, _uint vertex1, _uint vertex2, _uint vertex3);

public:
	static CVIBuffer_BuildingW* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END