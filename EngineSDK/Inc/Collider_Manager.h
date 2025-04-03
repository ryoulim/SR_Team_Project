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

	// 여기와 여기사이에 뭐 있음?
	_bool RaycastBetweenPoints(const _float3& Point1, const _float3& Point2, _uint ColliderGroupID);
	// 레이 캐스트 (피킹된 평면의 노말이 필요할시 Get_Last_Ray_Normal 호출)
	CGameObject* Raycast(const _float3& rayOrigin, const _float3& rayDir, _float rayLength, const initializer_list<_uint>& ColliderGroupIDs, _uint& _Out_ ColliderID);
	
	// 아래로의 레이 캐스트 (y 포지션 반환, 피킹된 평면의 노말이 필요할시 Get_Last_Ray_Normal 호출)
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