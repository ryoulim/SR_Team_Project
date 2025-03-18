#pragma once
#include "Collider.h"

BEGIN(Engine)

class ENGINE_DLL CCollider_OBB_Cube final : public CCollider
{
public:
	typedef struct tagCollider_OBB_Cube
	{
		// 콜라이더를 포함한 객체의 원본
		const CTransform* pTransform;
		// 콜라이더의 스케일
		_float3 vScale;
	}DESC;
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
	static CCollider_OBB_Cube* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
