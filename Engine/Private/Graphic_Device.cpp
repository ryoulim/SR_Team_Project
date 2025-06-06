
#include "Graphic_Device.h"
#include <assert.h>

CGraphic_Device::CGraphic_Device() 
	: m_pDevice(nullptr), m_pSDK(nullptr), m_pSprite(nullptr), m_pFont(nullptr)
{
}

HRESULT CGraphic_Device::Initialize(HWND hWnd, _bool isWindowed, _uint iWinSizeX, _uint iWinSizeY, LPDIRECT3DDEVICE9* ppOut)
{
	// 장치 초기화 과정
	// 1. 장치(그래픽 카드)를 조사할 객체 생성
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);
	
	// 2. 위에서 만든 객체로 장치를 조사(지원 수준)
	// 
	// HAL : 하드웨어 추상화 계층

	D3DCAPS9	DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	// GetDeviceCaps : 장치 정보를 얻어오는 함수
	// D3DADAPTER_DEFAULT : 정보를 얻으려는 기본 그래픽 카드

	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
	{
		MSG_BOX("GetDeviceCaps Failed");
		return E_FAIL;
	}

	DWORD		vp(0);		// 버텍스 프로세싱 = 정점 변환 + 조명 연산

	if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;

	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;

	D3DPRESENT_PARAMETERS		d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	Set_Parameters(d3dpp, hWnd, isWindowed, iWinSizeX, iWinSizeY);

	// 3. 장치(그래픽 카드)를 제어할 객체 생성

	// CreateDevice : 장치를 제어할 객체 생성
	// CreateDevice(어떤 그래픽 카드를 제어할 것인가, 어떤 정보로 장치에 접근할 것인가, 장치를 사용할 윈도우 핸들, 동작 방식, 사용 환경, 생성할 객체를 저장할 포인터)
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
	lstrcpy(tFontInfo.FaceName, L"궁서");

	if (FAILED(D3DXCreateFontIndirect(m_pDevice, &tFontInfo, &m_pFont)))
	{
		MSG_BOX("D3DXCreateFontIndirect Failed");
		return E_FAIL;
	}

	*ppOut = m_pDevice;

	Safe_AddRef(m_pDevice);

	return S_OK;
}

// 후면 버퍼의 동작 원리
// 1. 지운다
// 2. 다음 화면 그릴 것을 준비한다.
// 3. 서피스를 교환한다.
void CGraphic_Device::Render_Begin()
{
	m_pDevice->Clear(0,			// 렉트의 개수
		nullptr,	// 여러 렉트 중 첫 번째 렉트의 주소
		D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET,
		D3DCOLOR_ARGB(255, 0, 0, 255), // 후면 버퍼를 비우고서 채울 색상
		1.f,	// z버퍼 초기화 값
		0);		// 스텐실 버퍼 초기화 값

	m_pDevice->BeginScene();

	

}

void CGraphic_Device::Render_End(HWND hWnd)
{
	

	m_pDevice->EndScene();

	m_pDevice->Present(NULL, NULL, hWnd, NULL);

	// 1, 2, 4 번 인자 사용 조건 : D3DSWAPEFFECY_COPY로 작성했을 사용 가능
	// 3인자 : 출력 대상 윈도우 핸들, NULL인 경우에는 Set_Parameters에서 지정한 핸들 값으로 지정
}

HRESULT CGraphic_Device::Draw_Font(const _wstring& str, LPRECT pRect)
{
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	if (FAILED(m_pFont->DrawText(m_pSprite,
		str.c_str(), (INT)str.length(), pRect,
		DT_LEFT | DT_VCENTER | DT_WORDBREAK,
		D3DCOLOR_ARGB(255, 0, 0, 0))))
		return E_FAIL;

	m_pSprite->End();

	return S_OK;
}



void CGraphic_Device::Set_Parameters(D3DPRESENT_PARAMETERS& d3dpp, HWND hWnd, _bool isWindowed, _uint iWinSizeX, _uint iWinSizeY)
{
	d3dpp.BackBufferWidth = iWinSizeX;
	d3dpp.BackBufferHeight = iWinSizeY;

	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	// D3DSWAPEFFECT_DISCARD : 스왑 체인 방식
	// D3DSWAPEFFECT_FLIP : 버퍼 하나로 뒤집으면서 처리하는 방식
	// D3DSWAPEFFECT_COPY : 더블 버퍼링과 유사한 복사 방식

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	d3dpp.hDeviceWindow = hWnd;

	// 전체 화면 모드 or 창 모드
	d3dpp.Windowed = isWindowed;			// 창 모드 지정

	// 자동으로 깊이, 스텐실 버퍼 관리 여부
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	// 전체 화면 모드일 경우 적용

	// 전체 화면 시 모니터 재생률에 대한 설정
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

	// 재생률과 실제 창 시연 간격 설정
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
