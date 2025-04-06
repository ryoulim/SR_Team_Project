#include "Sound_Core.h"
#include "fmod.hpp"
#include "Transform.h"

CSound_Core::CSound_Core(FMOD::System* pCoreSystem, FMOD::Sound* pSound)
    :m_pCoreSystem(pCoreSystem)
    , m_pSound(pSound, [](FMOD::Sound* Sound) {Sound->release(); })
{
}

CSound_Core::CSound_Core(CSound_Core& Prototype)
    :m_pCoreSystem(Prototype.m_pCoreSystem)
    , m_pSound(Prototype.m_pSound)
{
}

void CSound_Core::Play()
{
    if (m_pCoreSystem->playSound(m_pSound.get(), nullptr, true, &m_pChannel))
    {
        m_pChannel = nullptr;
        return;
    }

    m_pChannel->setVolume(m_fVolume);
    m_pChannel->set3DMinMaxDistance(m_fMinDis, m_fMaxDis);
    m_pChannel->set3DAttributes(reinterpret_cast<FMOD_VECTOR*>(&m_vSoundPos), {});

    SetPaused(false);
}

void CSound_Core::Stop()
{
    if (m_pChannel)
    {
        m_pChannel->stop();
        m_pChannel = nullptr;
    }
}

void CSound_Core::Set_Volume(_float Volume)
{
    m_fVolume = Volume;
    if (m_pChannel)
        m_pChannel->setVolume(Volume);
}

void CSound_Core::Update3DPosition(const _float3& vPos)
{
    _float3 vSoundVel = vPos - m_vSoundPos;
    m_vSoundPos = vPos;
    if (m_pChannel)
        m_pChannel->set3DAttributes(reinterpret_cast<FMOD_VECTOR*>(&m_vSoundPos), reinterpret_cast<FMOD_VECTOR*>(&vSoundVel));

}

void CSound_Core::Set3DState(_float fMin, _float fMax)
{
    m_fMinDis = fMin;
    m_fMaxDis = fMax;
}

_bool CSound_Core::IsPlaying() const
{
    if (!m_pChannel) 
        return false;

    _bool playing = false;
    m_pChannel->isPlaying(&playing);
    return playing;
}

void CSound_Core::SetPaused(_bool paused)
{
    if (m_pChannel)
        m_pChannel->setPaused(paused);
}

CSound_Core* CSound_Core::Create(FMOD::System* pCoreSystem, FMOD::Sound* pSound)
{
	return new CSound_Core(pCoreSystem, pSound);
}

CSound_Core* CSound_Core::Clone()
{
    return new CSound_Core(*this);
}

void CSound_Core::Free()
{
    __super::Free();
    Stop();
}
