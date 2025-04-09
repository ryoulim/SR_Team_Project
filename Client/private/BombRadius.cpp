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
	
	return S_OK;
}

void CBombRadius::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CBombRadius::Update(_float fTimeDelta)
{
	//매 프레임마다 일정 수치만큼 밀림
	m_pTransformCom->Move({ 0.f,0.f,RACE_SPEED_PER_SEC }, fTimeDelta);

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
}
