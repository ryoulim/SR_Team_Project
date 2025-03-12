// 내 클래스 이름 : VIBuffer_Trapezoid
// 부모 클래스 이름 : CVIBuffer

#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Cabinet final : public CVIBuffer
{
private:
	CVIBuffer_Cabinet(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Cabinet(const CVIBuffer_Cabinet& Prototype);
	virtual ~CVIBuffer_Cabinet() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

private:
	void Set_IndexBuffer(_ushort* pIndices, _uint StartIndex, _uint LT, _uint RT, _uint RB, _uint LB);

public:
	static CVIBuffer_Cabinet* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free();
};

END

