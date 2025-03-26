// 내 클래스 이름 : Ammo
// 부모 클래스 이름 : UI

#pragma once
#include "UI.h"
#include "Weapon.h"


BEGIN(Client)

class CAmmo final : public CUI
{
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
	void	Set_Ammo(const CWeapon::AMMOINFO* ammoInfo) { m_pAmmoInfo = ammoInfo; }

private:
	CWeapon::TYPE				m_eAmmoType = { CWeapon::TYPE::LOVERBOY };
	const CWeapon::AMMOINFO*	m_pAmmoInfo = { nullptr };

public:
	static CAmmo* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END