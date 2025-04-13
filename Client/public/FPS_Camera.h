// �� Ŭ���� �̸� : FPS_Camera
// �θ� Ŭ���� �̸� : Camera

#pragma once
#include "Camera.h"
#include "Client_Defines.h"
#include "GameInstance.h"

BEGIN(Client)

class CFPS_Camera final : public CCamera
{
public:
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
	void StartTilt(_float fRadian, _float fDuration);
	void Zoom(_float fFOV,_float Time);

private:
	// ī�޶� �⺻ ����
	_float m_fMouseSensor{};

	// ī�޶� ����ũ ����
	_bool  m_bShake = FALSE;
	_float m_fShakeTime = 0.f;
	_float m_fShakeDuration = 0.f;
	_float m_fShakeIntensity = 0.f;
	_float m_fShakeFreqPos = 100.f; // ��ġ ���� �ӵ�
	_float m_fShakeFreqRot = 40.f;  // ȸ�� ���� �ӵ�
	_float3  m_vCurrentShakePos = { 0.f, 0.f, 0.f };
	_float3  m_vCurrentShakeRot = { 0.f, 0.f, 0.f };

	// �ѱ� �ݵ� ����
	_float3 m_vCurrentRecoilRot = { 0.f, 0.f, 0.f }; // ���� �����ӿ� ����� �ݵ� ȸ��
	_float3 m_vRecoilVelocity = { 0.f, 0.f, 0.f }; // ����� ������ �ӵ�
	_float3 m_vAccumulatedRecoil = { 0.f, 0.f, 0.f }; // ��ü ������ Recoil ȸ��
	_float3 m_vTargetRecoil = {};

	_float m_fRecoilTime = 0.f;
	_float m_fRecoilDuration = 0.15f;
	_float m_fRecoilIntensity = 2.f;
	_float m_fRecoilIntensityYaw = 0.5f;
	_bool  m_bRecoil = FALSE;
	_bool m_bRecoilRecovering = FALSE; 

	// ī�޶� ƿƮ ���� = Z�� ȸ��
	_bool  m_bTilt = FALSE;
	_float m_fTargetTiltRadian = 0.f;
	_float m_fTiltTime = 0.f;
	_float m_fTiltDuration = 0.f;
	_float m_fPrevTiltRadian{};

	//���� �ܾƿ� ����
	_bool	m_bZoom{ FALSE };
	_float	m_fOriginFov{};
	_float	m_fTargetFov{};
	_float	m_fZoomTimeLimit{};
	_float	m_fZoomTimer{};

private:
	virtual void		Update_View_Matrix() override;
	virtual void		Update_Projection_Matrix() override;
	virtual HRESULT		Ready_Components(void* pArg) override;

	void				Update_Camera_Shake(_float fTimedelta);
	void				Update_Weapon_Recoil(_float fTimedelta);
	void				Reset_Shake_And_Recoil();

	void				Update_Zoom(_float fTimedelta);
	void				Update_Tilt(_float fTimedelta);

public:
	static CFPS_Camera* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CCamera* Clone(void* pArg) override;
	virtual void Free();
};

END