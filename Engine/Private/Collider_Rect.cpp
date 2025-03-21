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

void CCollider_Rect::Update_Collider()
{
	m_tInfo.vCenter = *m_pTransform->Get_State(CTransform::STATE_POSITION) + m_vOffSet;
	m_tInfo.vRight = m_pTransform->Get_State(CTransform::STATE_RIGHT)->Normalize();
	m_tInfo.vUp = m_pTransform->Get_State(CTransform::STATE_UP)->Normalize();
}

void CCollider_Rect::Update_Scale(const _float3& vScale)
{
	m_tInfo.fHeight = vScale.y;
	m_tInfo.fWidth = vScale.x;

	_float fHalfW = m_tInfo.fWidth * 0.5f;
	_float fHalfH = m_tInfo.fHeight * 0.5f;

	m_fMaxLength = sqrtf(SQUARE(fHalfW) + SQUARE(fHalfH));
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

bool CCollider_Rect::RayCasting(const _float3& rayOrigin, const _float3& rayDir)
{
	_float3 normal = m_tInfo.GetNormal();
	float denominator = normal.Dot(rayDir);

	// 레이가 평면과 평행하면 충돌 없음
	if (fabs(denominator) < 1e-6f) 
		return FALSE;

	// 평면과의 교차점 계산
	float t = _float3(m_tInfo.vCenter - rayOrigin).Dot(normal) / denominator;

	// t가 0보다 작으면 레이가 반대 방향 (충돌 없음)
	if (t < 0) 
		return FALSE;

	m_vLast_Collision_Pos = rayOrigin + rayDir * t;
	m_vLast_Collision_Depth = normal;

	return TRUE;
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
