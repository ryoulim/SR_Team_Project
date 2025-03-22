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

	_bool Raycast(const _float3& rayOrigin, const _float3& rayDir, _uint iColliderGroupID);

	_bool Raycast_Downward(const _float3& rayOrigin, _uint iColliderGroupID);

private:
	_uint										m_iNumGroups = {};
	list<class CCollider*>*						m_pColliders = { nullptr };

public:
	static CCollider_Manager* Create(_uint iNumGroups);
	virtual void Free() override;
};

END