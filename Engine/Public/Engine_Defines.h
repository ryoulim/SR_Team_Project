#pragma once

#pragma warning (disable : 4251)
#pragma warning (disable : 4275) // D3DX 구조체 상속경고

#include <d3d9.h>
#include <d3dx9.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include <string>
#include <map>
#include <list>
#include <vector>
#include <unordered_map>
#include <forward_list>
using namespace std;

#include "Engine_Macro.h"
#include "Engine_Enum.h"
#include "Engine_Struct.h"
#include "Engine_Typedef.h"
#include "Engine_Function.h"

using namespace Engine;


#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif



