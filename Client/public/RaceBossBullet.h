#pragma once

#include "Bullet.h"

class CRaceBossBullet final : public CBullet
{
public:
	typedef struct tagRaceBossBulletDesc : public CBullet::DESC
	{
		_bool bAnimation = false;
		_float3 vLook;
		COLLIDER_GROUP ColliderGroup;
	}DESC;

private:
	CRaceBossBullet(LPDIRECT3DDEVICE9 pGraphic_Device);
	CRaceBossBullet(const CRaceBossBullet& Prototype);
	virtual ~CRaceBossBullet() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void On_Collision(_uint MyColliderID, _uint OtherColliderID) override;

private:
	virtual HRESULT Ready_Components(void* pArg) override;

public:
	static CRaceBossBullet* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

