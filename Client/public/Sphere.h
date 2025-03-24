#pragma once
#include "PSystem.h"

//스노우 파티클은 포지션이 아닌 바운딩 박스로 위치를 지정한다.

class CSphere : public CPSystem
{
public:
	CSphere(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName);
	CSphere(const CPSystem& Prototype);
	virtual ~CSphere() = default;

	HRESULT Ready_Components(void* pArg);
	virtual void resetParticle(Attribute* attribute);
	virtual EVENT Update(_float timeDelta);
	virtual HRESULT Render() override;

	virtual HRESULT Initialize(void* pArg)override;

	void	FrameUpdate(float timeDelta);
	HRESULT Ready_Particle();

public:
	static CSphere* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();

private:
	float		m_fFrame = 0;
	float		m_fRadius = 0;
	_float3		m_vCenter = { 100.0f, 50.0f, 50.0f };
};

