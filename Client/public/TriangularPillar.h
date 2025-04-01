#pragma once

#include "Map.h"

BEGIN(Client)

class CTriangularPillar final : public CMap
{
private:
	CTriangularPillar(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTriangularPillar(const CTriangularPillar& Prototype);
	virtual ~CTriangularPillar() = default;

	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	virtual HRESULT Ready_Components(void* pArg) override;

public:
	static CTriangularPillar* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
	
};

END