#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Boss final : public CLevel
{
private:
	CLevel_Boss(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_Boss() = default;

public:
	virtual HRESULT Initialize(class CLevelData* pLevelData);
	virtual void Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	virtual HRESULT Load_Map(_uint iLevelIdx, const _wstring& FileName);

	HRESULT Ready_Layer_Camera(const _wstring& strLayerTag);

public:
	static CLevel_Boss* Create(LPDIRECT3DDEVICE9 pGraphic_Device, class CLevelData* pLevelData);
	virtual void Free()override;

};

END