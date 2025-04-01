#pragma once

#include "PBState.h"

BEGIN(Client)

class CPBState_Normal final : public CPBState
{
public:
	CPBState_Normal();
	virtual ~CPBState_Normal() = default;

public:
	//상태 진입 시 한 번 호출
	void Enter(CPlayerOnBoat* pPlayerOnBoat, _float fTimeDelta) override;

	//해당 상태일 때 계속 호출
	void Execute(CPlayerOnBoat* pPlayerOnBoat, _float fTimeDelta) override;

	//상태 퇴장 시 한 번 호출
	void Exit(CPlayerOnBoat* pPlayerOnBoat) override;

private:
	virtual void Free();
};

END