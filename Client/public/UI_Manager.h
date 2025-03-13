// 내 클래스 이름 : UI_Manager
// 부모 클래스 이름 : Base

#pragma once
#include "Base.h"

BEGIN(Client)

class CUI_Manager final : public CBase
{
	DECLARE_SINGLETON(CUI_Manager)

private:
	CUI_Manager();
	virtual ~CUI_Manager() = default;

public:
	HRESULT Initialize_Prototype() ;
	HRESULT Initialize(void* pArg) ;
	void Priority_Update(_float fTimeDelta) ;
	void Update(_float fTimeDelta) ;
	void Late_Update(_float fTimeDelta) ;
	HRESULT Render() ;

public:
	//vector<class CUI*>		m_vecUIs[LEVEL_END];


public:
	enum FONTTYPE { MEDIUMBLUE, TINYBLUE, NEONLIGHTS, THICKNEONLIGHTS, BIGORANGE, BIGSILVER, GRAFFITI, FONT_END };
	vector<class CFont*>		m_Fonts[FONT_END];
		
public:
	virtual void Free();
};

END