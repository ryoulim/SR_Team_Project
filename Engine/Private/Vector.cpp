#include "Engine_Defines.h"

tagVector::tagVector()
{
    x = 0;
    y = 0;
    z = 0;
}

const tagVector& tagVector::Normalize()
{
    D3DXVec3Normalize(this, this);
    return *this;
}

FLOAT tagVector::Length() const
{
    return D3DXVec3Length(this);
}

FLOAT tagVector::Distance(const D3DXVECTOR3& _vector) const
{
    D3DXVECTOR3 vTmp = *this - _vector;
    
    return D3DXVec3Length(&vTmp);
}

FLOAT tagVector::Dot(const D3DXVECTOR3& _vector) const
{
    return D3DXVec3Dot(this, &_vector);
}

tagVector tagVector::Cross(const D3DXVECTOR3& _vector) const
{
    D3DXVECTOR3 vTmp{};
    return *D3DXVec3Cross(&vTmp, this, &_vector);
}

const tagVector& tagVector::TransformNormal(const D3DXMATRIX& _matrix)
{
    D3DXVec3TransformNormal(this, this, &_matrix);
    return *this;
}

const tagVector& tagVector::TransformCoord(const D3DXMATRIX& _matrix)
{
    D3DXVec3TransformCoord(this, this, &_matrix);
    return *this;
}
