// 내 클래스 이름 : TestBullet
// 부모 클래스 이름 : Bullet

#pragma once
#include "Bullet.h"

BEGIN(Client)

class CTestBullet final : public CBullet
{
public:
	typedef struct tagTestBulletDesc : public CBullet::DESC
	{

	}DESC;

private:
	CTestBullet(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTestBullet(const CTestBullet& Prototype);
	virtual ~CTestBullet() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	virtual HRESULT Ready_Components(void* pArg);

public:
	static CTestBullet* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END