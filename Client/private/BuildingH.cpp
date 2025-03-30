#include "BuildingH.h"
#include "GameInstance.h"

CBuildingH::CBuildingH(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CStatue(pGraphic_Device)
{
}

CBuildingH::CBuildingH(const CBuildingH& Prototype)
	: CStatue(Prototype)
{
}

HRESULT CBuildingH::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBuildingH::Initialize(void* pArg)
{
	m_szTextureID = TEXT("BuildingH");
	m_szBufferType = TEXT("BuildingH");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CBuildingH::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CBuildingH::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CBuildingH::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CBuildingH::Render()
{
	//__super::LightOn();

	m_fTextureNum = 0.f;

	if (FAILED(__super::Render()))
		return E_FAIL;

	m_fTextureNum = 1.f;

	if (FAILED(m_pTextureCom->Bind_Resource(static_cast<_uint>(m_fTextureNum))))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render(CVIBuffer_BuildingH::LEFT)))
		return E_FAIL;

	m_fTextureNum = 2.f;

	if (FAILED(m_pTextureCom->Bind_Resource(static_cast<_uint>(m_fTextureNum))))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render(CVIBuffer_BuildingH::MIDDLE)))
		return E_FAIL;

	m_fTextureNum = 3.f;

	if (FAILED(m_pTextureCom->Bind_Resource(static_cast<_uint>(m_fTextureNum))))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render(CVIBuffer_BuildingH::RIGHT)))
		return E_FAIL;

	return S_OK;
}

CBuildingH* CBuildingH::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBuildingH* pInstance = new CBuildingH(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBuildingH");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBuildingH::Clone(void* pArg)
{
	CBuildingH* pInstance = new CBuildingH(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CBuildingH");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBuildingH::Free()
{
	__super::Free();
}
