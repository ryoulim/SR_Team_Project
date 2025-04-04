#pragma once

#include "Base.h"

BEGIN(Engine)

class CTimer final : public CBase
{
private:
	CTimer();
	virtual ~CTimer() = default;

public:
	_float Get_TimeDelta() {
		return m_fTimeDelta;
	}
	_float Get_Scaled_TimeDelta() {
		return m_fTimeDelta * m_fDeltaScale;
	}
	void Set_TimeScale(_float fDeltaScale) {
		m_fDeltaScale = fDeltaScale;
	}
public:
	HRESULT Initialize();
	void Update();

private:
	LARGE_INTEGER			m_CurTime{};
	LARGE_INTEGER			m_OldTime{};
	LARGE_INTEGER			m_OriginTime{};
	LARGE_INTEGER			m_CpuTick{};

	_float					m_fTimeDelta{};
	_float					m_fDeltaScale{ 1.f };

public:
	static CTimer* Create();
	virtual void Free();
};

END