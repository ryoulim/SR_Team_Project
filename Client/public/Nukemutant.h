// 내 클래스 이름 : TestMonster
// 부모 클래스 이름 : Monster

#pragma once
#include "Monster.h"

BEGIN(Client)

class CNukemutant final : public CMonster
{
public:
	typedef struct tagTestMonsterDesc : public CMonster::DESC
	{
		//몬스터 고유 특성
	}DESC;
	enum MONSTER_DEGREE { D0, D45, D90, D135, D180, D_END };
private:
	CNukemutant(LPDIRECT3DDEVICE9 pGraphic_Device);
	CNukemutant(const CNukemutant& Prototype);
	virtual ~CNukemutant() = default;

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

private:
	enum MONSTER_STATE { STATE_MOVE, STATE_REVIVE, STATE_ATTACK, STATE_STAY, STATE_DEAD, STATE_END };
	enum STATE_MAXFRAME { MAX_MOVERUN = 4, MAX_REVIVE = 7, MAX_ATTACK = 5, MAX_DEAD = 6 };
	MONSTER_STATE	m_eCurMonsterState = { MONSTER_STATE::STATE_MOVE };
	MONSTER_STATE	m_ePrevMonsterState = { MONSTER_STATE::STATE_MOVE };


public:
	virtual void On_Collision(_uint MyColliderID, _uint OtherColliderID) override;

public:
	static CNukemutant* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END