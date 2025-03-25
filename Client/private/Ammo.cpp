// 내 클래스 이름 : Ammo
// 부모 클래스 이름 : UI

#include "Ammo.h"
#include "GameInstance.h"
#include "UI_Manager.h"

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
	Desc.vScale = _float3(48.f, 54.f, 1.f);
	Desc.vInitPos = _float3((g_iWinSizeX / 2.f) - 40.f, -(g_iWinSizeY / 2.f) + Desc.vScale.y / 2.f + 7.f, 0.f);
	//Desc.vScale = _float3(1.f, 1.f, 1.f);
	//Desc.vInitPos = _float3(0.f,0.f, 0.f);
	m_fTextureNum = 0;

	m_fDepth = 3.f;

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
		if (tmp > CWeapon::TYPE::CLUSTERPUCK)
			tmp = 0;
		m_eAmmoType = CWeapon::TYPE(tmp);
	}
	m_fTextureNum = static_cast<_float>(m_eAmmoType);
	switch (m_eAmmoType)
	{
	case Client::CWeapon::TYPE::LOVERBOY:
		m_vSize = _float3{ 21.f,24.f,1.f } * 2.f;
		break;
	case Client::CWeapon::TYPE::DISPERSER_SHELL:
		m_vSize = _float3{ 32.f,32.f,1.f } * 1.5f;
		break;
	case Client::CWeapon::TYPE::DISPERSER_GRENADE:
		m_vSize = _float3{ 32.f,34.f,1.f } * 1.41f;
		break;
	case Client::CWeapon::TYPE::PENETRATOR:
		m_vSize = _float3{ 37.f,37.f,1.f } * 1.29f;
		break;
	case Client::CWeapon::TYPE::CHAINGUN:
		m_vSize = _float3{ 75.f,45.f,1.f } * 1.06f;
		break;
	case Client::CWeapon::TYPE::BOWLINGBOMB:
		m_vSize = _float3{ 48.f,54.f,1.f } * 0.88f;
		break;
	case Client::CWeapon::TYPE::IONBOW:
		m_vSize = _float3{ 26.f,29.f,1.f } * 1.65f;
		break;
	case Client::CWeapon::TYPE::CLUSTERPUCK:
		m_vSize = _float3{ 64.f,64.f,1.f } * 0.75f;
		break;
	case Client::CWeapon::TYPE::AMMO_END:
		break;
	default:
		break;
	}
	m_vPos.x = (g_iWinSizeX / 2.f) - m_vSize.x * 0.5f - 15.f;

	return __super::Update(fTimeDelta);
}

void CAmmo::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CAmmo::Render()
{
	if (m_pAmmoInfo != nullptr)
	{
		RENDER_TEXT_BOC(m_pAmmoInfo->iCurAmmo,
			(g_iWinSizeX / 2.f) - m_vSize.x - 70.f,
			-(g_iWinSizeY / 2.f) + m_vSize.y / 2.f + 9.f, 1.1f);
		m_eAmmoType = m_pAmmoInfo->eType;
	}

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vPos);
	m_pTransformCom->Scaling(m_vSize);
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