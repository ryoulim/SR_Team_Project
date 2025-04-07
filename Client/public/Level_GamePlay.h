#pragma once
#include "Client_Defines.h"
#include "Level.h"
#include "GameInstance.h"
#include "Dynamic_Camera.h"
#include "CameraSprite.h"
#include <random>

BEGIN(Client)

class CLevel_GamePlay final : public CLevel
{
private:
	CLevel_GamePlay(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_GamePlay() = default;

public:
	virtual HRESULT Initialize(class CLevelData* pLevelData) override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

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

	void SpawnTtakkeun_i(const _float3& _Position, _bool m_bActive, _int _iNum, LEVEL _eLevel);

public: //몬스터소환(게임플레이)
	void SpawnWenteko(_float3 _Position, bool m_bActive, LEVEL _eLevel);
	void SpawnShotgunner(_float3 _Position, bool m_bActive, LEVEL _eLevel);
	void SpawnNukemutant(_float3 _Position, bool m_bActive, LEVEL _eLevel);
	void SpawnMechsect(_float3 _Position, bool m_bActive, LEVEL _eLevel);
	void SpawnGreater(_float3 _Position, bool m_bActive, LEVEL _eLevel);
	void SpawnDeacon(_float3 _Position, bool m_bActive, LEVEL _eLevel);
	void SpawnCultist(_float3 _Position, bool m_bActive, LEVEL _eLevel);
	void SpawnArchangel(_float3 _Position, bool m_bActive, LEVEL _eLevel);

private:
	class CCameraManager* m_pCameraManager{};
	_int m_iIndex{-1};
	_bool testbool = { false };

private:
	virtual HRESULT Load_Map(_uint iLevelIdx, const _wstring& FileName) override;

public:
	static CLevel_GamePlay* Create(LPDIRECT3DDEVICE9 pGraphic_Device, class CLevelData* pLevelData);
	virtual void Free() override;

};

END