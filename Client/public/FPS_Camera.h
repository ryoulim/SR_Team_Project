// 내 클래스 이름 : FPS_Camera
// 부모 클래스 이름 : Camera

#pragma once
#include "Camera.h"
#include "Client_Defines.h"
#include "GameInstance.h"

BEGIN(Client)

class CFPS_Camera final : public CCamera
{
	typedef struct tagFPS_CameraDesc : public CCamera::DESC
	{
		_float			fMouseSensor;
	}DESC;
private:
	CFPS_Camera(LPDIRECT3DDEVICE9 pGraphic_Device);
	CFPS_Camera(const CFPS_Camera& Prototype);
	virtual ~CFPS_Camera() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	void Mouse_Move();
	void Mouse_Fix();
	void StartShake(_float fIntensity, _float fDuration, _float fShakeFreqPos, _float fShakeFreqRot);
	void StartRecoil(_float fIntensity, _float fDuration);
	void Zoom(_float fFOV,_float Time);
	void Mode_Switch()
	{
		m_bMouseFixMod = !m_bMouseFixMod;
		ShowCursor(!m_bMouseFixMod);
	}

private:
	_float3 m_vInitLook{};
	_float3 m_vInitPos{};
	_float m_bMouseFixMod{FALSE};
	_float m_fMouseSensor{};

	_float m_fShakeTime = 0.f;
	_float m_fShakeDuration = 0.f;
	_float m_fShakeIntensity = 0.f;
	_bool  m_bShake = FALSE;
	_bool	m_bRecoil = FALSE;

	_float3  m_vCurrentShakePos = { 0.f, 0.f, 0.f};
	_float3  m_vCurrentShakeRot = { 0.f, 0.f, 0.f };

	_float m_fShakeFreqPos = 100.f; // 위치 진동 속도
	_float m_fShakeFreqRot = 40.f;  // 회전 진동 속도

	_bool	m_bZoom{ FALSE };
	_float	m_fOriginFov{};
	_float	m_fTargetFov{};
	_float	m_fZoomTimeLimit{};
	_float	m_fZoomTimer{};

private:
	virtual void		Update_Projection_Matrix() override;
	virtual HRESULT		Ready_Components(void* pArg) override;

	void				Update_Camera_Shake(_float fTimedelta);
	void				Update_Weapon_Recoil(_float fTimedelta);
	void				Reset_Shake_And_Recoil();

	void				Update_Zoom(_float fTimedelta);

public:
	static CFPS_Camera* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CCamera* Clone(void* pArg) override;
	virtual void Free();
};

END