#pragma once
#include "Base.h"
#include "Client_Defines.h"
#include "RaceBoss.h"

BEGIN(Client)

class CRBState abstract : public CBase
{
public:
	CRBState(CRaceBoss* pOwner) : m_pOwner(pOwner) {}
	virtual ~CRBState() = default;

public:
	//���� ���� �� �� �� ȣ��
	virtual void Enter(_float fTimeDelta) PURE;

	//�ش� ������ �� ��� ȣ��
	virtual void Execute(_float fTimeDelta) PURE;

	//���� ���� �� �� �� ȣ��
	virtual void Exit() PURE;

protected:
	CRaceBoss* m_pOwner;

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
	}
	virtual void Execute(_float fTimeDelta) override
	{
	}
	virtual void Exit() override
	{
	}
};

#pragma region �ѽ�� ����
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
	}
	virtual void Exit() override
	{
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
	}
	virtual void Exit() override
	{
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
	}
	virtual void Execute(_float fTimeDelta) override
	{
	}
	virtual void Exit() override
	{
	}
};
#pragma endregion

#pragma region ��������
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
		//���� �ö󰣴�, �׸��ڰ� �����
	}
	virtual void Exit() override
	{
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
		//���� �ݰ��� �׸���.
	}
	virtual void Exit() override
	{
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
		//��ź�� ������.
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
		//���� ��ġ�� �����Ѵ�.
	}
	virtual void Exit() override
	{
	}
};
#pragma endregion

END