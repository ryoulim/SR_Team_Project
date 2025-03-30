#pragma once
#include "Base.h"

BEGIN(FMOD)
BEGIN(Studio)
class EventInstance;
END
END

BEGIN(Engine)

// FMOD_STUDIO_API 기반 래핑 클래스
class ENGINE_DLL CSound_Event : public CBase
{
private:
    CSound_Event(class FMOD::Studio::EventInstance* pInstance);
    virtual ~CSound_Event() = default;

public:
    void Play();
    void Stop(_bool immediate = false);
    _bool isPlaying() const;

    void SetParameter(const string& name, float value);
    float GetParameter(const string& name) const;

    void Set3DAttributes(const _float3& pos, const _float3& vel = {});

    void SetVolume(_float volume);
    _float GetVolume() const;

    void SetPitch(_float pitch);
    _float GetPitch() const;

    void SetTimelinePosition(_int ms);
    _int GetTimelinePosition() const;

private:
    class FMOD::Studio::EventInstance* m_pInstance = { nullptr };

public:
    static CSound_Event* Create(class FMOD::Studio::EventInstance* pInstance);
    virtual void Free() override;
};

END