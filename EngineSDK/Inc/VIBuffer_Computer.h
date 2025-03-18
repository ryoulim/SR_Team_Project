#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Computer final : public CVIBuffer
{
public:
	enum CABINET_SURFACE { FRONT, BACK, MONITOR };

private:
	CVIBuffer_Computer(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Computer(const CVIBuffer& Prototype);
	virtual ~CVIBuffer_Computer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render(_uint Surface) override;

private:
	void Set_IndexBuffer(_ushort* pIndices, _uint StartIndex, _uint LT, _uint RT, _uint LB, _uint RB);


public:
	static CVIBuffer_Computer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END