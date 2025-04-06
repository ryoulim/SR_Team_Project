// 내 클래스 이름 : Door
// 부모 클래스 이름 : Interactive_Block

#include "Door.h"
#include "GameInstance.h"

CDoor::CDoor(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CInteractive_Block{ pGraphic_Device }
{
}

CDoor::CDoor(const CDoor& Prototype)
	: CInteractive_Block(Prototype)
{
}

HRESULT CDoor::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDoor::Initialize(void* pArg)
{
	m_szTextureID = TEXT("Test");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CDoor::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CDoor::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CDoor::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CDoor::Render()
{
	return __super::Render();
}

void CDoor::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	/* 똑똑 열려라 참깨 */
}

HRESULT CDoor::Ready_Components(void* pArg)
{
	__super::Ready_Components(pArg);

	return S_OK;
}

CDoor* CDoor::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CDoor* pInstance = new CDoor(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDoor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDoor::Clone(void* pArg)
{
	CDoor* pInstance = new CDoor(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CDoor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDoor::Free()
{
	__super::Free();
}