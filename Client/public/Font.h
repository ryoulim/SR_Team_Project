// �� Ŭ���� �̸� : Font
// �θ� Ŭ���� �̸� : UI

#pragma once
#include "UI.h"
#define FONTCLASS

BEGIN(Client)

class CFont : public CUI
{
public:
	enum FONTTYPE { MEDIUMBLUE, TINYBLUE, NEONLIGHTS, THICKNEONLIGHTS, BIGORANGE, BIGSILVER, GRAFFITI };
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
	virtual HRESULT Render_Text(const string& _text, FONTTYPE _type, FONTALIGN _align, _float _posX, _float _posY);
	// ���� ��� ��
	virtual HRESULT Render_Text(const _int _val, FONTTYPE _type, FONTALIGN _align, _float _posX, _float _posY);

protected:
	HRESULT Bind_Texture_To_Transform();

protected:
	_uint		m_uiTextWidth = {};
	FONTTYPE	m_eFontType = { MEDIUMBLUE };



public:
	//static CFont* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	//virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END