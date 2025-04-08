﻿#pragma once

/* 특정 헤더파일에 소속되지 않는 전역정보들을 선언해두는 역활 */
/* 기타 다른 정의파일(.h)들을 인클루드하는 역활. */
#include <Windows.h>
#include <process.h>

#define _CONSOL
//#define _IMGUI
#define START_LEVEL LEVEL_INDOOR

#ifdef _IMGUI
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#endif

#include "Client_Enum.h"
#include "Client_Macro.h"
#include "Client_Function.h"

namespace Client
{
	const unsigned int			g_iWinSizeX = 1280;
	const unsigned int			g_iWinSizeY = 720;
}

extern HINSTANCE g_hInst;
extern HWND g_hWnd;

using namespace Client;