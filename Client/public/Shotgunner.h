// 내 클래스 이름 : TestMonster
// 부모 클래스 이름 : Monster

#pragma once
#include "Monster.h"

BEGIN(Client)

class CShotgunner final : public CMonster
{
public:
	typedef struct tagTestMonsterDesc : public CMonster::DESC
	{
		//몬스터 고유 특성
	}DESC;


	enum MONSTER_DEGREE { D0, D45, D90, D135, D180, D_END };
	// 일반 몹에 다 주고싶은데 일반몹용 부모를 또 만들어야하는지 고민중 

private:
	CShotgunner(LPDIRECT3DDEVICE9 pGraphic_Device);
	CShotgunner(const CShotgunner& Prototype);
	virtual ~CShotgunner() = default;

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
	//virtual HRESULT Set_Animation();
	//virtual HRESULT Animate_Monster(_float fTimeDelta);

private:
	enum MONSTER_STATE { STATE_MOVE, STATE_FLY, STATE_ATTACK, STATE_STAY, STATE_DEAD, STATE_END };
	MONSTER_STATE	m_eCurMonsterState = { MONSTER_STATE::STATE_FLY };
	MONSTER_STATE	m_ePrevMonsterState = { MONSTER_STATE::STATE_FLY };

	enum FLY_DIR { DOWN = 0, LEFT = 1, RIGHT = 2, UP = 3, DIR_END = 4 };
	FLY_DIR			m_eCurFlyingDirection = { UP };
	FLY_DIR			m_ePrevFlyingDirection = { UP };

public:
	virtual void On_Collision(_uint MyColliderID, _uint OtherColliderID) override;

public:
	static CShotgunner* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END