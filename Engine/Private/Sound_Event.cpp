#include "Sound_Event.h"

#include "fmod.hpp"
#include "fmod_studio.hpp"

CSound_Event::CSound_Event(FMOD::Studio::EventInstance* pInstance)
	:m_pInstance(pInstance)
{

}

void CSound_Event::Play()
{
	m_pInstance->start();
}

void CSound_Event::Stop(_bool immediate)
{
	m_pInstance->stop(immediate ? FMOD_STUDIO_STOP_IMMEDIATE : FMOD_STUDIO_STOP_ALLOWFADEOUT);
}

_bool CSound_Event::isPlaying() const
{
	FMOD_STUDIO_PLAYBACK_STATE state;
	m_pInstance->getPlaybackState(&state);
	return state == FMOD_STUDIO_PLAYBACK_PLAYING;
}

void CSound_Event::SetParameter(const string& name, float value)
{
	m_pInstance->setParameterByName(name.c_str(), value);
}

float CSound_Event::GetParameter(const string& name) const
{
	float value;
	m_pInstance->getParameterByName(name.c_str(), &value);
	return value;
}

void CSound_Event::Set3DAttributes(const _float3& pos, const _float3& vel)
{
	FMOD_3D_ATTRIBUTES attr = {};
	attr.position = { pos.x, pos.y, pos.z };
	attr.velocity = { vel.x, vel.y, vel.z };

	// 안전한 기본 방향 벡터
	attr.forward = { 0.f, 0.f, 1.f }; // 정면
	attr.up = { 0.f, 1.f, 0.f }; // 위쪽

	m_pInstance->set3DAttributes(&attr);
}

void CSound_Event::SetVolume(_float volume)
{
	m_pInstance->setVolume(volume);
}

_float CSound_Event::GetVolume() const
{
	float volume;
	m_pInstance->getVolume(&volume);
	return volume;
}

void CSound_Event::SetPitch(_float pitch)
{
	m_pInstance->setPitch(pitch);
}

_float CSound_Event::GetPitch() const
{
	float pitch;
	m_pInstance->getPitch(&pitch);
	return pitch;
}

void CSound_Event::SetTimelinePosition(_int ms)
{
	m_pInstance->setTimelinePosition(ms);
}

_int CSound_Event::GetTimelinePosition() const
{
	int position;
	m_pInstance->getTimelinePosition(&position);
	return position;
}

CSound_Event* CSound_Event::Create(FMOD::Studio::EventInstance* pInstance)
{
	return new CSound_Event(pInstance);
}

void CSound_Event::Free()
{
	__super::Free();

	if (m_pInstance)
	{
		m_pInstance->release();
		m_pInstance = nullptr;
	}
}
