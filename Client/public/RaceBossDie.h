#pragma once
#include "PSystem.h"

class CRaceBossDie : public CPSystem
{
public:
	CRaceBossDie(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName);
	CRaceBossDie(const CPSystem& Prototype);
	virtual ~CRaceBossDie() = default;

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
	static CRaceBossDie* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();

private:
	float		m_fFrame = 0;
	_bool		m_bDoOnce = false;
	_bool		m_bDead = false;
};

