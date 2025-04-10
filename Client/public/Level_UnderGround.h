#pragma once
#include "Client_Defines.h"
#include "Level.h"

class CLevel_UnderGround final : public CLevel
{
private:
	CLevel_UnderGround(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_UnderGround() = default;

public:
	virtual HRESULT Initialize(class CLevelData* pLevelData);
	virtual void Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	virtual HRESULT Load_Map(_uint iLevelIdx, const _wstring& FileName);

	HRESULT Ready_Layer_UI(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Camera(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Pawn(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Monster(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Trigger(const _wstring& strLayerTag);

	void Check_Collision();

public:
	static CLevel_UnderGround* Create(LPDIRECT3DDEVICE9 pGraphic_Device, class CLevelData* pLevelData);
	virtual void Free()override;
};

