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


	enum STATE { STATE_ATTACK, STATE_DEATH, STATE_MOVE, STATE_SHOCK, STATE_END };
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

public:
	virtual void On_Collision(CGameObject* pCollisionedObject, const _wstring& strLayerTag);

public:
	static CShotgunner* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END