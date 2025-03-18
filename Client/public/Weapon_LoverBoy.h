// 내 클래스 이름 : Weapon_LoverBoy
// 부모 클래스 이름 : Weapon

#pragma once
#include "Weapon.h"

BEGIN(Client)

class CWeapon_LoverBoy final : public CWeapon
{
private:
	CWeapon_LoverBoy(LPDIRECT3DDEVICE9 pGraphic_Device);
	CWeapon_LoverBoy(const CWeapon_LoverBoy& Prototype);
	virtual ~CWeapon_LoverBoy() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void Set_State(STATE State) override;

private:
	virtual void Weak_Attack() override;
	virtual void Strong_Attack() override;
	virtual void Reload() override;
	virtual void Opening() override;
	void Positioning();

public:
	static CWeapon_LoverBoy* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END