#pragma once
#include "Bullet.h"

BEGIN(Client)
class CMonsterMissile;

class CMonsterGuidBullet final : public CBullet
{
public:
	typedef struct tagMonsterGuidBulletDesc : public CBullet::DESC
	{
		_float3 vDir;
	}DESC;

private:
	CMonsterGuidBullet(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMonsterGuidBullet(const CMonsterGuidBullet& Prototype);
	virtual ~CMonsterGuidBullet() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Ready_Components(void* pArg);
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;


	virtual void On_Collision(_uint MyColliderID, _uint OtherColliderID);

public:
	void	SetTargetDir();
	float	EaseInCubic(float currentTime, float duration, float maxValue);
	_float3 LerpVec(const _float3& a, const _float3& b, float t)
	{
		return a * (1.f - t) + b * t;
	}
	float	clamp(float value, float min, float max)
	{
		if (value < min) return min;
		if (value > max) return max;
		return value;
	}

public:
	static CMonsterGuidBullet* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();

protected:
	CGameObject* m_pTargetPlayer = nullptr;
	CGameObject* m_pMissile = nullptr;
	_float3					m_vTargetPos;

protected:
	_float3 m_vInitDir;
	_float3 m_vCurrentDir;
	float m_fPreChaseTime = 0.f;

	class CCameraManager* m_pCamera = nullptr;
};

END