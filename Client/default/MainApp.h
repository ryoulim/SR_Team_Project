#pragma once

#include "Client_Defines.h"
#include "../../EngineSDK/Inc/Base.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)
class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;

public:
	HRESULT Initialize();
	void Update(_float fTimeDelta);
	HRESULT Render();

private:
	CGameInstance*			m_pGameInstance = { nullptr };
	LPDIRECT3DDEVICE9		m_pGraphic_Device = { nullptr };
	_bool					m_bMouseToggle{};

private:
	HRESULT Open_Level(LEVEL eLevelID);
	HRESULT Ready_Default_Setting();
	HRESULT Ready_Component_For_Static();
	HRESULT Ready_Protype_Object_For_Static();
	HRESULT Ready_Object_For_Static();
	HRESULT Ready_Debug_Mode();

#ifdef _IMGUI
	HRESULT Ready_Imgui();
	HRESULT Free_Imgui();
#endif

public:
	static CMainApp* Create();
	virtual void Free() override;

};

END