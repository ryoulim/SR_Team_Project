// 내 클래스 이름 : CameraManager
// 부모 클래스 이름 : GameObject

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
	/// 카메라 흔들어 제껴
	/// </summary>
	/// <param name="fIntensity"> 카메라 흔드는 세기 </param>
	/// <param name="fDuration"> 카메라 흔드는 시간 </param>
	/// <param name="fShakeFreqPos"> 이동 진동 주파수(클수록 빨라짐) </param>
	/// <param name="fShakeFreqRot"> 회전 진동 주파수(클수록 빨라짐)</param>
	virtual void Shake_Camera(_float fIntensity = 1.f, _float fDuration = 1.f, _float fShakeFreqPos = 100.f, _float fShakeFreqRot = 40.f);
	void StartRecoil(_float fIntensity = 0.4f, _float fDuration = 0.04f);
	void Zoom(_float fFOV, _float Time);
	void Tilt(_float fRadian, _float _fDuration);
	void Set_Mouse_Fix(_bool isFixMode);

	void Start_CutScene(vector<_float3>* pMovePoints, vector<_float3>* pLookPoints, _float fCameraSpeed, _bool* _Out_ pEndFlag);
	void Start_CutScene(const _float3& vPos, const _float3& vLook);

	CGameObject* Get_Camera(CCameraManager::ID _ID) {
		return static_cast<CGameObject*>(m_Cameras[_ID]);
	}
	void Switch(CCameraManager::ID _ID); 
	CCameraManager::ID Get_CurCameraID() { return m_eID; }
private:

	CGameInstance* m_pGameInstance{ nullptr };
	CCameraManager::ID m_eID{ FPS };
	vector<CCamera*> m_Cameras;
public:
	static CCameraManager* Create();
	virtual void Free();
};

END