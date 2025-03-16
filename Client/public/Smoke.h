#pragma once
#include "PSystem.h"

//����� ��ƼŬ�� �������� �ƴ� �ٿ�� �ڽ��� ��ġ�� �����Ѵ�.

class CSmoke : public CPSystem
{
public:
	CSmoke(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName);
	CSmoke(const CPSystem& Prototype);
	virtual ~CSmoke() = default;

	HRESULT Ready_Components();
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
	float		m_fFrame = 0;
};

