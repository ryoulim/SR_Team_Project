#pragma once
#include "Camera.h"

BEGIN(Client)

class CDynamic_Camera final : public CCamera
{
public:
	typedef struct tagDynamic_CameraDesc : public CCamera::DESC
	{
		_float			fMouseSensor;

	}DESC;
private: 
	CDynamic_Camera(LPDIRECT3DDEVICE9 pGraphicDevice);
	CDynamic_Camera(const CDynamic_Camera& Prototype);
	virtual ~CDynamic_Camera() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual EVENT Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	_float				m_fMouseSensor = {};

private:
	virtual void		Update_Projection_Matrix() override;
	virtual HRESULT		Ready_Components(void* pArg) override;

	void				Key_Input(_float fTimeDelta);
	void				Mouse_Move();
	void				Mouse_Fix();

public:
	static CDynamic_Camera* Create(LPDIRECT3DDEVICE9 pGraphicDevice);
	virtual CCamera* Clone(void* pArg) override;
	virtual void Free() override;
};

END