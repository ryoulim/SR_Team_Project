// 내 클래스 이름 : RacingPanel
// 부모 클래스 이름 : UI

#pragma once
#include "UI.h"

BEGIN(Client)

class CRacingPanel final : public CUI
{
public:
	typedef struct tagRacingPanelDesc : public CUI::DESC
	{

	}DESC;

private:
	CRacingPanel(LPDIRECT3DDEVICE9 pGraphic_Device);
	CRacingPanel(const CRacingPanel& Prototype);
	virtual ~CRacingPanel() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	class CFont_Racing* m_pFont_Racing = { nullptr };


public:
	static CRacingPanel* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END