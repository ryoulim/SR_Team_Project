#pragma once
#include "Base.h"

#include "fmod.hpp"
#include "fmod_studio.hpp"

BEGIN(Engine)

class CSound_Device : public CBase
{
private:
	CSound_Device(const string& strBankFilePath, const string& strSoundFolderPath);
	virtual ~CSound_Device() = default;

public:
    HRESULT Initialize();
    void Update();

    HRESULT LoadSound(const string& Path, _bool is3D, _bool loop, _bool stream, unordered_map<string, class CSound_Core*>* _Out_ pOut);
    HRESULT LoadBank(const string& name);
    void UnloadBank(const string& name);

    class CSound_Control_Group* Create_Control_Group(const string& name);
    class CSound_Event* Create_Event_Instance(const string& eventPath);
 //   class CSound_Core* Create_Core_Instance(const string& strSoundtag);

    void Set_Listener_Position(const class CTransform* pTransform, const _float3& vel);

    void Set_Master_Volume(_float volume);

private:
    string m_strSoundFolderPath = {};
    string m_strBankFilePath = {};
    FMOD::Studio::System* m_pStudioSystem = nullptr;
    FMOD::System* m_pCoreSystem = nullptr;

    unordered_map<string, FMOD::Studio::Bank*> m_LoadedBanks;
    //unordered_map<string, FMOD::ChannelGroup*> m_LoadedBanks;

    // 사운드의 여러번 로딩을 막기위한 것
    using SOUNDLAYER = unordered_map <string, FMOD::Sound*>;
    unordered_map<_wstring, SOUNDLAYER*> m_SoundLayers;
    FMOD_3D_ATTRIBUTES m_ListenerAttr{};
public:
    static CSound_Device* Create(const string& strBankFilePath, const string& strSoundFolderPath);
    virtual void Free();
};
END