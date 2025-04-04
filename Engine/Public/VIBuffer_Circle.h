#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Circle final : public CVIBuffer
{
private:
	CVIBuffer_Circle(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Circle(const CVIBuffer& Prototype);
	virtual ~CVIBuffer_Circle() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render(_uint Surface = 0) override;

public:
	static CVIBuffer_Circle* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END