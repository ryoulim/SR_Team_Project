#pragma once
#include "Base.h"

BEGIN(FMOD)
class ChannelGroup;
class DSP;
END

BEGIN(Engine)

// FMOD의 Sound_Core를 그룹별로 관리할 수 있는 래핑 클래스
class ENGINE_DLL CSound_Control_Group : public CBase
{
private:
    CSound_Control_Group(FMOD::ChannelGroup* pChannelGroup);
    virtual ~CSound_Control_Group() = default;
    
public:
    void Add_Control_Group(class CSound_Control_Group* pControl_Group);
    void Add_Sound(class CSound_Core* pSound_Core);

private:
    FMOD::ChannelGroup* m_pChannelGroup = { nullptr };
    
public:
    static CSound_Control_Group* Create(FMOD::ChannelGroup* pChannelGroup);
    virtual void Free() override;
};

END