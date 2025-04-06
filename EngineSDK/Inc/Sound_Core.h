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
class CSound_Core : public CBase
{
private:
	friend class CSound_Control_Group;
	CSound_Core(FMOD::System* pCoreSystem, FMOD::Sound* pSound);
	CSound_Core(CSound_Core& Prototype);
	virtual ~CSound_Core() = default;

public:
	// ������ - �� Ŭ������ ���������� ����� ä�θ� �����ϰ� �ֽ��ϴ�.
	// �� ������ ȣ��ÿ��� ������ ����ߴ� ���带 �����ϰ�� ��� �Ұ����մϴ�.
	void Play();
	void Stop();
	void Set_Volume(_float Volume);
	// ���� ������ ������Ʈ
	void Update3DPosition(const _float3& vPos);
	//Ʈ�������� �Ҹ��� ������ ������ �˼�,�ִ� �Ÿ�
	void Set3DState(_float fMin, _float fMax);
	_bool IsPlaying() const;
	void SetPaused(_bool paused);

private:
	FMOD::System* m_pCoreSystem = { nullptr };
	shared_ptr<FMOD::Sound> m_pSound = {nullptr};
	FMOD::Channel* m_pChannel = { nullptr };

	_float m_fVolume{ 1.f };
	_float3		m_vSoundPos{};
	_float		m_fMinDis;
	_float		m_fMaxDis;

public:
	static CSound_Core* Create(FMOD::System* pCoreSystem, FMOD::Sound* pSound);
	CSound_Core* Clone();
	virtual void Free() override;
};

END