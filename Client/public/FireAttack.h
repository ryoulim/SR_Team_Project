#pragma once
#include "PSystem.h"
#include "Effect.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)

class CFireAttack : public CPSystem
{
public:
	typedef struct tagFireworkDesc : public CPSystem::DESC
	{
	}DESC;

public:
	CFireAttack(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName);
	CFireAttack(const CPSystem& Prototype);
	virtual ~CFireAttack() = default;


	HRESULT Initialize(void* pArg) override;
	HRESULT Ready_Particle() override;
	HRESULT Ready_Components(void* pArg);
	virtual void resetParticle(Attribute* attribute);
	virtual EVENT Update(_float timeDelta);
	virtual HRESULT Render() override;


	void FrameUpdate(float timeDelta);
	float GetRandomColor(float lowBound, float highBound);

public:
	static CFireAttack* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();

private:
	float		m_fFrame = 0;
};

END