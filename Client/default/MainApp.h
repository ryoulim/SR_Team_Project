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

private:
	HRESULT Open_Level(LEVEL eLevelID);
	HRESULT Ready_Component_For_Static();
	HRESULT Ready_Object_For_Static();

public:
	static CMainApp* Create();
	virtual void Free() override;

};

END