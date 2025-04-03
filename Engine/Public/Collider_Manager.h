#pragma once
#include "Base.h"

BEGIN(Engine)

class CCollider_Manager : public CBase
{
private:
	CCollider_Manager();
	virtual ~CCollider_Manager() = default;

public:
	// �ݶ��̴� �׷��� ������ �˾ƾ���
	HRESULT Initialize(_uint iNumGroups);
	// �ݶ��̴��� �޾Ƽ� �־���
	HRESULT Add_Collider(class CCollider* pCollider, _uint iColliderGroupID);
	// ������Ʈ Ű�� �޾Ƽ� �װſ� �ش��ϴ� �ݶ��̴��� �ϴ� ��������
	void Delete_Collider(const class CGameObject* pOwner);
	// �����Ҵ��� ���͸� ������ ���� ���� ��ȸ���鼭 ���빰�� �ϴ� �����.
	void Clear();

	void Intersect(_uint iColliderGroupID1, _uint iColliderGroupID2);

	// ����� ������̿� �� ����?
	_bool RaycastBetweenPoints(const _float3& Point1, const _float3& Point2, _uint ColliderGroupID);
	// ���� ĳ��Ʈ (��ŷ�� ����� �븻�� �ʿ��ҽ� Get_Last_Ray_Normal ȣ��)
	CGameObject* Raycast(const _float3& rayOrigin, const _float3& rayDir, _float rayLength, const initializer_list<_uint>& ColliderGroupIDs, _uint& _Out_ ColliderID);
	
	// �Ʒ����� ���� ĳ��Ʈ (y ������ ��ȯ, ��ŷ�� ����� �븻�� �ʿ��ҽ� Get_Last_Ray_Normal ȣ��)
	_float Raycast_Downward(const _float3& rayOrigin, _uint iColliderGroupID);

	const list<class CCollider*>* Get_Colliders(_uint iIndex) {
		if (iIndex >= m_iNumGroups)
			return nullptr;
		return &m_pColliders[iIndex];
	}

private:
	_uint										m_iNumGroups = {};
	list<class CCollider*>*						m_pColliders = { nullptr };

public:
	static CCollider_Manager* Create(_uint iNumGroups);
	virtual void Free() override;
};

END