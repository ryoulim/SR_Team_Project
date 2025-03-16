// 내 클래스 이름 : Button_Test
// 부모 클래스 이름 : Button

#pragma once
#include "Button.h"


BEGIN(Client)

class CButton_Test final : public CButton
{
private:
	CButton_Test(LPDIRECT3DDEVICE9 pGraphic_Device);
	CButton_Test(const CButton_Test& Prototype);
	virtual ~CButton_Test() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CButton_Test* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END