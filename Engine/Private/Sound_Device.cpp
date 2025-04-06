#include "Sound_Device.h"
#include "Sound_Control_Group.h"
#include "Sound_Event.h"
#include "Sound_Core.h"
#include "Transform.h"
#include <filesystem>

CSound_Device::CSound_Device(const string& strBankFilePath, const string& strSoundFolderPath)
	:m_strBankFilePath(strBankFilePath)
	, m_strSoundFolderPath(strSoundFolderPath)
{
}

HRESULT CSound_Device::Initialize()
{
	if (FMOD::Studio::System::create(&m_pStudioSystem) != FMOD_OK)
		return E_FAIL;

	if (m_pStudioSystem->getCoreSystem(&m_pCoreSystem) != FMOD_OK)
		return E_FAIL;

	// �⺻ ���: �ڵ� ����
	m_pCoreSystem->setOutput(FMOD_OUTPUTTYPE_AUTODETECT);

	// ����̹� Ž��
	int numDrivers = 0;
	m_pCoreSystem->getNumDrivers(&numDrivers);

	for (int i = 0; i < numDrivers; ++i)
	{
		char name[256] = {};
		FMOD_GUID guid = {};
		int systemRate = 0;
		FMOD_SPEAKERMODE speakerMode = FMOD_SPEAKERMODE_DEFAULT;
		int speakerModeChannels = 0;

		m_pCoreSystem->getDriverInfo(i, name, sizeof(name), &guid, &systemRate, &speakerMode, &speakerModeChannels);

		// ASIO ����̹� �켱 ����
		if (strstr(name, "ASIO"))
		{
			m_pCoreSystem->setOutput(FMOD_OUTPUTTYPE_ASIO);
			m_pCoreSystem->setDriver(i);
			m_pCoreSystem->setDSPBufferSize(64, 2); // ��������
			m_pCoreSystem->setSoftwareFormat(systemRate, speakerMode, 0);
			break;
		}
	}

	// WASAPI �� �Ϲ� ����̹��� ���: ������ ���� ����
	// (ASIO ����̹��� �� ������� �״�� ����)
	FMOD_OUTPUTTYPE outputType = FMOD_OUTPUTTYPE_AUTODETECT; // �ʱ�ȭ
	m_pCoreSystem->getOutput(&outputType);

	if (outputType != FMOD_OUTPUTTYPE_ASIO)
	{
		m_pCoreSystem->setDSPBufferSize(256, 4);
		m_pCoreSystem->setSoftwareFormat(48000, FMOD_SPEAKERMODE_DEFAULT, 0);
	}

	// ��� ����
	FMOD_ADVANCEDSETTINGS adv = {};
	adv.cbSize = sizeof(FMOD_ADVANCEDSETTINGS);
	adv.DSPBufferPoolSize = 512;

	m_pCoreSystem->setAdvancedSettings(&adv);

	// 3D ����: �⺻�� ����
	m_pCoreSystem->set3DSettings(1.0f, 1.0f, 1.0f);

	// Studio �ý��� �ʱ�ȭ
	if (m_pStudioSystem->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr) != FMOD_OK)
		return E_FAIL;

	return S_OK;
}

void CSound_Device::Update()
{
	m_pStudioSystem->update();
}

HRESULT CSound_Device::LoadSound(const string& Path, _bool is3D, _bool loop, _bool stream)
{
	namespace fs = std::filesystem;

	if (!m_pCoreSystem)
		return E_FAIL;

	string strFullpath = m_strSoundFolderPath + Path;
	fs::path fsPath(strFullpath);

	if (!fs::exists(fsPath))
		return E_FAIL;

	FMOD_MODE mode = FMOD_DEFAULT;

	if (is3D) mode |= FMOD_3D_CUSTOMROLLOFF;
	else      mode |= FMOD_2D;

	if (loop) mode |= FMOD_LOOP_NORMAL;
	else      mode |= FMOD_LOOP_OFF;

	if (stream) mode |= FMOD_CREATESTREAM;
	else        mode |= FMOD_CREATECOMPRESSEDSAMPLE;

	auto makeKey = [&](const fs::path& filePath) -> string {
	fs::path relPath = fs::relative(filePath, m_strSoundFolderPath);
	relPath.replace_extension(); // Ȯ���� ����
	return relPath.generic_string(); // '/' �ϰ��� ����
		};

	if (fs::is_directory(fsPath))
	{
		for (const auto& entry : fs::recursive_directory_iterator(fsPath))
		{
			if (!entry.is_regular_file())
				continue;

			fs::path filePath = entry.path();
			string key = makeKey(filePath);

			if (m_Sounds.count(key) > 0)
				continue;

			FMOD::Sound* pSound = nullptr;
			FMOD_RESULT result = m_pCoreSystem->createSound(filePath.string().c_str(), mode, nullptr, &pSound);
			if (result == FMOD_OK)
				m_Sounds[key] = pSound;
		}
	}
	else if (fs::is_regular_file(fsPath))
	{
		string key = makeKey(fsPath);

		if (m_Sounds.count(key) == 0)
		{
			FMOD::Sound* pSound = nullptr;
			FMOD_RESULT result = m_pCoreSystem->createSound(fsPath.string().c_str(), mode, nullptr, &pSound);
			if (result != FMOD_OK)
				return E_FAIL;

			m_Sounds[key] = pSound;
		}
	}
	else
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CSound_Device::LoadBank(const string& name)
{
	auto it = m_LoadedBanks.find(name);
	if (it != m_LoadedBanks.end())
		return E_FAIL;

	FMOD::Studio::Bank* bank = nullptr;
	string path = m_strBankFilePath + name + ".bank";
	if (m_pStudioSystem->loadBankFile(path.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &bank))
		return E_FAIL;

	m_LoadedBanks.emplace(name, bank);
	return S_OK;
}

void CSound_Device::UnloadBank(const string& name)
{
	auto it = m_LoadedBanks.find(name);
	if (it != m_LoadedBanks.end())
	{
		it->second->unload();
		m_LoadedBanks.erase(it);
	}
}

CSound_Control_Group* CSound_Device::Create_Control_Group(const string& name)
{
	FMOD::ChannelGroup* pChannelGroup { nullptr };
	m_pCoreSystem->createChannelGroup(name.c_str(), &pChannelGroup);

	return nullptr;
}

CSound_Event* CSound_Device::Create_Event_Instance(const string& eventPath)
{
	FMOD::Studio::EventDescription* desc = nullptr;
	if (m_pStudioSystem->getEvent(eventPath.c_str(), &desc))
		return nullptr;

	FMOD::Studio::EventInstance* instance = nullptr;
	if (desc->createInstance(&instance) || !instance) 
		return nullptr;

	return CSound_Event::Create(instance);
}

CSound_Core* CSound_Device::Create_Core_Instance(const string& strSoundtag)
{
	// ���尡 �ε��Ǿ� �־������ ������
	auto Iter = m_Sounds.find(strSoundtag);
	if (Iter == m_Sounds.end())
		return nullptr;

	// ���� FMod���� ���� ī���� ���� ��¥�� ����̽��� ���� �������� ������
	return CSound_Core::Create(m_pCoreSystem, Iter->second);
}

void CSound_Device::Set_Listener_Position(const CTransform* pTransform, const _float3& vel)
{
	_float3 vLook = pTransform->Get_State(CTransform::STATE_LOOK)->Normalize();
	_float3 vUp = pTransform->Get_State(CTransform::STATE_UP)->Normalize();
	memcpy(&m_ListenerAttr.position, pTransform->Get_State(CTransform::STATE_POSITION), sizeof _float3) ;
	memcpy(&m_ListenerAttr.forward, &vLook, sizeof _float3);
	memcpy(&m_ListenerAttr.up, &vUp, sizeof _float3);
	m_ListenerAttr.velocity = { vel.x, vel.y, vel.z };

	//m_pStudioSystem->setListenerAttributes(0, &m_ListenerAttr);

	m_pCoreSystem->set3DListenerAttributes(0, &m_ListenerAttr.position, &m_ListenerAttr.velocity,
		&m_ListenerAttr.forward, &m_ListenerAttr.up);
}

void CSound_Device::Set_Master_Volume(_float volume)
{
	FMOD::Studio::Bus* masterBus = nullptr;
	m_pStudioSystem->getBus("bus:/", &masterBus);
	if (masterBus)
		masterBus->setVolume(volume);
}

CSound_Device* CSound_Device::Create(const string& strBankFilePath, const string& strSoundFolderPath)
{
	CSound_Device* pInstance = new CSound_Device(strBankFilePath, strSoundFolderPath);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CSound_Device");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSound_Device::Free()
{
	__super::Free();

	for (auto& pair : m_LoadedBanks)
	{
		pair.second->unload();
	}
	m_LoadedBanks.clear();

	if (m_pStudioSystem)
	{
		m_pStudioSystem->unloadAll();
		m_pStudioSystem->release();
		m_pStudioSystem = nullptr;
	}
	m_pCoreSystem = nullptr;
}
