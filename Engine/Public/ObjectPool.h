#pragma once
#include "Base.h"
#include "GameInstance.h"

/* ��ü���� ��Ƴ��´�. */

BEGIN(Engine)

class ENGINE_DLL CObjectPool : public CBase
{
private:
	CObjectPool(_uint iPoolSize);
	virtual ~CObjectPool() = default;

public:
	HRESULT	Initialize(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg);
	void Active_Object(_uint iLevelIndex, const _wstring& strLayertag, void* pArg);

private:
	CGameInstance*	m_pGameInstance{ nullptr };
	vector<class CGameObject*>	m_Objects{};
	_uint	m_iCurIndex{};
	_uint	m_iPoolSize{};

public:
	static CObjectPool* Create(_uint iPoolSize, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg = nullptr);
	virtual void Free() override;
};

END