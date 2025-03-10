#include "Engine_Defines.h"

tagVector::tagVector()
{
    x = 0;
    y = 0;
    z = 0;
}

void tagVector::Normalize()
{
    D3DXVec3Normalize(this, this);
}

FLOAT tagVector::Length()
{
    return D3DXVec3Length(this);
}

FLOAT tagVector::Distance(const D3DXVECTOR3& _vector)
{
    D3DXVECTOR3 vTmp = *this - _vector;
    
    return D3DXVec3Length(&vTmp);
}

FLOAT tagVector::Dot(const D3DXVECTOR3& _vector)
{
    return D3DXVec3Dot(this, &_vector);
}

tagVector* tagVector::Cross(const D3DXVECTOR3& _vector)
{
    D3DXVECTOR3 vTmp{};
    return static_cast<tagVector*>(D3DXVec3Cross(&vTmp, this, &_vector));
}