#include "RaceTerrain.h"

CRaceTerrain::CRaceTerrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMap { pGraphic_Device }
{
}

CRaceTerrain::CRaceTerrain(const CRaceTerrain& Prototype)
	: CMap(Prototype)
{
}

HRESULT CRaceTerrain::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRaceTerrain::Initialize(void* pArg)
{
	m_szTextureID = TEXT("RaceTerrain");
	m_szBufferType = TEXT("RaceTerrain");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CRaceTerrain::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CRaceTerrain::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CRaceTerrain::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CRaceTerrain::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	MultiTextureShaderRender(2);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

CRaceTerrain* CRaceTerrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRaceTerrain* pInstance = new CRaceTerrain(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CRaceTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CRaceTerrain::Clone(void* pArg)
{
	CRaceTerrain* pInstance = new CRaceTerrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CRaceTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRaceTerrain::Free()
{
	__super::Free();
}
