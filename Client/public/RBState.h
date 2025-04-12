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
	//���� ���� �� �� �� ȣ��
	virtual void Enter(_float fTimeDelta) PURE;

	//�ش� ������ �� ��� ȣ��
	virtual void Execute(_float fTimeDelta) PURE;

	//���� ���� �� �� �� ȣ��
	virtual void Exit() PURE;

protected:
	CRaceBoss*		m_pOwner;
	CPlayerOnBoat*	m_pPlayer = { nullptr };
	const _float3*	m_pPlayerpos = { nullptr };
	_float			m_fTime = {};
	_float			m_fPosZ = {};
	_float			m_fPozY = {};

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
		//�� �����Ӹ��� ���� ��ġ��ŭ �и�
		m_pOwner->m_pTransformCom->Move({ 0.f,0.f,RACE_SPEED_PER_SEC }, fTimeDelta);
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
		//m_fRealTime = 0.f;
		v0 = { -1425.f, 1075.f, -600.f };
		vStartPos = { -1000.f, 1000.f,    0.f };  // ������
		vEndPos = { 450.f,  250.f, 1200.f };  // ����
		v3 = { 1475.f, -425.f, 1800.f };
	}

	virtual void Execute(_float fTimeDelta) override
	{
		//m_fRealTime += fTimeDelta;
		m_fTime += (1.f - m_fTime) * fTimeDelta * 1.5f;
		m_pOwner->MoveCatMullRom(v0, vStartPos, vEndPos, v3, m_fTime);

		m_fPosZ = m_pOwner->Compute_PosZ();

		if (m_fPosZ - m_pOwner->m_pPlayerpos->z > 1190.f)
			Exit();
	}

	virtual void Exit() override
	{
		m_fTime = 0.f;
		//������ �������� �̾���
		//m_pOwner->Set_State(CRaceBoss::SHOTREADY);
		m_pOwner->Set_State(CRaceBoss::READYBOMB);
	}

private:
	_float m_fRealTime{};
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
		m_fTime += fTimeDelta;

		const _float3& vPos = *m_pOwner->m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		const _float fDiffToPlayer = vPos.z -  m_pOwner->m_pPlayerpos->z;
		const _float fScaleZ = m_pOwner->m_pTransformCom->Compute_Scaled().z;

		//2000���� �Ÿ��� ����� ���� �� �����Ӹ��� ���� ��ġ��ŭ �и�
		if (fDiffToPlayer < 2000.f)
			m_pOwner->m_pTransformCom->Move({ 0.f,0.f,RACE_SPEED_PER_SEC }, fTimeDelta);


		// �÷��̾�� �ʹ� �־������� ���������
		if (fDiffToPlayer > 2500.f)
			m_pOwner->Set_State(CRaceBoss::CLOSE_TO_PLAYER);

		if (fDiffToPlayer - fScaleZ*2.f < 0.f)
			m_pOwner->m_pPlayer->Set_StartState(CPlayerOnBoat::AWAY_FROM_BOSS);


		if (m_pOwner->m_pPlayerpos->z > -3500.f &&
			m_pOwner->m_pPlayerpos->z < -2500.f)
			m_pOwner->Set_State(CRaceBoss::ENTRANCE);



		if (m_fTime > 2.f)
		{
			m_pOwner->Set_State(CRaceBoss::SHOTREADY);
			m_fTime = 0.f;
		}
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
		//�� �����Ӹ��� ���� ��ġ��ŭ �и�
		m_pOwner->m_pTransformCom->Move({ 0.f,0.f,RACE_SPEED_PER_SEC }, fTimeDelta);

		//m_pOwner->Go_Straight(fTimeDelta);
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
		//�� �����Ӹ��� ���� ��ġ��ŭ �и�
		m_pOwner->m_pTransformCom->Move({ 0.f,0.f,RACE_SPEED_PER_SEC * 1.5f }, fTimeDelta);

		//m_pOwner->Go_Straight(fTimeDelta);
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
		m_iTailBulletCount = 0;
		m_fTime = 0;

	}

	virtual void Execute(_float fTimeDelta) override
	{
		//�� �����Ӹ��� ���� ��ġ��ŭ �и�
		m_pOwner->m_pTransformCom->Move({ 0.f,0.f,RACE_SPEED_PER_SEC * 1.5f }, fTimeDelta);

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
			//������ �������� �̾���
			m_pOwner->Set_State(CRaceBoss::IDLE);
			m_pOwner->Set_HeadBulletCountZero();
		}

		else
			m_pOwner->Set_State(CRaceBoss::SHOTHEADBULLET);

		m_iHeadBulletCount = 0;
	}

private:
	_uint m_iHeadBulletCount = {};
	_uint m_iTailBulletCount = {};
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
		m_fTime = 0.f;
		// ������ Z �� ���̸� ���Ѵ�.
		m_fCurZDiff = m_pOwner->m_pTransformCom->Get_State(CTransform::STATE_POSITION)->z - m_pOwner->m_pPlayerpos->z;
		// �ʼ� ���ǵ� = (��ǥ �Ÿ� ���� - ���� �Ÿ� ����) / ����ð�
		m_fZSpeedPerSec = 1200 - m_fCurZDiff;
	}
	virtual void Execute(_float fTimeDelta) override
	{
		m_fTime += fTimeDelta;

		_float3 vPos = *m_pOwner->m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		_float3	vTargetPos = *m_pOwner->m_pPlayerpos;
		m_fCurZDiff += m_fZSpeedPerSec * fTimeDelta;

		vTargetPos.x = vPos.x;
		vTargetPos.y = vPos.y + 150 * fTimeDelta;
		vTargetPos.z += m_fCurZDiff;

		m_pOwner->m_pTransformCom->Set_State(CTransform::STATE_POSITION, vTargetPos);

		// �غ��Ϸ� �ٰ����� �ð� : 1��
		if (m_fTime > 1.0f)
			Exit();

#pragma region ���� �ڵ�
		////�� �����Ӹ��� ���� ��ġ��ŭ �и�
		//m_pOwner->m_pTransformCom->Move({ 0.f,0.f,RACE_SPEED_PER_SEC }, fTimeDelta);


		//m_fTime += fTimeDelta;
		//if (m_fTime > 0.5f)
		//{
		//	m_fPozY = m_pOwner->Compute_PozY();
		//	m_pOwner->Go_Up(fTimeDelta);

		//	if (m_fPozY > 280.f)
		//		Exit();
		//}
		//else
		//	m_pOwner->Go_Straight(fTimeDelta);
#pragma endregion
	}
	virtual void Exit() override
	{
		m_pOwner->Set_State(CRaceBoss::DRAWINGRADIUS);
	}
private:
	_float		m_fZSpeedPerSec{};
	_float		m_fCurZDiff{};
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
		//�� �����Ӹ��� ���� ��ġ��ŭ �и�
		m_pOwner->m_pTransformCom->Move({ 0.f,0.f,RACE_SPEED_PER_SEC }, fTimeDelta);
		m_pOwner->Set_BombRadius();
		Exit();
	}
	virtual void Exit() override
	{
		m_pOwner->Set_State(CRaceBoss::BOMBING);
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
		_float3 vPos = *m_pOwner->m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		m_fTime = 0.f;
		m_fRealTime = 0.f;
		//// Z�� �� ���̷�
		vPos.z -= m_pOwner->m_pPlayerpos->z;

		v0 = vPos + _float3{ 0.f, 1500.f, 500.f };
		vStartPos = vPos;
		vEndPos = { 450.f, 150.f, -500.f };
		v3 = { 450.f, 200.f, -2000.f };
	}
	virtual void Execute(_float fTimeDelta) override
	{
		m_fRealTime += fTimeDelta;

		m_fTime += fTimeDelta;

		//if (m_fRealTime > 1.f)
		//	m_fTime *= (1.f + fTimeDelta * 2.0f);
		//else
		//	m_fTime += (1.f - m_fTime) * fTimeDelta * 1.3f;
			
		if (m_fRealTime > 1.5f)
		{
			Exit();
			return;
		}
		else if (m_fRealTime > 1.f)
			return;

		m_pOwner->MoveCatMullRom(v0, vStartPos, vEndPos, v3, m_fTime);

#pragma region �����ڵ�
		//�� �����Ӹ��� ���� ��ġ��ŭ �и�
//m_pOwner->m_pTransformCom->Move({ 0.f,0.f,RACE_SPEED_PER_SEC }, fTimeDelta);


/*m_pOwner->Go_Backward(fTimeDelta * 3.f);
m_fTime += fTimeDelta;
m_fEndTime += fTimeDelta;
if (m_fTime > 0.2f)
{
	m_pOwner->Fire_Bomb4(iBombIndex, fTimeDelta);
	++iBombIndex;
	m_fTime = 0.f;
}*/

//m_pOwner->Go_Backward(fTimeDelta * 3.f);

//if (m_pOwner->Fire_Bomb4(iBombIndex, fTimeDelta))
//	++iBombIndex;

//if(iBombIndex == 19)
//	Exit();

#pragma endregion

	}
	virtual void Exit() override
	{
		vEndPos.z += m_pOwner->m_pPlayerpos->z;
		vEndPos.y = 1000.f;
		m_pOwner->m_pTransformCom->Set_State(CTransform::STATE_POSITION, vEndPos);

		//iBombIndex = 0;
		//m_fEndTime = 0.f;
		m_pOwner->Set_State(CRaceBoss::COMEBACK);
	}

private:
	_float m_fRealTime{};

	_float3	v0 = {};
	_float3	vStartPos = {};
	_float3 vEndPos = {};
	_float3 v3 = {};

	//_float m_fCurZDiff{};
	//_float m_fZSpeedPerSec{};
	//_float m_fEndTime = {};
	//_uint iBombIndex = { 0 };
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
		m_fSpeedY = 600.f;
		_float3 vPos = *m_pOwner->m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		vPos.z = 1000.f + m_pOwner->m_pPlayerpos->z;

		m_pOwner->m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	}
	virtual void Execute(_float fTimeDelta) override
	{
		m_fSpeedY *= powf(0.5f, fTimeDelta);;

		m_pOwner->m_pTransformCom->Move({ 0.f, -m_fSpeedY, RACE_SPEED_PER_SEC }, fTimeDelta);
		_float3 vPos = *m_pOwner->m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		if (vPos.y < 200)
			Exit();
		
		//���� ��ġ�� �����Ѵ�.
		//if (m_pOwner->Comeback(fTimeDelta))
		//	Exit();
	}
	virtual void Exit() override
	{
		m_pOwner->Set_State(CRaceBoss::IDLE);
	}

private:
	_float	m_fSpeedY{};
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
		m_fSpeedY = 100.f;
	}
	virtual void Execute(_float fTimeDelta) override
	{
		m_fSpeedY *= powf(2.0f, fTimeDelta);
		m_pOwner->m_pTransformCom->Move({ 0.f,m_fSpeedY,100.f }, fTimeDelta);

	}
	virtual void Exit() override
	{
	}
private:
	_float m_fSpeedY{};
};

class CRBState_Dead final : public CRBState
{
public:
	CRBState_Dead(CRaceBoss* pOwner)
		:CRBState(pOwner) {
	}
	virtual ~CRBState_Dead() = default;

public:
	virtual void Enter(_float fTimeDelta) override
	{

	}
	virtual void Execute(_float fTimeDelta) override
	{
		//�� �����Ӹ��� ���� ��ġ��ŭ �и� +  ���� �Ʒ��� ������ ��������
		m_pOwner->m_pTransformCom->Move({ -5.f,-100.f,RACE_SPEED_PER_SEC }, fTimeDelta);

		if (m_pOwner->m_pTransformCom->Get_State(CTransform::STATE_POSITION)->y < -100.f)
			Exit();
	}
	virtual void Exit() override
	{
		m_pOwner->m_bDead = TRUE;
	}
};

class CRBState_CloseToPlayer final : public CRBState
{
public:
	CRBState_CloseToPlayer(CRaceBoss* pOwner)
		:CRBState(pOwner) {
	}
	virtual ~CRBState_CloseToPlayer() = default;

public:
	virtual void Enter(_float fTimeDelta) override
	{
		// �ӵ����
		const _float3& vPos = *m_pOwner->m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		const _float fDiffToPlayer = vPos.z - m_pOwner->m_pPlayerpos->z;

		m_fTime = 0.f;
		m_fSpeedPerSec = fDiffToPlayer * 1.6f;
	}
	virtual void Execute(_float fTimeDelta) override
	{
		m_fTime += fTimeDelta;
		const _float3& vPos = *m_pOwner->m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		const _float fDiffToPlayer = vPos.z - m_pOwner->m_pPlayerpos->z;

		m_pOwner->m_pTransformCom->Move({ 0.f,0.f,-m_fSpeedPerSec }, fTimeDelta);
		m_fSpeedPerSec *= powf(0.15f, fTimeDelta);

		if (m_fTime > 2.f ||
			fDiffToPlayer < 750.f) // 750 ���� �������
			Exit();
	}
	virtual void Exit() override
	{
		m_pOwner->Set_State(CRaceBoss::IDLE);
	}
	
private:
	_float			m_fSpeedPerSec{};
};



END