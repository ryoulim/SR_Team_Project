// 내 클래스 이름 : CameraManager
// 부모 클래스 이름 : GameObject

#include "CameraManager.h"

#include "Dynamic_Camera.h"
#include "FPS_Camera.h"
#include "TPS_Camera.h"
#include "UI_Camera.h"

CCameraManager::CCameraManager()
	:m_pGameInstance(CGameInstance::Get_Instance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CCameraManager::Initialize()
{	
	CFPS_Camera::DESC FPSCameraDesc{};
	FPSCameraDesc.fFar = 2000.f;
	FPSCameraDesc.fNear = 0.1f;
	FPSCameraDesc.fMouseSensor = 0.1f;
	FPSCameraDesc.fFov = 60.f;
	FPSCameraDesc.vAt = { 0.f,0.f,1.f };
	FPSCameraDesc.vEye = { 0.f,0.f,0.f };
	FPSCameraDesc.fSpeedPerSec = 300.f;
	FPSCameraDesc.fRotationPerSec = 0.f;
	
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_FPS_Camera"),
		LEVEL_STATIC, TEXT("Layer_Camera"), &FPSCameraDesc)))
		return E_FAIL;
	
	CCamera::DESC TPSCameraDesc{};
	TPSCameraDesc.vEye = _float3(0.f, 0.f, -20.f);
	TPSCameraDesc.vAt = _float3();
	TPSCameraDesc.fFov = 60.f;
	TPSCameraDesc.fNear = 0.1f;
	TPSCameraDesc.fFar = 2000.f;
	
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_TPS_Camera"),
		LEVEL_STATIC, TEXT("Layer_Camera"), &TPSCameraDesc)))
		return E_FAIL;

	CDynamic_Camera::DESC DynamicCameraDesc{};
	DynamicCameraDesc.fFar = 2000.f;
	DynamicCameraDesc.fNear = 0.1f;
	DynamicCameraDesc.fMouseSensor = 0.1f;
	DynamicCameraDesc.fFov = 60.f;
	DynamicCameraDesc.vAt = { 0.f,0.f,1.f };
	DynamicCameraDesc.vEye = { 0.f,0.f,0.f };
	DynamicCameraDesc.fSpeedPerSec = 300.f;
	DynamicCameraDesc.fRotationPerSec = 0.f;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Dynamic_Camera"),
		LEVEL_STATIC, TEXT("Layer_Camera"), &DynamicCameraDesc)))
		return E_FAIL;

	CUI_Camera::DESC UICameraDesc{};
	UICameraDesc.fFar = 1000.f;
	UICameraDesc.fNear = 0.f;
	UICameraDesc.fFov = 0;
	UICameraDesc.vAt = { 0.f,0.f,1.f };
	UICameraDesc.vEye = { 0.f,0.f,0.f };
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Camera"),
		LEVEL_STATIC, TEXT("Layer_Camera"), &UICameraDesc)))
		return E_FAIL;

	auto Camera_List = m_pGameInstance->Find_Objects(LEVEL_STATIC, TEXT("Layer_Camera"));

	for (auto Camera : *Camera_List)
	{
		m_Cameras.push_back(static_cast<CCamera*>(Camera));
		Safe_AddRef(Camera);
	}

	m_Cameras[UI]->Set_Active(TRUE);

	return S_OK;
}

void CCameraManager::Priority_Update(_float fTimeDelta)
{
	m_Cameras[m_eID]->Priority_Update(fTimeDelta);
}

EVENT CCameraManager::Update(_float fTimeDelta)
{
	return	m_Cameras[m_eID]->Update(fTimeDelta);
}

void CCameraManager::Late_Update(_float fTimeDelta)
{
	m_Cameras[m_eID]->Late_Update(fTimeDelta);
}

HRESULT CCameraManager::Render()
{
	return S_OK;
}

void CCameraManager::Shake_Camera(_float fIntensity, _float fDuration, _float fShakeFreqPos, _float fShakeFreqRot)
{
	// 일단 지금은 FPS만 흔들자
	static_cast<CFPS_Camera*>(m_Cameras[m_eID])->StartShake(fIntensity, fDuration, fShakeFreqPos, fShakeFreqRot);
}

void CCameraManager::StartRecoil(_float fIntensity, _float fDuration)
{
	static_cast<CFPS_Camera*>(m_Cameras[m_eID])->StartRecoil(fIntensity, fDuration);
}

void CCameraManager::Zoom(_float fFOV, _float Time)
{
	static_cast<CFPS_Camera*>(m_Cameras[m_eID])->Zoom(fFOV, Time);
}

void CCameraManager::Switch(CCameraManager::ID _ID)
{
	if (_ID >= UI)
		return;

	//UI 카메라 전까지 돌기
	for (_uint i = FPS; i < UI; ++i)
		m_Cameras[i]->Set_Active(FALSE);

	m_Cameras[_ID]->Set_Active(TRUE);
}

void CCameraManager::Mouse_Fix_Mode_Switch()
{
	// 일단 FPS만 해
	static_cast<CFPS_Camera*>(m_Cameras[FPS])->Mode_Switch();
}

CCameraManager* CCameraManager::Create()
{
	CCameraManager* pInstance = new CCameraManager();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CCameraManager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCameraManager::Free()
{
	__super::Free();
	
	Safe_Release(m_pGameInstance);
	for (auto& Camera : m_Cameras)
		Safe_Release(Camera);

	m_Cameras.clear();
}