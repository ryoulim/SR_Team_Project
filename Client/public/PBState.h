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
	//���� ���� �� �� �� ȣ��
	virtual void Enter(CPlayerOnBoat* pPlayerOnBoat, _float fTimeDelta) PURE;

	//�ش� ������ �� ��� ȣ��
	virtual void Execute(CPlayerOnBoat* pPlayerOnBoat, _float fTimeDelta) PURE;

	//���� ���� �� �� �� ȣ��
	virtual void Exit(CPlayerOnBoat* pPlayerOnBoat) PURE;

protected:
	_float		m_fPosZ = {};

public:
	virtual void Free();
};

END