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
	HRESULT Ready_Layer_Terrain(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Camera(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Statue(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Item(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Pawn(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Monster(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Particle(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Effect(const _wstring& strLayerTag);
	HRESULT Ready_Layer_UI(const _wstring& strLayerTag);
	HRESULT Ready_Light();
	HRESULT Ready_Layer_Trigger(const _wstring& strLayerTag);
	void Check_Collision();

	virtual HRESULT Load_Map(_uint iLevelIdx, const _wstring& FileName);


private:
	void SpawnTtakkeun_i(const _float3& _Position, _bool m_bActive, _int _iNum);

public:
	static CLevel_Boss* Create(LPDIRECT3DDEVICE9 pGraphic_Device, class CLevelData* pLevelData);
	virtual void Free()override;

};

END