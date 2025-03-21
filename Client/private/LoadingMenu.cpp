// 내 클래스 이름 : LoadingMenu
// 부모 클래스 이름 : UI

#include "LoadingMenu.h"
#include "GameInstance.h"
#include <UI_Manager.h>

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
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, _wstring(TEXT("Prototype_Component_Texture_")) + L"Loading_Anim",
		TEXT("Com_Texture_Anim"), reinterpret_cast<CComponent**>(&m_pTextureComForLoading[LOADERTEX_ANIM]))))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, _wstring(TEXT("Prototype_Component_Texture_")) + L"Loading_BarBack",
		TEXT("Com_Texture_BarBack"), reinterpret_cast<CComponent**>(&m_pTextureComForLoading[LOADERTEX_BARBACK]))))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, _wstring(TEXT("Prototype_Component_Texture_")) + L"Loading_Bar",
		TEXT("Com_Texture_Bar"), reinterpret_cast<CComponent**>(&m_pTextureComForLoading[LOADERTEX_BAR]))))
		return E_FAIL;


	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, _wstring(TEXT("Prototype_Component_VIBuffer_")) + m_szBufferType,
		TEXT("Com_VIBuffer_Anim"), reinterpret_cast<CComponent**>(&m_pVIBufferComForLoading[LOADERTEX_ANIM]))))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, _wstring(TEXT("Prototype_Component_VIBuffer_")) + m_szBufferType,
		TEXT("Com_VIBuffer_BarBack"), reinterpret_cast<CComponent**>(&m_pVIBufferComForLoading[LOADERTEX_BARBACK]))))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, _wstring(TEXT("Prototype_Component_VIBuffer_")) + m_szBufferType,
		TEXT("Com_VIBuffer_Bar"), reinterpret_cast<CComponent**>(&m_pVIBufferComForLoading[LOADERTEX_BAR]))))
		return E_FAIL;


	CTransform::DESC pArg{};

	pArg.fRotationPerSec = 1.f;
	pArg.fSpeedPerSec = 1.f;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform_Anim"), reinterpret_cast<CComponent**>(&m_pTransformComForLoading[LOADERTEX_ANIM]), &pArg)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform_BarBack"), reinterpret_cast<CComponent**>(&m_pTransformComForLoading[LOADERTEX_BARBACK]), &pArg)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform_Bar"), reinterpret_cast<CComponent**>(&m_pTransformComForLoading[LOADERTEX_BAR]), &pArg)))
		return E_FAIL;
	

	return S_OK;
}

HRESULT CLoadingMenu::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CLoadingMenu::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_LOADING;
	m_szTextureID = TEXT("LoadingMenu");
	m_szBufferType = TEXT("Rect");

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, _wstring(TEXT("Prototype_Component_Texture_")) + m_szTextureID,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, _wstring(TEXT("Prototype_Component_VIBuffer_")) + m_szBufferType,
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), pArg)))
		return E_FAIL;

	Ready_LoadingComponents();
	if (FAILED(m_pTextureCom->Get_TextureSize(0, &m_vSize)))
		return E_FAIL;
	// 세로비 대로 이미지 맞춤
	m_vSize.x *= g_iWinSizeY / m_vSize.y; m_vSize.y = g_iWinSizeY;
	m_pTransformCom->Scaling(m_vSize);

	m_vPos = { 0.f,0.f,0.9f };
	m_fDepth = m_vPos.z;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vPos);
	m_pTransformComForLoading[LOADERTEX_ANIM]->Set_State(CTransform::STATE_POSITION, { (g_iWinSizeX*0.5f - 130.f), -(g_iWinSizeY * 0.5f) + 130.f, 0.1f });
	m_pTransformComForLoading[LOADERTEX_ANIM]->Scaling({ 128.f, 128.f, 1.f });
	m_pTransformComForLoading[LOADERTEX_BARBACK]->Set_State(CTransform::STATE_POSITION, { (g_iWinSizeX * 0.5f - 130.f), -(g_iWinSizeY * 0.5f) + 50.f, 0.1f });
	m_pTransformComForLoading[LOADERTEX_BARBACK]->Scaling({ 180.f, 28.f, 1.f });
	m_pTransformComForLoading[LOADERTEX_BAR]->Set_State(CTransform::STATE_POSITION, { (g_iWinSizeX * 0.5f - 130.f) - 126.f * 0.5f, -(g_iWinSizeY * 0.5f) + 50.f, 0.1f });
	m_pTransformComForLoading[LOADERTEX_BAR]->Scaling({ 1.f, 6.f, 1.f });
	return S_OK;
}

void CLoadingMenu::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CLoadingMenu::Update(_float fTimeDelta)
{
	if (m_fTextureNum < 1.f)
		m_fTextureNum += fTimeDelta;
	else
		m_fTextureNum += fTimeDelta * 15;
	if (m_fTextureNum >= 7.9f)
		m_fTextureNum = 0.f;

		
	__super::Update(fTimeDelta);
	return EVN_NONE;
}

void CLoadingMenu::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CLoadingMenu::Render()
{
   	if (FAILED(m_pTextureCom->Bind_Resource(0)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	_uint temp{};

	temp = _uint(m_fLoadingGauge / 0.2f);

	if (m_fLoadingGauge > 0.2f)
		CUI_Manager::Get_Instance(m_pGameInstance)->Render_Text("Press bar to skip Loading Level !!!", CFont::MEDIUMBLUE, CFont::LEFT, -600.f, -325.f + 26.f * (int(m_fLoadingGauge / 0.2f) - 1));
	if (m_fLoadingGauge > 0.4f)
		CUI_Manager::Get_Instance(m_pGameInstance)->Render_Text("Loading ... \"ION FURY\"", CFont::MEDIUMBLUE, CFont::LEFT, -600.f, -325.f + 26.f * (int(m_fLoadingGauge / 0.2f) - 2));
	if (m_fLoadingGauge > 0.6f)
		CUI_Manager::Get_Instance(m_pGameInstance)->Render_Text("And now I have no idea ", CFont::MEDIUMBLUE, CFont::LEFT, -600.f, -325.f + 26.f * (int(m_fLoadingGauge / 0.2f) - 3));
	if (m_fLoadingGauge > 0.8f)
		CUI_Manager::Get_Instance(m_pGameInstance)->Render_Text("HINT : hint hint hint hint hint hint.", CFont::MEDIUMBLUE, CFont::LEFT, -600.f, -325.f + 26.f * (int(m_fLoadingGauge / 0.2f) - 4));
	if (m_fLoadingGauge > 1.f)
	CUI_Manager::Get_Instance(m_pGameInstance)->Render_Text("Loading Complete !! ", CFont::MEDIUMBLUE, CFont::LEFT, -600.f, -325.f + 26.f * (int(m_fLoadingGauge / 0.2f) - 5));

	Render_Animation();
	Render_LoadingBar();

	return S_OK;
}

HRESULT CLoadingMenu::Render_Animation()
{
	if (FAILED(m_pTextureComForLoading[LOADERTEX_ANIM]->Bind_Resource(static_cast<_uint>(m_fTextureNum))))
		return E_FAIL;

	if (FAILED(m_pTransformComForLoading[LOADERTEX_ANIM]->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pVIBufferComForLoading[LOADERTEX_ANIM]->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferComForLoading[LOADERTEX_ANIM]->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoadingMenu::Render_LoadingBar()
{
	if (FAILED(m_pTextureComForLoading[LOADERTEX_BARBACK]->Bind_Resource(0)))
		return E_FAIL;

	if (FAILED(m_pTransformComForLoading[LOADERTEX_BARBACK]->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pVIBufferComForLoading[LOADERTEX_BARBACK]->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferComForLoading[LOADERTEX_BARBACK]->Render()))
		return E_FAIL;

	/********************************************************************************/

	if (FAILED(m_pTextureComForLoading[LOADERTEX_BAR]->Bind_Resource(0)))
		return E_FAIL;
	_float fsize = 126.f * m_fLoadingGauge;
	m_pTransformComForLoading[LOADERTEX_BAR]->Set_State(CTransform::STATE_POSITION,
		{ (g_iWinSizeX * 0.5f - 130.f) - (126.f  - 126.f * m_fLoadingGauge) * 0.5f,
		-(g_iWinSizeY * 0.5f) + 50.f, 0.01f });
	m_pTransformComForLoading[LOADERTEX_BAR]->Scaling({ fsize, 6.f, 1.f });

	if (FAILED(m_pTransformComForLoading[LOADERTEX_BAR]->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pVIBufferComForLoading[LOADERTEX_BAR]->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferComForLoading[LOADERTEX_BAR]->Render()))
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

	for (auto comp : m_pTextureComForLoading)
		Safe_Release(comp);
	for (auto comp : m_pTransformComForLoading)
		Safe_Release(comp);
	for (auto comp : m_pVIBufferComForLoading)
		Safe_Release(comp);

}