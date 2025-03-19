// 내 클래스 이름 : Weapon_Chaingun
// 부모 클래스 이름 : Weapon

#pragma once
#include "Weapon.h"

BEGIN(Client)

class CWeapon_Chaingun final : public CWeapon
{
public:
	typedef struct tagWeapon_ChaingunDesc : public CWeapon::DESC
	{

	}DESC;

private:
	CWeapon_Chaingun(LPDIRECT3DDEVICE9 pGraphic_Device);
	CWeapon_Chaingun(const CWeapon_Chaingun& Prototype);
	virtual ~CWeapon_Chaingun() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void Set_State(STATE State) override {};

private:
	virtual void Opening(_float fTimeDelta) override;
	virtual void Walk(_float fTimeDelta) override;
	virtual void Weak_Attack(_float fTimeDelta) override;
	virtual void Strong_Attack(_float fTimeDelta) override;
	virtual void Reload(_float fTimeDelta) override;
	virtual void Ending(_float fTimeDelta) override;


public:
	static CWeapon_Chaingun* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END