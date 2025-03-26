#pragma once
#include "PSystem.h"
#include "Effect.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)

class CFlyEffect : public CPSystem
{
public:
	typedef struct tagFireworkDesc : public CPSystem::DESC
	{
	}DESC;

public:
	CFlyEffect(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName);
	CFlyEffect(const CPSystem& Prototype);
	virtual ~CFlyEffect() = default;


	HRESULT Initialize(void* pArg) override;
	HRESULT Ready_Particle() override;
	HRESULT Ready_Components(void* pArg);
	virtual void resetParticle(Attribute* attribute);
	virtual EVENT Update(_float timeDelta);
	virtual HRESULT Render() override;


	void FrameUpdate(float timeDelta);
	float GetRandomColor(float lowBound, float highBound);
	void SetPosition(_float3 Position);
	void SetDead() { m_bDead = true; }

public:
	static CFlyEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();

private:
	_float		m_fFrame = 0;
	_bool		m_bDead = false;
};

END