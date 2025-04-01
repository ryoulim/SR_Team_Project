#include "Forest.h"
#include "GameInstance.h"

CForest::CForest(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CStatue(pGraphic_Device)
{
}

CForest::CForest(const CForest& Prototype)
	: CStatue(Prototype)
{
}

HRESULT CForest::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CForest::Initialize(void* pArg)
{
	m_szTextureID = TEXT("Forest");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	
	return S_OK;
}

void CForest::Priority_Update(_float fTimeDelta)
{
	m_pTransformCom->Billboard();

	__super::Priority_Update(fTimeDelta);
}

EVENT CForest::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CForest::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CForest::Render()
{
	//__super::LightOn();
	return __super::Render();
}

CForest* CForest::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CForest* pInstance = new CForest(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CForest");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CForest::Clone(void* pArg)
{
	CForest* pInstance = new CForest(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CForest");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CForest::Free()
{
	__super::Free();
}
