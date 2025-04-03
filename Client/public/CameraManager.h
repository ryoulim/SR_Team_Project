// �� Ŭ���� �̸� : CameraManager
// �θ� Ŭ���� �̸� : GameObject

#pragma once
#include "Client_Defines.h"
#include "GameInstance.h"
#include "Camera.h"

#define CAMERA_MANAGER static_cast<CCameraManager*>(m_pGameInstance->Find_Manager(TEXT("Camera_Manager")))

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)

class CCameraManager final : public CBase
{
public:
	enum ID { FPS, TPS, DYNAMIC, CUTSCENE, UI  };
private:
	CCameraManager();
	virtual ~CCameraManager() = default;

public:
	HRESULT Initialize();

	/// <summary>
	/// ī�޶� ���� ����
	/// </summary>
	/// <param name="fIntensity"> ī�޶� ���� ���� </param>
	/// <param name="fDuration"> ī�޶� ���� �ð� </param>
	/// <param name="fShakeFreqPos"> �̵� ���� ���ļ�(Ŭ���� ������) </param>
	/// <param name="fShakeFreqRot"> ȸ�� ���� ���ļ�(Ŭ���� ������)</param>
	virtual void Shake_Camera(_float fIntensity = 1.f, _float fDuration = 1.f, _float fShakeFreqPos = 100.f, _float fShakeFreqRot = 40.f);
	void StartRecoil(_float fIntensity = 1.f, _float fDuration = 1.f);
	void Zoom(_float fFOV, _float Time);
	void Set_Mouse_Fix(_bool isFixMode);
	void Start_CutScene(vector<_float3>* pMovePoints, vector<_float3>* pLookPoints, _float fCameraSpeed, _bool* _Out_ pEndFlag);

	CGameObject* Get_Camera(CCameraManager::ID _ID) {
		return static_cast<CGameObject*>(m_Cameras[_ID]);
	}
	void Switch(CCameraManager::ID _ID);
private:
	CGameInstance* m_pGameInstance{ nullptr };
	CCameraManager::ID m_eID{ FPS };
	vector<CCamera*> m_Cameras;
public:
	static CCameraManager* Create();
	virtual void Free();
};

END