#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_RaceSecond : public CLevel
{
private:
	CLevel_RaceSecond(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_RaceSecond() = default;

public:
	virtual HRESULT Initialize(class CLevelData* pLevelData);
	virtual void Update(_float fTimeDelta);
	virtual HRESULT Render();
	
private:
	HRESULT Ready_Layer_Camera();
	HRESULT Ready_Layer_Terrain(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Statue(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Pawn(const _wstring& strLayerTag);
	HRESULT Ready_Layer_RaceBoss(const _wstring& strLayerTag);

	void Check_Collision();

public:
	static CLevel_RaceSecond* Create(LPDIRECT3DDEVICE9 pGraphic_Device, class CLevelData* pLevelData);
	virtual void Free()override;
};

END

