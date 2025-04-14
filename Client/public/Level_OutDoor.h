#pragma once

#include "Client_Defines.h"
#include "Level.h"
#include "GameInstance.h"

class CLevel_OutDoor final : public CLevel
{
private:
	CLevel_OutDoor(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_OutDoor() = default;

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
	HRESULT Ready_Layer_Boat(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Statue(const _wstring& strLayerTag);

	void Check_Collision();

private:
	CSound_Core* m_pBGM = { nullptr };
	CGameObject* m_pGenerator = { nullptr };
	_bool m_bChangeBGM = { false };
	_bool m_bDeadGenerator = { false };

public:
	static CLevel_OutDoor* Create(LPDIRECT3DDEVICE9 pGraphic_Device, class CLevelData* pLevelData);
	virtual void Free()override;
};

