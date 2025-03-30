// 내 클래스 이름 : AnimeBlock
// 부모 클래스 이름 : Map

#include "AnimeBlock.h"
#include "GameInstance.h"

CAnimeBlock::CAnimeBlock(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMap{ pGraphic_Device }
{
}

CAnimeBlock::CAnimeBlock(const CAnimeBlock& Prototype)
	: CMap(Prototype)
{
}

HRESULT CAnimeBlock::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAnimeBlock::Initialize(void* pArg)
{
	m_szTextureID = TEXT("Test");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CAnimeBlock::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CAnimeBlock::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CAnimeBlock::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CAnimeBlock::Render()
{
	return __super::Render();
}

void CAnimeBlock::Move_Frame(_float fTimeDelta)
{
	m_fTextureIdx += m_fMaxFrame * fTimeDelta;

	if (m_fTextureIdx >= m_fMaxFrame)
		m_fTextureIdx = 0.f;
}

CAnimeBlock* CAnimeBlock::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CAnimeBlock* pInstance = new CAnimeBlock(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CAnimeBlock");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CAnimeBlock::Clone(void* pArg)
{
	CAnimeBlock* pInstance = new CAnimeBlock(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CAnimeBlock");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAnimeBlock::Free()
{
	__super::Free();
}