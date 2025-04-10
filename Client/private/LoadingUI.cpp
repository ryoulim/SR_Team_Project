// 내 클래스 이름 : LoadingUI
// 부모 클래스 이름 : UI

#include "LoadingUI.h"
#include "GameInstance.h"
#include "LoadingCutscene.h"

CLoadingUI::CLoadingUI(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI{ pGraphic_Device }
{
}

CLoadingUI::CLoadingUI(const CLoadingUI& Prototype)
	: CUI(Prototype)
{
}

HRESULT CLoadingUI::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLoadingUI::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_STATIC;
	m_szTextureID = TEXT("LoadingUI");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	DESC* pDesc = static_cast<DESC*>(pArg);
	m_eNextLevel = pDesc->eNextLevel;
	m_eCurLevel = pDesc->eCurLevel;

	Initialize_LoadingCutscene(pArg);

	return S_OK;
}

HRESULT CLoadingUI::Initialize_LoadingCutscene(void* pArg)
{
	_wstring strCutsceneType;
	switch (m_eNextLevel)
	{
	case Client::LEVEL_LOGO:
		strCutsceneType = L"ToLogo";
		break;
	case Client::LEVEL_GAMEPLAY:
		strCutsceneType = L"ToBoss";
		break;
	case Client::LEVEL_RACEFIRST:
		if (LEVEL_RACING(m_eCurLevel))
			strCutsceneType = L"OnRacing";
		else
			strCutsceneType = L"ToRace";
		break;
	case Client::LEVEL_RACESECOND:
		strCutsceneType = L"OnRacing";
		break;
	case Client::LEVEL_RACETHIRD:
		strCutsceneType = L"OnRacing";
		break;
	case Client::LEVEL_INDOOR:
		strCutsceneType = L"ToIn";
		break;
	case Client::LEVEL_OUTDOOR:
		strCutsceneType = L"ToOut";
		break;
	case Client::LEVEL_UNDERGROUND:
		strCutsceneType = L"ToUnderground";
		break;

	default:
		MSG_BOX("이게 먼 레벨이죠?");
		return E_FAIL;
	}

	CGameObject* pObj = { nullptr };
	CUI::DESC pDesc = {};

	if (FAILED(m_pGameInstance->Add_GameObjectReturn(LEVEL_STATIC, _wstring(L"Prototype_GameObject_Loading_") + strCutsceneType,
		m_eLevelID, L"Layer_LoadingUI", &pObj, &pDesc)))
		return E_FAIL;
	if (m_pLoadingCutscene != nullptr)
	{
		m_pLoadingCutscene = static_cast<CLoadingCutscene*>(pObj);
		Safe_AddRef(m_pLoadingCutscene);
	}
	else
	{
		MSG_BOX("바보래요");
		return E_FAIL;
	}

	return S_OK;
}

void CLoadingUI::Priority_Update(_float fTimeDelta)
{
	if (m_pLoadingCutscene != nullptr)
		m_pLoadingCutscene->Priority_Update(fTimeDelta);
}

EVENT CLoadingUI::Update(_float fTimeDelta)
{
	if (m_pLoadingCutscene != nullptr)
		return m_pLoadingCutscene->Update(fTimeDelta);
	return EVN_NONE;
}

void CLoadingUI::Late_Update(_float fTimeDelta)
{
	if (m_pLoadingCutscene != nullptr)
		m_pLoadingCutscene->Late_Update(fTimeDelta);
}

HRESULT CLoadingUI::Render()
{
	if (m_pLoadingCutscene != nullptr)
		return m_pLoadingCutscene->Render();
	return S_OK;
}

CLoadingUI* CLoadingUI::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLoadingUI* pInstance = new CLoadingUI(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLoadingUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLoadingUI::Clone(void* pArg)
{
	CLoadingUI* pInstance = new CLoadingUI(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CLoadingUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoadingUI::Free()
{
	Safe_Release(m_pLoadingCutscene);
	__super::Free();
}