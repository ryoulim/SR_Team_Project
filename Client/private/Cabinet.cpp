// 내 클래스 이름 : Cabinet
// 부모 클래스 이름 : Statue

#include "Cabinet.h"
#include "GameInstance.h"

CCabinet::CCabinet(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CStatue{ pGraphic_Device }
{
}

CCabinet::CCabinet(const CCabinet& Prototype)
	: CStatue(Prototype)
{
}

HRESULT CCabinet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCabinet::Initialize(void* pArg)
{
	m_szTextureID = TEXT("Cabinet");
	m_szBufferType = TEXT("Cabinet");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CCabinet::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CCabinet::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CCabinet::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CCabinet::Render()
{
	MultiTextureShaderRender(3);
	return S_OK;
}

CCabinet* CCabinet::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCabinet* pInstance = new CCabinet(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCabinet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCabinet::Clone(void* pArg)
{
	CCabinet* pInstance = new CCabinet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CCabinet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCabinet::Free()
{
	__super::Free();
}