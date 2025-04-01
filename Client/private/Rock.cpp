#include "Rock.h"
#include "GameInstance.h"

CRock::CRock(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CStatue(pGraphic_Device)
{
}

CRock::CRock(const CRock& Prototype)
	: CStatue(Prototype)
{
}

HRESULT CRock::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRock::Initialize(void* pArg)
{
	m_szTextureID = TEXT("Rock");
	m_szBufferType = TEXT("Cube");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	
	return S_OK;
}

void CRock::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CRock::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CRock::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CRock::Render()
{
	//__super::LightOn();
	return __super::Render();
}

CRock* CRock::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRock* pInstance = new CRock(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CRock");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CRock::Clone(void* pArg)
{
	CRock* pInstance = new CRock(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CRock");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRock::Free()
{
	__super::Free();
}
