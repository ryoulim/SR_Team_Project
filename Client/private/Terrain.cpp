// 내 클래스 이름 : Terrain
// 부모 클래스 이름 : Statue

#include "Terrain.h"
#include "GameInstance.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMap{ pGraphic_Device }
{
}

CTerrain::CTerrain(const CTerrain& Prototype)
	: CMap(Prototype)
{
}

HRESULT CTerrain::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTerrain::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	DESC* pDesc = static_cast<DESC*>(pArg);

	m_eLevelID = pDesc->eNextLevel;
	m_szTextureID = TEXT("Terrain");
	m_szBufferType = TEXT("Terrain");

	if (FAILED(Ready_Material()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	//m_pTransformCom->Scaling(_float3(BOSSSCALE, BOSSSCALE, BOSSSCALE));
	return S_OK;
}

void CTerrain::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CTerrain::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CTerrain::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CTerrain::Render()
{
	if(FAILED(m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE)))
		return E_FAIL;
	if (FAILED(__super::Render()))
		return E_FAIL;
	if (FAILED(m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrain::Ready_Material()
{
	D3DMATERIAL9		tMtrl;
	ZeroMemory(&tMtrl, sizeof(D3DMATERIAL9));

	tMtrl.Diffuse = { 1.f, 1.f, 1.f, 1.f };
	tMtrl.Specular = { 1.f, 1.f, 1.f, 1.f };
	tMtrl.Ambient = { 0.2f, 0.2f, 0.2f, 1.f };

	tMtrl.Emissive = { 0.f, 0.f, 0.f, 0.f };
	tMtrl.Power = 0.f;

	m_pGraphic_Device->SetMaterial(&tMtrl);

	return S_OK;
}

CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTerrain* pInstance = new CTerrain(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTerrain::Clone(void* pArg)
{
	CTerrain* pInstance = new CTerrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrain::Free()
{
	__super::Free();
}