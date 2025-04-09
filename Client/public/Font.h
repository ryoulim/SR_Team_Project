// 내 클래스 이름 : Font
// 부모 클래스 이름 : UI

#pragma once
#include "UI.h"
#define FONTCLASS

BEGIN(Client)

class CFont : public CUI
{
public:
	enum FONTTYPE { MEDIUMBLUE, TINYBLUE, BIGORANGE, BIGSILVER, RACING, FONT_END };
	enum FONTALIGN { LEFT, CENTER, RIGHT };

protected:
	CFont(LPDIRECT3DDEVICE9 pGraphic_Device);
	CFont(const CFont& Prototype);
	virtual ~CFont() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	// 텍스트 출력 용, 매개변수 wstring 아님에 주의
	virtual HRESULT Render_Text(const string& _text, FONTALIGN _align, _float _posX, _float _posY, _float vSizeMul = 1.f, CShader* pShader = nullptr);
	// 숫자 출력 용
	virtual HRESULT Render_Text(const _int _val, FONTALIGN _align, _float _posX, _float _posY, _float vSizeMul = 1.f, CShader* pShader = nullptr);
	virtual HRESULT Render_Text_Colored(const string& _text, FONTALIGN _align, _float _posX, _float _posY, _float _hueShift, _float vSizeMul = 1.f);
	void	Set_Brightness(_float val) { m_fShadeVal = val; }

	virtual HRESULT Set_Font_Settings(FONTTYPE _eFontType, FONTALIGN _Align, CShader::TYPE _eShaderType, _tchar _ShaderVar, _float vSizeMultiplier = 1.f) { return S_OK; };
	virtual HRESULT imsi_Render_Text(const string& _Text, _float3 vPos) { return S_OK; };

protected:
	HRESULT Bind_Texture_To_Transform();

protected:
	_uint		m_uiTextWidth = {};
	FONTTYPE	m_eFontType = { FONT_END };
	_float		m_fShadeVal = {1.f};
	FONTALIGN	m_eAlign = { LEFT };
	_float		m_fSizeMultiplier = { 1.f };

public:
	virtual void Free();
};

END