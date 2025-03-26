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

private:
	virtual HRESULT Ready_Components(void* pArg) override;

public:
	static CGrenadeBullet* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END