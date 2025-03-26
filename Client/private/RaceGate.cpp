#include "RaceGate.h"
#include "GameInstance.h"

CRaceGate::CRaceGate(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CStatue(pGraphic_Device)
{
}

CRaceGate::CRaceGate(const CRaceGate& Prototype)
	: CStatue(Prototype)
{
}

HRESULT CRaceGate::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRaceGate::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_RACE;
	m_szTextureID = TEXT("RaceGate");
	m_szBufferType = TEXT("RaceGate");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CRaceGate::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CRaceGate::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CRaceGate::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CRaceGate::Render()
{
	//__super::LightOn();
	return __super::Render();
}

CRaceGate* CRaceGate::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRaceGate* pInstance = new CRaceGate(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CRaceGate");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CRaceGate::Clone(void* pArg)
{
	CRaceGate* pInstance = new CRaceGate(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CRaceGate");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRaceGate::Free()
{
	__super::Free();
}
