#pragma once
#include "GameObject.h"
#include "Transform.h"

BEGIN(Engine)

class ENGINE_DLL CCamera abstract : public CGameObject
{
public:
	typedef struct tagCameraDesc : public CTransform::DESC
	{
		_float3		vEye; //À§Ä¡
		_float3		vAt;

		_float		fFov;
		_float		fNear;
		_float		fFar;

	}DESC;

protected:
	CCamera(LPDIRECT3DDEVICE9 pGraphicDevice);
	CCamera(const CCamera& Prototype);
	virtual ~CCamera() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	_float4x4				m_ViewMatrix;
	_float4x4				m_ProjMatrix;
	_float					m_fFov = {};
	_float					m_fAspect = {};
	_float					m_fNear = {};
	_float					m_fFar = {};

	class CTransform* m_pTransformCom = { nullptr };

protected:
	HRESULT Bind_Resource();

private:
	virtual void Update_Projection_Matrix() PURE;
	virtual HRESULT Ready_Components(void* pArg) PURE;

public:
	virtual CCamera* Clone(void* pArg) PURE;
	virtual void Free() override;
};

END