// 내 클래스 이름 : Armor
// 부모 클래스 이름 : UI

#pragma once
#include "UI.h"

BEGIN(Client)

class CArmor final : public CUI
{
private:
	CArmor(LPDIRECT3DDEVICE9 pGraphic_Device);
	CArmor(const CArmor& Prototype);
	virtual ~CArmor() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CArmor* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END