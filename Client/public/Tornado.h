#pragma once
#include "PSystem.h"

//����� ��ƼŬ�� �������� �ƴ� �ٿ�� �ڽ��� ��ġ�� �����Ѵ�.

class CTornado : public CPSystem
{
public:
	CTornado(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName);
	CTornado(const CPSystem& Prototype);
	virtual ~CTornado() = default;

	HRESULT Ready_Components(void* pArg);
	virtual void resetParticle(Attribute* attribute);
	virtual EVENT Update(_float timeDelta);
	virtual HRESULT Render() override;

	virtual HRESULT Initialize(void* pArg)override;

	void	FrameUpdate(float timeDelta);
	HRESULT Ready_Particle();

public:
	static CTornado* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();

private:
	float		m_fFrame = 0;
};

