// 내 클래스 이름 : GrenadeBullet
// 부모 클래스 이름 : Bullet

#pragma once
#include "Bullet.h"

BEGIN(Client)

class CGrenadeBullet final : public CBullet
{
public:
	typedef struct tagGrenadeBulletDesc : public CBullet::DESC
	{
		// Look의 Y는 반드시 0보다 커야합니다.
		_float3 vLook;
		_float fInitJumpPower;
		_float fTimeLimit;
		COLLIDER_GROUP ColliderGroup;
	}DESC;
private:
	CGrenadeBullet(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGrenadeBullet(const CGrenadeBullet& Prototype);
	virtual ~CGrenadeBullet() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void On_Collision(_uint MyColliderID, _uint OtherColliderID) override;

private:
	CGravity* m_pGravityCom = { nullptr };

	_float m_fJumpPower{30.f};
	_float m_fSpeedperSec{};

private:
	virtual HRESULT Ready_Components(void* pArg) override;
	virtual void Friction();

public:
	static CGrenadeBullet* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END