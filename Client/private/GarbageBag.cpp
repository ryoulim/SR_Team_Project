// 내 클래스 이름 : GarbageBag
// 부모 클래스 이름 : Interactive_Block

#include "GarbageBag.h"
#include "GameInstance.h"

CGarbageBag::CGarbageBag(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CInteractive_Block{ pGraphic_Device }
{
}

CGarbageBag::CGarbageBag(const CGarbageBag& Prototype)
	: CInteractive_Block(Prototype)
{
}

HRESULT CGarbageBag::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGarbageBag::Initialize(void* pArg)
{
	m_szTextureID = TEXT("GarbageBag");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CGarbageBag::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CGarbageBag::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CGarbageBag::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CGarbageBag::Render()
{
	return __super::Render();
}

CGarbageBag* CGarbageBag::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGarbageBag* pInstance = new CGarbageBag(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CGarbageBag");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGarbageBag::Clone(void* pArg)
{
	CGarbageBag* pInstance = new CGarbageBag(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CGarbageBag");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGarbageBag::Free()
{
	__super::Free();
}