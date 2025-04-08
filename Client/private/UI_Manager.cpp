// 내 클래스 이름 : UI_Manager
// 부모 클래스 이름 : Base

#include "UI_Manager.h"
#include "Client_Defines.h"
#include "Font_MediumBlue.h"
#include "FadeUI.h"
#include "Player.h"
#include "Portrait.h"
#include "Armor.h"
#include <Aim.h>
#include <ItemDialog.h>

CUI_Manager* CUI_Manager::m_pInstance = nullptr;

CUI_Manager* CUI_Manager::Get_Instance()
{
	if (nullptr == m_pInstance)
	{
		m_pInstance = new CUI_Manager;
		m_pInstance->Initialize();
	}
	return m_pInstance;
}

unsigned int CUI_Manager::Destroy_Instance()	
{						
	unsigned int iRefCnt = {};						
	if (nullptr != m_pInstance)						
	{												
		//m_pInstance->Free();
		iRefCnt = m_pInstance->Release();				
	if (0 == iRefCnt)								
		m_pInstance = nullptr;						
	}												
	return iRefCnt;									
}

CUI_Manager::CUI_Manager()
{
}

HRESULT CUI_Manager::Initialize()
{
	m_pGameInstance = CGameInstance::Get_Instance();
	Initialize_Font();
	//Initialize_FadeUI();

	return S_OK;
}

void CUI_Manager::Priority_Update(_float fTimeDelta)
{
}

void CUI_Manager::Update(_float fTimeDelta)
{
}

void CUI_Manager::Late_Update(_float fTimeDelta)
{
}

HRESULT CUI_Manager::Render()
{
	return S_OK;
}

HRESULT CUI_Manager::Render_Text(const string& _text, CFont::FONTTYPE _type, CFont::FONTALIGN _align, _float _posX, _float _posY, _float vSizeMul, CShader* pShader)
{
	if (m_Fonts[_type] == nullptr)
		return E_FAIL;
	return m_Fonts[_type]->Render_Text(_text, _align, _posX, _posY, vSizeMul, pShader);
}

HRESULT CUI_Manager::Render_Text(const _int _val, CFont::FONTTYPE _type, CFont::FONTALIGN _align, _float _posX, _float _posY, _float vSizeMul, CShader* pShader)
{
	if (m_Fonts[_type] == nullptr)
		return E_FAIL;
	return m_Fonts[_type]->Render_Text(_val, _align, _posX, _posY, vSizeMul, pShader);
}

void CUI_Manager::Fade_In()
{
	CFadeUI::DESC desc;
	desc.eLevelID = LEVEL_STATIC;
	desc.fRotationPerSec = 0.f;
	desc.fSpeedPerSec = 0.f;
	desc.vInitPos = { 0.f,0.f,0.f };
	desc.vScale = { 1280.f,720.f,1.f };
	desc.m_isFadeIn = true;
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_FadeUI"),
		LEVEL_STATIC, L"Layer_FadeUI",&desc )))
		return;
}

void CUI_Manager::Fade_Out()
{
	CFadeUI::DESC desc;
	desc.eLevelID = LEVEL_STATIC;
	desc.fRotationPerSec = 0.f;
	desc.fSpeedPerSec = 0.f;
	desc.vInitPos = { 0.f,0.f,0.f };
	desc.vScale = { 1280.f,720.f,1.f };
	desc.m_isFadeIn = false;
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_FadeUI"),
		LEVEL_STATIC, L"Layer_FadeUI", &desc)))
		return;
}

HRESULT CUI_Manager::Initialize_GamePlayUI(LEVEL eLevelID)
{
	m_GameUIs[GUI_AIM]		= m_pGameInstance->Find_Object(eLevelID, L"Layer_UI", GUI_AIM);
	m_GameUIs[GUI_PORTRAIT] = m_pGameInstance->Find_Object(eLevelID, L"Layer_UI", GUI_PORTRAIT);
	m_GameUIs[GUI_ARMOR]	= m_pGameInstance->Find_Object(eLevelID, L"Layer_UI", GUI_ARMOR);
	m_GameUIs[GUI_AMMO]		= m_pGameInstance->Find_Object(eLevelID, L"Layer_UI", GUI_AMMO);

	for (size_t i = 0; i < GUI_END; i++)
	{
		if (nullptr == m_GameUIs[i])
			return E_FAIL;
		//Safe_AddRef(m_GameUIs[i]);
	}

	return S_OK;
}

HRESULT CUI_Manager::Initialize_Player()
{
	m_pPlayer = dynamic_cast<CPlayer*>(GET_PLAYER);
	if (nullptr == m_pPlayer)
		return E_FAIL;
	//Safe_AddRef(m_pPlayer);
	return S_OK;
}

HRESULT CUI_Manager::Clear_GamePlayUI()
{
	for (size_t i = 0; i < GUI_END; i++)
	{
		//Safe_Release(m_GameUIs[i]);
		m_GameUIs[i] = nullptr;
	}
	//Safe_Release(m_pPlayer);
	m_pPlayer = nullptr;
	return S_OK;
}


HRESULT CUI_Manager::Change_Weapon(const CWeapon::AMMOINFO* pAmmoInfo)
{
	dynamic_cast<CAmmo*>(m_GameUIs[GUI_AMMO])->Set_Ammo(pAmmoInfo);
	static_cast<CAim*>(m_GameUIs[GUI_AIM])->Set_Ammo(pAmmoInfo);

	return S_OK;
}

HRESULT CUI_Manager::Init_UI_To_Player(const CPlayer::INFO* pPlayerInfo)
{
	/* 플레이어 합치고 다시 키기 */
	static_cast<CPortrait*>(m_GameUIs[GUI_PORTRAIT])->Set_Info(pPlayerInfo);
	static_cast<CArmor*>(m_GameUIs[GUI_ARMOR])->Set_Info(pPlayerInfo);
	return S_OK;
}

HRESULT CUI_Manager::Set_Face(CPortrait::PORTRAITSTATUS eStatus)
{
	static_cast<CPortrait*>(m_GameUIs[GUI_PORTRAIT])->Set_Face(eStatus);
	return S_OK;
}

HRESULT CUI_Manager::Initialize_Font()
{
	CUI::DESC Desc{};
	Desc.eLevelID = LEVEL_STATIC;
	Desc.fDepth = _float(UI_FONT);
	Desc.vScale = _float3(1.f, 1.f, 1.f);
	Desc.vInitPos = _float3(0.1f, 0.1f, 0.1f);

	m_Fonts[CFont::MEDIUMBLUE] = dynamic_cast<CFont*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_GAMEOBJECT, LEVEL_STATIC, TEXT("Prototype_GameObject_Font_MediumBlue"), &Desc));
	if (nullptr == m_Fonts[CFont::MEDIUMBLUE])
		return E_FAIL;
	m_Fonts[CFont::BIGORANGE] = dynamic_cast<CFont*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_GAMEOBJECT, LEVEL_STATIC, TEXT("Prototype_GameObject_Font_BigOrange"), &Desc));
	if (nullptr == m_Fonts[CFont::BIGORANGE])
		return E_FAIL;
	return S_OK;
}

void CUI_Manager::Insert_DialogQueue(const string& strMsg)
{
	if (m_pDialog == nullptr)
	{
		MSG_BOX("다이얼로그 초기화가 안 된 것 같은디요?? ");
		return;
	}
	static_cast<CItemDialog*>(m_pDialog)->Insert_DialogQueue(strMsg);
}

void CUI_Manager::Initialize_Dialog(CGameObject* pObj)
{
	m_pDialog = pObj;
	Safe_AddRef(m_pDialog);
}

void CUI_Manager::Free()
{ 
	__super::Free();

 	for (size_t i = 0; i < CFont::FONT_END; i++)
	{
		if (m_Fonts[i] != nullptr)
			Safe_Release(m_Fonts[i]);
	} 
	Clear_GamePlayUI();
	Safe_Release(m_pDialog);
	Safe_Release(m_pGameInstance);


} 