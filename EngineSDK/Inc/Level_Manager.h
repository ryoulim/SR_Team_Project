#pragma once

#include "Base.h"

/* ���� ������ �ı��ϰ� ���Ӱ� �Ҵ�� ������ �ּҸ� ��� �ִ´�. */
/* ���� �۾��� ���� ��, ���� ������ �ڿ��� ��� �ı��Ѵ�. */
/* ���� �Ҵ�� ������ �����ϰ� �����ش�. */

BEGIN(Engine)

class CLevel_Manager final : public CBase
{
private:
	CLevel_Manager();
	virtual ~CLevel_Manager() = default;

public:
	HRESULT Change_Level(_uint iLevelIndex, class CLevel* pNewLevel);
	void Update(_float fTimeDelta);
	HRESULT Render();
	
	void Change_Level(_uint iLevelIndex);
	_uint Get_CurrentLevelIndex() { return m_iCurrentLevelIndex; }
	_uint Get_PreviousLevelIndex() { return m_iPreviousLevelIndex; }

private:
	_uint					m_iCurrentLevelIndex = {};
	_uint					m_iPreviousLevelIndex = {};
	class CLevel*			m_pCurrentLevel = { nullptr };
	class CGameInstance*	m_pGameInstance = { nullptr };

public:
	static CLevel_Manager* Create();
	virtual void Free() override;
};

END