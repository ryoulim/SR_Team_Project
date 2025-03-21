#pragma once
#include "Collider.h"

BEGIN(Engine)

class ENGINE_DLL CCollider_Rect final : public CCollider
{
public:
	typedef struct tagRectInfo
	{
		_float3 vCenter;   // 중심점
		_float3 vRight;    // 가로 방향 벡터 (정규화된 벡터)
		_float3 vUp;       // 세로 방향 벡터 (정규화된 벡터)
		_float fWidth;     // 가로 길이
		_float fHeight;

		_float3 GetNormal() const {
			return vRight.Cross(vUp).Normalize();
		}
	}INFO;
private:
	CCollider_Rect(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCollider_Rect(const CCollider_Rect& Prototype);
	virtual ~CCollider_Rect() = default;

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
	static CCollider_Rect* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
