// �� Ŭ���� �̸� : Terrain
// �θ� Ŭ���� �̸� : Statue

#pragma once
#include "Statue.h"

BEGIN(Client)

class CTerrain final : public CStatue
{
public:
	typedef struct tagTerrainDesc : public CStatue::DESC
	{

	}DESC;

private:
	CTerrain(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTerrain(const CTerrain& Prototype);
	virtual ~CTerrain() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CTerrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END