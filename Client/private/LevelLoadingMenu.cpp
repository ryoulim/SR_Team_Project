// 내 클래스 이름 : LevelLoadingMenu
// 부모 클래스 이름 : UI

#include "LevelLoadingMenu.h"
#include "GameInstance.h"
#include <UI_Manager.h>

CLevelLoadingMenu::CLevelLoadingMenu(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI{ pGraphic_Device }
{
}

CLevelLoadingMenu::CLevelLoadingMenu(const CLevelLoadingMenu& Prototype)
	: CUI(Prototype)
{
}

HRESULT CLevelLoadingMenu::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLevelLoadingMenu::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_STATIC;
	m_szTextureID = TEXT("LevelLoadingMenu");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Get_TextureSize(0, &m_vSize)))
		return E_FAIL;
	// 세로비 대로 이미지 맞춤
	m_vSize.x *= g_iWinSizeY / m_vSize.y; m_vSize.y = g_iWinSizeY;
	m_pTransformCom->Scaling(m_vSize);

	m_fDepth = 10.f;

	return S_OK;
}

void CLevelLoadingMenu::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CLevelLoadingMenu::Update(_float fTimeDelta)
{
	if (m_fLoadingGauge > 0.25f)
		m_fTextureNum = 1.f;
	if (m_fLoadingGauge > 0.50f)
		m_fTextureNum = 2.f;
	if (m_fLoadingGauge > 0.75f)
		m_fTextureNum = 3.f;
	if (m_fLoadingGauge > 0.9f)
	{
		if (!m_isFinished)
		{
			CUI_Manager::Get_Instance(m_pGameInstance)->Fade_Out();
			m_isFinished = true;
		}
	}

	return __super::Update(fTimeDelta);
}

void CLevelLoadingMenu::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CLevelLoadingMenu::Render()
{
	return __super::Render();
}

CLevelLoadingMenu* CLevelLoadingMenu::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevelLoadingMenu* pInstance = new CLevelLoadingMenu(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLevelLoadingMenu");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLevelLoadingMenu::Clone(void* pArg)
{
	CLevelLoadingMenu* pInstance = new CLevelLoadingMenu(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CLevelLoadingMenu");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevelLoadingMenu::Free()
{
	__super::Free();
}