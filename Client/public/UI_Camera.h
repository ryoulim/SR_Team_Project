#pragma once
#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CUI_Camera final : public CCamera
{
public:
	typedef struct tagUI_CameraDesc : public CCamera::DESC
	{

	}DESC;
private:
	CUI_Camera(LPDIRECT3DDEVICE9 pGraphicDevice);
	CUI_Camera(const CUI_Camera& Prototype);
	virtual ~CUI_Camera() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual EVENT Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	virtual void		Update_Projection_Matrix() override;
	virtual HRESULT		Ready_Components(void* pArg) override;

public:
	static CUI_Camera* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CCamera* Clone(void* pArg) override;
	virtual void Free() override;
};

END