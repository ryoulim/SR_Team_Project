// 내 클래스 이름 : MainMenu
// 부모 클래스 이름 : UI

#include "MainMenu.h"
#include "GameInstance.h"
#include <UI_Manager.h>

CMainMenu::CMainMenu(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI{ pGraphic_Device }
{
}

CMainMenu::CMainMenu(const CMainMenu& Prototype)
	: CUI(Prototype)
{
}

HRESULT CMainMenu::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMainMenu::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_LOGO;
	m_szTextureID = TEXT("MainMenu");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Get_TextureSize(0, &m_vSize)))
		return E_FAIL;

	// 세로비 대로 이미지 맞춤
	m_vSize.x *= g_iWinSizeY / m_vSize.y; m_vSize.y = g_iWinSizeY;
	m_fDepth = 10.f;

	m_pTransformCom->Scaling(m_vSize);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 0.f,0.f,0.9f });

	if (testbool == false)
	{
		CUI_Manager::Get_Instance(m_pGameInstance)->Fade_In();
		testbool = true;
	}

	return S_OK;
}

void CMainMenu::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CMainMenu::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	return EVN_NONE;
}

void CMainMenu::Late_Update(_float fTimeDelta)
{

	__super::Late_Update(fTimeDelta);
}

HRESULT CMainMenu::Render()
{

	if (FAILED(m_pTextureCom->Bind_Resource(static_cast<_uint>(m_fTextureNum))))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

CMainMenu* CMainMenu::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMainMenu* pInstance = new CMainMenu(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMainMenu");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMainMenu::Clone(void* pArg)
{
	CMainMenu* pInstance = new CMainMenu(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CMainMenu");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainMenu::Free()
{
	__super::Free();

}