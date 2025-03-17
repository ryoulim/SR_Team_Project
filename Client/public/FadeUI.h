// 내 클래스 이름 : FadeUI
// 부모 클래스 이름 : UI

#pragma once
#include "UI.h"

BEGIN(Client)

class CFadeUI final : public CUI
{
private:
	CFadeUI(LPDIRECT3DDEVICE9 pGraphic_Device);
	CFadeUI(const CFadeUI& Prototype);
	virtual ~CFadeUI() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CFadeUI* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END