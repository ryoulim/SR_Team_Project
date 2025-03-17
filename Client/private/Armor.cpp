// 내 클래스 이름 : Armor
// 부모 클래스 이름 : UI

#include "Armor.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CArmor::CArmor(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI{ pGraphic_Device }
{
}

CArmor::CArmor(const CArmor& Prototype)
	: CUI(Prototype)
{
}

HRESULT CArmor::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CArmor::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_GAMEPLAY;
	m_szTextureID = TEXT("Armor");
	m_szBufferType = TEXT("Rect");

	DESC Desc{};
	Desc.vScale = _float3(75.f, 75.f, 1.f);
	Desc.vInitPos = _float3(-(g_iWinSizeX / 2.f) + 208.f, -(g_iWinSizeY / 2.f) + Desc.vScale.y / 2.f, 1.f);
	m_fTextureNum = 1;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	return S_OK;
}

void CArmor::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CArmor::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CArmor::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CArmor::Render()
{
	RENDER_TEXT_BOL(200,
		-(g_iWinSizeX / 2.f) + m_vSize.x*3.4f,
		-(g_iWinSizeY / 2.f) + m_vSize.y / 2.f - 3.f, 1.1f);
	return __super::Render();
}

CArmor* CArmor::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CArmor* pInstance = new CArmor(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CArmor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CArmor::Clone(void* pArg)
{
	CArmor* pInstance = new CArmor(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CArmor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CArmor::Free()
{
	__super::Free();
}