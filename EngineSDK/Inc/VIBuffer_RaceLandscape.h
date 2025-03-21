#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_RaceLandscape final : public CVIBuffer
{
private:
	CVIBuffer_RaceLandscape(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_RaceLandscape(const CVIBuffer_RaceLandscape& Prototype);
	virtual ~CVIBuffer_RaceLandscape() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

private:
	_uint m_iNumber = {};

private:
	void Set_IndexBuffer(_ushort* pIndices, _uint StartIndex, _uint LT, _uint RT, _uint LB, _uint RB);
	VECTOR ComputeNormalVector(VTXPOSNORTEX* pVertices, _uint vertex1, _uint vertex2, _uint vertex3);

public:
	static CVIBuffer_RaceLandscape* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END