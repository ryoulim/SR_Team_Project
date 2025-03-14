#include "Signboard.h"

CSignboard::CSignboard(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CStatue { pGraphic_Device }
{
}

CSignboard::CSignboard(const CSignboard& Prototype)
	: CStatue(Prototype)
{
}

HRESULT CSignboard::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSignboard::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_GAMEPLAY;
	m_szTextureID = TEXT("Signboard");
	m_szBufferType = TEXT("Signboard");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CSignboard::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CSignboard::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CSignboard::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CSignboard::Render()
{
	return __super::Render();
}

CSignboard* CSignboard::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSignboard* pInstance = new CSignboard(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSignboard");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSignboard::Clone(void* pArg)
{
	CSignboard* pInstance = new CSignboard(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CSignboard");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSignboard::Free()
{
	__super::Free();
}
