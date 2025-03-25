#include "BuildingU.h"
#include "GameInstance.h"

CBuildingU::CBuildingU(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CStatue(pGraphic_Device)
{
}

CBuildingU::CBuildingU(const CBuildingU& Prototype)
	: CStatue(Prototype)
{
}

HRESULT CBuildingU::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBuildingU::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_RACE;
	m_szTextureID = TEXT("BuildingU");
	m_szBufferType = TEXT("BuildingU");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pTransformCom->Scaling(_float3(800.f, 500.f, 720.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(470.f, 350.f, 700.f));

	return S_OK;
}

void CBuildingU::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CBuildingU::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CBuildingU::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CBuildingU::Render()
{
	__super::LightOn();
	return __super::Render();
}

CBuildingU* CBuildingU::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBuildingU* pInstance = new CBuildingU(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBuildingU");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBuildingU::Clone(void* pArg)
{
	CBuildingU* pInstance = new CBuildingU(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CBuildingU");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBuildingU::Free()
{
	__super::Free();
}
