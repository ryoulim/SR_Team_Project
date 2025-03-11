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
	// �Ű������� �´� ������Ʈ�� ã�Ƽ� ��ȯ���ش�.
	class CGameObject* Find_Object(_uint iLevelIndex, const _wstring& strLayerTag, _uint iVectorIndex = 0);
	// �Ű������� �´� ���̾��� ������Ʈ ���͸� ã�Ƽ� ��ȯ���ش�.
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