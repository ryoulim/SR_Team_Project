#pragma once
#include "PSystem.h"
#include "Effect.h"

BEGIN(Engine)
class CGameObject;
END

class CBulletSmoke : public CPSystem
{
public:
	typedef struct tagFireworkDesc : public CPSystem::DESC
	{
	}DESC;

public:
	CBulletSmoke(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName);
	CBulletSmoke(const CPSystem& Prototype);
	virtual ~CBulletSmoke() = default;


	HRESULT Initialize(void* pArg) override;
	virtual HRESULT Reset(void* pArg) override;
	HRESULT Ready_Particle() override;
	HRESULT Ready_Components(void* pArg);
	virtual void resetParticle(Attribute* attribute);
	virtual EVENT Update(_float timeDelta);
	virtual HRESULT Render() override;


	float GetRandomColor(float lowBound, float highBound);

public:
	static CBulletSmoke* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();

};

