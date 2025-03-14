// 내 클래스 이름 : UI_Manager
// 부모 클래스 이름 : Base

#pragma once
#include "Base.h"
#include "GameInstance.h"
#include "Client_Defines.h"
#include "Font.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

class CUI_Manager final : public CBase
{
#pragma region DECLARE_SINGLETON
private:
	CUI_Manager();
	virtual ~CUI_Manager() = default;
	CUI_Manager(const CUI_Manager&) = delete;				
	CUI_Manager& operator=(const CUI_Manager&) = delete;						

public:												
	static CUI_Manager* Get_Instance(CGameInstance* pGameInstance);
	static unsigned int Destroy_Instance();	

private:											
	static CUI_Manager* m_pInstance;
	CGameInstance* m_pGameInstance = { nullptr };

#pragma endregion

public:
public:
	HRESULT Initialize() ;
	void Priority_Update(_float fTimeDelta) ;
	void Update(_float fTimeDelta) ;
	void Late_Update(_float fTimeDelta);
	HRESULT Render();

private:
	void	Set_GameInstance(CGameInstance* pGameInstance) { m_pGameInstance = pGameInstance; Safe_AddRef(m_pGameInstance); }

public:
	HRESULT Render_Text(const string& _text, CFont::FONTTYPE _type, CFont::FONTALIGN _align, _float _posX, _float _posY) {
		if (m_Fonts[_type] == nullptr)
			return E_FAIL;
		return m_Fonts[_type]->Render_Text(_text, _align, _posX, _posY);
	}
	HRESULT Render_Text(const _int _val, CFont::FONTTYPE _type, CFont::FONTALIGN _align, _float _posX, _float _posY) {
		if (m_Fonts[_type] == nullptr)
			return E_FAIL;
		return m_Fonts[_type]->Render_Text(_val, _align, _posX, _posY);
	}


	
public:
	class CFont*			m_Fonts[CFont::FONT_END] = {nullptr,};
	vector<class CUI*>		m_vecUIs[LEVEL_END];

public:
	virtual void Free();
};

END