#include "MyComputer.h"
#include "GameInstance.h"

CMyComputer::CMyComputer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CStatue(pGraphic_Device)
{
}

CMyComputer::CMyComputer(const CMyComputer& Prototype)
	: CStatue(Prototype)
{
}

HRESULT CMyComputer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMyComputer::Initialize(void* pArg)
{
	m_szTextureID = TEXT("MyComputer");
	m_szBufferType = TEXT("Computer");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CMyComputer::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CMyComputer::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CMyComputer::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CMyComputer::Render()
{
	m_fTextureNum = 0.f;

	if (FAILED(__super::Render()))
		return E_FAIL;

	m_fTextureNum = 1.f;

	if (FAILED(m_pTextureCom->Bind_Resource(static_cast<_uint>(m_fTextureNum))))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render(CVIBuffer_Computer::BACK)))
		return E_FAIL;

	m_fTextureNum = 2.f;

	if (FAILED(m_pTextureCom->Bind_Resource(static_cast<_uint>(m_fTextureNum))))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render(CVIBuffer_Computer::MONITOR)))
		return E_FAIL;

	return S_OK;
}

CMyComputer* CMyComputer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMyComputer* pInstance = new CMyComputer(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMyComputer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMyComputer::Clone(void* pArg)
{
	CMyComputer* pInstance = new CMyComputer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CMyComputer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMyComputer::Free()
{
	__super::Free();
}
