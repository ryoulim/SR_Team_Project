// 내 클래스 이름 : Weapon_Dispenser
// 부모 클래스 이름 : Weapon

#pragma once
#include "Weapon.h"

BEGIN(Client)

class CWeapon_Dispenser final : public CWeapon
{
public:
	typedef struct tagWeapon_DispenserDesc : public CWeapon::DESC
	{

	}DESC;

private:
	CWeapon_Dispenser(LPDIRECT3DDEVICE9 pGraphic_Device);
	CWeapon_Dispenser(const CWeapon_Dispenser& Prototype);
	virtual ~CWeapon_Dispenser() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual const AMMOINFO* Get_Info();

private:
	_bool m_bTrigger{};
	_bool m_bGrenadeMode{};
	AMMOINFO m_tGrenadeInfo{};
	AMMOINFO* m_pCurAmmo{ nullptr };

private:
	virtual HRESULT Ready_Components(void* pArg);

	virtual void Set_State(STATE State) override;
	virtual void Key_Input() override;
	virtual void Opening(_float fTimeDelta) override;
	virtual void Weak_Attack(_float fTimeDelta) override;
	virtual void Strong_Attack(_float fTimeDelta) override;
	virtual void Reload(_float fTimeDelta) override;
	virtual void Ending(_float fTimeDelta) override;
	virtual void Create_Bullet() override;
	_bool Update_Frame_Reverse(_float fTimeDelta);

public:
	static CWeapon_Dispenser* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END