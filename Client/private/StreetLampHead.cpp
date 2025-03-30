#include "StreetLampHead.h"
#include "GameInstance.h"

CStreetLampHead::CStreetLampHead(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CStatue(pGraphic_Device)
{
}

CStreetLampHead::CStreetLampHead(const CStreetLampHead& Prototype)
	: CStatue(Prototype)
{
}

HRESULT CStreetLampHead::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CStreetLampHead::Initialize(void* pArg)
{
	m_szTextureID = TEXT("StreetLampHead");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CStreetLampHead::Priority_Update(_float fTimeDelta)
{
	m_pTransformCom->Billboard();

	__super::Priority_Update(fTimeDelta);
}

EVENT CStreetLampHead::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CStreetLampHead::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CStreetLampHead::Render()
{
	//__super::LightOn();
	return __super::Render();
}

CStreetLampHead* CStreetLampHead::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CStreetLampHead* pInstance = new CStreetLampHead(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CStreetLampHead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStreetLampHead::Clone(void* pArg)
{
	CStreetLampHead* pInstance = new CStreetLampHead(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CStreetLampHead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStreetLampHead::Free()
{
	__super::Free();
}
