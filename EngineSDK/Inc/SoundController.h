#pragma once
#include "Component.h"

BEGIN(FMOD)
class ChannelGroup;
class DSP;
class Sound;
END

BEGIN(Engine)

class ENGINE_DLL CSoundController : public CComponent
{
private:
	CSoundController();
    CSoundController(const CSoundController& Prototype);
	virtual	~CSoundController() = default;

public:
    virtual HRESULT Initialize_Prototype(const string& strFolderPath, _bool is3D, _bool loop, _bool stream);
    virtual HRESULT Initialize(void* pArg) override;

    void Play(const string& strTag);
    void Stop(const string& strTag);
    void SetVolume(const string& strTag,_float Volume);
    // 사운드 포지션 업데이트
    void Update3DPosition(const string& strTag, const _float3& vPos);
    //트랜스폼과 소리의 감쇠곡선을 결정할 죄소,최대 거리
    void Set3DState(const string& strTag, _float fMin, _float fMax);
    _bool IsPlaying(const string& strTag) const;

    void SetVolume(_float Volume);
    // 사운드 포지션 업데이트
    void Update3DPosition(const _float3& vPos);
    //트랜스폼과 소리의 감쇠곡선을 결정할 죄소,최대 거리
    void Set3DState(_float fMin, _float fMax);

private:
    unordered_map<string, class CSound_Core*> m_Sounds;
    class CSound_Core* Find_Sound(const string& strTag) const;

public:
    static CSoundController* Create(const string& strFolderPath, _bool is3D = FALSE, _bool loop = FALSE, _bool stream = FALSE);
    virtual CComponent* Clone(void* pArg) override;
    virtual void Free() override;
};

END