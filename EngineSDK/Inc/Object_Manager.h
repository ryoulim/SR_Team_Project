#pragma once

#include "Base.h"

/* �������� �����Ͽ� �纻��ü�� �����Ѵ�. */
/* ����Ʈ��� �����Ѵ�. ����Ʈ == ��ü�� == ���� ������ ���� ���ؿ����� �׷����´�. */

BEGIN(Engine)

class CObject_Manager final : public CBase
{
private:
	CObject_Manager();
	virtual ~CObject_Manager() = default;

public:
	HRESULT Initialize(_uint iNumLevels);
	HRESULT Add_GameObject(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg);
	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);
	void Clear(_uint iLevelIndex);

public:
	class CGameObject* Find_Object(_uint iLevelIndex, const _wstring& strLayerTag, _uint iVectorIndex = 0);
	vector<CGameObject*>& Find_Objects(_uint iLevelIndex, const _wstring& strLayerTag);

	// ������Ʈ Ǯ ����
	HRESULT Create_Object_Pool(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, const _wstring& strObjectTag, _uint iPoolSize);
	// ������Ʈ Ǯ ����
	HRESULT Release_Object_Pool(const _wstring& strObjectTag);
	// Ǯ���� ������Ʈ�� ������ Ȱ��ȭ (�̴ϼȶ����� ��ȣ��)
	_uint Active_Object(const _wstring& strObjectTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg);
	// ������Ʈ�� �ٽ� Ǯ�� ��ȯ (�ʱ�ȭ���� ����)
	_uint Deactive_Object(const _wstring& strObjectTag, class CGameObject* pObject);
	// A�׷�� B�׷��� �浹�� �˻��Ѵ�.
	void Intersect(_uint iLevelIndex, const _wstring& strLayerTag1, const _wstring& strLayerTag2);
public:

private:	
	_uint								m_iNumLevels = {};
	unordered_map<_wstring, class CObjectPool*>	m_ObjectPool;
	typedef unordered_map<_wstring, class CLayer*> LAYERS;
	LAYERS*								m_pLayers = { nullptr };
	class CGameInstance*				m_pGameInstance = { nullptr };

private:
	class CLayer* Find_Layer(_uint iLevelIndex, const _wstring& strLayerTag);
	class CObjectPool* Find_Object_Pool(const _wstring& strObjectTag);

public:
	static CObject_Manager* Create(_uint iNumLevels);
	virtual void Free() override;
};

END