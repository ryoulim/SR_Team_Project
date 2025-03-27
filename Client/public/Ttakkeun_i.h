// 내 클래스 이름 : TestMonster
// 부모 클래스 이름 : Monster

#pragma once
#include "Monster.h"

BEGIN(Client)

class CTtakkeun_i final : public CMonster
{
public:
	typedef struct tagTestMonsterDesc : public CMonster::DESC
	{
		//몬스터 고유 특성
	}DESC;

	enum MONSTER_STATE { STATE_WALK, STATE_FLY, STATE_FLY_ATTACK, STATE_JUMP, STATE_STAY, STATE_END };
	enum STATE_MAXFRAME { MAX_WALK = 12, MAX_FLY = 1, MAX_FLY_ATTACK = 2, MAX_JUMP = 5 };
	enum FLY_DIR { DOWN = 0, LEFT = 1, RIGHT = 2, UP = 3, DIR_END = 4};
	enum BOSS_DEGREE { D0, D22, D45, D67, D90, D112, D135, D157, D180, D_END };

	//enum BOSS_STATUS { FLY_DOWN = 0, JUMP = 9, WALK = 54, STATUS_END = 162 };
	//enum BOSS_STATUS_FRAME { FLY_DOWN = 1, JUMP = 5, WALK = 12 }; 안씀 안쓰고싶었는데

private:
	CTtakkeun_i(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTtakkeun_i(const CTtakkeun_i& Prototype);
	virtual ~CTtakkeun_i() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	virtual HRESULT Ready_Components(void* pArg);
	virtual HRESULT Ready_Textures();
	virtual HRESULT Set_Animation();
	virtual HRESULT Animate_Monster(_float fTimeDelta);

	//virtual HRESULT Set_TextureType();


public: //해당객체의 몬스터 패턴
	virtual void DoIdle(_float dt);
	virtual void DoBattle(_float dt);
	virtual void AttackPattern(_float dt);

public: //따끈이 패턴
	void BasicAttackSet(_float dt);
	void LazerAttack(_float dt);
	void MissileAttack(_float dt);
	void SpawnAttack(_float dt);
	void FireAttack(_float dt);
	void LavaAttack(_float dt);
	void FlyAttack(_float dt);
	void StartCooldown(_float dt);
	void SpawnMissile(_float dt);

public: //따끈이 전용 이펙트
	void FlyEffect();


public: //행동 오버라이딩
	void DoReturn(_float dt);


private:
	MONSTER_STATE	m_eCurMonsterState = { MONSTER_STATE::STATE_WALK };
	MONSTER_STATE	m_ePrevMonsterState = { MONSTER_STATE::STATE_WALK };

	CGameObject*	m_pFlyEffect = nullptr;

	FLY_DIR			m_eCurFlyingDirection = { UP };
	FLY_DIR			m_ePrevFlyingDirection = { UP };
	_bool			m_bFlyAttack = false;
	_bool			m_bFlyEffect = false;
	_float			m_fFlyAttack = 0.f;
	_float			m_fSpawnMissile = 0.f;
	_int			m_iMissileCount = 0;

public:
	virtual void On_Collision(_uint MyColliderID, _uint OtherColliderID) override;

public:
	static CTtakkeun_i* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END