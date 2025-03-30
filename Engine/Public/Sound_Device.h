#pragma once
#include "Base.h"

#include "fmod.hpp"
#include "fmod_studio.hpp"

BEGIN(Engine)

class CSound_Device : public CBase
{
private:
	CSound_Device(const string& strBankFilePath);
	virtual ~CSound_Device() = default;

public:
    HRESULT Initialize();
    void Update();

    HRESULT LoadBank(const string& name);
    void UnloadBank(const string& name);

    class CSound_Control_Group* Create_Control_Group(const string& name);
    class CSound_Event* Create_Event_Instance(const string& eventPath);
    class CSound_Core* Create_Core_Instance(const string& path, _bool is3D, _bool loop, _bool stream);

    void Set_Listener_Position(const class CTransform* pTransform, const _float3& vel);

    void Set_Master_Volume(_float volume);

private:
    string m_strBankFilePath = {};
    FMOD::Studio::System* m_pStudioSystem = nullptr;
    FMOD::System* m_pCoreSystem = nullptr;

    unordered_map<string, FMOD::Studio::Bank*> m_LoadedBanks;
    //unordered_map<string, FMOD::ChannelGroup*> m_LoadedBanks;
    FMOD_3D_ATTRIBUTES m_ListenerAttr{};
public:
    static CSound_Device* Create(const string& strBankFilePath);
    virtual void Free();
};
END