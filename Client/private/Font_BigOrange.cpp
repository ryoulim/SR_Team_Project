// 내 클래스 이름 : Font_ItemDialog
// 부모 클래스 이름 : Font

#include "Font_BigOrange.h"
#include "GameInstance.h"

CFont_BigOrange::CFont_BigOrange(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CFont{ pGraphic_Device }
{
}

CFont_BigOrange::CFont_BigOrange(const CFont_BigOrange& Prototype)
	: CFont(Prototype)
{
}

HRESULT CFont_BigOrange::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFont_BigOrange::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_STATIC;
	m_szTextureID = TEXT("Font_BigOrange");
	//m_szBufferType = TEXT("Rect");
	m_eFontType = BIGORANGE;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CFont_BigOrange::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CFont_BigOrange::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CFont_BigOrange::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CFont_BigOrange::Render()
{
	return __super::Render();
}

HRESULT CFont_BigOrange::Render_Text(const string& _text, FONTALIGN _align, _float _posX, _float _posY, _float vSizeMul)
{
	m_uiTextWidth = static_cast<_uint>(_text.length());

	_float startPosX = {};
	if (_align == LEFT)
		startPosX = _posX;
	else if (_align == CENTER)
		startPosX = _posX - m_uiTextWidth / 2.f * 14.f;

	/***********************************************************/
	if (m_pEffect != nullptr)
	{
		m_pEffect->SetFloat("darknessFactor", m_fShadeVal);
		m_pEffect->Begin(NULL, 0);
		m_pEffect->BeginPass(0);
	}
	/***********************************************************/


	_float fontWidth{};
	for (auto ch : _text)
	{
		if (ch != L' ')
		{
			if (ch >= 'a' && ch <= 'z')
				ch = toupper(ch);
			if (ch >= '0' && ch <= '9')
				m_fTextureNum = static_cast<_float>(toascii(ch) - '0');
			else if (ch >= 'A' && ch <= 'Z')
				m_fTextureNum = static_cast<_float>(toascii(ch) - 55);
			else if (ch == '.') m_fTextureNum = 36.f;
			else if (ch == ',') m_fTextureNum = 37.f;
			else if (ch == '!') m_fTextureNum = 38.f;
			else if (ch == '?') m_fTextureNum = 39.f;
			else if (ch == ';') m_fTextureNum = 40.f;
			else if (ch == ':') m_fTextureNum = 41.f;
			else if (ch == '/') m_fTextureNum = 42.f;
			else if (ch == '%') m_fTextureNum = 43.f;
			else if (ch == '\'') m_fTextureNum = 44.f;
			else if (ch == '-') m_fTextureNum = 45.f;
			else m_fTextureNum = 39.f;
			if (FAILED(m_pTextureCom->Get_TextureSize(static_cast<_uint>(m_fTextureNum), &m_vSize)))
				return E_FAIL;
			if (_text == "QUIT" && ch == 'T')
				int a = 0;

			m_vSize *= vSizeMul;
			fontWidth += m_vSize.x * 0.5f;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(startPosX + fontWidth, _posY, 0.f));
			/***********************************************************/



			/***********************************************************/
			Render();
			fontWidth += m_vSize.x * 0.5f + 1.f;
		}
		else
			fontWidth += 10.f;
	}
	/***********************************************************/
	if (m_pEffect != nullptr)
	{
		m_pEffect->EndPass();
		m_pEffect->End();
	}
	/***********************************************************/
	return S_OK;
}

HRESULT CFont_BigOrange::Render_Text(const _int _val, FONTALIGN _align, _float _posX, _float _posY, _float vSizeMul)
{
	char buffer[64]{};
	_itoa_s(_val, buffer, 10);
	string temp = buffer;

	Render_Text(temp, _align, _posX, _posY, vSizeMul);

	return S_OK;
}

CFont_BigOrange* CFont_BigOrange::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CFont_BigOrange* pInstance = new CFont_BigOrange(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CFont_BigOrange");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFont_BigOrange::Clone(void* pArg)
{
	CFont_BigOrange* pInstance = new CFont_BigOrange(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CFont_BigOrange");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFont_BigOrange::Free()
{
	__super::Free();
}