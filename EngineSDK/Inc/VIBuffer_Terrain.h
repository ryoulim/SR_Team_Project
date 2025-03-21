#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Terrain final : public CVIBuffer
{
private:
	CVIBuffer_Terrain(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Terrain(const CVIBuffer_Terrain& Prototype);
	virtual ~CVIBuffer_Terrain() = default;

public:
	_uint Get_NumVerticesX() {
		return m_iNumVerticesX;
	}

	_uint Get_NumVerticesZ() {
		return m_iNumVerticesZ;
	}

public:
	virtual HRESULT Initialize_Prototype(_uint iNumVerticesX, _uint iNumVerticesZ, const _tchar* szHeightImagePath);
	virtual HRESULT Initialize_Prototype(_uint iNumVerticesX, _uint iNumVerticesZ);
	virtual HRESULT Initialize(void* pArg) override;
	// 이 함수의 반환값 콘스트 캐스트나 mutable로 값 바꾸면 대머리
	const _float3*	Get_VertexPos() { return m_pVertexPos; }

private:
	_uint				m_iNumVerticesX = {};
	_uint				m_iNumVerticesZ = {};
	_float3*			m_pVertexPos = {};
	_uint				m_iCloneCount{};

public:
	static CVIBuffer_Terrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iNumVerticesX, _uint iNumVerticesZ, const _tchar* szHeightImagePath);
	static CVIBuffer_Terrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iNumVerticesX, _uint iNumVerticesZ);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END