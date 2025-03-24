#include "DebugDraw.h"

void CDebugDraw::DrawLine(LPDIRECT3DDEVICE9 pDevice, const _float3& start, const _float3& end, D3DCOLOR color)
{
    ID3DXLine* pLine = nullptr;
    if (FAILED(D3DXCreateLine(pDevice, &pLine)))
        return;

    // 명확히 D3DXVECTOR3로 선언
    D3DXVECTOR3 line[2] = {
        D3DXVECTOR3(start.x, start.y, start.z),
        D3DXVECTOR3(end.x, end.y, end.z)
    };

    // View, Projection 행렬 받아오기
    D3DXMATRIX matView, matProj;
    pDevice->GetTransform(D3DTS_VIEW, &matView);
    pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

    D3DXMATRIX matVP = matView * matProj;


    pLine->SetWidth(1.f);
    pLine->Begin();
    pLine->DrawTransform(line, 2, &matVP, color);
    pLine->End();

    Safe_Release(pLine);
}
