#pragma once
#include "Component.h"
#define _COLLIDERRENDER

BEGIN(Engine)

class ENGINE_DLL CCollider abstract : public CComponent
{
public:
	friend class CCollider_Manager;
	typedef struct tagCColliderDesc
	{
		//this를 넣어라
		class CGameObject* pOwner;
		// 트랜스폼을 넣어라
		class CTransform* pTransform;
		// 콜라이더의 크기 (구(반지름)와 라인(길이)은 x값만 씁니다)
		_float3 vScale;
		// 실제 트랜스폼의 위치와 콜라이더 위치의 간격
		_float3 vOffSet;
		// 이 콜라이더의 그룹이 뭔지 넣어라
		_uint iColliderGroupID;
		// 이 콜라이더가 충돌되면 어떻게 처리할지 판별할 고유 아이디를 넣어라
		_uint iColliderID;
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
	void Update_OffSet(const _float3& vOffSet) {
		m_vOffSet = vOffSet;
		Update_Collider();
	}
#ifdef _COLLIDERRENDER
public:
	virtual void Render();
	static _bool m_bColliderRender;

protected:
	class CTransform* m_pRenderTransform = { nullptr };
	class CVIBuffer* m_pRenderBuffer = { nullptr };
public:
#endif
	// 다른 콜라이더와의 충돌을 감지합니다.
	_bool Check_Intersect(const CCollider* pOther);

	// 최적화를 위해 필요한 함수들
	virtual const _float3& Get_Pos() const PURE;
	virtual const _float Get_MaxLength() const PURE;

	class CGameObject* Get_Owner() const { 
		return m_pOwner; 
	}
	const COLLIDER_TYPE Get_Type() const { return m_eType; }
	const _uint Get_ID() const { return m_iColliderID; }
	static const _float3& Get_Last_Collision_Depth() { return m_vLast_Collision_Depth; }
	static const _float3& Get_Last_Collision_Pos() { return m_vLast_Collision_Pos; }
	virtual _bool RayCasting(const _float3& rayOrigin, const _float3& rayDir) { return FALSE; }
	virtual _bool RayCast_Downward(const _float3& rayOrigin) { return FALSE; }

protected:
	static _float3 m_vLast_Collision_Depth;
	static _float3 m_vLast_Collision_Pos;

protected:
	class CGameObject* m_pOwner{ nullptr };
	class CTransform* m_pTransform { nullptr };
	_float3			m_vOffSet{};

private:
	COLLIDER_TYPE m_eType{};
	_uint m_iColliderID{};

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