#pragma once

#include "Base.h"

/* 레벨별로 구분하여 사본객체를 보관한다. */
/* 리스트들로 보관한다. 리스트 == 객체들 == 내가 나누고 싶은 기준에따라 그룹짓는다. */

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
	// 매개변수에 맞는 오브젝트를 찾아서 반환해준다.
	class CGameObject* Find_Object(_uint iLevelIndex, const _wstring& strLayerTag, _uint iVectorIndex = 0);
	// 매개변수에 맞는 레이어의 오브젝트 벡터를 찾아서 반환해준다.
	vector<CGameObject*>& Find_Objects(_uint iLevelIndex, const _wstring& strLayerTag);

private:	
	_uint								m_iNumLevels = {};
	typedef unordered_map<_wstring, class CLayer*> LAYERS;
	LAYERS*								m_pLayers = { nullptr };
	class CGameInstance*				m_pGameInstance = { nullptr };

private:
	class CLayer* Find_Layer(_uint iLevelIndex, const _wstring& strLayerTag);

public:
	static CObject_Manager* Create(_uint iNumLevels);
	virtual void Free() override;
};

END