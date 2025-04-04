#include "Collider.h"
#include "GameObject.h"
#include "Transform.h"
#include "GameInstance.h"

_float3 CCollider::m_vLast_Collision_Depth{};
_float3 CCollider::m_vLast_Collision_Pos{};

#ifdef _COLLIDERRENDER
_bool	CCollider::m_bColliderRender{ FALSE };
#endif

CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent{ pGraphic_Device }
{
}

#ifdef _COLLIDERRENDER

#include "VIBuffer_Cube.h"
#include "VIBuffer_Circle.h"

CCollider::CCollider(const CCollider& Prototype)
	: CComponent(Prototype)
	, m_eType{ Prototype.m_eType }
	, m_pRenderBuffer{ Prototype.m_pRenderBuffer }
{
	Safe_AddRef(m_pRenderBuffer);
}

HRESULT CCollider::Initialize_Prototype(COLLIDER_TYPE Type)
{
	m_eType = Type;

	if (m_eType == SPHERE)
		m_pRenderBuffer = CVIBuffer_Circle::Create(m_pGraphic_Device);
	else
		m_pRenderBuffer = CVIBuffer_Cube::Create(m_pGraphic_Device);

	return S_OK;
}

HRESULT CCollider::Initialize(void* pArg)
{
	if (pArg == nullptr)
	{
		MSG_BOX("콜라이더 pArg에 nullptr을 넣어주면 어떡해...");
		return E_FAIL;
	}

	DESC* pDesc = static_cast<DESC*>(pArg);
	m_pOwner = pDesc->pOwner;
	//Safe_AddRef(m_pOwner); 순환참조 때문에 어쩔수가 없다 쉬팔
	m_pTransform = pDesc->pTransform;
	Safe_AddRef(m_pTransform);
	m_vOffSet = pDesc->vOffSet;
	m_iColliderID = pDesc->iColliderID;
	
	m_pRenderTransform = CTransform::Create(m_pGraphic_Device);

	m_pRenderTransform->Set_WorldMatrix(pDesc->pTransform->Get_WorldMatrix());
	m_pRenderTransform->Scaling(pDesc->vScale);
	m_pRenderTransform->Move(pDesc->vOffSet);

	Update_Scale(pDesc->vScale);
	Update_Collider();
	m_pGameInstance->Add_Collider(this, pDesc->iColliderGroupID);
	
	return S_OK;
}


void CCollider::Render()
{
	if (!m_bColliderRender)
		return;

	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphic_Device->SetTexture(0, nullptr);
	m_pRenderTransform->Bind_Resource();
	m_pRenderBuffer->Bind_Buffers();
	m_pRenderBuffer->Render();
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

#else

CCollider::CCollider(const CCollider& Prototype)
	: CComponent(Prototype)
	, m_eType{ Prototype.m_eType }
{
}

HRESULT CCollider::Initialize_Prototype(COLLIDER_TYPE Type)
{
	m_eType = Type;
	return S_OK;
}

HRESULT CCollider::Initialize(void* pArg)
{
	if (pArg == nullptr)
	{
		MSG_BOX("콜라이더 pArg에 nullptr을 넣어주면 어떡해...");
		return E_FAIL;
	}

	DESC* pDesc = static_cast<DESC*>(pArg);
	m_pOwner = pDesc->pOwner;
	//Safe_AddRef(m_pOwner); 순환참조 때문에 어쩔수가 없다 쉬팔
	m_pTransform = pDesc->pTransform;
	Safe_AddRef(m_pTransform);
	m_vOffSet = pDesc->vOffSet;
	m_iColliderID = pDesc->iColliderID;

	Update_Scale(pDesc->vScale);
	Update_Collider();
	m_pGameInstance->Add_Collider(this, pDesc->iColliderGroupID);
	return S_OK;
}

#endif


_bool CCollider::Check_Intersect(const CCollider* Collider)
{
	if ((Collider->Get_Pos() - Get_Pos()).Length() >
		Get_MaxLength() + Collider->Get_MaxLength())
		return FALSE;

	switch (Collider->Get_Type())
	{
	case AABB_CUBE:
		return Intersect_With_AABB_Cube(Collider);
	case OBB_CUBE:
		return Intersect_With_OBB_Cube(Collider);
	case SPHERE:
		return Intersect_With_Sphere(Collider);
	case CAPSULE:
		return Intersect_With_Capsule(Collider);
	case LINE:
		return Intersect_With_Line(Collider);
	case RECT:
		return Intersect_With_Rect(Collider);
	default:
		return FALSE;
	}

	return FALSE;
}

void CCollider::Free()
{
	__super::Free();
	Safe_Release(m_pTransform);
	//Safe_Release(m_pOwner); 난 최선을 다했어

#ifdef _COLLIDERRENDER
	Safe_Release(m_pRenderTransform);
	Safe_Release(m_pRenderBuffer);
#endif
}