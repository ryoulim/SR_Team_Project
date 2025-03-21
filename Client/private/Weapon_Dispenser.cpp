// 내 클래스 이름 : Weapon_Dispenser
// 부모 클래스 이름 : Weapon

#include "Weapon_Dispenser.h"

CWeapon_Dispenser::CWeapon_Dispenser(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CWeapon{ pGraphic_Device }
{
}

CWeapon_Dispenser::CWeapon_Dispenser(const CWeapon_Dispenser& Prototype)
	: CWeapon(Prototype)
{
}

HRESULT CWeapon_Dispenser::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWeapon_Dispenser::Initialize(void* pArg)
{
	m_szTextureID = TEXT("Weapon_Dispenser");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CWeapon_Dispenser::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CWeapon_Dispenser::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CWeapon_Dispenser::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CWeapon_Dispenser::Render()
{
	return __super::Render();
}

void CWeapon_Dispenser::Set_State(STATE State)
{
}

void CWeapon_Dispenser::Key_Input()
{
}

void CWeapon_Dispenser::Opening(_float fTimeDelta)
{
}

void CWeapon_Dispenser::Weak_Attack(_float fTimeDelta)
{
}

void CWeapon_Dispenser::Strong_Attack(_float fTimeDelta)
{
}

void CWeapon_Dispenser::Reload(_float fTimeDelta)
{
}

void CWeapon_Dispenser::Ending(_float fTimeDelta)
{
}

CWeapon_Dispenser* CWeapon_Dispenser::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CWeapon_Dispenser* pInstance = new CWeapon_Dispenser(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CWeapon_Dispenser");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWeapon_Dispenser::Clone(void* pArg)
{
	CWeapon_Dispenser* pInstance = new CWeapon_Dispenser(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CWeapon_Dispenser");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWeapon_Dispenser::Free()
{
	__super::Free();
}
