#pragma once
#include "Base.h"

BEGIN(FMOD)
class ChannelGroup;
class DSP;
END

BEGIN(Engine)

// FMOD�� Sound_Core�� �׷캰�� ������ �� �ִ� ���� Ŭ����
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