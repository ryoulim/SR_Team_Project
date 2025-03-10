#pragma once

#include "Base.h"

/* 1. ������Ʈ �Ŵ��� �ϳ������� �׷����ϴ� ����, ��Ƴ����ϴ� ����, ���l���� ����ϱⰡ ��ƴ�. */
/* 2. ȭ�鿡 �׷������� ��ü���� �׸��� ������� ��Ƴ��´�. */

BEGIN(Engine)

class CRenderer final : public CBase
{
public:
	enum RENDERGROUP { RG_PRIORITY, RG_NONBLEND, RG_BLEND, RG_UI, RG_END };
private:
	CRenderer(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CRenderer() = default;

public:
	HRESULT Add_RenderGroup(RENDERGROUP eRenderGroup, class CGameObject* pRenderObject);
	HRESULT Draw();

private:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = { nullptr };

private:
	list<class CGameObject*>	m_RenderObjects[RG_END];

private:
	HRESULT Render_Priority();
	HRESULT Render_NonBlend();
	HRESULT Render_Blend();
	HRESULT Render_UI();
public:
	static CRenderer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END