#include "Frustum.h"

CFrustum::CFrustum(LPDIRECT3DDEVICE9 pGraphic_Device)
	:m_pGraphic_Device{ pGraphic_Device }
{
	Safe_AddRef(m_pGraphic_Device);
}

_bool CFrustum::IsPointInFrustum(const _float3& Point, _float fRadius)
{
	for (int i = 0; i < 6; ++i)
	{
		const _float3* pNormal = reinterpret_cast<const _float3*>(&m_FrustumPlanes[i]); // ����� abc�� ��� ����
		_float d = m_FrustumPlanes[i].d; // ������ D��

		_float dist = pNormal->Dot(Point) + d;

		if (dist < -fRadius)
			return false; // �ٱ��� ����
	}
	return true;
}

void CFrustum::Update_Frustum()
{
	_float4x4 view, Proj, viewProj;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &view);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &Proj);
	viewProj = view * Proj;

	// �¿�, ����, ����/���Ÿ� ����� ����
	m_FrustumPlanes[0] = { viewProj._14 + viewProj._11, viewProj._24 + viewProj._21, viewProj._34 + viewProj._31, viewProj._44 + viewProj._41 }; // Left
	m_FrustumPlanes[1] = { viewProj._14 - viewProj._11, viewProj._24 - viewProj._21, viewProj._34 - viewProj._31, viewProj._44 - viewProj._41 }; // Right
	m_FrustumPlanes[2] = { viewProj._14 - viewProj._12, viewProj._24 - viewProj._22, viewProj._34 - viewProj._32, viewProj._44 - viewProj._42 }; // Top
	m_FrustumPlanes[3] = { viewProj._14 + viewProj._12, viewProj._24 + viewProj._22, viewProj._34 + viewProj._32, viewProj._44 + viewProj._42 }; // Bottom
	m_FrustumPlanes[4] = { viewProj._14 + viewProj._13, viewProj._24 + viewProj._23, viewProj._34 + viewProj._33, viewProj._44 + viewProj._43 }; // Near
	m_FrustumPlanes[5] = { viewProj._14 - viewProj._13, viewProj._24 - viewProj._23, viewProj._34 - viewProj._33, viewProj._44 - viewProj._43 }; // Far

	// ����ȭ
	for (int i = 0; i < 6; i++)
		D3DXPlaneNormalize(&m_FrustumPlanes[i], &m_FrustumPlanes[i]);
}

CFrustum* CFrustum::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	return new CFrustum(pGraphic_Device);
}

void CFrustum::Free()
{
	__super::Free();
	Safe_Release(m_pGraphic_Device);

}