#pragma once

#include "Base.h"

BEGIN(Engine)

class CImgui_Device final : public CBase
{
private:
	CImgui_Device(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CImgui_Device() = default;

public:
	HRESULT		Initialize(HWND hWnd);
	void  Render_Begin();
	void  Render_End();
private:
	LPDIRECT3DDEVICE9				m_pGraphic_Device = { nullptr };

public:
	static CImgui_Device* Create(LPDIRECT3DDEVICE9 pGraphic_Device, HWND hWnd);
	virtual void Free() override;
};

END