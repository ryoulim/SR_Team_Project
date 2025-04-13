#pragma once

// 내 클래스 이름 : TPS_Camera
// 부모 클래스 이름 : Camera

#pragma once
#include "Camera.h"
#include "Client_Defines.h"
#include "GameInstance.h"

BEGIN(Client)

class CTPS_Camera final : public CCamera
{
private:
	CTPS_Camera(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTPS_Camera(const CTPS_Camera& Prototype);
	virtual ~CTPS_Camera() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	void	StartShake(_float fIntensity, _float fDuration, _float fShakeFreqPos, _float fShakeFreqRot);
	void	StartRecoil(_float fIntensity, _float fDuration);
	void	Smooth_Damping(_float3 vTargetPos,_float fSmoothTime);

private: 
	_float3			m_vTargetPos{};
	_float3			m_vCurrentVelocity{};
	_float			m_fOmega{};

	// 카메라 쉐이크 관련
	_bool  m_bShake = FALSE;
	_float m_fShakeTime = 0.f;
	_float m_fShakeDuration = 0.f;
	_float m_fShakeIntensity = 0.f;
	_float m_fShakeFreqPos = 100.f; // 위치 진동 속도
	_float m_fShakeFreqRot = 40.f;  // 회전 진동 속도
	_float3  m_vCurrentShakePos = { 0.f, 0.f, 0.f };
	_float3  m_vCurrentShakeRot = { 0.f, 0.f, 0.f };

	// 총기 반동 관련
	_float3 m_vCurrentRecoilRot = { 0.f, 0.f, 0.f }; // 현재 프레임에 적용된 반동 회전
	_float3 m_vRecoilVelocity = { 0.f, 0.f, 0.f }; // 감쇠용 스프링 속도
	_float3 m_vAccumulatedRecoil = { 0.f, 0.f, 0.f }; // 전체 누적된 Recoil 회전
	_float3 m_vTargetRecoil = {};

	_float m_fRecoilTime = 0.f;
	_float m_fRecoilDuration = 0.15f;
	_float m_fRecoilIntensity = 2.f;
	_float m_fRecoilIntensityYaw = 0.5f;
	_bool  m_bRecoil = FALSE;
	_bool m_bRecoilRecovering = FALSE;

private:
	virtual void		Update_View_Matrix() override;
	virtual void		Update_Projection_Matrix() override;
	virtual HRESULT		Ready_Components(void* pArg) override;
	void				Update_Camera_Shake(_float fTimedelta);
	void				Update_Weapon_Recoil(_float fTimedelta);

public:
	static CTPS_Camera* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CCamera* Clone(void* pArg) override;
	virtual void Free();
};

END

