#pragma once

#include "PBState.h"

BEGIN(Client)

class CPBState_Lerp final : public CPBState
{
public:
	CPBState_Lerp();
	virtual ~CPBState_Lerp() = default;

public:
	//���� ���� �� �� �� ȣ��
	void Enter(CPlayerOnBoat* pPlayerOnBoat, _float fTimeDelta) override;

	//�ش� ������ �� ��� ȣ��
	void Execute(CPlayerOnBoat* pPlayerOnBoat, _float fTimeDelta) override;

	//���� ���� �� �� �� ȣ��
	void Exit(CPlayerOnBoat* pPlayerOnBoat) override;

private:
	_float m_fStartPosX = {};
	_float m_fTime = {};

private:
	virtual void Free();
};

END
