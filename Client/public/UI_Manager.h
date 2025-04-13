// �� Ŭ���� �̸� : UI_Manager
// �θ� Ŭ���� �̸� : Base

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

	/* �ؽ�Ʈ ��� */
public:
	HRESULT Render_Text(const string& _text, CFont::FONTTYPE _type, CFont::FONTALIGN _align, _float _posX, _float _posY, _float vSizeMul = 1.f, CShader* pShader = nullptr);
	HRESULT Render_Text(const _int _val, CFont::FONTTYPE _type, CFont::FONTALIGN _align, _float _posX, _float _posY, _float vSizeMul = 1.f, CShader* pShader = nullptr);
	HRESULT Render_Text_Colored(const string& _text, CFont::FONTTYPE _type, CFont::FONTALIGN _align, _float _posX, _float _posY, _float _hueShift, _float vSizeMul = 1.f);
	void	Set_ButtonBrightness(_float val, CFont::FONTTYPE _type) { m_Fonts[_type]->Set_Brightness(val); }

	/* ���̵� �� �ƿ� */
public:
	void	Fade_In();
	void	Fade_Out(LEVEL eLevelID = LEVEL_STATIC);

	/* ���� �Լ� */
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

	/* ��Ʈ ���� �� ������ */
private:
	HRESULT	Initialize_Font();
	
private:
										/* ��Ʈ */
	class CFont*						m_Fonts[CFont::FONT_END] = {nullptr};
										/* UI */ 
	array<class CGameObject*, GUI_END>	m_GameUIs = { nullptr };
										/* ���̽� */
	class CGameObject*					m_pRacingUI = { nullptr };
										/* ���� HP �� */
	class CGameObject*					m_pBossHPUI = { nullptr };

private: // �»�� �ؽ�Ʈ ���̾�α� ���� (������ ���� ��)
	CGameObject*						m_pDialog = { nullptr };

public:
	void Insert_DialogQueue(const string &strMsg);
	void Initialize_Dialog(CGameObject* pObj);

public:
	virtual void Free();
};


#pragma region ��Ʈ ��ũ��
// Render_Text BigOrangeCenter
// ��� �۾� �߾� ����
#define RENDER_TEXT_BOC(message, fX, fY, fSize) \
CUI_Manager::Get_Instance()->Set_ButtonBrightness(1.f, CFont::BIGORANGE);\
CUI_Manager::Get_Instance()->Render_Text(message, CFont::BIGORANGE, CFont::CENTER, fX, fY, fSize)
// Render_Text BigOrangeLeft
// ��� �۾� ���� ����
#define RENDER_TEXT_BOL(message, fX, fY, fSize) \
CUI_Manager::Get_Instance()->Set_ButtonBrightness(1.f, CFont::BIGORANGE);\
CUI_Manager::Get_Instance()->Render_Text(message, CFont::BIGORANGE, CFont::LEFT, fX, fY, fSize)

// Render_Text BigOrangeCenter Dark
// ��� �۾� �߾� ����, ��Ӱ�
#define RENDER_TEXT_BOC_DARK(message, fX, fY, fSize) \
CUI_Manager::Get_Instance()->Set_ButtonBrightness(0.6f, CFont::BIGORANGE);\
CUI_Manager::Get_Instance()->Render_Text(message, CFont::BIGORANGE, CFont::CENTER, fX, fY, fSize)
// Render_Text BigOrangeLeft Dark
// ��� �۾� ���� ����, ��Ӱ�
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