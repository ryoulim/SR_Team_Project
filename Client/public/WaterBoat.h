#pragma once
#include "PSystem.h"

//스노우 파티클은 포지션이 아닌 바운딩 박스로 위치를 지정한다.

class CWaterBoat : public CPSystem
{
public:
	CWaterBoat(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName);
	CWaterBoat(const CPSystem& Prototype);
	virtual ~CWaterBoat() = default;

	HRESULT Ready_Components(void* pArg);
	virtual void resetParticle(Attribute* attribute);
	void SetPosition(_float3 Position);
	virtual EVENT Update(_float timeDelta);

	HRESULT SetUp_RenderState();
	virtual HRESULT Render() override;
	HRESULT Release_RenderState();

	virtual HRESULT Initialize(void* pArg)override;

	HRESULT Ready_Particle();

	void SetDead() { m_bDead = true; }
	inline float Lerp(float a, float b, float t)
	{
		return a + (b - a) * t;
	}

public:
	static CWaterBoat* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();

private:
	float		m_fFrame = 0;
	_bool		m_bDoOnce = false;
	_bool		m_bDead = false;
};

