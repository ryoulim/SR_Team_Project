#pragma once
#include "Camera.h"

BEGIN(Client)

class CDynamic_Camera final : public CCamera
{
private: 
	CDynamic_Camera(LPDIRECT3DDEVICE9 pGraphicDevice);
	CDynamic_Camera(const CDynamic_Camera& Prototype);
	virtual ~CDynamic_Camera() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	void				Key_Input(_float fTimeDelta);
	void				Mouse_Move();
	void				Mouse_Fix();

	HRESULT				Ready_Components(void* pArg);

public:
	static CDynamic_Camera* Create(LPDIRECT3DDEVICE9 pGraphicDevice);
	virtual CCamera* Clone(void* pArg) override;
	virtual void Free() override;
};

END