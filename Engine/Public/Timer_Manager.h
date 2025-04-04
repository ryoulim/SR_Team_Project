#pragma once

#include "Base.h"

BEGIN(Engine)

class CTimer_Manager final : public CBase
{
private:
	CTimer_Manager();
	virtual ~CTimer_Manager() = default;

public:
	_float Get_TimeDelta(const _wstring& strTimerTag);
	_float Get_Scaled_TimeDelta(const _wstring& strTimerTag);
	void Set_TimeScale(const _wstring& strTimerTag, _float fDeltaScale);

public:
	HRESULT Add_Timer(const _wstring& strTimerTag);
	void Update(const _wstring& strTimerTag);

private:
	map<const _wstring, class CTimer*>			m_Timers;

private:
	class CTimer* Find_Timer(const _wstring& strTimerTag);

public:
	static CTimer_Manager* Create();
	virtual void Free() override;
};

END