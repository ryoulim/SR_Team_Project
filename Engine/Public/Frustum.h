#pragma once
#include "Base.h"

BEGIN(Engine)

class CFrustum : public CBase
{
private:
	CFrustum(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CFrustum() = default;

public:
	_bool IsPointInFrustum(const _float3& Point, _float fRadius);
	void Update_Frustum();

private:
	LPDIRECT3DDEVICE9					m_pGraphic_Device = { nullptr };
	D3DXPLANE							m_FrustumPlanes[6] = {};

public:
	static CFrustum* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;

};

END