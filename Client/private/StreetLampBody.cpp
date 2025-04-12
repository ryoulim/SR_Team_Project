#include "StreetLampBody.h"
#include "GameInstance.h"

CStreetLampBody::CStreetLampBody(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CStatue(pGraphic_Device)
{
}

CStreetLampBody::CStreetLampBody(const CStreetLampBody& Prototype)
	: CStatue(Prototype)
{
}

HRESULT CStreetLampBody::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CStreetLampBody::Initialize(void* pArg)
{
	m_szTextureID = TEXT("StreetLampBody");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CStreetLampBody::Priority_Update(_float fTimeDelta)
{
	m_pTransformCom->Billboard();

	__super::Priority_Update(fTimeDelta);
}

EVENT CStreetLampBody::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CStreetLampBody::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CStreetLampBody::Render()
{
	//__super::LightOn();
	return __super::Render();
}

CStreetLampBody* CStreetLampBody::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CStreetLampBody* pInstance = new CStreetLampBody(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CStreetLampBody");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStreetLampBody::Clone(void* pArg)
{
	CStreetLampBody* pInstance = new CStreetLampBody(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CStreetLampBody");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStreetLampBody::Free()
{
	__super::Free();
}
