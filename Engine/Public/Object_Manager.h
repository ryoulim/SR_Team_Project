#pragma once

#include "Base.h"

/* �������� �����Ͽ� �纻��ü�� �����Ѵ�. */
/* ����Ʈ��� �����Ѵ�. ����Ʈ == ��ü�� == ���� ������ ���� ���ؿ����� �׷����´�. */

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

	// ������Ʈ Ǯ ����
	HRESULT Create_Object_Pool(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, const _wstring& strObjectTag, _uint iPoolSize, void* pArg);
	// ������Ʈ Ǯ ����
	HRESULT Release_Object_Pool(const _wstring& strObjectTag);
	// Ǯ���� ������Ʈ�� ������ Ȱ��ȭ (���� ȣ��)
	_uint Active_Object(const _wstring& strObjectTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg);
	// ������Ʈ�� �ٽ� Ǯ�� ��ȯ (�ʱ�ȭ���� ����)
	_uint Deactive_Object(const _wstring& strObjectTag, class CGameObject* pObject);
	// A�׷�� B�׷��� �浹�� �˻��Ѵ�.

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