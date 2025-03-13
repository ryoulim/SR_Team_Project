#pragma once
#include "PSystem.h"
#include "Dynamic_Camera.h"

class CGun : public CPSystem
{
public:
	CGun(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName);
	CGun(const CPSystem& Prototype);
	virtual ~CGun() = default;

	virtual void resetParticle(Attribute* attribute);
	virtual EVENT Update(_float timeDelta);
	virtual HRESULT SetUp_RenderState() override;
	virtual HRESULT Render() override;
	virtual HRESULT Release_RenderState() override;

	virtual HRESULT Initialize(void* pArg)override;

	HRESULT Ready_Components();
	
	float GetRandomColor(float lowBound, float highBound);
	void FrameUpdate(float timeDelta);

public:
	static CGun* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();

private:
	float m_fFrame = 0;

};

