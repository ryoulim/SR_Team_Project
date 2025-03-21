#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_CubeEx final : public CVIBuffer
{
private:
	CVIBuffer_CubeEx(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_CubeEx(const CVIBuffer_CubeEx& Prototype);
	virtual ~CVIBuffer_CubeEx() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	static CVIBuffer_CubeEx* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END