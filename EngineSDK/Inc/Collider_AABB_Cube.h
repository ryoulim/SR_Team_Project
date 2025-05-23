#pragma once
#include "Collider.h"

BEGIN(Engine)

class ENGINE_DLL CCollider_AABB_Cube final : public CCollider
{
public:
	typedef struct tagAABB_CubeInfo
	{
		_float3 vCenter;
		// 큐브의 월드 스페이스상 가장 작은 버텍스 좌표
		_float3 vMinPos;
		// 큐브의 월드 스페이스상 가장 큰 버텍스 좌표
		_float3 vMaxPos;
	}INFO;
private:
	CCollider_AABB_Cube(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCollider_AABB_Cube(const CCollider_AABB_Cube& Prototype);
	virtual ~CCollider_AABB_Cube() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual void Update_Collider() override;
	virtual void Update_Scale(const _float3& vScale) override;
	virtual _bool RayCasting(const _float3& rayOrigin, const _float3& rayDir) override;
	virtual _bool RayCast_Downward(const _float3& rayOrigin) override;

	void Update_Rotation(const _float3& radEuler); // 직각으로 회전된 AABB 큐브용 셋팅함수

	const INFO* Get_Info() const { return &m_tInfo; };
	virtual const _float3& Get_Pos() const override {
		return m_tInfo.vCenter;
	}
	virtual const _float Get_MaxLength() const override {
		return m_fMaxLength;
	}
private:
	INFO m_tInfo{};
	_float3 m_vHalfScale{};
	_float m_fMaxLength{};

	virtual _bool Intersect_With_AABB_Cube(const CCollider* pOther) override;
	virtual _bool Intersect_With_OBB_Cube(const CCollider* pOther) override;
	virtual _bool Intersect_With_Sphere(const CCollider* pOther) override;
	virtual _bool Intersect_With_Capsule(const CCollider* pOther) override;
	virtual _bool Intersect_With_Rect(const CCollider* pOther) override;
	virtual _bool Intersect_With_Line(const CCollider* pOther) override;

public:
	static CCollider_AABB_Cube* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
