// 내 클래스 이름 : Item_Ammo
// 부모 클래스 이름 : Item

#include "Item_Ammo.h"
#include "GameInstance.h"

CItem_Ammo::CItem_Ammo(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CItem{ pGraphic_Device }
{
}

CItem_Ammo::CItem_Ammo(const CItem_Ammo& Prototype)
	: CItem(Prototype)
{
}

HRESULT CItem_Ammo::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CItem_Ammo::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_STATIC;
	m_szTextureID = TEXT("Item_Ammo");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CItem_Ammo::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CItem_Ammo::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
	__super::Update(fTimeDelta);
	return EVN_NONE;
}

void CItem_Ammo::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CItem_Ammo::Render()
{

	__super::Render();
	m_pTransformCom->Billboard();

	return S_OK;
}

CItem_Ammo* CItem_Ammo::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CItem_Ammo* pInstance = new CItem_Ammo(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CItem_Ammo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CItem_Ammo::Clone(void* pArg)
{
	CItem_Ammo* pInstance = new CItem_Ammo(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CItem_Ammo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CItem_Ammo::Free()
{
	__super::Free();
}