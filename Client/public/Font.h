// �� Ŭ���� �̸� : Font
// �θ� Ŭ���� �̸� : UI

#pragma once
#include "UI.h"
#define FONTCLASS

BEGIN(Client)

class CFont : public CUI
{
public:	
	enum FONTTYPE { MEDIUMBLUE, TINYBLUE, NEONLIGHTS, THICKNEONLIGHTS, BIGORANGE, BIGSILVER, GRAFFITI, FONT_END };
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
	// �ؽ�Ʈ ��� ��, �Ű����� wstring �ƴԿ� ����
	virtual HRESULT Render_Text(const string& _text, FONTALIGN _align, _float _posX, _float _posY, _float vSizeMul = 1.f);
	// ���� ��� ��
	virtual HRESULT Render_Text(const _int _val, FONTALIGN _align, _float _posX, _float _posY, _float vSizeMul = 1.f);

protected:
	HRESULT Bind_Texture_To_Transform();

protected:
	_uint		m_uiTextWidth = {};
	FONTTYPE	m_eFontType = { FONT_END };



public:
	//static CFont* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	//virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END