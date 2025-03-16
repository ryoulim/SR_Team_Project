#include "Collider_AABB_Cube.h"

CCollider_AABB_Cube::CCollider_AABB_Cube(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCollider{ pGraphic_Device }
{
}

CCollider_AABB_Cube::CCollider_AABB_Cube(const CCollider_AABB_Cube& Prototype)
	: CCollider(Prototype)
{
}

HRESULT CCollider_AABB_Cube::Initialize_Prototype()
{
	// 타입을 프로토타입 생성시에 확정하고 이후 바꾸지 못하도록 제한	
	return __super::Initialize_Prototype(AABB_CUBE);
}

HRESULT CCollider_AABB_Cube::Initialize(void* pArg)
{
	if (pArg == nullptr)
	{
		MSG_BOX("콜라이더 pArg에 nullptr을 넣어주면 어떡해...");
		return E_FAIL;
	}

	DESC* pDesc = static_cast<DESC*>(pArg);
	m_vHalfScale = pDesc->vScale * 0.5f;
	m_tInfo.vCenter = *pDesc->pTransform->Get_State(CTransform::STATE_POSITION);

	m_tInfo.vMinPos = m_tInfo.vCenter - m_vHalfScale;
	m_tInfo.vMaxPos = m_tInfo.vCenter + m_vHalfScale;

	return S_OK;
}


void CCollider_AABB_Cube::Update_Collider(const CTransform* pTransform)
{
	m_tInfo.vCenter = *pTransform->Get_State(CTransform::STATE_POSITION);
	m_tInfo.vMinPos = m_tInfo.vCenter - m_vHalfScale;
	m_tInfo.vMaxPos = m_tInfo.vCenter + m_vHalfScale;
}

_bool CCollider_AABB_Cube::Intersect_With_AABB_Cube(const CCollider* pOther)
{
	auto pOtherInfo = dynamic_cast<const CCollider_AABB_Cube*>(pOther)->Get_Info();
	
	return (m_tInfo.vMaxPos.x > pOtherInfo->vMinPos.x && m_tInfo.vMinPos.x < pOtherInfo->vMaxPos.x) &&
		(m_tInfo.vMaxPos.y > pOtherInfo->vMinPos.y && m_tInfo.vMinPos.y < pOtherInfo->vMaxPos.y) &&
		(m_tInfo.vMaxPos.z > pOtherInfo->vMinPos.z && m_tInfo.vMinPos.z < pOtherInfo->vMaxPos.z);
}

_bool CCollider_AABB_Cube::Intersect_With_OBB_Cube(const CCollider* pOther)
{
	return _bool();
}

_bool CCollider_AABB_Cube::Intersect_With_Sphere(const CCollider* pOther)
{
	return _bool();
}

_bool CCollider_AABB_Cube::Intersect_With_Capsule(const CCollider* pOther)
{
	return _bool();
}

CCollider_AABB_Cube* CCollider_AABB_Cube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCollider_AABB_Cube* pInstance = new CCollider_AABB_Cube(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCollider_AABB_Cube");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CCollider_AABB_Cube::Clone(void* pArg)
{
	CCollider_AABB_Cube* pInstance = new CCollider_AABB_Cube(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CCollider_AABB_Cube");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollider_AABB_Cube::Free()
{
	__super::Free();
}
