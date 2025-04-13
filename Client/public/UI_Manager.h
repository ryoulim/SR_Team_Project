// 내 클래스 이름 : UI_Manager
// 부모 클래스 이름 : Base

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
	enum GAMEUI { GUI_AIM, GUI_PORTRAIT, GUI_ARMOR, GUI_AMMO, GUI_BOSSHP, GUI_RACING, GUI_WEAPONS, GUI_END };
	enum SPECUI { UI_BOSSHP, UI_RACING };
public:
	HRESULT Initialize() ;
	void Priority_Update(_float fTimeDelta) ;
	void Update(_float fTimeDelta) ;
	void Late_Update(_float fTimeDelta);
	HRESULT Render();

	/* 텍스트 출력 */
public:
	HRESULT Render_Text(const string& _text, CFont::FONTTYPE _type, CFont::FONTALIGN _align, _float _posX, _float _posY, _float vSizeMul = 1.f, CShader* pShader = nullptr);
	HRESULT Render_Text(const _int _val, CFont::FONTTYPE _type, CFont::FONTALIGN _align, _float _posX, _float _posY, _float vSizeMul = 1.f, CShader* pShader = nullptr);
	HRESULT Render_Text_Colored(const string& _text, CFont::FONTTYPE _type, CFont::FONTALIGN _align, _float _posX, _float _posY, _float _hueShift, _float vSizeMul = 1.f);
	void	Set_ButtonBrightness(_float val, CFont::FONTTYPE _type) { m_Fonts[_type]->Set_Brightness(val); }

	/* 페이드 인 아웃 */
public:
	void	Fade_In();
	void	Fade_Out(LEVEL eLevelID = LEVEL_STATIC);

	/* 연동 함수 */
public:
	HRESULT Initialize_GamePlayUI(LEVEL eLevelID);
	HRESULT Initialize_RacingUI(LEVEL eLevelID);
	HRESULT Initialize_BossHPUI(LEVEL eLevelID);

	HRESULT Change_Weapon(const CWeapon::AMMOINFO* pAmmoInfo);
	HRESULT Change_Weapon(_int iCurWeaponIndex);
	HRESULT Init_UI_To_Player(const CPawn::INFO* pPlayerInfo);
	HRESULT	Set_Face(CPortrait::PORTRAITSTATUS eStatus);
	HRESULT Set_RacingSpeed(_int iSpeed);
	HRESULT Set_Ttakkeun_HP_Settings(_int* pBossHP, _int* pBossHP2, _int iBossMaxHP);
	HRESULT Set_RacingBoss_HP_Settings(_int* pBossHP, _int iBossMaxHP);
	HRESULT Start_Rendering_BossHPUI();
	HRESULT Init_Weapons(const vector<class CWeapon*>* pWeapons);

	HRESULT Clear_GamePlayUI();

	/* 폰트 생성 및 들고댕기기 */
private:
	HRESULT	Initialize_Font();
	
private:
										/* 폰트 */
	class CFont*						m_Fonts[CFont::FONT_END] = {nullptr};
										/* UI */ 
	array<class CGameObject*, GUI_END>	m_GameUIs = { nullptr };
										/* 레이싱 */
	class CGameObject*					m_pRacingUI = { nullptr };
										/* 보스 HP 바 */
	class CGameObject*					m_pBossHPUI = { nullptr };

private: // 좌상단 텍스트 다이얼로그 관리 (아이템 습득 등)
	CGameObject*						m_pDialog = { nullptr };

public:
	void Insert_DialogQueue(const string &strMsg);
	void Initialize_Dialog(CGameObject* pObj);

public:
	virtual void Free();
};


#pragma region 폰트 매크로
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
// 노란 글씨 좌측 정렬, 어둡게
#define RENDER_TEXT_BOL_DARK(message, fX, fY, fSize) \
CUI_Manager::Get_Instance()->Set_ButtonBrightness(0.6f, CFont::BIGORANGE);\
CUI_Manager::Get_Instance()->Render_Text(message, CFont::BIGORANGE, CFont::LEFT, fX, fY, fSize)

#define RENDER_TEXT_MBL(message, fX, fY, fSize) \
CUI_Manager::Get_Instance()->Set_ButtonBrightness(1.f, CFont::MEDIUMBLUE);\
CUI_Manager::Get_Instance()->Render_Text(message, CFont::MEDIUMBLUE, CFont::LEFT, fX, fY, fSize)

#define RENDER_TEXT_MBC(message, fX, fY, fSize) \
CUI_Manager::Get_Instance()->Set_ButtonBrightness(1.f, CFont::MEDIUMBLUE);\
CUI_Manager::Get_Instance()->Render_Text(message, CFont::MEDIUMBLUE, CFont::CENTER, fX, fY, fSize)

#pragma endregion


END