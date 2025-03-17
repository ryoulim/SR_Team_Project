// 내 클래스 이름 : Ammo
// 부모 클래스 이름 : UI

#include "Ammo.h"
#include "GameInstance.h"

CAmmo::CAmmo(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI{ pGraphic_Device }
{
}

CAmmo::CAmmo(const CAmmo& Prototype)
	: CUI(Prototype)
{
}

HRESULT CAmmo::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAmmo::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_GAMEPLAY;
	m_szTextureID = TEXT("Ammo");
	m_szBufferType = TEXT("Rect");

	DESC Desc{};
	Desc.vScale = _float3(48.f, 55.f, 1.f);
	Desc.vInitPos = _float3((g_iWinSizeX / 2.f) - 40.f, -(g_iWinSizeY / 2.f) + Desc.vScale.y / 2.f + 10.f, 1.f);
	m_fTextureNum = 0;


	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	return S_OK;
}

void CAmmo::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CAmmo::Update(_float fTimeDelta)
{
	_uint tmp = m_eAmmoType;
	if (GetKeyState('3') & 0x8000)
	{
		++tmp;
		if (tmp > CLUSTERPUCK)
			tmp = 0;
		m_eAmmoType = AMMOTYPE(tmp);
	}
	m_fTextureNum = static_cast<_float>(m_eAmmoType);
	return __super::Update(fTimeDelta);
}

void CAmmo::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CAmmo::Render()
{
	return __super::Render();
}

CAmmo* CAmmo::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CAmmo* pInstance = new CAmmo(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CAmmo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CAmmo::Clone(void* pArg)
{
	CAmmo* pInstance = new CAmmo(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CAmmo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAmmo::Free()
{
	__super::Free();
}