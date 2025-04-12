#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_RaceBoss final : public CVIBuffer
{
public:
	enum Surface { COMMON, MUZZLE1, MUZZLE2, MUZZLE3, MUZZLE4, MIDDLE, OUTLINE, TRIANGLE };

private:
	CVIBuffer_RaceBoss(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_RaceBoss(const CVIBuffer_RaceBoss& Prototype);
	virtual ~CVIBuffer_RaceBoss() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render(_uint Surface) override;

private:
	void Set_IndexBuffer(_ushort* pIndices, _uint StartIndex, _uint LT, _uint RT, _uint LB, _uint RB);
	VECTOR ComputeNormalVector(VTXPOSNORTEX* pVertices, _uint vertex1, _uint vertex2, _uint vertex3);

public:
	static CVIBuffer_RaceBoss* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END