#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Test : public CLevel
{
private:
	CLevel_Test(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_Test() = default;

public:
	virtual HRESULT Initialize(class CLevelData* pLevelData);
	virtual void Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Load_Map(LEVEL eLevelIdx, const _wstring& FileName);

	HRESULT Ready_Layer_Terrain(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Camera(const _wstring& strLayerTag);

public:
	static CLevel_Test* Create(LPDIRECT3DDEVICE9 pGraphic_Device, class CLevelData* pLevelData);
	virtual void Free()override;
};

END