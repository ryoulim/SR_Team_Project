// 내 클래스 이름 : Item_Ammo_Loverboy
// 부모 클래스 이름 : Item_Ammo

#include "Item_Ammo_Loverboy.h"
#include "GameInstance.h"

CItem_Ammo_Loverboy::CItem_Ammo_Loverboy(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CItem_Ammo{ pGraphic_Device }
{
}

CItem_Ammo_Loverboy::CItem_Ammo_Loverboy(const CItem_Ammo_Loverboy& Prototype)
	: CItem_Ammo(Prototype)
{
}

HRESULT CItem_Ammo_Loverboy::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CItem_Ammo_Loverboy::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_STATIC;
	m_szTextureID = TEXT("Item_Ammo_Loverboy");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CItem_Ammo_Loverboy::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CItem_Ammo_Loverboy::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	return EVN_NONE;
}

void CItem_Ammo_Loverboy::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CItem_Ammo_Loverboy::Render()
{
	return __super::Render();
}

CItem_Ammo_Loverboy* CItem_Ammo_Loverboy::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CItem_Ammo_Loverboy* pInstance = new CItem_Ammo_Loverboy(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CItem_Ammo_Loverboy");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CItem_Ammo_Loverboy::Clone(void* pArg)
{
	CItem_Ammo_Loverboy* pInstance = new CItem_Ammo_Loverboy(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CItem_Ammo_Loverboy");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CItem_Ammo_Loverboy::Free()
{
	__super::Free();
}