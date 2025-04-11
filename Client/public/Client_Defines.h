#pragma once

/* 특정 헤더파일에 소속되지 않는 전역정보들을 선언해두는 역활 */
/* 기타 다른 정의파일(.h)들을 인클루드하는 역활. */
#include <Windows.h>
#include <process.h>

//#define _CONSOL
//#define _IMGUI
#define START_LEVEL LEVEL_UNDERGROUND

// 레이스 맵 플레이어의 이동속도 : 보정때메 여기저기서 써야함 이제 안쓸듯
#define RACE_SPEED_PER_SEC 400.f

// 레이스 레벨의 고정된 플레이어 총알관련 계수 왔다갔다 하기 싫어서 일단 박아둠
#define RACE_PBULLET_DIR	_float3{ 0.f, 0.164f, 0.986f }
#define RACE_PBULLET_SPEED	1150.f

/* [ 레벨별 안개정도 ] */	
#define INDOOR_START_FOG 300.f;
#define INDOOR_END_FOG 1500.f;

#define OUTDOOR_START_FOG 2000.f;
#define OUTDOOR_END_FOG 2700.f;

#define GAMEPLAY_START_FOG 2500.f;
#define GAMEPLAY_END_FOG 3500.f;

#define UNDERGROUND_START_FOG 0.f;
#define UNDERGROUND_END_FOG 350.f;
/* ↑START_FOG = 플레이어 기준 안개시작 지점↑ */
/* ↑END_FOG   = 안개가 제일 찐하게 먹는 지점↑ */


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