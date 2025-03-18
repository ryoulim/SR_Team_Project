#pragma once
#include "PSystem.h"

//스노우 파티클은 포지션이 아닌 바운딩 박스로 위치를 지정한다.

class CSmoke : public CPSystem
{
public:
	typedef struct tagSmokeDesc : public CPSystem::DESC
	{
		bool		isLoop = false;
		_float		fFrame;
		_float		fVelocity;
		_float3		vecMinDirection;
		_float3		vecMaxDirection;
	}DESC;

public:
	CSmoke(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName);
	CSmoke(const CPSystem& Prototype);
	virtual ~CSmoke() = default;

	HRESULT Ready_Components(void* pArg);
	virtual void resetParticle(Attribute* attribute);
	virtual EVENT Update(_float timeDelta);
	virtual HRESULT Render() override;

	virtual HRESULT Initialize(void* pArg)override;

	void	FrameUpdate(float timeDelta);
	HRESULT Ready_Particle();

	float GetRandomColor(float lowBound, float highBound);

public:
	static CSmoke* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();

private:
	bool		m_bIsLoop = false;
	_float		m_fFrame = 0.f;
	_float		m_fVelocity = 0.f;
	_float3		m_vecMinDirection = { 0.f, 0.f, 0.f };
	_float3		m_vecMaxDirection = { 0.f, 0.f, 0.f };
};

