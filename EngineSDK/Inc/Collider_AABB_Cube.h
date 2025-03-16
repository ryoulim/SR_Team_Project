#pragma once
#include "Collider.h"

BEGIN(Engine)

class ENGINE_DLL CCollider_AABB_Cube final : public CCollider
{
public:
	typedef struct tagCollider_AABB_Cube
	{
		// 콜라이더를 포함한 객체의 원본
		const CTransform* pTransform;
		// 콜라이더의 스케일
		_float3 vScale;
	}DESC;
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
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Update_Collider(const CTransform* pTransform) override;

	const INFO* Get_Info() const { return &m_tInfo; };
private:
	INFO m_tInfo{};
	_float3 m_vHalfScale{};

	virtual _bool Intersect_With_AABB_Cube(const CCollider* pOther) override;
	virtual _bool Intersect_With_OBB_Cube(const CCollider* pOther) override;
	virtual _bool Intersect_With_Sphere(const CCollider* pOther) override;
	virtual _bool Intersect_With_Capsule(const CCollider* pOther) override;

public:
	static CCollider_AABB_Cube* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
