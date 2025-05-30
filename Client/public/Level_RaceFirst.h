#pragma once

#include "Client_Defines.h"
#include "Level.h"
#include "GameInstance.h"

BEGIN(Client)

class CLevel_RaceFirst : public CLevel
{
private:
	CLevel_RaceFirst(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_RaceFirst() = default;

public:
	virtual HRESULT Initialize(class CLevelData* pLevelData);
	virtual void Update(_float fTimeDelta);
	virtual HRESULT Render();
	
private:
	HRESULT Ready_Layer_Camera();
	HRESULT Ready_Layer_Terrain(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Statue(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Statue2(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Pawn(const _wstring& strLayerTag);
	HRESULT Ready_Layer_RaceBoss(const _wstring& strLayerTag);
	HRESULT Ready_Layer_UI(const _wstring& strLayerTag);

	void Check_Collision();
	void Manage_Light();

private:
	typedef struct tagLightData
	{
		_float fDistanceToPlayer;
		_uint iLightNumber;
	}LIGHTDATA;

	//거리를 기준으로 빛 구조체 번호 관리
	list<LIGHTDATA*> m_LightData;

private:
	CSound_Core* m_pBGM = { nullptr };

public:
	static CLevel_RaceFirst* Create(LPDIRECT3DDEVICE9 pGraphic_Device, class CLevelData* pLevelData);
	virtual void Free()override;
};

END