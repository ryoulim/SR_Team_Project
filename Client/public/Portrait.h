// 내 클래스 이름 : Portrait
// 부모 클래스 이름 : UI

#pragma once
#include "UI.h"
#include "Font_MediumBlue.h"

BEGIN(Client)

class CPortrait final : public CUI
{
public:
	enum HPSTATUS {
		HP100 = 0,
		HP80 = 5,
		HP40 = 10,
		HP25 = 15,
		HP10 = 18,
		HP0 = 21,
		HP200 = 24
	};
	enum PORTRAITSTATUS
	{
		PORTRAIT_IDLE,
		PORTRAIT_ANGER,
		PORTRAIT_SMILE,
		PORTRAIT_LOOKL,
		PORTRAIT_LOOKR,
		PORTRAIT_DEAD
	};

private:
	CPortrait(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPortrait(const CPortrait& Prototype);
	virtual ~CPortrait() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	void	Change_Face(_float fTimeDelta);

private:
	CFont*	m_pFont = { nullptr };
	_float	m_fAnimTick = {};
	PORTRAITSTATUS	m_eFace = { PORTRAIT_IDLE };
	HPSTATUS		m_eHPStatus = { HP100 };

public:
	static CPortrait* Create(LPDIRECT3DDEVICE9 pGraphicDevice);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END