// 내 클래스 이름 : TestMonster
// 부모 클래스 이름 : Monster

#pragma once
#include "Monster.h"

BEGIN(Client)

class CDeacon final : public CMonster
{
public:
	typedef struct tagTestMonsterDesc : public CMonster::DESC
	{
		//몬스터 고유 특성
	}DESC;
	enum MONSTER_DEGREE { D0, D45, D90, D135, D180, D_END };

private:
	CDeacon(LPDIRECT3DDEVICE9 pGraphic_Device);
	CDeacon(const CDeacon& Prototype);
	virtual ~CDeacon() = default;

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
	virtual void On_Collision(_uint MyColliderID, _uint OtherColliderID) override;

/* 몬스터 행동 */
private: //해당객체의 몬스터 패턴
	virtual void	MonsterTick(_float dt);
	virtual void	AttackPattern(_float dt);
	virtual void	ChasePlayer(_float dt);
	virtual void	ChasePlayer(_float dt, _float fChaseDist);
	virtual void	DoBattle(_float dt);
	void			DoReady(_float dt);
	void			DoDetect(_float dt);
	_bool			IsMonsterAbleToAttack();
/* 애니메이션 */
private:
	virtual HRESULT Ready_Textures();
	virtual HRESULT Set_Animation();
	virtual HRESULT Animate_Monster(_float fTimeDelta);

private:
	enum MONSTER_STATE { STATE_FLY, STATE_ATTACK, STATE_STAY, STATE_DEAD, STATE_END };
	enum STATE_MAXFRAME { MAX_FLY = 1, MAX_ATTACK = 4, MAX_DEAD = 8 };
	MONSTER_STATE	m_eCurMonsterState = { MONSTER_STATE::STATE_FLY };
	MONSTER_STATE	m_ePrevMonsterState = { MONSTER_STATE::STATE_FLY };

	enum FLY_DIR { DOWN = 0, LEFT = 1, RIGHT = 2, UP = 3, DIR_END = 4 };
	FLY_DIR			m_eCurFlyingDirection = { UP };
	FLY_DIR			m_ePrevFlyingDirection = { UP };

public:
	static CDeacon* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END