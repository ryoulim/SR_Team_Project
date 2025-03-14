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
	HRESULT Render_Text(const string& _text, CFont::FONTTYPE _type, CFont::FONTALIGN _align, _float _posX, _float _posY, _float vSizeMul = 1.f) {
		if (m_Fonts[_type] == nullptr)
			return E_FAIL;
		return m_Fonts[_type]->Render_Text(_text, _align, _posX, _posY, vSizeMul);
	}
	HRESULT Render_Text(const _int _val, CFont::FONTTYPE _type, CFont::FONTALIGN _align, _float _posX, _float _posY, _float vSizeMul = 1.f) {
		if (m_Fonts[_type] == nullptr)
			return E_FAIL;
		return m_Fonts[_type]->Render_Text(_val, _align, _posX, _posY, vSizeMul);
	}


	
public:
	class CFont*			m_Fonts[CFont::FONT_END] = {nullptr,};
	vector<class CUI*>		m_vecUIs[LEVEL_END];

public:
	virtual void Free();
};

#define RENDER_ITEMDIALOG(message, fposY) Render_Text(message, CFont::LEFT, -(g_iWinSizeX / 2.f) + 20.f, fposY)

// Render_Text BigOrangeCenter
// 노란 글씨 중앙 정렬
#define RENDER_TEXT_BOC(message, fX, fY, fSize) CUI_Manager::Get_Instance(m_pGameInstance)->Render_Text(message, CFont::BIGORANGE, CFont::CENTER, fX, fY, fSize)
// Render_Text BigOrangeLeft
// 노란 글씨 좌측 정렬
#define RENDER_TEXT_BOL(message, fX, fY, fSize) CUI_Manager::Get_Instance(m_pGameInstance)->Render_Text(message, CFont::BIGORANGE, CFont::LEFT, fX, fY, fSize)


END