#pragma once

#include "Client_Defines.h"
#include "Base.h"
#include <LoadingMenu.h>

BEGIN(Engine)
class CGameInstance;
class CLevelData;
END

BEGIN(Client)

class CLoader final : public CBase
{
private:
	CLoader(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLoader() = default;

public:
	_bool isFinished() const {
		return m_isFinished;
	}
	CLevelData* Get_LevelData() const {
		return m_pData;
	}

	_float Get_LoadPercent() const {
		return m_fLoadPercent;
	}

public:
	HRESULT Initialize(LEVEL eNextLevelID);
	HRESULT Loading();

	void Output_LoadingText();
	
private:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = { nullptr };
	LEVEL						m_eNextLevelID = { LEVEL_END };
	_bool						m_isFinished = { false };
	_float						m_fLoadPercent = {0.1f};
	CGameInstance*				m_pGameInstance = { nullptr };

private:
	HANDLE						m_hThread = {};
	CRITICAL_SECTION			m_CriticalSection = {};
	_tchar						m_szLoadingText[MAX_PATH] = {};
	CLevelData*					m_pData = { nullptr };

private:
	void Clear_MapData();

private:
	HRESULT Loding_For_Static();
	HRESULT Loading_For_Logo();
	HRESULT Loading_For_GamePlay();
	HRESULT Loading_For_RaceFirst();
	HRESULT Loading_For_RaceSecond();
	HRESULT Loading_For_RaceThird();
	HRESULT Loading_For_Boss();
	HRESULT Loading_For_Indoor();
	HRESULT Add_Data(const _wstring& strFilePath);
	HRESULT Load_For_Terrain(const _wstring& strFileTag);
public:
	static CLoader* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevelID);
	virtual void Free() override;
};

END