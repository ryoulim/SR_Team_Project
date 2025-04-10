//#pragma once
//
//#include "Client_Defines.h"
//#include "Level.h"
//
//BEGIN(Client)
//
//class CLevel_RaceEnd : public CLevel
//{
//private:
//	CLevel_RaceEnd(LPDIRECT3DDEVICE9 pGraphic_Device);
//	virtual ~CLevel_RaceEnd() = default;
//
//public:
//	virtual HRESULT Initialize(class CLevelData* pLevelData);
//	virtual void Update(_float fTimeDelta);
//	virtual HRESULT Render();
//
//private:
//	HRESULT Ready_Layer_Camera();
//	HRESULT Ready_Layer_Terrain(const _wstring& strLayerTag);
//	HRESULT Ready_Layer_Statue(const _wstring& strLayerTag);
//	HRESULT Ready_Layer_Pawn(const _wstring& strLayerTag);
//	HRESULT Ready_Layer_RaceBoss(const _wstring& strLayerTag);
//	HRESULT Ready_Layer_UI(const _wstring& strLayerTag);
//
//public:
//	static CLevel_RaceEnd* Create(LPDIRECT3DDEVICE9 pGraphic_Device, class CLevelData* pLevelData);
//	virtual void Free()override;
//};
//
//END


///뭔가 하려고 하는중