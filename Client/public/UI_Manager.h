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
	enum GAMEUI { GUI_AIM, GUI_PORTRAIT, GUI_ARMOR, GUI_AMMO, GUI_END };
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
	void	Set_ButtonBrightness(_float val, CFont::FONTTYPE _type) { m_Fonts[_type]->Set_Brightness(val); }

	/* 페이드 인 아웃 */
public:
	void	Fade_In();
	void	Fade_Out();

	/* 연동 함수 */
public:
	HRESULT Initialize_GamePlayUI(LEVEL eLevelID);
	HRESULT Initialize_Player();
	HRESULT Clear_GamePlayUI();
	HRESULT Change_Weapon(const CWeapon::AMMOINFO* pAmmoInfo);
	HRESULT Init_UI_To_Player(const CPlayer::INFO* pPlayerInfo);
	HRESULT	Set_Face(CPortrait::PORTRAITSTATUS eStatus); 

	/* 폰트 생성 및 들고댕기기 */
private:
	HRESULT	Initialize_Font();
	
private:
	class CFont*						m_Fonts[CFont::FONT_END] = {nullptr,};
	array<class CGameObject*, GUI_END>	m_GameUIs = { nullptr };
	class CPlayer*						m_pPlayer = { nullptr };
	
private: // 좌상단 텍스트 다이얼로그 관리 (아이템 습득 등)
	CGameObject* m_pDialog = { nullptr };

public:
	void Insert_DialogQueue(const string &strMsg);
	void Initialize_Dialog(CGameObject* pObj);

public:
	virtual void Free();
};


#pragma region 폰트 매크로
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
// 노란 글씨 좌측 정렬, 어둡게
#define RENDER_TEXT_BOL_DARK(message, fX, fY, fSize) \
CUI_Manager::Get_Instance()->Set_ButtonBrightness(0.6f, CFont::BIGORANGE);\
CUI_Manager::Get_Instance()->Render_Text(message, CFont::BIGORANGE, CFont::LEFT, fX, fY, fSize)
#pragma endregion



END