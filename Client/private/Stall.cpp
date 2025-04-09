#include "Stall.h"
#include "GameInstance.h"

CStall::CStall(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CStatue(pGraphic_Device)
{
}

CStall::CStall(const CStall& Prototype)
	: CStatue(Prototype)
{
}

HRESULT CStall::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CStall::Initialize(void* pArg)
{
	m_szTextureID = TEXT("Stall");
	m_szBufferType = TEXT("Stall");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CStall::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CStall::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CStall::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CStall::Render()
{
	MultiTextureShaderRender(2);

#ifdef _COLLIDERRENDER
	if (m_pColliderCom)
		m_pColliderCom->Render();
#endif
	return S_OK;
}

HRESULT CStall::Ready_Components(void* pArg)
{
	__super::Ready_Components(pArg);

	DESC* pDesc = static_cast<DESC*>(pArg);

	if (nullptr != pArg)
	{
		/* For.Com_Collider */

		CCollider::DESC ColliderDesc{};
		ColliderDesc.pTransform = m_pTransformCom;
		ColliderDesc.vScale = m_pTransformCom->Compute_Scaled();
		ColliderDesc.pOwner = this;
		ColliderDesc.iColliderGroupID = CG_BLOCK;
		ColliderDesc.iColliderID = CI_BLOCK_COMMON;

		auto& vAngle = static_cast<DESC*>(pArg)->vAngle;

		const _float rightAngles[] = {
			0.f,
			0.5f * PI,
			1.0f * PI,
			1.5f * PI
		};
		_bool isRightX = false, isRightY = false, isRightZ = false;
		_bool isZeroX = fabsf(vAngle.x) < FLT_EPSILON;
		_bool isZeroY = fabsf(vAngle.y) < FLT_EPSILON;
		_bool isZeroZ = fabsf(vAngle.z) < FLT_EPSILON;

		// 직각 각도인지 체크
		for (_uint i = 0; i < 4; ++i)
		{
			if (fabsf(vAngle.x - rightAngles[i]) < FLT_EPSILON) isRightX = true;
			if (fabsf(vAngle.y - rightAngles[i]) < FLT_EPSILON) isRightY = true;
			if (fabsf(vAngle.z - rightAngles[i]) < FLT_EPSILON) isRightZ = true;
		}

		_bool isAllZero = isZeroX && isZeroY && isZeroZ;
		_bool isAllRightAngle = isRightX && isRightY && isRightZ;

		if (isAllRightAngle)
		{
			if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB_Cube"),
				TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
				return E_FAIL;

			if (!isAllZero)
			{
				static_cast<CCollider_AABB_Cube*>(m_pColliderCom)->Update_Rotation(vAngle);
			}
		}
		else
		{
			if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB_Cube"),
				TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
				return E_FAIL;
		}
	}

	return S_OK;
}


CStall* CStall::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CStall* pInstance = new CStall(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CStall");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStall::Clone(void* pArg)
{
	CStall* pInstance = new CStall(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CStall");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStall::Free()
{
	__super::Free();
}
