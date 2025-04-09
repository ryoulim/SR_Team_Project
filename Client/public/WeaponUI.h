#pragma once
#include "UI.h"

BEGIN(Client)

class CWeaponUI final : public CUI
{
public:
	typedef struct tagWeaponUIDesc : public CUI::DESC
	{

	}DESC;

private:
	CWeaponUI(LPDIRECT3DDEVICE9 pGraphic_Device);
	CWeaponUI(const CWeaponUI& Prototype);
	virtual ~CWeaponUI() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Ready_Components(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:


public:
	static CWeaponUI* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END