#pragma once
#include "Base.h"

BEGIN(Engine)

class CCollider_Manager : public CBase
{
private:
	CCollider_Manager();
	virtual ~CCollider_Manager() = default;

public:
	// 콜라이더 그룹의 갯수를 알아야함
	HRESULT Initialize(_uint iNumGroups);
	// 콜라이더를 받아서 넣어줌
	HRESULT Add_Collider(class CCollider* pCollider, _uint iColliderGroupID);
	// 오브젝트 키를 받아서 그거에 해당하는 콜라이더를 싹다 지워버림
	void Delete_Collider(const class CGameObject* pOwner);
	// 동적할당한 벡터를 지우진 말고 벡터 순회돌면서 내용물만 싹다 비우자.
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