// 내 클래스 이름 : FPS_Camera
// 부모 클래스 이름 : Camera

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

	void				Mouse_Move();
	void				Mouse_Fix();


private:
	virtual void		Update_Projection_Matrix() override;
	virtual HRESULT		Ready_Components(void* pArg) override;
	_float3				m_vInitPos{};
	_float3				m_vInitLook{};
	_float				m_fMouseSensor{};
	_bool				m_bBouseFixMod{ TRUE };

public:
	static CFPS_Camera* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CCamera* Clone(void* pArg) override;
	virtual void Free();
};

END