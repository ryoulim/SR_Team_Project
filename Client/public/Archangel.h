// 내 클래스 이름 : TestMonster
// 부모 클래스 이름 : Monster

#pragma once
#include "Monster.h"

BEGIN(Client)

class CArchangel final : public CMonster
{
public:
	typedef struct tagTestMonsterDesc : public CMonster::DESC
	{
		//몬스터 고유 특성
	}DESC;
	enum MONSTER_DEGREE { D0, D45, D90, D135, D180, D_END };
private:
	CArchangel(LPDIRECT3DDEVICE9 pGraphic_Device);
	CArchangel(const CArchangel& Prototype);
	virtual ~CArchangel() = default;

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
	void			FlyPattern(_float dt);
	void			FirePattern(_float dt);	
	virtual void	ChasePlayer(_float dt, _float fChaseDist);
	virtual void	DoBattle(_float dt);
	virtual void	DoIdle(_float dt);
	void			DoReady(_float dt);
	void			DoDetect(_float dt);

private:
	virtual HRESULT Ready_Components(void* pArg);
	virtual HRESULT Ready_Textures();
	virtual HRESULT Set_Animation();
	virtual HRESULT Animate_Monster(_float fTimeDelta);

private:
	enum MONSTER_STATE { STATE_MOVE, STATE_FLY, STATE_ATTACK, STATE_STAY, STATE_DEAD, STATE_END };
	enum STATE_MAXFRAME { MAX_MOVERUN = 4, MAX_FLY = 4, MAX_ATTACK = 4, MAX_STAY = 4, MAX_DEAD = 12 };
	// attack : 0, 1  ready / 3, 4 shoot
	// fly : 0123 , 3 moving
	// dead : 01234, 567 loop, 8 9 10 11 death
	MONSTER_STATE	m_eCurMonsterState = { MONSTER_STATE::STATE_MOVE };
	MONSTER_STATE	m_ePrevMonsterState = { MONSTER_STATE::STATE_MOVE };
	
	/* 공격 패턴 */
	enum ATTACKPATTERN { ATTACK_FLY1, ATTACK_FLY2, ATTACK_FIRE1, ATTACK_FIRE2, ATTACK_END };
	ATTACKPATTERN	m_eAttackPattern = { ATTACKPATTERN::ATTACK_FIRE1 };
	_bool			m_bGravity = { true };
	_float			m_fFlyingUpTime = { 0.f };
	_float3			m_vTargetPos = {};
	_float			m_fFireReadyTime = { 0.f };

private:
	/* 잔상을 만들어보자.. */
	typedef struct TrailData {
		_float3 position;
		_float3 size;
		_float4x4 matWorld;
		float timeElapsed; // 잔상이 점차 사라지도록 시간 기록
	}TRAILDATA;

	queue<TRAILDATA> m_TrailDataQueue;
	_float			 m_fTrailDuration = 0.5f; // 잔상이 사라지는 시간	
	_float			 m_fTrailTimer = {};	  // 잔상 생성 딜레이

private:
	/* 잔상 시간 업데이트 */
	void Update_TrailData(_float dt);
	void Render_TrailData();
	void Trail_Billboard(_float4x4 &matWorld, _bool isInverse = false) const;
	void Make_TrailData(_float dt);
	queue<TRAILDATA> Sorted_TrailData();

private:
	CShader* m_pShaderCom = { nullptr };

public:
	virtual void On_Collision(_uint MyColliderID, _uint OtherColliderID) override;

public:
	static CArchangel* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();

private:
	struct TrailDataCompare {
		_float3 vCamPos;

		TrailDataCompare(const _float3& camPos) : vCamPos(camPos) {}

		bool operator()(const TRAILDATA& a, const TRAILDATA& b) const {
			return (a.position - vCamPos).Length() > (b.position - vCamPos).Length();
		}
	};
};

END