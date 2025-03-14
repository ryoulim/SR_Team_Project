#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Signboard final : public CVIBuffer
{
private:
	CVIBuffer_Signboard(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Signboard(const CVIBuffer_Signboard& Prototype);
	virtual ~CVIBuffer_Signboard() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

private:
	void Set_IndexBuffer(_ushort* pIndices, _uint StartIndex, _uint LT, _uint RT, _uint LB, _uint RB);
	void Set_IndexBuffer2(_ushort* pIndices, _uint StartIndex, _uint LB, _uint LT, _uint MB, _uint RT, _uint RB);

public:
	static CVIBuffer_Signboard* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END