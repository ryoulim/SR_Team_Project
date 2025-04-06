#pragma once
#include "Base.h"

#include "fmod.hpp"
#include "fmod_studio.hpp"

BEGIN(Engine)

class CSound_Device : public CBase
{
private:
	CSound_Device();
	virtual ~CSound_Device() = default;

public:
    HRESULT Initialize();
    void Update();

    HRESULT LoadSound(const string& Path, _bool is3D, _bool loop, _bool stream, unordered_map<string, class CSound_Core*>* _Out_ pOut);
    void Set_Listener_Position(const class CTransform* pTransform, const _float3& vel);

    void Set_Master_Volume(_float volume);
private:
    FMOD::System* m_pCoreSystem = nullptr;
    FMOD_3D_ATTRIBUTES m_ListenerAttr{};

public:
    static CSound_Device* Create();
    virtual void Free();
};
END