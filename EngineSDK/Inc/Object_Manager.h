#pragma once

#include "Base.h"

/* 레벨별로 구분하여 사본객체를 보관한다. */
/* 리스트들로 보관한다. 리스트 == 객체들 == 내가 나누고 싶은 기준에따라 그룹짓는다. */

BEGIN(Engine)
class CGameObject;
class CObject_Manager final : public CBase
{
private:
	CObject_Manager();
	virtual ~CObject_Manager() = default;

public:
	HRESULT Initialize(_uint iNumLevels);
	HRESULT Add_GameObject(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg);
	HRESULT Add_GameObjectReturn(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, CGameObject** ppOut, void* pArg);
	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);
	void Clear(_uint iLevelIndex);

public:
	class CGameObject* Find_Object(_uint iLevelIndex, const _wstring& strLayerTag, _uint iVectorIndex = 0);
	list<CGameObject*>* Find_Objects(_uint iLevelIndex, const _wstring& strLayerTag);

	// 오브젝트 풀 생성
	HRESULT Create_Object_Pool(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, const _wstring& strObjectTag, _uint iPoolSize, void* pArg);
	// 오브젝트 풀 삭제
	HRESULT Release_Object_Pool(const _wstring& strObjectTag);
	// 풀에서 오브젝트를 꺼내서 활성화 (리셋 호출)
	_uint Active_Object(const _wstring& strObjectTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg);
	// 오브젝트를 다시 풀로 반환 (초기화따윈 없음)
	_uint Deactive_Object(const _wstring& strObjectTag, class CGameObject* pObject);
	// A그룹과 B그룹의 충돌을 검사한다.

#define _COLLIDER_MANAGER
#ifndef _COLLIDER_MANAGER
	void Intersect(_uint iLevelIndex, const _wstring& strLayerTag1, const _wstring& strLayerTag2);

	_bool Raycast(const _float3& rayOrigin, const _float3& rayDir, _uint iLevelIndex, const _wstring& strLayerTag);

	_bool Raycast_Downward(const _float3& rayOrigin, _uint iLevelIndex, const _wstring& strLayerTag);
#endif // !COLLIDER_MANAGER

	_bool IsPointInFrustum(const _float3& Point);
	void Update_Frustum(const _float4x4& viewProj);


private:	
	_uint								m_iNumLevels = {};
	unordered_map<_wstring, class CObjectPool*>	m_ObjectPool;
	typedef unordered_map<_wstring, class CLayer*> LAYERS;
	LAYERS*								m_pLayers = { nullptr };
	class CGameInstance*				m_pGameInstance = { nullptr };
	D3DXPLANE							m_Frustum[6] = {};

private:
	class CLayer* Find_Layer(_uint iLevelIndex, const _wstring& strLayerTag);
	class CObjectPool* Find_Object_Pool(const _wstring& strObjectTag);

public:
	static CObject_Manager* Create(_uint iNumLevels);
	virtual void Free() override;
};

END