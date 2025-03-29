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
	DESC* pDesc = static_cast<DESC*>(pArg);

	m_eLevelID = pDesc->eNextLevel;
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
#pragma region ¾çÂÊ ±âµÕ
	m_fTextureNum = 0.f;

	if (FAILED(__super::Render()))
		return E_FAIL;
#pragma endregion

#pragma region ´ëÇü ÆÇ³Ú »çÀÌµå
	m_fTextureNum = 1.f;

	if (FAILED(m_pTextureCom->Bind_Resource(static_cast<_uint>(m_fTextureNum))))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render(CVIBuffer_Signboard::PANELSIDE)))
		return E_FAIL;
#pragma endregion

#pragma region ´ëÇü ÆÇ³Ú
	m_fTextureNum = 2.f;

	if (FAILED(m_pTextureCom->Bind_Resource(static_cast<_uint>(m_fTextureNum))))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render(CVIBuffer_Signboard::PANEL)))
		return E_FAIL;
#pragma endregion

#pragma region ¹ßÆÇ
	m_fTextureNum = 3.f;

	if (FAILED(m_pTextureCom->Bind_Resource(static_cast<_uint>(m_fTextureNum))))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render(CVIBuffer_Signboard::FOOTSTONE)))
		return E_FAIL;
#pragma endregion

#pragma region ¹ßÆÇ ºø¸é
	m_fTextureNum = 4.f;

	if (FAILED(m_pTextureCom->Bind_Resource(static_cast<_uint>(m_fTextureNum))))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render(CVIBuffer_Signboard::CONNECTION)))
		return E_FAIL;
#pragma endregion
	return S_OK;
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
