// 내 클래스 이름 : TestMonster
// 부모 클래스 이름 : Monster

#pragma once
#include "Monster.h"

BEGIN(Client)

class CWenteko final : public CMonster
{
public:
	typedef struct tagTestMonsterDesc : public CMonster::DESC
	{
		//몬스터 고유 특성
	}DESC;
	enum MONSTER_DEGREE { D0, D45, D90, D135, D180, D_END };
	enum JUMP_STATE { JUMP_BEFORE, JUMP_ING, JUMP_AFTER };

private:
	CWenteko(LPDIRECT3DDEVICE9 pGraphic_Device);
	CWenteko(const CWenteko& Prototype);
	virtual ~CWenteko() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	/* 몬스터 행동 */
private: //해당객체의 몬스터 패턴
	virtual void	MonsterTick(_float dt);
	virtual void	AttackPattern(_float dt);
	virtual void	ChasePlayer(_float dt, _float fChaseDist);
	virtual void	DoBattle(_float dt);
	virtual void	DoIdle(_float dt);
	void			DoReady(_float dt);
	void			DoDetect(_float dt);
	void			JumpPattern(_float dt);

private:
	virtual HRESULT Ready_Components(void* pArg);
	virtual HRESULT Ready_Textures();
	virtual HRESULT Set_Animation();
	virtual HRESULT Animate_Monster(_float fTimeDelta);

private:
	enum MONSTER_STATE { STATE_MOVE, STATE_JUMP, STATE_STAY, STATE_DEAD, STATE_END };
	enum STATE_MAXFRAME { MAX_MOVERUN = 4, MAX_JUMP = 4, MAX_DEAD = 7 };
	MONSTER_STATE	m_eCurMonsterState = { MONSTER_STATE::STATE_MOVE };
	MONSTER_STATE	m_ePrevMonsterState = { MONSTER_STATE::STATE_MOVE };
	JUMP_STATE		m_eJumpState = { JUMP_BEFORE };
	_bool			m_bJump = false;
	_bool			m_bJumpEnd = false;
	_float			m_fJumpFinished = {};

public:
	virtual void On_Collision(_uint MyColliderID, _uint OtherColliderID) override;

public:
	static CWenteko* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END