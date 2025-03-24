#include "BuildingW.h"
#include "GameInstance.h"

CBuildingW::CBuildingW(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CStatue(pGraphic_Device)
{
}

CBuildingW::CBuildingW(const CBuildingW& Prototype)
	: CStatue(Prototype)
{
}

HRESULT CBuildingW::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBuildingW::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_RACE;
	m_szTextureID = TEXT("BuildingW");
	m_szBufferType = TEXT("BuildingW");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pTransformCom->Scaling(_float3(800.f, 500.f, 720.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(470.f, 300.f, 700.f));

	return S_OK;
}

void CBuildingW::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CBuildingW::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CBuildingW::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CBuildingW::Render()
{
	//__super::LightOn();
	return __super::Render();
}

CBuildingW* CBuildingW::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBuildingW* pInstance = new CBuildingW(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBuildingW");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBuildingW::Clone(void* pArg)
{
	CBuildingW* pInstance = new CBuildingW(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CBuildingW");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBuildingW::Free()
{
	__super::Free();
}
