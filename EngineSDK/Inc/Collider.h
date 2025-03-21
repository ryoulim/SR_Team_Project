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
		// �ݶ��̴��� ũ�� (��(������)�� ����(����)�� x���� ���ϴ�)
		_float3 vScale;
		// ���� Ʈ�������� ��ġ�� �ݶ��̴� ��ġ�� ����
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
	// ����� �������� ������Ʈ �ؾ���(Ʈ�������� ��ġ�� ����)
	virtual void Update_Collider() PURE;
	// �ݶ��̴��� ũ�� (��(������)�� ����(����)�� x���� ���ϴ�)
	virtual void Update_Scale(const _float3& vScale) PURE;
	// �ٸ� �ݶ��̴����� �浹�� �����մϴ�.
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