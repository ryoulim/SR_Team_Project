// 내 클래스 이름 : Race
// 부모 클래스 이름 : Map

#pragma once
#include "Map.h"

BEGIN(Client)

class CRaceTerrain final : public CMap
{
private:
	CRaceTerrain(LPDIRECT3DDEVICE9 pGraphic_Device);
	CRaceTerrain(const CRaceTerrain& Prototype);
	virtual ~CRaceTerrain() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CRaceTerrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END

