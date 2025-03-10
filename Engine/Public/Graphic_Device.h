#pragma once

#include "Base.h"

BEGIN(Engine)

class CGraphic_Device final : public CBase
{
private:
	CGraphic_Device();
	virtual ~CGraphic_Device() = default;

public:
	HRESULT		Initialize(HWND hWnd, _bool isWindowed, _uint iWinSizeX, _uint iWinSizeY, LPDIRECT3DDEVICE9* ppOut);
	void		Render_Begin();
	void		Render_End(HWND hWnd = nullptr);	

private:
	void		Set_Parameters(D3DPRESENT_PARAMETERS& d3dpp,
		HWND hWnd, _bool isWindowed, _uint iWinSizeX, _uint iWinSizeY);

private:
	LPDIRECT3D9			m_pSDK;		// 1���� �ش��ϴ� ��ü
	LPDIRECT3DDEVICE9	m_pDevice;	// 3���� �ش��ϴ� ��ü

	LPD3DXSPRITE		m_pSprite;	// 2d �̹����� ����ϱ� ���� �뵵
	LPD3DXFONT			m_pFont;

public:
	static CGraphic_Device* Create(HWND hWnd, _bool isWindowed, _uint iWinSizeX, _uint iWinSizeY, LPDIRECT3DDEVICE9*	ppOut);
	virtual void Free() override;
};

END