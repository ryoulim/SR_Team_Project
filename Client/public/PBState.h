#pragma once
#include "Base.h"
#include "Client_Defines.h"
#include "PlayerOnBoat.h"

BEGIN(Client)

class CPBState abstract : public CBase
{
public:
	CPBState(CPlayerOnBoat* pOwner) : m_pOwner(pOwner) {}
	virtual ~CPBState() = default;

public:
	//상태 진입 시 한 번 호출
	virtual void Enter(_float fTimeDelta) PURE;

	//해당 상태일 때 계속 호출
	virtual void Execute(_float fTimeDelta) PURE;

	//상태 퇴장 시 한 번 호출
	virtual void Exit() PURE;

protected:
	CPlayerOnBoat* m_pOwner;
	_float		m_fPosZ = {};

public:
	virtual void Free() 
	{ 
		__super::Free();
	}
};

class CPBState_Accel final : public CPBState
{
public:
	CPBState_Accel(CPlayerOnBoat* pOwner)
		:CPBState(pOwner) {}
	virtual ~CPBState_Accel() = default;

public:
	virtual void Enter(_float fTimeDelta) override
	{
		//초기 속도 설정
		m_fAccelSpeed = fTimeDelta * 5.f;
		m_pOwner->Set_Speed(RACE_SPEED_PER_SEC);
	}
	virtual void Execute(_float fTimeDelta) override
	{
		m_fPosZ = m_pOwner->Compute_CurPosZ();

		m_fAccelSpeed *= 1.01f;

		m_pOwner->Go_Straight(m_fAccelSpeed);

		if (m_fPosZ > -5600.f && m_fPosZ < -5500.f)
			Exit();
	}
	virtual void Exit() override
	{
		m_fPosZ = 0.f;

		//끝날 때 속도 저장
		m_pOwner->StoreSlidingSpeed(m_fAccelSpeed);

		//속도 초기화
		m_fAccelSpeed = 0.f;

		m_pOwner->Set_State(CPlayerOnBoat::DECEL);
	}

private:
	_float m_fAccelSpeed = {};
};

class CPBState_Decel final : public CPBState
{
public:
	CPBState_Decel(CPlayerOnBoat* pOwner)
		:CPBState(pOwner) {}
	virtual ~CPBState_Decel() = default;

public:
	virtual void Enter(_float fTimeDelta) override
	{
		if (0.f != m_pOwner->GetSlidingSpeed())
			m_fDecelSpeed = m_pOwner->GetSlidingSpeed();

		else
			m_fDecelSpeed = fTimeDelta * 5.f;
	}
	virtual void Execute(_float fTimeDelta) override
	{
		m_fPosZ = m_pOwner->Compute_CurPosZ();

		m_fDecelSpeed *= 0.98f;

		m_pOwner->Go_Straight(m_fDecelSpeed);

		if (m_fPosZ > -300.f)
			Exit();
	}
	virtual void Exit() override
	{
		m_fPosZ = 0.f;
		m_fDecelSpeed = 0.f;

		m_pOwner->Set_State(CPlayerOnBoat::NORMAL);
	}

private:
	_float m_fDecelSpeed = {};
};

class CPBState_Lerp final : public CPBState
{
public:
	CPBState_Lerp(CPlayerOnBoat* pOwner)
		:CPBState(pOwner) {}
	virtual ~CPBState_Lerp() = default;

public:
	virtual void Enter(_float fTimeDelta) override
	{
		auto pTransfrom = static_cast<CTransform*>(m_pOwner->Find_Component(TEXT("Com_Transform")));
		pTransfrom->Rotation_Reset();
		pTransfrom->Set_SpeedPerSec(RACE_SPEED_PER_SEC);

		m_fStartPosX = m_pOwner->Compute_StartPosX();
	}

	virtual void Execute(_float fTimeDelta) override
	{
		m_fPosZ = m_pOwner->Compute_CurPosZ();

		m_fTime += fTimeDelta * 1.4f;

		m_pOwner->Move_Lerp(m_fStartPosX, m_fTime);

		if (m_fPosZ > 9800.f)
			Exit();
	}

	virtual void Exit() override
	{
		m_fPosZ = 0.f;
		m_fStartPosX = 0.f;
		m_fTime = 0.f;

		m_pOwner->Set_State(CPlayerOnBoat::ACCEL);
	}

private:
	_float m_fStartPosX = {};
	_float m_fTime = {};
};

class CPBState_Normal final : public CPBState
{
public:
	CPBState_Normal(CPlayerOnBoat* pOwner)
		:CPBState(pOwner) {}
	virtual ~CPBState_Normal() = default;

public:
	virtual void Enter(_float fTimeDelta) override {}

	virtual void Execute(_float fTimeDelta) override
	{
		m_fPosZ = m_pOwner->Compute_CurPosZ();

		m_pOwner->Go_Straight(fTimeDelta);
		m_pOwner->Key_Input(fTimeDelta);

		if (m_fPosZ > 9000.f)
			Exit();
	}

	virtual void Exit() override
	{
		m_fPosZ = 0.f;

		m_pOwner->Set_State(CPlayerOnBoat::LERP);
	}
};

END