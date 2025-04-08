// 내 클래스 이름 : RacingPanel
// 부모 클래스 이름 : UI

#include "RacingPanel.h"
#include "GameInstance.h"

CRacingPanel::CRacingPanel(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI{ pGraphic_Device }
{
}

CRacingPanel::CRacingPanel(const CRacingPanel& Prototype)
	: CUI(Prototype)
{
}

HRESULT CRacingPanel::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRacingPanel::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_STATIC;
	m_szTextureID = TEXT("RacingPanel");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CRacingPanel::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CRacingPanel::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CRacingPanel::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CRacingPanel::Render()
{
	return __super::Render();
}

CRacingPanel* CRacingPanel::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRacingPanel* pInstance = new CRacingPanel(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CRacingPanel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CRacingPanel::Clone(void* pArg)
{
	CRacingPanel* pInstance = new CRacingPanel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CRacingPanel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRacingPanel::Free()
{
	__super::Free();
}