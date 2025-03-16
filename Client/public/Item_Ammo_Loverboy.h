// 내 클래스 이름 : Item_Ammo_Loverboy
// 부모 클래스 이름 : Item_Ammo

#pragma once
#include "Item_Ammo.h"

BEGIN(Client)

class CItem_Ammo_Loverboy final : public CItem_Ammo
{
public:
	typedef struct tagItem_Ammo_LoverboyDesc : public CItem_Ammo::DESC
	{

	}DESC;

private:
	CItem_Ammo_Loverboy(LPDIRECT3DDEVICE9 pGraphic_Device);
	CItem_Ammo_Loverboy(const CItem_Ammo_Loverboy& Prototype);
	virtual ~CItem_Ammo_Loverboy() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CItem_Ammo_Loverboy* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END