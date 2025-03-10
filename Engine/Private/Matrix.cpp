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

tagMatrix::tagMatrix(const D3DXVECTOR3& vector, FLOAT Angle)
{
	D3DXMatrixRotationAxis(this, &vector, Angle);
}

const tagMatrix& tagMatrix::MakeViewMat(const D3DXVECTOR3& _vCamera, const D3DXVECTOR3& _vLook, const D3DXVECTOR3& _vUp)
{
	D3DXMatrixLookAtLH(this, &_vCamera, &_vLook, &_vUp);
	return *this;
}

const tagMatrix& tagMatrix::MakePerspectiveProjMat(FLOAT _fFOV, FLOAT _fAspect, FLOAT _fNear, FLOAT _fFar)
{
	D3DXMatrixPerspectiveFovLH(
		this,
		_fFOV,
		_fAspect,
		_fNear,
		_fFar);
	return *this;

}

const tagMatrix& tagMatrix::MakeOrthoProjMat(FLOAT _fWinSizeX, FLOAT _fWinSizeY, FLOAT _fNear, FLOAT _fFar)
{
	D3DXMatrixOrthoLH(
		this,
		_fWinSizeX,
		_fWinSizeY,
		_fNear,
		_fFar);
	return *this;
}

const tagMatrix& tagMatrix::MakeInverseMat(const D3DXMATRIX& Matrix)
{
	D3DXMatrixInverse(this, nullptr, &Matrix);
	return *this;
}

const tagMatrix& tagMatrix::MakeRotationMat(const D3DXVECTOR3& vector, FLOAT Angle)
{
	D3DXMatrixRotationAxis(this, &vector, Angle);
	return *this;
}
