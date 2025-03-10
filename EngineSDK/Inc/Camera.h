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

		_float		fAspect;
		_float		fFovy;
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

public:
	void Add_Fov(_float fAspect) { m_CameraDesc.fFovy += fAspect; }

protected:
	HRESULT Bind_Projection_Transform();

protected:
	class CTransform* m_pTransform = nullptr;
	DESC			m_CameraDesc = {};

public:
	virtual CCamera* Clone(void* pArg) PURE;
	virtual void Free() override;
};

END