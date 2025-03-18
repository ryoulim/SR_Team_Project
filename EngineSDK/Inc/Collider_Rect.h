#pragma once
#include "Collider.h"

BEGIN(Engine)

class ENGINE_DLL CCollider_Rect final : public CCollider
{
public:
	typedef struct tagCollider_Rect
	{
		// �ݶ��̴��� ������ ��ü�� ����
		const CTransform* pTransform;
		// �ݶ��̴��� ������
		_float2 vScale;
	}DESC;
	typedef struct tagRectInfo
	{
		_float3 vCenter;   // �߽���
		_float3 vRight;    // ���� ���� ���� (����ȭ�� ����)
		_float3 vUp;       // ���� ���� ���� (����ȭ�� ����)
		_float fWidth;     // ���� ����
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
	static CCollider_Rect* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
