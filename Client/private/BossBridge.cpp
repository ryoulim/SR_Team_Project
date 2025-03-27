// 내 클래스 이름 : BossBridge
// 부모 클래스 이름 : Map

#include "BossBridge.h"
#include "GameInstance.h"

CBossBridge::CBossBridge(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMap{ pGraphic_Device }
{
}

CBossBridge::CBossBridge(const CBossBridge& Prototype)
	: CMap(Prototype)
{
}

HRESULT CBossBridge::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBossBridge::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_GAMEPLAY;
	m_szTextureID = TEXT("Test");
	m_szBufferType = TEXT("Cube");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CBossBridge::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CBossBridge::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CBossBridge::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CBossBridge::Render()
{
	return __super::Render();
}

CBossBridge* CBossBridge::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBossBridge* pInstance = new CBossBridge(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBossBridge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBossBridge::Clone(void* pArg)
{
	CBossBridge* pInstance = new CBossBridge(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CBossBridge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBossBridge::Free()
{
	__super::Free();
}