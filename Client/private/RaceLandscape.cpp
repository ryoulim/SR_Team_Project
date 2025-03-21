#include "RaceLandscape.h"

CRaceLandscape::CRaceLandscape(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CStatue { pGraphic_Device }
{
}

CRaceLandscape::CRaceLandscape(const CRaceLandscape& Prototype)
	: CStatue (Prototype)
{
}

HRESULT CRaceLandscape::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRaceLandscape::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_RACE;
	m_szTextureID = TEXT("RaceLandscape");
	m_szBufferType = TEXT("RaceLandscape");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	//m_pTransformCom->Scaling(_float3(28.f, 28.f, 28.f));

	return S_OK;
}

void CRaceLandscape::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CRaceLandscape::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CRaceLandscape::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CRaceLandscape::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

CRaceLandscape* CRaceLandscape::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRaceLandscape* pInstance = new CRaceLandscape(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CRaceLandscape");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CRaceLandscape::Clone(void* pArg)
{
	CRaceLandscape* pInstance = new CRaceLandscape(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CRaceLandscape");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRaceLandscape::Free()
{
	__super::Free();
}
