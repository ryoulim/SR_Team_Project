
#include "Graphic_Device.h"

CGraphic_Device::CGraphic_Device() 
	: m_pDevice(nullptr), m_pSDK(nullptr), m_pSprite(nullptr), m_pFont(nullptr)
{
}

HRESULT CGraphic_Device::Initialize(HWND hWnd, _bool isWindowed, _uint iWinSizeX, _uint iWinSizeY, LPDIRECT3DDEVICE9* ppOut)
{
	// ��ġ �ʱ�ȭ ����
	// 1. ��ġ(�׷��� ī��)�� ������ ��ü ����
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);
	
	// 2. ������ ���� ��ü�� ��ġ�� ����(���� ����)
	// 
	// HAL : �ϵ���� �߻�ȭ ����

	D3DCAPS9	DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	// GetDeviceCaps : ��ġ ������ ������ �Լ�
	// D3DADAPTER_DEFAULT : ������ �������� �⺻ �׷��� ī��

	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
	{
		MSG_BOX("GetDeviceCaps Failed");
		return E_FAIL;
	}

	DWORD		vp(0);		// ���ؽ� ���μ��� = ���� ��ȯ + ���� ����

	if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;

	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;

	D3DPRESENT_PARAMETERS		d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	Set_Parameters(d3dpp, hWnd, isWindowed, iWinSizeX, iWinSizeY);

	// 3. ��ġ(�׷��� ī��)�� ������ ��ü ����

	// CreateDevice : ��ġ�� ������ ��ü ����
	// CreateDevice(� �׷��� ī�带 ������ ���ΰ�, � ������ ��ġ�� ������ ���ΰ�, ��ġ�� ����� ������ �ڵ�, ���� ���, ��� ȯ��, ������ ��ü�� ������ ������)
	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT,
							D3DDEVTYPE_HAL, hWnd, vp, &d3dpp, &m_pDevice)))
	{
		MSG_BOX("CreateDevice Failed");
		return E_FAIL;
	}

	// sprite
	if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	{
		MSG_BOX("D3DXCreateSprite Failed");
		return E_FAIL;
	}

	//font

	D3DXFONT_DESCW		tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));

	tFontInfo.Height = 20;
	tFontInfo.Width = 10;
	tFontInfo.Weight = FW_HEAVY;
	tFontInfo.CharSet = HANGEUL_CHARSET;
	lstrcpy(tFontInfo.FaceName, L"�ü�");

	if (FAILED(D3DXCreateFontIndirect(m_pDevice, &tFontInfo, &m_pFont)))
	{
		MSG_BOX("D3DXCreateFontIndirect Failed");
		return E_FAIL;
	}

	*ppOut = m_pDevice;

	Safe_AddRef(m_pDevice);

	return S_OK;
}

// �ĸ� ������ ���� ����
// 1. �����
// 2. ���� ȭ�� �׸� ���� �غ��Ѵ�.
// 3. ���ǽ��� ��ȯ�Ѵ�.
void CGraphic_Device::Render_Begin()
{
	m_pDevice->Clear(0,			// ��Ʈ�� ����
		nullptr,	// ���� ��Ʈ �� ù ��° ��Ʈ�� �ּ�
		D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET,
		D3DCOLOR_ARGB(255, 0, 0, 255), // �ĸ� ���۸� ���� ä�� ����
		1.f,	// z���� �ʱ�ȭ ��
		0);		// ���ٽ� ���� �ʱ�ȭ ��

	m_pDevice->BeginScene();

	

}

void CGraphic_Device::Render_End(HWND hWnd)
{
	

	m_pDevice->EndScene();

	m_pDevice->Present(NULL, NULL, hWnd, NULL);

	// 1, 2, 4 �� ���� ��� ���� : D3DSWAPEFFECY_COPY�� �ۼ����� ��� ����
	// 3���� : ��� ��� ������ �ڵ�, NULL�� ��쿡�� Set_Parameters���� ������ �ڵ� ������ ����
}



void CGraphic_Device::Set_Parameters(D3DPRESENT_PARAMETERS& d3dpp, HWND hWnd, _bool isWindowed, _uint iWinSizeX, _uint iWinSizeY)
{
	d3dpp.BackBufferWidth = iWinSizeX;
	d3dpp.BackBufferHeight = iWinSizeY;

	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	// D3DSWAPEFFECT_DISCARD : ���� ü�� ���
	// D3DSWAPEFFECT_FLIP : ���� �ϳ��� �������鼭 ó���ϴ� ���
	// D3DSWAPEFFECT_COPY : ���� ���۸��� ������ ���� ���

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	d3dpp.hDeviceWindow = hWnd;

	// ��ü ȭ�� ��� or â ���
	d3dpp.Windowed = isWindowed;			// â ��� ����

	// �ڵ����� ����, ���ٽ� ���� ���� ����
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	// ��ü ȭ�� ����� ��� ����

	// ��ü ȭ�� �� ����� ������� ���� ����
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

	// ������� ���� â �ÿ� ���� ����
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}

CGraphic_Device* CGraphic_Device::Create(HWND hWnd, _bool isWindowed, _uint iWinSizeX, _uint iWinSizeY, LPDIRECT3DDEVICE9* ppOut)
{
	CGraphic_Device* pInstance = new CGraphic_Device();

	if (FAILED(pInstance->Initialize(hWnd, isWindowed, iWinSizeX, iWinSizeY, ppOut)))
	{
		MSG_BOX("Failed to Created : CGraphic_Device");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGraphic_Device::Free()
{
	Safe_Release(m_pFont);
	Safe_Release(m_pSprite);

	if (0 != Safe_Release(m_pDevice))
		MSG_BOX("Failed to Release : IDirect3DDevice9");	
	if(0 != Safe_Release(m_pSDK))
		MSG_BOX("Failed to Release : IDirect3D9");

}
