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
	CStatue::DESC* desc = static_cast<CStatue::DESC*>(pArg);

	m_szTextureID = TEXT("RaceLandscape");
	m_szBufferType = TEXT("RaceLandscape");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CRaceLandscape::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CRaceLandscape::Update(_float fTimeDelta)
{
	m_pTransformCom->Move({ 0.f,0.f,-RACE_SPEED_PER_SEC }, fTimeDelta);
	return __super::Update(fTimeDelta);
}

void CRaceLandscape::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CRaceLandscape::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CRaceLandscape::Surface_Plane()
{
	CGameObject* pRaceTerrain = m_pGameInstance->Find_Object(LEVEL_RACEFIRST, TEXT("Layer_RaceTerrain"), 0);
	Safe_AddRef(pRaceTerrain);

	//m_pTransformCom->Set_State()
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
