// 내 클래스 이름 : Button_Main
// 부모 클래스 이름 : UI

#pragma once
#include "Font.h"

BEGIN(Client)

class CButton_Main : public CUI
{
public:
	typedef struct tagButton_MainDesc : public CUI::DESC
	{
		CFont::FONTALIGN eAlign;
	}DESC;
	enum BUTTONTYPE {GAMESTART, OPTIONS, QUIT, BUTTON_END};
protected:
	CButton_Main(LPDIRECT3DDEVICE9 pGraphic_Device);
	CButton_Main(const CButton_Main& Prototype);
	virtual ~CButton_Main() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	void	Initialize_ButtonRect();
	HRESULT	Pick_Button();

private:
	CFont::FONTALIGN m_eAlign = { CFont::LEFT };
	RECT	m_tButton[BUTTON_END] = { };
	_bool	m_bNextLevel = { false };

public:
	static CButton_Main* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END