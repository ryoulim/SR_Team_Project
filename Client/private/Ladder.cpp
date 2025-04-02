// �� Ŭ���� �̸� : Ladder
// �θ� Ŭ���� �̸� : Map

#include "Ladder.h"
#include "GameInstance.h"

CLadder::CLadder(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMap{ pGraphic_Device }
{
}

CLadder::CLadder(const CLadder& Prototype)
	: CMap(Prototype)
{
}

HRESULT CLadder::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLadder::Initialize(void* pArg)
{
	m_szTextureID = TEXT("Test");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CLadder::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CLadder::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CLadder::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CLadder::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 40);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return __super::Render();

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CLadder::Ready_Components(void* pArg)
{
	__super::Ready_Components(pArg);

	if (nullptr != pArg)
	{
		DESC* pDesc = static_cast<DESC*>(pArg);

		if (pDesc->bCollision)
		{
			CCollider::DESC ColliderDesc{};
			ColliderDesc.pTransform = m_pTransformCom;
			ColliderDesc.vScale = m_pTransformCom->Compute_Scaled();
			ColliderDesc.pOwner = this;
			ColliderDesc.iColliderGroupID = CG_BLOCK;
			ColliderDesc.iColliderID = CI_BLOCK_INVISIBLE;

			auto& vAngle = static_cast<DESC*>(pArg)->vAngle;
			_wstring ColliderTag = vAngle.x == 0 && vAngle.y == 0 && vAngle.z == 0 ?
				TEXT("Prototype_Component_Collider_AABB_Cube") :
				TEXT("Prototype_Component_Collider_OBB_Cube");

			/* For.Com_Collider */
			if (FAILED(__super::Add_Component(LEVEL_STATIC, ColliderTag,
				TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
				return E_FAIL;
		}
	}

	return S_OK;
}

CLadder* CLadder::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLadder* pInstance = new CLadder(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLadder");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLadder::Clone(void* pArg)
{
	CLadder* pInstance = new CLadder(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CLadder");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLadder::Free()
{
	__super::Free();
}