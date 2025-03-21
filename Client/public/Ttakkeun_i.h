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

	enum STATE { STATE_FLY, STATE_FLY_ATTACK, STATE_JUMP, STATE_LAVA_ATTACK, STATE_LAVA_DIVEIN, STATE_WALK, STATE_END };
	enum FLY_DIR { DOWN = 0, LEFT = 2, RIGHT = 4, UP = 6, DIR_END = 8};
	enum BOSS_DEGREE { D0, D22, D45, D67, D90, D112, D135, D157, D180, D_END };

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
	virtual HRESULT Set_TextureType();

public:
	static CTtakkeun_i* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END