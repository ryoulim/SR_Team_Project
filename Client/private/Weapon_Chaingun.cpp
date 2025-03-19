// 내 클래스 이름 : Weapon_Chaingun
// 부모 클래스 이름 : Weapon

#include "Weapon_Chaingun.h"

CWeapon_Chaingun::CWeapon_Chaingun(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CWeapon{ pGraphic_Device }
{
}

CWeapon_Chaingun::CWeapon_Chaingun(const CWeapon_Chaingun& Prototype)
	: CWeapon(Prototype)
{
}

HRESULT CWeapon_Chaingun::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWeapon_Chaingun::Initialize(void* pArg)
{
	m_szTextureID = TEXT("Weapon_Chaingun");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CWeapon_Chaingun::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CWeapon_Chaingun::Update(_float fTimeDelta)
{
	Update_Frame(fTimeDelta);
	return __super::Update(fTimeDelta);
}

void CWeapon_Chaingun::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CWeapon_Chaingun::Render()
{
	return __super::Render();
}

void CWeapon_Chaingun::Opening(_float fTimeDelta)
{
}

void CWeapon_Chaingun::Walk(_float fTimeDelta)
{
}

void CWeapon_Chaingun::Weak_Attack(_float fTimeDelta)
{
}

void CWeapon_Chaingun::Strong_Attack(_float fTimeDelta)
{
}

void CWeapon_Chaingun::Reload(_float fTimeDelta)
{
}

void CWeapon_Chaingun::Ending(_float fTimeDelta)
{
}

CWeapon_Chaingun* CWeapon_Chaingun::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CWeapon_Chaingun* pInstance = new CWeapon_Chaingun(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CWeapon_Chaingun");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWeapon_Chaingun::Clone(void* pArg)
{
	CWeapon_Chaingun* pInstance = new CWeapon_Chaingun(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CWeapon_Chaingun");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWeapon_Chaingun::Free()
{
	__super::Free();
}