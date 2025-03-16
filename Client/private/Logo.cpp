// 내 클래스 이름 : Logo
// 부모 클래스 이름 : UI

#include "Logo.h"
#include "GameInstance.h"

CLogo::CLogo(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI{ pGraphic_Device }
{
}

CLogo::CLogo(const CLogo& Prototype)
	: CUI(Prototype)
{
}

HRESULT CLogo::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLogo::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_STATIC;
	m_szTextureID = TEXT("Logo");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CLogo::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CLogo::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CLogo::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CLogo::Render()
{
	return __super::Render();
}

CLogo* CLogo::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLogo* pInstance = new CLogo(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLogo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLogo::Clone(void* pArg)
{
	CLogo* pInstance = new CLogo(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CLogo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLogo::Free()
{
	__super::Free();
}