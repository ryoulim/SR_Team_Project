#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CGravity final : public CComponent
{
public:
	typedef struct tagGravityDesc
	{
		const _float3*	pTerrainVtxPos;
		_uint			iTerrainVtxNumX;
		_uint			iTerrainVtxNumZ;
		_float			fTerrainScale;
	}DESC;

private:
	CGravity(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGravity(const CGravity& Prototype);
	virtual ~CGravity() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	static void Set_TerrainInfo(const DESC& Desc);
	void Update();

private:
	_float3*				m_pPosition{nullptr};
	_float3					m_vOldPosition{};

	static const _float3*	m_pTerrainVtxPos;
	static _uint			m_iTerrainVtxNumX;
	static _uint			m_iTerrainVtxNumZ;
	static _float			m_fTerrainScale;

public:
	static CGravity* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

END