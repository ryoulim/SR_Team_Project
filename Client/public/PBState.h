#pragma once

#include "Base.h"
#include "Client_Defines.h"
#include "PlayerOnBoat.h"

BEGIN(Client)

class CPBState abstract : public CBase
{
public:
	CPBState();
	virtual ~CPBState() = default;

public:
	//상태 진입 시 한 번 호출
	virtual void Enter(CPlayerOnBoat* pPlayerOnBoat, _float fTimeDelta) PURE;

	//해당 상태일 때 계속 호출
	virtual void Execute(CPlayerOnBoat* pPlayerOnBoat, _float fTimeDelta) PURE;

	//상태 퇴장 시 한 번 호출
	virtual void Exit(CPlayerOnBoat* pPlayerOnBoat) PURE;

protected:
	_float		m_fPosZ = {};

public:
	virtual void Free();
};

END