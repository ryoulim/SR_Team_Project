#pragma once

#include "PBState.h"

BEGIN(Client)

class CPBState_Normal final : public CPBState
{
public:
	CPBState_Normal();
	virtual ~CPBState_Normal() = default;

public:
	//���� ���� �� �� �� ȣ��
	void Enter(CPlayerOnBoat* pPlayerOnBoat, _float fTimeDelta) override;

	//�ش� ������ �� ��� ȣ��
	void Execute(CPlayerOnBoat* pPlayerOnBoat, _float fTimeDelta) override;

	//���� ���� �� �� �� ȣ��
	void Exit(CPlayerOnBoat* pPlayerOnBoat) override;

private:
	virtual void Free();
};

END