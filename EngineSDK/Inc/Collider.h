#pragma once
#include "Component.h"
#include "Transform.h"

BEGIN(Engine)

class ENGINE_DLL CCollider abstract : public CComponent
{
public:
	typedef struct tagCColliderDesc
	{
		CTransform* pTransform;
		// 콜라이더의 크기 (구(반지름)와 라인(길이)은 x값만 씁니다)
		_float3 vScale;
		// 실제 트랜스폼의 위치와 콜라이더 위치의 간격
		_float3 vOffSet;
	}DESC;

protected:
	enum COLLIDER_TYPE { AABB_CUBE,OBB_CUBE,CAPSULE,SPHERE,LINE,RECT };
protected:
	CCollider(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCollider(const CCollider& Prototype);
	virtual ~CCollider() = default;

public:
	virtual HRESULT Initialize_Prototype(COLLIDER_TYPE Type);
	virtual HRESULT Initialize(void* pArg) override;
	// 요놈은 매프레임 업데이트 해야함(트랜스폼의 위치로 연동)
	virtual void Update_Collider() PURE;
	// 콜라이더의 크기 (구(반지름)와 라인(길이)은 x값만 씁니다)
	virtual void Update_Scale(const _float3& vScale) PURE;
	// 다른 콜라이더와의 충돌을 감지합니다.
	_bool Check_Intersect(const CCollider* pOther);

	const COLLIDER_TYPE Get_Type() const { return m_eType; }
	static const _float3& Get_Last_Collision_Depth() { return m_vLast_Collision_Depth; }
	static const _float3& Get_Last_Collision_Pos() { return m_vLast_Collision_Pos; }
	virtual _bool RayCasting(const _float3& rayOrigin, const _float3& rayDir) { return FALSE; }

protected:
	static _float3 m_vLast_Collision_Depth;
	static _float3 m_vLast_Collision_Pos;

protected:
	class CTransform* m_pTransform { nullptr };
	//_float3			m_vDirectionVector{}; 
	_float3			m_vOffSet{};

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