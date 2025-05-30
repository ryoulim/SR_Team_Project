#include "Timer_Manager.h"
#include "Timer.h"

CTimer_Manager::CTimer_Manager()
{
}

_float CTimer_Manager::Get_TimeDelta(const _wstring& strTimerTag)
{
	CTimer* pTimer = Find_Timer(strTimerTag);
	if (nullptr == pTimer)
		return 0.f;

	return pTimer->Get_TimeDelta();	
}

_float CTimer_Manager::Get_Scaled_TimeDelta(const _wstring& strTimerTag)
{
	CTimer* pTimer = Find_Timer(strTimerTag);
	if (nullptr == pTimer)
		return 0.f;

	return pTimer->Get_Scaled_TimeDelta();
}

void CTimer_Manager::Set_TimeScale(const _wstring& strTimerTag, _float fDeltaScale)
{
	CTimer* pTimer = Find_Timer(strTimerTag);
	if (nullptr == pTimer)
		return;

	pTimer->Set_TimeScale(fDeltaScale);
}

HRESULT CTimer_Manager::Add_Timer(const _wstring& strTimerTag)
{
	if (nullptr != Find_Timer(strTimerTag))
		return E_FAIL;

	CTimer* pTimer = CTimer::Create();
	if (nullptr == pTimer)
		return E_FAIL;

	m_Timers.emplace(strTimerTag, pTimer);

	return S_OK;
}

void CTimer_Manager::Update(const _wstring& strTimerTag)
{
	CTimer* pTimer = Find_Timer(strTimerTag);
	if (nullptr == pTimer)
		return;

	pTimer->Update();
}

CTimer* CTimer_Manager::Find_Timer(const _wstring& strTimerTag)
{
	auto	iter = m_Timers.find(strTimerTag);

	if (iter == m_Timers.end())
		return nullptr;

	return iter->second;	
}

CTimer_Manager* CTimer_Manager::Create()
{
	return new CTimer_Manager();	
}

void CTimer_Manager::Free()
{
	__super::Free();

	for (auto& Pair : m_Timers)
		Safe_Release(Pair.second);

	m_Timers.clear();

}
