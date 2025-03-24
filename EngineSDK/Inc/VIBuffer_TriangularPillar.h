#pragma once

#include "VIBuffer.h"

class ENGINE_DLL CVIBuffer_TriangularPillar final : public CVIBuffer
{
private:
	CVIBuffer_TriangularPillar(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_TriangularPillar(CVIBuffer_TriangularPillar& Prototype);
	virtual ~CVIBuffer_TriangularPillar() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

private:
	void Set_IndexBuffer(_ushort* pIndices, _uint StartIndex, _uint LT, _uint RT, _uint RB, _uint LB);

public:
	static CVIBuffer_TriangularPillar* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

