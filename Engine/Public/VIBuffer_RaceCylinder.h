#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_RaceCylinder final : public CVIBuffer
{
public:
	enum Surface { COMMON, MIDDLE, SIDE };

private:
	CVIBuffer_RaceCylinder(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_RaceCylinder(const CVIBuffer_RaceCylinder& Prototype);
	virtual ~CVIBuffer_RaceCylinder() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render(_uint Surface) override;

private:
	void Set_IndexBuffer(_ushort* pIndices, _uint StartIndex, _uint LT, _uint RT, _uint LB, _uint RB);
	VECTOR ComputeNormalVector(VTXPOSNORTEX* pVertices, _uint vertex1, _uint vertex2, _uint vertex3);

public:
	static CVIBuffer_RaceCylinder* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END