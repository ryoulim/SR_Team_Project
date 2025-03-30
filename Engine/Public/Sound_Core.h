#pragma once
#include "Base.h"

BEGIN(FMOD)
class System;
class Sound;
class Channel;
class ChannelGroup;
END

BEGIN(Engine)

// FMOD_CORE_API ��� ���� Ŭ����
class ENGINE_DLL CSound_Core : public CBase
{
private:
	friend class CSound_Control_Group;
	CSound_Core(FMOD::System* pCoreSystem, FMOD::Sound* pSound);
	virtual ~CSound_Core() = default;

public:
	// ������ - �� Ŭ������ ���������� ����� ä�θ� �����ϰ� �ֽ��ϴ�.
	// �� ������ ȣ��ÿ��� ������ ����ߴ� ���带 �����ϰ�� ��� �Ұ����մϴ�.
	void Play(_float Volume);
	void Stop();
	void Set_Volume(_float Volume);
	// �Ҹ��� ���� ���� ������, �������� �ʴ� ��ü�� ��� fTimedelta�� 0���� �ξ��ּ���
	void Set3DPosition(const _float3& pos, _float fTimedelta = 0.f);
	// �Ҹ��� ������ ������ �˼�,�ִ� �Ÿ�
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