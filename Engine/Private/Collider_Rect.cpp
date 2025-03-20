#include "Collider_Rect.h"

CCollider_Rect::CCollider_Rect(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CCollider{ pGraphic_Device }
{
}

CCollider_Rect::CCollider_Rect(const CCollider_Rect& Prototype)
	:CCollider(Prototype)
{
}

HRESULT CCollider_Rect::Initialize_Prototype()
{
	return __super::Initialize_Prototype(RECT);
}

HRESULT CCollider_Rect::Initialize(void* pArg)
{
	if (pArg == nullptr)
	{
		MSG_BOX("콜라이더 pArg에 nullptr을 넣어주면 어떡해...");
		return E_FAIL;
	}

	const DESC* pDesc = static_cast<DESC*>(pArg);
	_float3 vScale = pDesc->pTransform->Compute_Scaled();
	m_tInfo.fHeight = vScale.y;
	m_tInfo.fWidth = vScale.x;
	Update_Collider(pDesc->pTransform);

	return S_OK;
}

void CCollider_Rect::Update_Collider(const CTransform* pTransform)
{
	m_tInfo.vCenter = *pTransform->Get_State(CTransform::STATE_POSITION);
	m_tInfo.vRight = *pTransform->Get_State(CTransform::STATE_RIGHT);
	m_tInfo.vUp = *pTransform->Get_State(CTransform::STATE_UP);
}

_bool CCollider_Rect::Intersect_With_AABB_Cube(const CCollider* pOther)
{
	return _bool();
}

_bool CCollider_Rect::Intersect_With_OBB_Cube(const CCollider* pOther)
{
	return _bool();
}

_bool CCollider_Rect::Intersect_With_Sphere(const CCollider* pOther)
{
	return _bool();
}

_bool CCollider_Rect::Intersect_With_Capsule(const CCollider* pOther)
{
	return _bool();
}

_bool CCollider_Rect::Intersect_With_Rect(const CCollider* pOther)
{
	return _bool();
}

_bool CCollider_Rect::Intersect_With_Line(const CCollider* pOther)
{
	return _bool();
}

CCollider_Rect* CCollider_Rect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCollider_Rect* pInstance = new CCollider_Rect(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCollider_Rect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CCollider_Rect::Clone(void* pArg)
{
	CCollider_Rect* pInstance = new CCollider_Rect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CCollider_Rect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollider_Rect::Free()
{
	__super::Free();
}
