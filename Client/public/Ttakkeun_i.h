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

	enum MONSTER_STATE { STATE_WALK, STATE_FLY, STATE_FLY_ATTACK, STATE_JUMP, STATE_LAVA_ATTACK, STATE_LAVA_DIVEIN, STATE_END };
	enum STATE_MAXFRAME { MAX_WALK = 12, MAX_FLY = 1, MAX_FLY_ATTACK = 2, MAX_JUMP = 5, MAX_LAVA_ATTACK = 2, MAX_LAVA_DIVEIN = 4 };
	enum FLY_DIR { DOWN = 0, LEFT = 2, RIGHT = 4, UP = 6, DIR_END = 8};
	enum BOSS_DEGREE { D0, D22, D45, D67, D90, D112, D135, D157, D180, D_END };
	//enum BOSS_STATUS { FLY_DOWN = 0, JUMP = 9, WALK = 54, STATUS_END = 162 };
	//enum BOSS_STATUS_FRAME { FLY_DOWN = 1, JUMP = 5, WALK = 12 }; 안씀

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
	virtual HRESULT Set_MaxFrame();
	//virtual HRESULT Set_TextureType();


public: //해당객체의 몬스터 패턴
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


private:
	MONSTER_STATE m_eMonsterState = { MONSTER_STATE::STATE_WALK };


public:
	virtual void On_Collision(_uint MyColliderID, _uint OtherColliderID) override;

public:
	static CTtakkeun_i* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END