#pragma once
#include "Bullet.h"

BEGIN(Client)
class CMonsterMissile;

class CMonsterNormalBullet final : public CBullet
{
public:
	typedef struct tagGrenadeBulletDesc : public CBullet::DESC
	{

	}DESC;

private:
	CMonsterNormalBullet(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMonsterNormalBullet(const CMonsterNormalBullet& Prototype);
	virtual ~CMonsterNormalBullet() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Ready_Components(void* pArg);
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual void On_Collision(_uint MyColliderID, _uint OtherColliderID);
	virtual HRESULT Render() override;

public:
	void	SetTargetDir();

public:
	static CMonsterNormalBullet* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();

protected:
	CGameObject* m_pTargetPlayer = nullptr;
	CGameObject* m_pMissile = nullptr;
	_float3					m_vTargetPos;

	class CCameraManager* m_pCamera = nullptr;
};

END