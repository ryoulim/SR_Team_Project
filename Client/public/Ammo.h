// 내 클래스 이름 : Ammo
// 부모 클래스 이름 : UI

#pragma once
#include "UI.h"

BEGIN(Client)

class CAmmo final : public CUI
{
public:	
	enum AMMOTYPE {
		LOVERBOY,
		DISPERSER_SHELL,
		DISPERSER_GRENADE,
		PENETRATOR,
		CHAINGUN,
		BOWLINGBOMB,
		IONBOW,
		CLUSTERPUCK,
		AMMO_END
	};

private:
	CAmmo(LPDIRECT3DDEVICE9 pGraphic_Device);
	CAmmo(const CAmmo& Prototype);
	virtual ~CAmmo() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void	Set_Ammo(AMMOTYPE _type, _uint _Ammo) { m_eAmmoType = _type;  m_uiAmmo[_type] = _Ammo; Update(0.f); }

private:
	AMMOTYPE	m_eAmmoType = { LOVERBOY };
	_uint		m_uiAmmo[AMMO_END] = {};


public:
	static CAmmo* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END