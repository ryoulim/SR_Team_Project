#pragma once
#include "Collider.h"

BEGIN(Engine)

//Y���� ���ĵ� ĸ���Դϴ�.
class ENGINE_DLL CCollider_Capsule final : public CCollider
{
public:
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
	virtual void Update_Collider() override;
	virtual void Update_Scale(const _float3& vScale) override;
	virtual _bool RayCasting(const _float3& rayOrigin, const _float3& rayDir) override;

	const INFO* Get_Info() const { return &m_tInfo; };
	virtual const _float3& Get_Pos() const override {
		return m_tInfo.vCenter;
	}
	virtual const _float Get_MaxLength() const override {
		return m_fMaxLength;
	}
private:
	INFO m_tInfo{};
	_float m_fMaxLength{};

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
