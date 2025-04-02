// ³» Å¬·¡½º ÀÌ¸§ : UI_Manager
// ºÎ¸ð Å¬·¡½º ÀÌ¸§ : Base

#pragma once
#include "Base.h"
#include "GameInstance.h"
#include "Client_Defines.h"

#include "Font.h"
#include "FadeUI.h"

#include "Ammo.h"
#include "Aim.h"
#include "Armor.h"
#include "Portrait.h"
//#include "Player.h"

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
	HRESULT Initialize_GamePlayUI(LEVEL eLevelID);
	HRESULT Initialize_Player();
	HRESULT Clear_GamePlayUI();
	HRESULT Change_Weapon(const CWeapon::AMMOINFO* pAmmoInfo);
	HRESULT Init_UI_To_Player(const CPlayer::INFO* pPlayerInfo);

private:
	HRESULT	Initialize_Font();
	HRESULT Initialize_FadeUI();
	
private:
	class CFont*						m_Fonts[CFont::FONT_END] = {nullptr,};
	array<class CGameObject*, GUI_END>	m_GameUIs = { nullptr };
	class CPlayer*						m_pPlayer = { nullptr };

public:
	virtual void Free();
};


#pragma region ÆùÆ® ¸ÅÅ©·Î
// Render_Text MediumBlueLeft
// ÆÄ¶õ ±Û¾¾ (¾ÆÀÌÅÛ È¹µæ dialog¿ëµµ ÁÂÃø »ó´Ü °íÁ¤)
#define RENDER_ITEMDIALOG(message, fposY)  CUI_Manager::Get_Instance()->Render_Text(message, CFont::MEDIUMBLUE, CFont::LEFT, -(g_iWinSizeX / 2.f) + 20.f, fposY)

// Render_Text BigOrangeCenter
// ³ë¶õ ±Û¾¾ Áß¾Ó Á¤·Ä
#define RENDER_TEXT_BOC(message, fX, fY, fSize) \
CUI_Manager::Get_Instance()->Set_ButtonBrightness(1.f, CFont::BIGORANGE);\
CUI_Manager::Get_Instance()->Render_Text(message, CFont::BIGORANGE, CFont::CENTER, fX, fY, fSize)
// Render_Text BigOrangeLeft
// ³ë¶õ ±Û¾¾ ÁÂÃø Á¤·Ä
#define RENDER_TEXT_BOL(message, fX, fY, fSize) \
CUI_Manager::Get_Instance()->Set_ButtonBrightness(1.f, CFont::BIGORANGE);\
CUI_Manager::Get_Instance()->Render_Text(message, CFont::BIGORANGE, CFont::LEFT, fX, fY, fSize)

// Render_Text BigOrangeCenter Dark
// ³ë¶õ ±Û¾¾ Áß¾Ó Á¤·Ä, ¾îµÓ°Ô
#define RENDER_TEXT_BOC_DARK(message, fX, fY, fSize) \
CUI_Manager::Get_Instance()->Set_ButtonBrightness(0.6f, CFont::BIGORANGE);\
CUI_Manager::Get_Instance()->Render_Text(message, CFont::BIGORANGE, CFont::CENTER, fX, fY, fSize)
// Render_Text BigOrangeLeft Dark
// ³ë¶õ ±Û¾¾ ÁÂÃø Á¤·Ä, ¾îµÓ°Ô
#define RENDER_TEXT_BOL_DARK(message, fX, fY, fSize) \
CUI_Manager::Get_Instance()->Set_ButtonBrightness(0.6f, CFont::BIGORANGE);\
CUI_Manager::Get_Instance()->Render_Text(message, CFont::BIGORANGE, CFont::LEFT, fX, fY, fSize)
#pragma endregion



END