#pragma once
#include "Vector.h"
#include "Matrix.h"

BEGIN(Engine)

typedef struct tagEngineDesc 
{
	HINSTANCE				hInst;
	HWND					hWnd;
	bool					isWindowed;
	unsigned int			iWinSizeX;
	unsigned int			iWinSizeY;
	unsigned int			iNumLevels;		
}ENGINE_DESC;

/*D3DDECLUSAGE*/

typedef struct tagVertexPositionTexcoord	
{		
	VECTOR			vPosition;
	D3DXVECTOR2		vTexcoord;	
}VTXPOSTEX;

typedef struct tagVertexPositionNormalTexcoord
{
	VECTOR			vPosition;
	VECTOR			vNormal;
	D3DXVECTOR2		vTexcoord;

}VTXPOSNORTEX;

typedef struct tagVertexNormalCube
{
	D3DXVECTOR3 vPosition;
	D3DXVECTOR3 vNormal;
	D3DXVECTOR3 vTexcoord;
}VTXCUBE;

END