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
	DIRECTION* dir = reinterpret_cast<DIRECTION*>(pArg);

	m_eLevelID = LEVEL_RACE;
	m_szTextureID = TEXT("RaceLandscape");
	m_szBufferType = TEXT("RaceLandscape");

	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	if (*dir == RIGHT)
	{
		m_pTransformCom->LookAt(_float3(0.f, 0.f, -1.f));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(650.f, 36.f, 7150.f));
	}

	else
	{
		m_pTransformCom->LookAt(_float3(0.f, 0.f, 1.f));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(250.f, 36.f, 36.f));
	}

	m_pTransformCom->Scaling(_float3(72.f, 72.f, 72.f));

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
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

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
