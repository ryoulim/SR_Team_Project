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

	void	Smooth_Damping(_float3 vTargetPos,_float fSmoothTime);

private: 
	_float3			m_vTargetPos{};
	_float3			m_vCurrentVelocity{};
	_float			m_fOmega{};

private:
	virtual void		Update_Projection_Matrix() override;
	virtual HRESULT		Ready_Components(void* pArg) override;

public:
	static CTPS_Camera* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CCamera* Clone(void* pArg) override;
	virtual void Free();
};

END

