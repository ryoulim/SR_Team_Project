#include "Sound_Core.h"
#include "fmod.hpp"
#include "Transform.h"

_float3 CSound_Core::m_vCurve[6]{};
const _float CSound_Core::m_fCurveRatios[6] = { 0.0f, 0.1f, 0.25f, 0.5f, 0.8f, 1.0f };  // 0~1 °Å¸® ºñÀ²
const _float CSound_Core::m_fCurveVolumes[6] = {1.0f, 0.9f, 0.75f, 0.5f, 0.2f, 0.0f};  // °¨¼è º¼·ý


CSound_Core::CSound_Core(FMOD::System* pCoreSystem, FMOD::Sound* pSound)
    :m_pCoreSystem(pCoreSystem)
    , m_pSound(pSound)
{
}

void CSound_Core::Play(_float Volume)
{
    if (m_pCoreSystem->playSound(m_pSound, m_pChannelGroup, true, &m_pChannel))
    {
        m_pChannel = nullptr;
        return;
    }

    m_pChannel->set3DCustomRolloff(reinterpret_cast<FMOD_VECTOR*>(m_vCurve),
        sizeof(m_vCurve) / sizeof(FMOD_VECTOR));

    m_pChannel->setVolume(Volume);
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
    if (m_pChannel)
        m_pChannel->setVolume(Volume);
}

void CSound_Core::Update3DPosition()
{
    const _float3& vCurPos = *m_pTransform->Get_State(CTransform::STATE_POSITION);

    _float3 vSoundVel = vCurPos - m_vSoundPos;
    m_vSoundPos = vCurPos;
    if (m_pChannel)
        m_pChannel->set3DAttributes(reinterpret_cast<FMOD_VECTOR*>(&m_vSoundPos), reinterpret_cast<FMOD_VECTOR*>(&vSoundVel));

}

void CSound_Core::Set3DState(CTransform* pTransfrom, _float fMin, _float fMax)
{
    m_pTransform = pTransfrom;
    Safe_AddRef(m_pTransform);

    const _float3& vCurPos = *m_pTransform->Get_State(CTransform::STATE_POSITION);
    m_pChannel->set3DAttributes(reinterpret_cast<FMOD_VECTOR*>(&m_vSoundPos), {});

    for (_uint i = 0; i < 6; ++i)
    {
        m_vCurve[i].x = fMin + (fMax - fMin) * m_fCurveRatios[i];  // °Å¸® ¸®¸ÅÇÎ
        m_vCurve[i].y = m_fCurveVolumes[i];                        // º¼·ý
    }
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

void CSound_Core::Free()
{
    __super::Free();
    Stop();

    Safe_Release(m_pTransform);
}
