// 내 클래스 이름 : Button
// 부모 클래스 이름 : UI

#pragma once
#include "Font.h"

BEGIN(Client)

class CButton : public CUI
{
public:
	typedef struct tagButtonDesc : public CUI::DESC
	{
		CFont::FONTALIGN eAlign;
	}DESC;
protected:
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

protected:
	CFont::FONTALIGN m_eAlign = { CFont::LEFT };

public:
	static CButton* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END