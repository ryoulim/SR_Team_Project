#pragma once

#include "Base.h"

class CDebugDraw final : public CBase
{
public:
    static void DrawLine(LPDIRECT3DDEVICE9 pDevice, const _float3& start, const _float3& end, D3DCOLOR color);
};
