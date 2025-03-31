// 내 클래스 이름 : Interactive_Block
// 부모 클래스 이름 : Map

#include "Interactive_Block.h"

CInteractive_Block::CInteractive_Block(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMap{ pGraphic_Device }
{
}

CInteractive_Block::CInteractive_Block(const CInteractive_Block& Prototype)
	: CMap(Prototype)
{
}

HRESULT CInteractive_Block::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CInteractive_Block::Initialize(void* pArg)
{
	m_szTextureID = TEXT("Interactive_Block");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CInteractive_Block::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CInteractive_Block::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CInteractive_Block::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CInteractive_Block::Render()
{
	return __super::Render();
}

CInteractive_Block* CInteractive_Block::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CInteractive_Block* pInstance = new CInteractive_Block(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CInteractive_Block");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CInteractive_Block::Clone(void* pArg)
{
	CInteractive_Block* pInstance = new CInteractive_Block(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CInteractive_Block");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CInteractive_Block::Free()
{
	__super::Free();
}