#pragma once
#include "Base.h"
#include "Client_Defines.h"
#include "RaceBoss.h"
#include "PlayerOnBoat.h"

BEGIN(Client)

class CRBState abstract : public CBase
{
public:
	CRBState(CRaceBoss* pOwner) : m_pOwner(pOwner) {}
	virtual ~CRBState() = default;

public:
	//상태 진입 시 한 번 호출
	virtual void Enter(_float fTimeDelta) PURE;

	//해당 상태일 때 계속 호출
	virtual void Execute(_float fTimeDelta) PURE;

	//상태 퇴장 시 한 번 호출
	virtual void Exit() PURE;

protected:
	CRaceBoss*		m_pOwner;
	CPlayerOnBoat*	m_pPlayer = { nullptr };
	const _float3*	m_pPlayerpos = { nullptr };
	_float			m_fTime = {};
	_float			m_fPosZ = {};

public:
	virtual void Free()
	{
		__super::Free();
	}
};

class CRBState_WaitPlayer final : public CRBState
{
public:
	CRBState_WaitPlayer(CRaceBoss* pOwner)
		:CRBState(pOwner) {}
	virtual ~CRBState_WaitPlayer() = default;

public:
	virtual void Enter(_float fTimeDelta) override
	{
	}
	virtual void Execute(_float fTimeDelta) override
	{
	}
	virtual void Exit() override
	{
	}
};

class CRBState_Entrance final : public CRBState
{
public:
	CRBState_Entrance(CRaceBoss* pOwner)
		:CRBState(pOwner) {}
	virtual ~CRBState_Entrance() = default;

public:
	virtual void Enter(_float fTimeDelta) override
	{
		v0 = { 0.f, 1750.f, -1300.f };
		vStartPos = { 0.f, 1000.f, -500.f };
		vEndPos = { 450.f, 250.f, 1300.f };
		v3 = { 450.f, 2000.f, 2000.f };
	}

	virtual void Execute(_float fTimeDelta) override
	{
		m_fTime += (1.f - m_fTime) * fTimeDelta;
		m_pOwner->MoveCatMullRom(v0, vStartPos, vEndPos, v3, m_fTime);

		m_fPosZ = m_pOwner->Compute_PosZ();
		if (m_fPosZ >= 1200.f)
			Exit();
	}

	virtual void Exit() override
	{
		m_fTime = 0.f;
		//랜덤한 패턴으로 이어짐
		m_pOwner->Set_State(CRaceBoss::READYBOMB);
	}

private:
	_float3	v0 = {};
	_float3	vStartPos = {};
	_float3 vEndPos = {};
	_float3 v3 = {};
};

class CRBState_IDLE final : public CRBState
{
public:
	CRBState_IDLE(CRaceBoss* pOwner)
		:CRBState(pOwner) {}
	virtual ~CRBState_IDLE() = default;

public:
	virtual void Enter(_float fTimeDelta) override
	{
	}
	virtual void Execute(_float fTimeDelta) override
	{
		//m_pOwner->Go_Straight(fTimeDelta);
	}
	virtual void Exit() override
	{
	}
};

#pragma region 총쏘는 패턴
class CRBState_ReadyShot final : public CRBState
{
public:
	CRBState_ReadyShot(CRaceBoss* pOwner)
		:CRBState(pOwner) {}
	virtual ~CRBState_ReadyShot() = default;

public:
	virtual void Enter(_float fTimeDelta) override
	{

	}

	virtual void Execute(_float fTimeDelta) override
	{
		m_pOwner->Go_Straight(fTimeDelta);
		m_fTime += fTimeDelta;
		if (m_fTime > 2.f)
			Exit();
	}

	virtual void Exit() override
	{
		m_fTime = 0.f;
		m_pOwner->Set_State(CRaceBoss::SHOTHEADBULLET);
	}
};

class CRBState_ShotHeadBullet final : public CRBState
{
public:
	CRBState_ShotHeadBullet(CRaceBoss* pOwner)
		:CRBState(pOwner) {}
	virtual ~CRBState_ShotHeadBullet() = default;

public:
	virtual void Enter(_float fTimeDelta) override
	{
		
	}

	virtual void Execute(_float fTimeDelta) override
	{
		m_pOwner->Go_Straight(fTimeDelta);
		m_pOwner->ShuffleandPop();
		m_pOwner->Fire_HeadBullet(fTimeDelta);
		Exit();
	}

	virtual void Exit() override
	{
		m_pOwner->Set_State(CRaceBoss::SHOTTAILBULLET);
	}
};

class CRBState_ShotTailBullet final : public CRBState
{
public:
	CRBState_ShotTailBullet(CRaceBoss* pOwner)
		:CRBState(pOwner) {}
	virtual ~CRBState_ShotTailBullet() = default;

public:
	virtual void Enter(_float fTimeDelta) override
	{
		m_iHeadBulletCount = m_pOwner->Get_HeadBulletCount();
	}

	virtual void Execute(_float fTimeDelta) override
	{
		m_pOwner->Go_Straight(fTimeDelta);
		m_fTime += fTimeDelta;
		if (m_fTime > 0.02f)
		{
			m_pOwner->Fire_TailBullet(fTimeDelta);
			++m_iTailBulletCount;
			m_fTime = 0;
		}
		if (m_iTailBulletCount > 3)
			Exit();
	}

	virtual void Exit() override
	{
		if (m_iHeadBulletCount > 4)
		{
			//랜덤한 패턴으로 이어짐
			m_pOwner->Set_State(CRaceBoss::SHOTREADY);
			m_pOwner->Set_HeadBulletCountZero();
		}

		else
			m_pOwner->Set_State(CRaceBoss::SHOTHEADBULLET);

		m_iHeadBulletCount = 0;
		m_iTailBulletCount = 0;
		m_fTime = 0;
	}

private:
	_uint m_iHeadBulletCount = {};
	_uint m_iTailBulletCount = {};
};

#pragma endregion

#pragma region 폭격패턴
class CRBState_ReadyBombing final : public CRBState
{
public:
	CRBState_ReadyBombing(CRaceBoss* pOwner)
		:CRBState(pOwner) {}
	virtual ~CRBState_ReadyBombing() = default;

public:
	virtual void Enter(_float fTimeDelta) override
	{
	}
	virtual void Execute(_float fTimeDelta) override
	{
		m_fTime += fTimeDelta;
		if (m_fTime > 1.f)
		{
			m_pOwner->Go_Up(fTimeDelta);
			Exit();
		}
		else
			m_pOwner->Go_Straight(fTimeDelta);
	}
	virtual void Exit() override
	{
		m_fTime = 0.f;
		m_pOwner->Set_State(CRaceBoss::DRAWINGRADIUS);
	}
};

class CRBState_DrawingRadius final : public CRBState
{
public:
	CRBState_DrawingRadius(CRaceBoss* pOwner)
		:CRBState(pOwner) {}
	virtual ~CRBState_DrawingRadius() = default;

public:
	virtual void Enter(_float fTimeDelta) override
	{
	}
	virtual void Execute(_float fTimeDelta) override
	{
		m_pOwner->Draw_BombRadius();
		Exit();
	}
	virtual void Exit() override
	{
		m_pOwner->Set_State(CRaceBoss::IDLE);
	}
};

class CRBState_Bombing final : public CRBState
{
public:
	CRBState_Bombing(CRaceBoss* pOwner)
		:CRBState(pOwner) {}
	virtual ~CRBState_Bombing() = default;

public:
	virtual void Enter(_float fTimeDelta) override
	{
	}
	virtual void Execute(_float fTimeDelta) override
	{
		//폭탄을 떨군다.
	}
	virtual void Exit() override
	{
	}
};

class CRBState_Comeback final : public CRBState
{
public:
	CRBState_Comeback(CRaceBoss* pOwner)
		:CRBState(pOwner) {}
	virtual ~CRBState_Comeback() = default;

public:
	virtual void Enter(_float fTimeDelta) override
	{
	}
	virtual void Execute(_float fTimeDelta) override
	{
		//원래 위치로 복귀한다.
	}
	virtual void Exit() override
	{
	}
};
#pragma endregion

class CRBState_Leave final : public CRBState
{
public:
	CRBState_Leave(CRaceBoss* pOwner)
		:CRBState(pOwner) {}
	virtual ~CRBState_Leave() = default;

public:
	virtual void Enter(_float fTimeDelta) override
	{
	}
	virtual void Execute(_float fTimeDelta) override
	{
	}
	virtual void Exit() override
	{
	}
};

END