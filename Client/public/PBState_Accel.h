#pragma once

#include "PBState.h"

BEGIN(Client)

class CPBState_Accel final : public CPBState
{
public:
	CPBState_Accel();
	virtual ~CPBState_Accel() = default;

public:
	//���� ���� �� �� �� ȣ��
	void Enter(CPlayerOnBoat* pPlayerOnBoat, _float fTimeDelta) override;

	//�ش� ������ �� ��� ȣ��
	void Execute(CPlayerOnBoat* pPlayerOnBoat, _float fTimeDelta) override;

	//���� ���� �� �� �� ȣ��
	void Exit(CPlayerOnBoat* pPlayerOnBoat) override;

private:
	_float m_fAccelSpeed = {};

private:
	virtual void Free();
};

END
