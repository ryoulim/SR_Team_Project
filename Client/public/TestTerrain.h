// 내 클래스 이름 : TextTerrain
// 부모 클래스 이름 : Terrain

#pragma once

#include "Map.h"

BEGIN(Client)

class CTestTerrain final : public CMap
{
public:
	typedef struct tagTextTerrainDesc : public CMap::DESC
	{

	}DESC;

private:
	CTestTerrain(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTestTerrain(const CTestTerrain& Prototype);
	virtual ~CTestTerrain() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CTestTerrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END

