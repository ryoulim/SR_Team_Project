// 내 클래스 이름 : Button
// 부모 클래스 이름 : UI

#include "Button.h"
#include "GameInstance.h"

CButton::CButton(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI{ pGraphic_Device }
{
}

CButton::CButton(const CButton& Prototype)
	: CUI(Prototype)
{
}

HRESULT CButton::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CButton::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_STATIC;
	m_szTextureID = TEXT("Button");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CButton::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CButton::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	return EVN_NONE;
}

void CButton::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CButton::Render()
{
	return __super::Render();
}

CButton* CButton::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CButton* pInstance = new CButton(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CButton::Clone(void* pArg)
{
	CButton* pInstance = new CButton(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CButton::Free()
{
	__super::Free();
}