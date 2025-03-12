// 내 클래스 이름 : Ammo
// 부모 클래스 이름 : UI

#pragma once
#include "UI.h"

BEGIN(Client)

class CAmmo final : public CUI
{
public:
	typedef struct tagAmmoDesc : public CUI::DESC
	{

	}DESC;

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
	static CAmmo* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END