#pragma once
#include "PSystem.h"
#include "Dynamic_Camera.h"

class CEmptyBullet : public CPSystem
{
public:
	typedef struct tagPSystemDesc
	{
		_float3 vMoveCamera;
	}DESC;

public:
	CEmptyBullet(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName);
	CEmptyBullet(const CPSystem& Prototype);
	virtual ~CEmptyBullet() = default;

	virtual void resetParticle(Attribute* attribute);
	virtual EVENT Update(_float timeDelta);
	virtual HRESULT Render() override;

	virtual HRESULT Initialize(void* pArg)override;

	HRESULT Ready_Components(void* pArg);
	
	float GetRandomColor(float lowBound, float highBound);
	void FrameUpdate(float timeDelta);

public:
	static CEmptyBullet* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();

private:
	float	m_fFrame = 0;
};

