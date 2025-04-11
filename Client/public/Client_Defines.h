#pragma once

/* Ư�� ������Ͽ� �Ҽӵ��� �ʴ� ������������ �����صδ� ��Ȱ */
/* ��Ÿ �ٸ� ��������(.h)���� ��Ŭ����ϴ� ��Ȱ. */
#include <Windows.h>
#include <process.h>

//#define _CONSOL
//#define _IMGUI
#define START_LEVEL LEVEL_UNDERGROUND

// ���̽� �� �÷��̾��� �̵��ӵ� : �������� �������⼭ ����� ���� �Ⱦ���
#define RACE_SPEED_PER_SEC 400.f

// ���̽� ������ ������ �÷��̾� �Ѿ˰��� ��� �Դٰ��� �ϱ� �Ⱦ �ϴ� �ھƵ�
#define RACE_PBULLET_DIR	_float3{ 0.f, 0.164f, 0.986f }
#define RACE_PBULLET_SPEED	1150.f

/* [ ������ �Ȱ����� ] */	
#define INDOOR_START_FOG 300.f;
#define INDOOR_END_FOG 1500.f;

#define OUTDOOR_START_FOG 2000.f;
#define OUTDOOR_END_FOG 2700.f;

#define GAMEPLAY_START_FOG 2500.f;
#define GAMEPLAY_END_FOG 3500.f;

#define UNDERGROUND_START_FOG 0.f;
#define UNDERGROUND_END_FOG 350.f;
/* ��START_FOG = �÷��̾� ���� �Ȱ����� ������ */
/* ��END_FOG   = �Ȱ��� ���� ���ϰ� �Դ� ������ */


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