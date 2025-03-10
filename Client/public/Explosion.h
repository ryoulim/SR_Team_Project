// 내 클래스 이름 : Explosion
// 부모 클래스 이름 : Effect

#pragma once
#include "Effect.h"

BEGIN(Client)

class CExplosion final : public CEffect
{
public:
	typedef struct tagExplosionDesc : public CEffect::DESC
	{

	}DESC;

private:
	CExplosion(LPDIRECT3DDEVICE9 pGraphic_Device);
	CExplosion(const CExplosion& Prototype);
	virtual ~CExplosion() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CExplosion* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END