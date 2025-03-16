#pragma once
#include "PSystem.h"
#include "Dynamic_Camera.h"

class CBulletTracer : public CPSystem
{
public:
	typedef struct tagPSystemDesc
	{
		_float3 vMoveCamera;
	}DESC;

public:
	CBulletTracer(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName);
	CBulletTracer(const CPSystem& Prototype);
	virtual ~CBulletTracer() = default;

	virtual void resetParticle(Attribute* attribute);
	virtual EVENT Update(_float timeDelta);
	virtual HRESULT Render() override;

	virtual HRESULT Initialize(void* pArg)override;

	HRESULT Ready_Components();
	
	float GetRandomColor(float lowBound, float highBound);
	void FrameUpdate(float timeDelta);

public:
	static CBulletTracer* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();

private:
	float	m_fFrame = 0;
};

