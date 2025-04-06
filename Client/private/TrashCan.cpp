// 내 클래스 이름 : TrashCan
// 부모 클래스 이름 : Interactive_Block

#include "TrashCan.h"
#include "GameInstance.h"

CTrashCan::CTrashCan(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CInteractive_Block{ pGraphic_Device }
{
}

CTrashCan::CTrashCan(const CTrashCan& Prototype)
	: CInteractive_Block(Prototype)
{
}

HRESULT CTrashCan::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTrashCan::Initialize(void* pArg)
{
	m_szTextureID = TEXT("TrashCan");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CTrashCan::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CTrashCan::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CTrashCan::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CTrashCan::Render()
{
	return __super::Render();
}

CTrashCan* CTrashCan::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTrashCan* pInstance = new CTrashCan(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTrashCan");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTrashCan::Clone(void* pArg)
{
	CTrashCan* pInstance = new CTrashCan(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CTrashCan");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTrashCan::Free()
{
	__super::Free();
}