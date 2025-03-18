#pragma once
#include "Component.h"
#include "Transform.h"

BEGIN(Engine)

class ENGINE_DLL CCollider abstract : public CComponent
{
protected:
	enum COLLIDER_TYPE {AABB_CUBE,OBB_CUBE,CAPSULE,SPHERE,LINE,RECT};
protected:
	CCollider(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCollider(const CCollider& Prototype);
	virtual ~CCollider() = default;

public:
	virtual HRESULT Initialize_Prototype(COLLIDER_TYPE Type);
	virtual HRESULT Initialize(void* pArg) override;
	// 콜라이더를 현재의 트랜스폼을 기반으로 업데이트 합니다.
	virtual void Update_Collider(const CTransform* pTransform) PURE;
	// 다른 콜라이더와의 충돌을 감지합니다.
	_bool Check_Intersect(const CCollider* pOther);

	const COLLIDER_TYPE Get_Type() const { return m_eType; }
	const _float3& Get_Last_Collision_Depth() { return m_vLast_Collision_Depth; }
	const _float3& Get_Last_Collision_Pos() { return m_vLast_Collision_Pos; }

protected:
	static _float3 m_vLast_Collision_Depth;
	static _float3 m_vLast_Collision_Pos;

private:
	COLLIDER_TYPE m_eType{};

private:
	virtual _bool Intersect_With_AABB_Cube(const CCollider* pOther) PURE;
	virtual _bool Intersect_With_OBB_Cube(const CCollider* pOther) PURE;
	virtual _bool Intersect_With_Sphere(const CCollider* pOther) PURE;
	virtual _bool Intersect_With_Capsule(const CCollider* pOther) PURE;
	virtual _bool Intersect_With_Rect(const CCollider* pOther) PURE;
	virtual _bool Intersect_With_Line(const CCollider* pOther) PURE;

public:
	virtual CComponent* Clone(void* pArg) PURE;
	virtual void Free() override;
};

END