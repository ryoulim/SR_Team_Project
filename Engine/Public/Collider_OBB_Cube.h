#pragma once
#include "Collider.h"

BEGIN(Engine)

class ENGINE_DLL CCollider_OBB_Cube final : public CCollider
{
public:
	typedef struct tagOBB_CubeInfo
	{
		_float3 vPosition;
		_float3 vAxis[3];
		_float3 vHalfScale;
	}INFO;
private:
	CCollider_OBB_Cube(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCollider_OBB_Cube(const CCollider_OBB_Cube& Prototype);
	virtual ~CCollider_OBB_Cube() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual void Update_Collider() override;
	virtual void Update_Scale(const _float3& vScale) override;

	virtual _bool RayCasting(const _float3& rayOrigin, const _float3& rayDir) override;
	virtual _bool RayCast_Downward(const _float3& rayOrigin) override;

public:
	const INFO* Get_Info() const { return &m_tInfo; };
	virtual const _float3& Get_Pos() const override {
		return m_tInfo.vPosition;
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
	static CCollider_OBB_Cube* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
