#pragma once

/* V : Vertex */
/* I : Index */
/* Buffer : �޸𸮰��� */
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract : public CComponent
{
protected:
	CVIBuffer(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer(const CVIBuffer& Prototype);
	virtual ~CVIBuffer() = default;

public:
	LPDIRECT3DVERTEXBUFFER9 Get_VB() {
		return m_pVB;
	}

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render(_uint Surface = 0);

public:
	HRESULT Bind_Buffers();
	LPDIRECT3DVERTEXBUFFER9 DeepCopyVertexBuffer(LPDIRECT3DVERTEXBUFFER9 pOriginVB);
	void Set_VertexBuffer(LPDIRECT3DVERTEXBUFFER9 pNewVB);

protected:
	/*  ���� �迭�� ������ �ִ� �� ��ü. */
	LPDIRECT3DVERTEXBUFFER9				m_pVB = { nullptr };
	LPDIRECT3DINDEXBUFFER9				m_pIB = { nullptr };

	_uint								m_iNumVertices = { };
	_uint								m_iVertexStride = { };
	_uint								m_iFVF = {};
	_uint								m_iNumPritimive = {};

	_uint								m_iNumIndices = {};
	_uint								m_iIndexStride = {};
	D3DFORMAT							m_eIndexFormat = {};

protected:
	HRESULT Create_VertexBuffer();
	HRESULT Create_IndexBuffer();
	VECTOR ComputeNormal(_float3* p0, _float3* p1, _float3* p2);
	void Test();

public:
	virtual CComponent* Clone(void* pArg) PURE;
	virtual void Free() override;
};

END