#include "Engine_Defines.h"

tagMatrix::tagMatrix()
{
	D3DXMatrixIdentity(this);
}

tagMatrix::tagMatrix(const D3DXVECTOR3& _vCamera, const D3DXVECTOR3& _vLook, const D3DXVECTOR3& _vUp)
{
	D3DXMatrixLookAtLH(this, &_vCamera, &_vLook, &_vUp);

}

tagMatrix::tagMatrix(FLOAT _fFOV, FLOAT _fWinSizeX, FLOAT _fWinSizeY, FLOAT _fNear, FLOAT _fFar)
{
	D3DXMatrixPerspectiveFovLH(
		this,
		_fFOV,
		_fWinSizeX / _fWinSizeY,
		_fNear,
		_fFar);
}

tagMatrix::tagMatrix(FLOAT _fWinSizeX, FLOAT _fWinSizeY, FLOAT _fNear, FLOAT _fFar)
{
	D3DXMatrixOrthoLH(
		this,
		_fWinSizeX,
		_fWinSizeY,
		_fNear,
		_fFar);
}

void tagMatrix::MakeViewMat(const D3DXVECTOR3& _vCamera, const D3DXVECTOR3& _vLook, const D3DXVECTOR3& _vUp)
{
	D3DXMatrixLookAtLH(this, &_vCamera, &_vLook, &_vUp);
}

void tagMatrix::MakePerspectiveProjMat(FLOAT _fFOV, FLOAT _fWinSizeX, FLOAT _fWinSizeY, FLOAT _fNear, FLOAT _fFar)
{
	D3DXMatrixPerspectiveFovLH(
		this,
		_fFOV,
		_fWinSizeX / _fWinSizeY,
		_fNear,
		_fFar);
}

void tagMatrix::MakeOrthoProjMat(FLOAT _fWinSizeX, FLOAT _fWinSizeY, FLOAT _fNear, FLOAT _fFar)
{
	D3DXMatrixOrthoLH(
		this,
		_fWinSizeX,
		_fWinSizeY,
		_fNear,
		_fFar);

}

void tagMatrix::MakeInverseMat(const D3DXMATRIX& Matrix)
{
	D3DXMatrixInverse(this, nullptr, &Matrix);
}
