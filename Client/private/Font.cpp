// 내 클래스 이름 : Font
// 부모 클래스 이름 : UI

#include "Font.h"
#include "GameInstance.h"

CFont::CFont(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI{ pGraphic_Device }
{
}

CFont::CFont(const CFont& Prototype)
	: CUI(Prototype)
{
}

HRESULT CFont::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFont::Initialize(void* pArg)
{
	//m_eLevelID = LEVEL_STATIC;
	//m_szTextureID = TEXT("Font");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_fDepth = _float(UI_FONT);
	
	__super::Ready_ShaderComponent();

	return S_OK;
}

void CFont::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CFont::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CFont::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CFont::Render()
{
	if (FAILED(Bind_Texture_To_Transform()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

// 0~9/A~Z/a~z 외 특수문자 다수 포함 용    // 그 외는 각 객체들에서 오버라이딩
HRESULT CFont::Render_Text(const string& _text,FONTALIGN _align, _float _posX, _float _posY, _float vSizeMul, CShader* pShader)
{
	m_uiTextWidth = static_cast<_uint>(_text.length());

	_float startPosX = {};
	if (_align == LEFT)
		startPosX = _posX;
	else if (_align == CENTER)
		startPosX = _posX - m_uiTextWidth / 2.f * 14.f;
	_float fontWidth{};
	for (auto ch : _text)
	{
		if (ch != L' ')
		{
			m_fTextureNum = static_cast<_float>(toascii(ch) - 33);
			if (FAILED(m_pTextureCom->Get_TextureSize(static_cast<_uint>(m_fTextureNum), &m_vSize)))
				return E_FAIL;
			m_vSize *= vSizeMul;
			fontWidth += m_vSize.x * 0.5f;

			if (_posY < -200)
				int a = 0;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(startPosX + fontWidth, _posY, 0.5f));
			if (pShader != nullptr)
			{
				m_pTextureCom->Bind_Shader_To_Texture(pShader, "Tex", static_cast<_uint>(m_fTextureNum));
				pShader->Begin(CShader::ALPHA);
			}
			Render();
			if (pShader != nullptr)
				pShader->End();
			fontWidth += m_vSize.x * 0.5f + 1.f;
		}
		else
			fontWidth += 10.f;
	}

	return S_OK;
}

HRESULT CFont::Render_Text(const _int _val, FONTALIGN _align, _float _posX, _float _posY, _float vSizeMul, CShader* pShader)
{
	char buffer[64]{};
	_itoa_s(_val, buffer, 10);
	string temp = buffer;

	Render_Text(temp, _align, _posX, _posY, vSizeMul, pShader);

	return S_OK;
}

HRESULT CFont::Bind_Texture_To_Transform()
{
	if (m_pShaderCom == nullptr)
	{
		if (FAILED(m_pTextureCom->Bind_Resource(static_cast<_uint>(m_fTextureNum))))
			return E_FAIL;
	}

	m_pTransformCom->Scaling(m_vSize);

	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;
	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;
	return S_OK;
}

void CFont::Free()
{
	__super::Free();

}