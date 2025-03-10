// 내 클래스 이름 : Aim
// 부모 클래스 이름 : UI

#include "Aim.h"
#include "GameInstance.h"

CAim::CAim(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI{ pGraphic_Device }
{
}

CAim::CAim(const CAim& Prototype)
	: CUI(Prototype)
{
}

HRESULT CAim::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAim::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_GAMEPLAY;
	m_szTextureID = TEXT("Aim");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CAim::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CAim::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CAim::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CAim::Render()
{
	return __super::Render();
}

CAim* CAim::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CAim* pInstance = new CAim(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CAim");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CAim::Clone(void* pArg)
{
	CAim* pInstance = new CAim(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CAim");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAim::Free()
{
	__super::Free();
}