#include "Canopy.h"
#include "GameInstance.h"
#include "Player.h"
#include "Client_Defines.h"
#include "FXMgr.h"

CCanopy::CCanopy(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CStatue(pGraphic_Device)
{
}

CCanopy::CCanopy(const CCanopy& Prototype)
	: CStatue(Prototype)
{
}

HRESULT CCanopy::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCanopy::Initialize(void* pArg)
{
	m_szTextureID = TEXT("Canopy");
	m_szBufferType = TEXT("Canopy");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CCanopy::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CCanopy::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CCanopy::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CCanopy::Render()
{
#ifdef _COLLIDERRENDER
	if (m_pColliderCom)
	{
		m_pColliderCom->Render();
		m_pSubColliderCom[0]->Render();
		m_pSubColliderCom[1]->Render();
	}

#endif

	MultiTextureShaderRender(2);

	return S_OK;
}

HRESULT CCanopy::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	CCollider::DESC ColliderDesc{};
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vScale = m_pTransformCom->Compute_Scaled();
	ColliderDesc.pOwner = this;
	ColliderDesc.iColliderGroupID = CG_BLOCK;
	ColliderDesc.iColliderID = CI_BLOCK_COMMON;
	ColliderDesc.vOffSet = _float3( 0.f, ColliderDesc.vScale.y * 0.4375f, 0.f);
	ColliderDesc.vScale.y *= 0.125f;
	ColliderDesc.vScale.x *= 0.5f;

	_float3 vAngle = DEGREE(static_cast<DESC*>(pArg)->vAngle);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB_Cube"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	ColliderDesc.vScale = m_pTransformCom->Compute_Scaled();
	
	if(180.f == vAngle.y)
		ColliderDesc.vOffSet = _float3(ColliderDesc.vScale.x * 0.925f, 0.f, -ColliderDesc.vScale.z * 0.45f);
	else
		ColliderDesc.vOffSet = _float3(-ColliderDesc.vScale.x * 0.925f, 0.f, ColliderDesc.vScale.z * 0.45f);

	ColliderDesc.vScale.x *= 0.125f;
	ColliderDesc.vScale.z *= 0.125f;


	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB_Cube"),
		TEXT("Com_SubCollider0"), reinterpret_cast<CComponent**>(&m_pSubColliderCom[0]), &ColliderDesc)))
		return E_FAIL;

	ColliderDesc.vScale = m_pTransformCom->Compute_Scaled();

	if (180.f == vAngle.y)
		ColliderDesc.vOffSet = _float3(ColliderDesc.vScale.x * 0.925f, 0.f, ColliderDesc.vScale.x * 0.45f);
	else
		ColliderDesc.vOffSet = _float3(-ColliderDesc.vScale.x * 0.925f, 0.f, -ColliderDesc.vScale.x * 0.45f);

	ColliderDesc.vScale.x *= 0.125f;
	ColliderDesc.vScale.z *= 0.125f;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB_Cube"),
		TEXT("Com_SubCollider1"), reinterpret_cast<CComponent**>(&m_pSubColliderCom[1]), &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}


CCanopy* CCanopy::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCanopy* pInstance = new CCanopy(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCanopy");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCanopy::Clone(void* pArg)
{
	CCanopy* pInstance = new CCanopy(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CCanopy");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCanopy::Free()
{
	__super::Free();
	for(_uint i = 0; i < 2; i++)
		Safe_Release(m_pSubColliderCom[i]);
}
