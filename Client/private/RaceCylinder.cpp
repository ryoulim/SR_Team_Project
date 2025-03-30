#include "RaceCylinder.h"
#include "GameInstance.h"

CRaceCylinder::CRaceCylinder(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CStatue(pGraphic_Device)
{
}

CRaceCylinder::CRaceCylinder(const CRaceCylinder& Prototype)
	: CStatue(Prototype)
{
}

HRESULT CRaceCylinder::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRaceCylinder::Initialize(void* pArg)
{
	m_szTextureID = TEXT("RaceCylinder");
	m_szBufferType = TEXT("RaceCylinder");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CRaceCylinder::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CRaceCylinder::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CRaceCylinder::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CRaceCylinder::Render()
{
	//__super::LightOn();

	m_fTextureNum = 0.f;

	if (FAILED(__super::Render()))
		return E_FAIL;

	m_fTextureNum = 1.f;

	if (FAILED(m_pTextureCom->Bind_Resource(static_cast<_uint>(m_fTextureNum))))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render(CVIBuffer_RaceCylinder::MIDDLE)))
		return E_FAIL;

	m_fTextureNum = 2.f;

	if (FAILED(m_pTextureCom->Bind_Resource(static_cast<_uint>(m_fTextureNum))))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render(CVIBuffer_RaceCylinder::SIDE)))
		return E_FAIL;

	return S_OK;
}

CRaceCylinder* CRaceCylinder::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRaceCylinder* pInstance = new CRaceCylinder(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CRaceCylinder");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CRaceCylinder::Clone(void* pArg)
{
	CRaceCylinder* pInstance = new CRaceCylinder(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CRaceCylinder");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRaceCylinder::Free()
{
	__super::Free();
}
