// 내 클래스 이름 : MainMenu
// 부모 클래스 이름 : UI

#pragma once
#include "UI.h"

BEGIN(Client)

class CMainMenu final : public CUI
{
private:
	CMainMenu(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMainMenu(const CMainMenu& Prototype);
	virtual ~CMainMenu() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
private:
	bool testbool = { false };

public:
	static CMainMenu* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END