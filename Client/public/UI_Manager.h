// 내 클래스 이름 : UI_Manager
// 부모 클래스 이름 : Base

#pragma once
#include "Base.h"
#include "GameInstance.h"
#include "Client_Defines.h"
#include "Font.h"
#include "FadeUI.h"
#include "Ammo.h"

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
	static CUI_Manager* Get_Instance();
	static CUI_Manager* Get_Instance(CGameInstance* pGameInstance);
	static unsigned int Destroy_Instance();	

private:											
	static CUI_Manager* m_pInstance;
	CGameInstance* m_pGameInstance = { nullptr };

#pragma endregion
		
public:
	enum GAMEUI { GUI_AIM, GUI_PORTRAIT, GUI_ARMOR, GUI_AMMO, GUI_END };
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
	void	Set_ButtonBrightness(_float val, CFont::FONTTYPE _type) { 
		m_Fonts[_type]->Set_Brightness(val); 
	}
	void	Fade_In();
	void	Fade_Out();

public:
	HRESULT Initialize_GamePlayUI();
	HRESULT Clear_GamePlayUI();
	HRESULT Update_GameUI(GAMEUI eUIType, CAmmo::AMMOTYPE eAmmoType = CAmmo::AMMOTYPE::LOVERBOY);

private:
	HRESULT	Initialize_Font();
	HRESULT Initialize_FadeUI();
	
private:
	class CFont*						m_Fonts[CFont::FONT_END] = {nullptr,};
	class CFadeUI*						m_FadeUI = { nullptr };
	array<class CGameObject*, GUI_END>	m_GameUIs = { nullptr };
	class CPlayer*						m_pPlayer = { nullptr };


public:
	virtual void Free();
};


#pragma region UI_MANAGER 용 매크로 
// Render_Text MediumBlueLeft
// 파란 글씨 (아이템 획득 dialog용도 좌측 상단 고정)
#define RENDER_ITEMDIALOG(message, fposY)  CUI_Manager::Get_Instance()->Render_Text(message, CFont::MEDIUMBLUE, CFont::LEFT, -(g_iWinSizeX / 2.f) + 20.f, fposY)

// Render_Text BigOrangeCenter
// 노란 글씨 중앙 정렬
#define RENDER_TEXT_BOC(message, fX, fY, fSize) \
CUI_Manager::Get_Instance()->Set_ButtonBrightness(1.f, CFont::BIGORANGE);\
CUI_Manager::Get_Instance()->Render_Text(message, CFont::BIGORANGE, CFont::CENTER, fX, fY, fSize)
// Render_Text BigOrangeLeft
// 노란 글씨 좌측 정렬
#define RENDER_TEXT_BOL(message, fX, fY, fSize) \
CUI_Manager::Get_Instance()->Set_ButtonBrightness(1.f, CFont::BIGORANGE);\
CUI_Manager::Get_Instance()->Render_Text(message, CFont::BIGORANGE, CFont::LEFT, fX, fY, fSize)

// Render_Text BigOrangeCenter Dark
// 노란 글씨 중앙 정렬, 어둡게
#define RENDER_TEXT_BOC_DARK(message, fX, fY, fSize) \
CUI_Manager::Get_Instance()->Set_ButtonBrightness(0.6f, CFont::BIGORANGE);\
CUI_Manager::Get_Instance()->Render_Text(message, CFont::BIGORANGE, CFont::CENTER, fX, fY, fSize)
// Render_Text BigOrangeLeft Dark
// 노란 글씨 좌측 정렬
#define RENDER_TEXT_BOL_DARK(message, fX, fY, fSize) \
CUI_Manager::Get_Instance()->Set_ButtonBrightness(0.6f, CFont::BIGORANGE);\
CUI_Manager::Get_Instance()->Render_Text(message, CFont::BIGORANGE, CFont::LEFT, fX, fY, fSize)


// HP를 UI가 Get해오도록 전달
#define UPDATE_HP() CUI_Manager::Get_Instance()->Update_GameUI(CUI_Manager::GUI_PORTRAIT)
// Armor를 UI가 Get해오도록 전달
#define UPDATE_ARMOR() CUI_Manager::Get_Instance()->Update_GameUI(CUI_Manager::GUI_ARMOR);
// Ammo를..
#define UPDATE_AMMO(Type) CUI_Manager::Get_Instance()->Update_GameUI(CUI_Manager::GUI_AMMO, Type);


#pragma endregion


END