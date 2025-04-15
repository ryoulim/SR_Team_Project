#include "Signboard.h"

CSignboard::CSignboard(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CStatue { pGraphic_Device }
{
}

CSignboard::CSignboard(const CSignboard& Prototype)
	: CStatue(Prototype)
{
}

HRESULT CSignboard::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSignboard::Initialize(void* pArg)
{
	m_szTextureID = TEXT("Signboard");
	m_szBufferType = TEXT("Signboard");

	m_eLevelID = static_cast<DESC*>(pArg)->eLevelID;

	Ready_Components(pArg);

	return S_OK;
}

void CSignboard::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CSignboard::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CSignboard::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CSignboard::Render()
{
#ifdef _COLLIDERRENDER
	if (m_pColliderCom)
		m_pColliderCom->Render();
#endif

	MultiTextureShaderRender(5);

	return S_OK;
}

HRESULT CSignboard::Ready_Components(void* pArg)
{
	__super::Ready_Components(pArg);

	if (nullptr != pArg)
	{
		/* For.Com_Collider */

		CCollider::DESC ColliderDesc{};
		ColliderDesc.pTransform = m_pTransformCom;
		ColliderDesc.vScale = {
			m_pTransformCom->Compute_Scaled().x * 2.f,
			m_pTransformCom->Compute_Scaled().y * 2.f,
			m_pTransformCom->Compute_Scaled().z * 0.25f
		};
		ColliderDesc.vOffSet = {
			ColliderDesc.vScale.z,
			0.f,
			0.f
		};
		ColliderDesc.pOwner = this;
		ColliderDesc.iColliderGroupID = CG_BLOCK;
		ColliderDesc.iColliderID = CI_BLOCK_COMMON;

		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB_Cube"),
			TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
			return E_FAIL;
	}

	return S_OK;
}

CSignboard* CSignboard::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSignboard* pInstance = new CSignboard(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSignboard");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSignboard::Clone(void* pArg)
{
	CSignboard* pInstance = new CSignboard(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CSignboard");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSignboard::Free()
{
	__super::Free();
}
