// 내 클래스 이름 : Font
// 부모 클래스 이름 : UI

#pragma once
#include "UI.h"
#define FONTCLASS

BEGIN(Client)

class CFont final : public CUI
{
public:
	//enum FONTSORT { FNUM = 0, FUP = 10, FLO = 36 };
	//enum class MEDIUMBLUE { F0 = 2837, FLA = 2886, FUA = 2854 };
	//enum class TINYBLUE { F0 = 3087, FLA = 3104, FUA = 3104 };
	//enum class NEONLIGHTS { F0 = 10020, FLA = 10030, FUA = 10030 };
	//enum class THICKNEONLIGHTS { F0 = 10095, FLA = 10069, FUA = 10069 };
	//enum class BIGORANGE { F0 = 2930, FLA = 2940, FUA = 2940 };
	//enum class BIGSILVER { F0 = 2992, FLA = 2966, FUA = 2966 };
	//enum class GRAFFITI { F0 = 10566, FLA = 10540, FUA = 10540 };

	enum FONTSORT {

	};

	enum FONTTYPE {
		MEDIUMBLUE,
		TINYBLUE,
		NEONLIGHTS,
		THICKNEONLIGHTS,
		BIGORANGE,
		BIGSILVER,
		GRAFFITI
	};

	enum FONTALIGN {
		LEFT,
		CENTER,
		RIGHT
	};

private:
	CFont(LPDIRECT3DDEVICE9 pGraphic_Device);
	CFont(const CFont& Prototype);
	virtual ~CFont() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Render_Text(const wstring& _text, FONTTYPE _type, FONTALIGN _align, _uint _x, _uint _y);

private:
	void	Calc_TextWidth(const wstring& _text);

private:
	_uint		m_uiTextWidth = {};

public:
	static CFont* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END