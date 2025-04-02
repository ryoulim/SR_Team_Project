// 내 클래스 이름 : Lava
// 부모 클래스 이름 : AnimeRect

#include "Lava.h"
#include "GameInstance.h"

CLava::CLava(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CAnimeRect{ pGraphic_Device }
{
}

CLava::CLava(const CLava& Prototype)
	: CAnimeRect(Prototype)
{
}

HRESULT CLava::Initialize_Prototype()
{
	m_fMaxFrame = 16.f;

	return S_OK;
}

HRESULT CLava::Initialize(void* pArg)
{
	m_szTextureID = TEXT("Lava");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CLava::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CLava::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CLava::Late_Update(_float fTimeDelta)
{
	Move_Frame(fTimeDelta);
	__super::Late_Update(fTimeDelta);
}

HRESULT CLava::Render()
{
	return __super::Render();
}

HRESULT CLava::Ready_Components(void* pArg)
{
	__super::Ready_Components(pArg);

	DESC* pDesc = static_cast<DESC*>(pArg);

	CCollider::DESC ColliderDesc{};
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vScale = m_pTransformCom->Compute_Scaled();
	ColliderDesc.pOwner = this;
	ColliderDesc.iColliderGroupID = CG_INTERACTIVE;
	ColliderDesc.iColliderID = CI_INTERACTIVE_LAVA;
	ColliderDesc.vOffSet.y  = -ColliderDesc.vScale.z * 0.5f;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB_Cube"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	static_cast<CCollider_AABB_Cube*>(m_pColliderCom)->Update_Rotation(pDesc->vAngle);

	return S_OK;
}

CLava* CLava::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLava* pInstance = new CLava(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLava");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLava::Clone(void* pArg)
{
	CLava* pInstance = new CLava(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CLava");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLava::Free()
{
	__super::Free();
}