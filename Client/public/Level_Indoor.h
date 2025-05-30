#pragma once

#include "Client_Defines.h"
#include "Level.h"
#include "GameInstance.h"

BEGIN(Client)

class CLevel_Indoor : public CLevel
{
private:
	CLevel_Indoor(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_Indoor() = default;

public:
	virtual HRESULT Initialize(class CLevelData* pLevelData);
	virtual void Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	virtual HRESULT Load_Map(_uint iLevelIdx, const _wstring& FileName);

	HRESULT Ready_Layer_Trigger(const _wstring& strLayerTag);
	HRESULT Ready_Layer_UI(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Camera(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Pawn(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Monster(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Item(const _wstring& strLayerTag);

	void Check_Collision();

private:
	CSound_Core* m_pBGM = { nullptr };

public:
	static CLevel_Indoor* Create(LPDIRECT3DDEVICE9 pGraphic_Device, class CLevelData* pLevelData);
	virtual void Free()override;
};

END