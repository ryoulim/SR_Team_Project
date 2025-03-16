#pragma once
#include "Client_Defines.h"
#include "Level.h"
#include "GameInstance.h"
#include "Dynamic_Camera.h"
#include <random>
//#include "Dynamic_Camera.h"
//#include "MyCube.h"
//#include "Aim.h"
//#include "Portrait.h"
//#include "Terrain.h"

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
	HRESULT Ready_Layer_Item(const _wstring& strLayerTag);
	HRESULT Ready_Light();


public:
	void	SpawnExplosion(_float3 _vPosition);
	void	SpawnFire(_float3 _vPosition);
	void	SpawnGunFire();
	void	SpawnTornado(_float3 _vPosition);
	void	SpawnMultipleExplosions(_float fTimeDelta);

private:
	_int m_iIndex{-1};
public:
	static CLevel_GamePlay* Create(LPDIRECT3DDEVICE9 pGraphic_Device, class CLevelData* pLevelData);
	virtual void Free() override;

public:
	float GetRandomValue(float min, float max)
	{
		uniform_real_distribution<float> dist(min, max);
		return dist(m_Gen);
	}

private:
	random_device m_Rd;
	mt19937 m_Gen;
	uniform_real_distribution<float> m_Dist;
};

END