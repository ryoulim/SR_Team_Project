// 내 클래스 이름 : Button
// 부모 클래스 이름 : UI

#pragma once
#include "UI.h"

BEGIN(Client)

class CButton final : public CUI
{
public:
	typedef struct tagButtonDesc : public CUI::DESC
	{
		
	}DESC;

private:
	CButton(LPDIRECT3DDEVICE9 pGraphic_Device);
	CButton(const CButton& Prototype);
	virtual ~CButton() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CButton* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END