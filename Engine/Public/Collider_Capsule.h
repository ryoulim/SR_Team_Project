#pragma once
#include "Collider.h"

BEGIN(Engine)

//YÃàÀÌ Á¤·ÄµÈ Ä¸½¶ÀÔ´Ï´Ù.
class ENGINE_DLL CCollider_Capsule final : public CCollider
{
public:
	typedef struct tagCollider_Capsule
	{
		const CTransform* pTransform;
	}DESC;
	typedef struct tagCapsuleInfo
	{
		_float3 vCenter;
		_float fHeight;
		_float fRadius;
	}INFO;
private:
	CCollider_Capsule(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCollider_Capsule(const CCollider_Capsule& Prototype);
	virtual ~CCollider_Capsule() = default;

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
	static CCollider_Capsule* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
