#include "BombRadius.h"
#include "GameInstance.h"

CBombRadius::CBombRadius(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CStatue(pGraphic_Device)
{
}

CBombRadius::CBombRadius(const CBombRadius& Prototype)
	: CStatue(Prototype)
{
}

HRESULT CBombRadius::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBombRadius::Initialize(void* pArg)
{
	m_szTextureID = TEXT("BombRadius");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pPlayer = GET_PLAYER;
	Safe_AddRef(m_pPlayer);

	return S_OK;
}

void CBombRadius::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CBombRadius::Update(_float fTimeDelta)
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		_float3(m_pTransformCom->Get_State(CTransform::STATE_POSITION)->x,
			1.f,
			static_cast<CTransform*>(m_pPlayer->Find_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION)->z));
	
	return __super::Update(fTimeDelta);
}

void CBombRadius::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CBombRadius::Render()
{
	//__super::LightOn();
	return __super::Render();
}

CBombRadius* CBombRadius::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBombRadius* pInstance = new CBombRadius(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBombRadius");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBombRadius::Clone(void* pArg)
{
	CBombRadius* pInstance = new CBombRadius(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CBombRadius");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBombRadius::Free()
{
	__super::Free();

	Safe_Release(m_pPlayer);
}
