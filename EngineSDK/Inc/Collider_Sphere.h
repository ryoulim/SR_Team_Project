#pragma once
#include "Collider.h"

BEGIN(Engine)

class ENGINE_DLL CCollider_Sphere final : public CCollider
{
public:
	typedef struct tagCollider_AABB_Cube
	{
		const CTransform* pTransform;
		_float fRadius;
	}DESC;
	typedef struct tagSphereInfo
	{
		_float3 vPosition;
		_float	fRadius;
	}INFO;
private:
	CCollider_Sphere(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCollider_Sphere(const CCollider_Sphere& Prototype);
	virtual ~CCollider_Sphere() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Update_Collider(const CTransform* pTransform) override;

	const INFO* Get_Info() const { return &m_tInfo; };
private:
	INFO m_tInfo{};

	virtual _bool Intersect_With_AABB_Cube(const CCollider* pOther) override;
	virtual _bool Intersect_With_OBB_Cube(const CCollider* pOther) override;
	virtual _bool Intersect_With_Sphere(const CCollider* pOther) override;
	virtual _bool Intersect_With_Capsule(const CCollider* pOther) override;
	virtual _bool Intersect_With_Rect(const CCollider* pOther) override;
	virtual _bool Intersect_With_Line(const CCollider* pOther) override;

public:
	static CCollider_Sphere* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
