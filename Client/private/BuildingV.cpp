#include "BuildingV.h"
#include "GameInstance.h"

CBuildingV::CBuildingV(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CStatue(pGraphic_Device)
{
}

CBuildingV::CBuildingV(const CBuildingV& Prototype)
	: CStatue(Prototype)
{
}

HRESULT CBuildingV::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBuildingV::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_RACE;
	m_szTextureID = TEXT("BuildingV");
	m_szBufferType = TEXT("BuildingV");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pTransformCom->Scaling(_float3(800.f, 500.f, 720.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(470.f, 350.f, 700.f));

	return S_OK;
}

void CBuildingV::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CBuildingV::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CBuildingV::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CBuildingV::Render()
{
	//__super::LightOn();
	return __super::Render();
}

CBuildingV* CBuildingV::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBuildingV* pInstance = new CBuildingV(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBuildingV");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBuildingV::Clone(void* pArg)
{
	CBuildingV* pInstance = new CBuildingV(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CBuildingV");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBuildingV::Free()
{
	__super::Free();
}
