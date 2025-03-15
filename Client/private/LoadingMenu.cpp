// 내 클래스 이름 : LoadingMenu
// 부모 클래스 이름 : UI

#include "LoadingMenu.h"
#include "GameInstance.h"

CLoadingMenu::CLoadingMenu(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI{ pGraphic_Device }
{
}

CLoadingMenu::CLoadingMenu(const CLoadingMenu& Prototype)
	: CUI(Prototype)
{
}

HRESULT CLoadingMenu::Ready_LoadingComponents()
{


	return S_OK;
}

HRESULT CLoadingMenu::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLoadingMenu::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_STATIC;
	m_szTextureID = TEXT("LoadingMenu");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CLoadingMenu::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CLoadingMenu::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	return EVN_NONE;
}

void CLoadingMenu::Late_Update(_float fTimeDelta)
{

	__super::Late_Update(fTimeDelta);
}

HRESULT CLoadingMenu::Render()
{
	if (FAILED(m_pTextureCom->Bind_Resource(static_cast<_uint>(m_fTextureNum))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Get_TextureSize(static_cast<_uint>(m_fTextureNum), &m_vSize)))
		return E_FAIL;


	// 세로비 대로 이미지 맞춤
	m_vSize.x *= g_iWinSizeY / m_vSize.y; m_vSize.y = g_iWinSizeY;


	m_pTransformCom->Scaling(m_vSize);

	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

CLoadingMenu* CLoadingMenu::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLoadingMenu* pInstance = new CLoadingMenu(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLoadingMenu");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLoadingMenu::Clone(void* pArg)
{
	CLoadingMenu* pInstance = new CLoadingMenu(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CLoadingMenu");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoadingMenu::Free()
{
	__super::Free();

}