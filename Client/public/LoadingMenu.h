// 내 클래스 이름 : LoadingMenu
// 부모 클래스 이름 : UI

#pragma once
#include "UI.h"

BEGIN(Client)

class CLoadingMenu final : public CUI
{
public:
	typedef struct tagLoadingMenuDesc : public CUI::DESC
	{

	}DESC;
	enum LOADERTEXTURES { LOADERTEX_ANIM, LOADERTEX_BARBACK, LOADERTEX_BAR, LOADERTEX_END };

private:
	CLoadingMenu(LPDIRECT3DDEVICE9 pGraphic_Device);
	CLoadingMenu(const CLoadingMenu& Prototype);
	virtual ~CLoadingMenu() = default;

private:
	HRESULT Ready_LoadingComponents();
	HRESULT Render_Animation();
	HRESULT Render_LoadingBar();

private:
	CTexture* m_pTextureComForLoading[LOADERTEX_END] = {nullptr};
	CVIBuffer*	m_pVIBufferComForLoading[LOADERTEX_END] = { nullptr };
	CTransform* m_pTransformComForLoading[LOADERTEX_END] = { nullptr };
	_float		m_fAnimTick{};
	_float		m_fLoadingGauge{0.01f};
	_float		m_fCurLoadingGauge{ 0.f };

public:
	void	Set_LoadingGauge(const _float percent) { m_fCurLoadingGauge = percent; }
	_bool	IsLoadingComplete() { return m_fLoadingGauge >= 1.19f; }

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CLoadingMenu* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END