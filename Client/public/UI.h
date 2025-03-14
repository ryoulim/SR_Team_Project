#pragma once

#include "Client_Defines.h"
#include "GameInstance.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
class CTransform;
END

BEGIN(Client)

class CUI abstract : public CGameObject
{
public:
	typedef struct tagUIDesc : public CTransform::DESC
	{
		_float3 vInitPos;
		_float3 vScale;
	}DESC;

protected:
	CUI(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUI(const CUI& Prototype);
	virtual ~CUI() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components(void* pArg);
    virtual void Update_Rect();

protected:
	LEVEL           m_eLevelID = { LEVEL_END };
	const _tchar*   m_szTextureID = { nullptr };
	const _tchar*   m_szBufferType = { nullptr };
	CTexture*       m_pTextureCom = { nullptr };
	CVIBuffer*      m_pVIBufferCom = { nullptr };
	CTransform*     m_pTransformCom = { nullptr };
    _float3         m_vPos = {};
    _float3         m_vSize = {};
    RECT            m_tRect = {};
    _float          m_fDepth = {};


public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free();
};

END

/*#include <d3d9.h>
#include <d3dx9.h>
#include <map>
#include <string>
#include <vector>

std::map<char, std::string> fontMap;
std::vector<LPDIRECT3DTEXTURE9> fontTextures;

// Direct3D ����̽�
LPDIRECT3DDEVICE9 pD3DDevice = nullptr;

void LoadFontImages() {
    fontMap['H'] = "images/H.png";
    fontMap['e'] = "images/e.png";
    fontMap['l'] = "images/l.png";
    fontMap['o'] = "images/o.png";
    fontMap['W'] = "images/W.png";
    fontMap['r'] = "images/r.png";
    fontMap['d'] = "images/d.png";
    fontMap['!'] = "images/!.png";
}

void LoadTextures() {
    for (const auto& pair : fontMap) {
        LPDIRECT3DTEXTURE9 pTexture = nullptr;
        D3DXCreateTextureFromFile(pD3DDevice, pair.second.c_str(), &pTexture);
        fontTextures.push_back(pTexture);
    }
}

void RenderText(const std::string& text, int x, int y) {
    int offsetX = 0;
    for (char c : text) {
        if (c == ' ') {
            offsetX += 20;  // ���� ���ڴ� ������ ��ġ��ŭ �̵�
        } else {
            auto it = fontMap.find(c);
            if (it != fontMap.end()) {
                LPDIRECT3DTEXTURE9 pTexture = fontTextures[std::distance(fontMap.begin(), it)];
                RECT srcRect = { 0, 0, 50, 50 };  // �� ���� �̹����� ũ�⿡ �°� ����
                RECT destRect = { x + offsetX, y, x + offsetX + 50, y + 50 };
                pD3DDevice->StretchRect(pTexture, &srcRect, nullptr, nullptr, D3DTEXF_NONE);

                // ���� ������ ��ġ�� �̵�
                offsetX += 50;  // ���� �̹����� ���� ũ��
            }
        }
    }
}

int main() {
    // Direct3D �ʱ�ȭ �� ����̽� ���� �ڵ�� ����

    LoadFontImages();
    LoadTextures();

    // ���� �������� ���ڿ��� ������
    while (true) {
        pD3DDevice->BeginScene();

        RenderText("Hello World!", 100, 100);  // "Hello World!" ���ڿ� ������

        pD3DDevice->EndScene();
        pD3DDevice->Present(nullptr, nullptr, nullptr, nullptr);
    }

    // Direct3D ����̽� �� ���ҽ� ���� �ڵ�� ����

    return 0;
}
*/