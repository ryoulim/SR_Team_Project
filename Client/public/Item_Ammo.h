// 내 클래스 이름 : Item_Ammo
// 부모 클래스 이름 : Item

#pragma once
#include "Item.h"
#include "Ammo.h"

BEGIN(Client)

class CItem_Ammo : public CItem
{
public:
	typedef struct tagItem_AmmoDesc : public CItem::DESC
	{

	}DESC;

private:
	CItem_Ammo(LPDIRECT3DDEVICE9 pGraphic_Device);
	CItem_Ammo(const CItem_Ammo& Prototype);
	virtual ~CItem_Ammo() = default;

private:
	//CItem_Ammo* Ammos[CAmmo::AMMO_END];

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CItem_Ammo* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END