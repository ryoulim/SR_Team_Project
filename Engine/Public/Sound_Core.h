#pragma once
#include "Base.h"

BEGIN(FMOD)
class System;
class Sound;
class Channel;
class ChannelGroup;
END

BEGIN(Engine)

// FMOD_CORE_API 기반 래핑 클래스
class ENGINE_DLL CSound_Core : public CBase
{
private:
	friend class CSound_Control_Group;
	CSound_Core(FMOD::System* pCoreSystem, FMOD::Sound* pSound);
	virtual ~CSound_Core() = default;

public:
	// 주의점 - 이 클래스는 마지막으로 재생된 채널만 저장하고 있습니다.
	// 즉 여러번 호출시에는 마지막 재생했던 사운드를 제외하고는 제어가 불가능합니다.
	void Play(_float Volume);
	void Stop();
	void Set_Volume(_float Volume);
	// 소리가 나는 월드 포지션, 움직이지 않는 물체의 경우 fTimedelta를 0으로 두어주세요
	void Set3DPosition(const _float3& pos, _float fTimedelta = 0.f);
	// 소리의 감쇠곡선을 결정할 죄소,최대 거리
	void Set3DMinMaxDistance(_float fMin, _float fMax);
	_bool IsPlaying() const;
	void SetPaused(_bool paused);
private:
	FMOD::System* m_pCoreSystem = { nullptr };
	FMOD::Sound* m_pSound = { nullptr };
	FMOD::Channel* m_pChannel = { nullptr };
	FMOD::ChannelGroup* m_pChannelGroup = { nullptr };
	_float3		m_vSoundPos{};
	_float3		m_vSoundVel{};

private:
	static _float3		m_vCurve[6];
	static const _float m_fCurveRatios[6];
	static const _float m_fCurveVolumes[6];

public:
	static CSound_Core* Create(FMOD::System* pCoreSystem, FMOD::Sound* pSound);
	virtual void Free() override;
};

END