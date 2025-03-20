#pragma once
#include "Collider.h"

BEGIN(Engine)

class ENGINE_DLL CCollider_Line final : public CCollider
{
public:
	typedef struct tagLineInfo
	{
		_float3 vStart;
		_float3 vEnd;
	}INFO;
private:
	CCollider_Line(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCollider_Line(const CCollider_Line& Prototype);
	virtual ~CCollider_Line() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual void Update_Collider() override;
	virtual void Update_Scale(const _float3& vScale) override;
	
	const INFO* Get_Info() const { return &m_tInfo; };
private:
	INFO m_tInfo{};
	_float3 m_vDiffFromCenter{};

	virtual _bool Intersect_With_AABB_Cube(const CCollider* pOther) override;
	virtual _bool Intersect_With_OBB_Cube(const CCollider* pOther) override;
	virtual _bool Intersect_With_Sphere(const CCollider* pOther) override;
	virtual _bool Intersect_With_Capsule(const CCollider* pOther) override;
	virtual _bool Intersect_With_Rect(const CCollider* pOther) override;
	virtual _bool Intersect_With_Line(const CCollider* pOther) override;

public:
	static CCollider_Line* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
