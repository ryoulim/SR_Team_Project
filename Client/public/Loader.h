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

	void Output_LoadingText() {
		SetWindowText(g_hWnd, m_szLoadingText);

		auto LoadingMenu = m_pGameInstance->Find_Object(LEVEL_LOADING, TEXT("Layer_UI"));
		dynamic_cast<CLoadingMenu*>(LoadingMenu)->Set_LoadingGauge(m_fLoadPercent);
	}
	
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
	HRESULT Loading_For_Logo();
	HRESULT Loading_For_GamePlay();
	HRESULT Loading_For_Race();
	HRESULT Loading_For_Test();
	HRESULT Add_Data(const _wstring& strFilePath);
	HRESULT Load_For_Terrain(const _wstring& strFileTag);
public:
	static CLoader* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevelID);
	virtual void Free() override;
};

END