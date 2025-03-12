// 내 클래스 이름 : Portrait
// 부모 클래스 이름 : UI

#include "Portrait.h"
#include "GameInstance.h"

CPortrait::CPortrait(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI{ pGraphic_Device }
{
}

CPortrait::CPortrait(const CPortrait& Prototype)
	: CUI(Prototype)
{
}

HRESULT CPortrait::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPortrait::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_GAMEPLAY;
	m_szTextureID = TEXT("Portrait");
	m_szBufferType = TEXT("Rect");

	//if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Font_ItemDialog"),
	//	LEVEL_STATIC, L"Layer_UI")))



	m_pFont = dynamic_cast<CFont*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_GAMEOBJECT, LEVEL_STATIC, TEXT("Prototype_GameObject_Font_ItemDialog")));
	if (nullptr == m_pFont)
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CPortrait::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CPortrait::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CPortrait::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CPortrait::Render()
{
	m_pFont->Render_Text(L"Obtained Loverboy AMMO * 12", CFont::MEDIUMBLUE, CFont::LEFT, -(g_iWinSizeX / 2.f) + 20.f, g_iWinSizeY / 2.f - 20.f);
	return __super::Render();
}

CPortrait* CPortrait::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPortrait* pInstance = new CPortrait(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPortrait");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPortrait::Clone(void* pArg)
{
	CPortrait* pInstance = new CPortrait(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CPortrait");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPortrait::Free()
{
	__super::Free();
	Safe_Release(m_pFont);
}