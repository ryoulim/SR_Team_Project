// 내 클래스 이름 : Portrait
// 부모 클래스 이름 : UI

#pragma once
#include "UI.h"
#include <Font_ItemDialog.h>

BEGIN(Client)

class CPortrait final : public CUI
{
public:
	typedef struct tagPortraitDesc : public CUI::DESC
	{

	}DESC;

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
	CFont* m_pFont = { nullptr };

public:
	static CPortrait* Create(LPDIRECT3DDEVICE9 pGraphicDevice);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END