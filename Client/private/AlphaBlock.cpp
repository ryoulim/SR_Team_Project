// 내 클래스 이름 : AlphaBlock
// 부모 클래스 이름 : Map

#include "AlphaBlock.h"
#include "GameInstance.h"

CAlphaBlock::CAlphaBlock(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMap{ pGraphic_Device }
{
}

CAlphaBlock::CAlphaBlock(const CAlphaBlock& Prototype)
	: CMap(Prototype)
{
}

HRESULT CAlphaBlock::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAlphaBlock::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_GAMEPLAY;
	m_szTextureID = TEXT("Test");
	m_szBufferType = TEXT("Cube");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CAlphaBlock::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CAlphaBlock::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CAlphaBlock::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CAlphaBlock::Render()
{
	return __super::Render();
}

CAlphaBlock* CAlphaBlock::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CAlphaBlock* pInstance = new CAlphaBlock(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CAlphaBlock");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CAlphaBlock::Clone(void* pArg)
{
	CAlphaBlock* pInstance = new CAlphaBlock(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CAlphaBlock");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAlphaBlock::Free()
{
	__super::Free();
}