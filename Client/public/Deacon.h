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
	enum STATE { STATE_ATTACK, STATE_DEATH, STATE_MOVE, STATE_SHOCK, STATE_END };
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
	virtual HRESULT Ready_Textures();

public:
	static CDeacon* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END