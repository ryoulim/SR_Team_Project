// 내 클래스 이름 : FadeUI
// 부모 클래스 이름 : UI

#pragma once
#include "UI.h"

BEGIN(Client)

class CFadeUI final : public CUI
{
public:
	typedef struct tagFadeUIDesc : public CUI::DESC
	{
		_bool	m_isFadeIn;
		LEVEL	eNextLevel = LEVEL_STATIC;
	}DESC;

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
	void	Fade_In();
	void	Fade_Out();

private:
	_float		m_fFadeOpacity = {};
	_bool		m_isRenderOn = { false };
	_bool		m_isFadeIn = { false };
	_bool		m_isFadeOut = { false };
	LEVEL		m_eNextLevelID = LEVEL_STATIC;
public:
	static CFadeUI* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END