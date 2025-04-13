#include "RazerRadius.h"
#include "GameInstance.h"

CRazerRadius::CRazerRadius(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CStatue(pGraphic_Device)
{
}

CRazerRadius::CRazerRadius(const CRazerRadius& Prototype)
	: CStatue(Prototype)
{
}

HRESULT CRazerRadius::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRazerRadius::Initialize(void* pArg)
{
	m_szTextureID = TEXT("RazerRadius");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pPlayer = GET_PLAYER;
	Safe_AddRef(m_pPlayer);

	return S_OK;
}

void CRazerRadius::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CRazerRadius::Update(_float fTimeDelta)
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		_float3(
		m_pTransformCom->Get_State(CTransform::STATE_POSITION)->x,
		1.f,
		static_cast<CTransform*>(m_pPlayer->Find_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION)->z)
		);
	
	return __super::Update(fTimeDelta);
}

void CRazerRadius::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CRazerRadius::Render()
{
	//__super::LightOn();
	return __super::Render();
}

CRazerRadius* CRazerRadius::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRazerRadius* pInstance = new CRazerRadius(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CRazerRadius");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CRazerRadius::Clone(void* pArg)
{
	CRazerRadius* pInstance = new CRazerRadius(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CRazerRadius");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRazerRadius::Free()
{
	__super::Free();

	Safe_Release(m_pPlayer);
}
