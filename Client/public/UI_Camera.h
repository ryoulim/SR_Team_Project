#pragma once
#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CUI_Camera final : public CCamera
{
private:
	CUI_Camera(LPDIRECT3DDEVICE9 pGraphicDevice);
	CUI_Camera(const CUI_Camera& Prototype);
	virtual ~CUI_Camera() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Bind_Projection_Transform();
	HRESULT	Ready_Components();

public:
	static CUI_Camera* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CCamera* Clone(void* pArg) override;
	virtual void Free() override;
};

END