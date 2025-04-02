#pragma once

#include "PBState.h"

BEGIN(Client)

class CPBState_Decel final : public CPBState
{
public:
	CPBState_Decel();
	virtual ~CPBState_Decel() = default;

public:
	//���� ���� �� �� �� ȣ��
	void Enter(CPlayerOnBoat* pPlayerOnBoat, _float fTimeDelta) override;

	//�ش� ������ �� ��� ȣ��
	void Execute(CPlayerOnBoat* pPlayerOnBoat, _float fTimeDelta) override;

	//���� ���� �� �� �� ȣ��
	void Exit(CPlayerOnBoat* pPlayerOnBoat) override;

private:
	_float m_fDecelSpeed = {};

private:
	virtual void Free();
};

END