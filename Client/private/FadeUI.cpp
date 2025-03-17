// 내 클래스 이름 : FadeUI
// 부모 클래스 이름 : UI

#include "FadeUI.h"
#include "GameInstance.h"

CFadeUI::CFadeUI(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI{ pGraphic_Device }
{
}

CFadeUI::CFadeUI(const CFadeUI& Prototype)
	: CUI(Prototype)
{
}

HRESULT CFadeUI::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFadeUI::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_STATIC;
	m_szTextureID = TEXT("FadeUI");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CFadeUI::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CFadeUI::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CFadeUI::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CFadeUI::Render()
{
	return __super::Render();
}

CFadeUI* CFadeUI::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CFadeUI* pInstance = new CFadeUI(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CFadeUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFadeUI::Clone(void* pArg)
{
	CFadeUI* pInstance = new CFadeUI(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CFadeUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFadeUI::Free()
{
	__super::Free();
}