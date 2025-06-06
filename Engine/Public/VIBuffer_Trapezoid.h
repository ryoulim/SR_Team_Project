﻿// 내 클래스 이름 : VIBuffer_Trapezoid
// 부모 클래스 이름 : CVIBuffer

#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Trapezoid final : public CVIBuffer
{
public:
	enum SURFACE {COMMON,TOP};
  
private:
	CVIBuffer_Trapezoid(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Trapezoid(const CVIBuffer_Trapezoid& Prototype);
	virtual ~CVIBuffer_Trapezoid() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render(_uint Surface) override;

private:
	void Set_IndexBuffer(_ushort* pIndices, _uint StartIndex, _uint LT, _uint RT, _uint RB, _uint LB);
	VECTOR ComputeNormalVector(VTXPOSNORTEX* pVertices, _uint vertex1, _uint vertex2, _uint vertex3);

public:
	static CVIBuffer_Trapezoid* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free();
};

END

