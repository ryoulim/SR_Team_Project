// 내 클래스 이름 : LevelLoadingMenu
// 부모 클래스 이름 : UI

#pragma once
#include "UI.h"

BEGIN(Client)

class CLevelLoadingMenu final : public CUI
{
public:
	typedef struct tagLevelLoadingMenuDesc : public CUI::DESC
	{

	}DESC;

private:
	CLevelLoadingMenu(LPDIRECT3DDEVICE9 pGraphic_Device);
	CLevelLoadingMenu(const CLevelLoadingMenu& Prototype);
	virtual ~CLevelLoadingMenu() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	_float		m_fLoadingGauge{ 0.1f };
	_bool		m_isFinished = { false };

public:
	void	Set_LoadingGauge(const _float percent) { m_fLoadingGauge = percent; }


public:
	static CLevelLoadingMenu* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END