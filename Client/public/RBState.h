#pragma once
#include "Base.h"
#include "Client_Defines.h"
#include "RaceBoss.h"
#include "PlayerOnBoat.h"
#include "CameraManager.h"
#include "WaterBoat.h"
#include "FXMgr.h"

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
		m_pOwner->m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(0.f, 250.f, 0.f));

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
		//m_pOwner->Set_State(CRaceBoss::READYBOMB);
		//m_pOwner->Set_State(CRaceBoss::MOMBACKREADY);
		m_pOwner->Set_State(CRaceBoss::IDLE);
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
		m_pOwner->m_vSavedRight = *m_pOwner->m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		m_pOwner->m_vSavedUp = *m_pOwner->m_pTransformCom->Get_State(CTransform::STATE_UP);
		m_pOwner->m_vSavedLook = *m_pOwner->m_pTransformCom->Get_State(CTransform::STATE_LOOK);
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

		if (m_pOwner->m_pPlayerpos->z > -1500.f &&
			fDiffToPlayer - fScaleZ*2.f < 0.f)
			m_pOwner->m_pPlayer->Set_StartState(CPlayerOnBoat::AWAY_FROM_BOSS);

		if (m_pOwner->m_pPlayerpos->z > -3500.f &&	m_pOwner->m_pPlayerpos->z < -2500.f)
			m_pOwner->Set_State(CRaceBoss::ENTRANCE);

		///////////////////////////////////////
		// �÷��̾� ��Ȧ �� �÷��̾ ��ġ�� ���� �ߵ��Ǽ� �Ÿ�������� ���� �߽��ϴ�.
		///////////////////////////////////////

		/* [ �÷��̾��� ���°� �븻�� ���� ������ ��ȿ�ϰ� �϶� ] */
		//if (m_pOwner->m_pPlayer->GetState() == CPlayerOnBoat::NORMAL)
		//{
		//	if (m_pOwner->m_bChangeLevel)
		//	{
		//		m_pOwner->m_bChangeLevel = false;
		//		m_pOwner->Set_State(CRaceBoss::ENTRANCE);
		//	}

		if (m_pOwner->m_pPlayerpos->z > -2500)
		{
			if (m_fTime > 0.3f) // ��¦�� �� �ֱ�
			{
				/* [ SHOTREADY, READYBOMB, MOMBACKREADY ] */
				int iRandomPattern = GetRandomInt(0, 100);

				if (iRandomPattern > 95)
					m_pOwner->Set_State(CRaceBoss::MOMBACKREADY);
				else if (iRandomPattern > 90)
					m_pOwner->Set_State(CRaceBoss::READYBOMB);
				else
					m_pOwner->Set_State(CRaceBoss::SHOTREADY);

				m_fTime = 0.f;
			}
		}
		//}
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
		/* [ �⺻���� Z�� �̵� ] */
		m_pOwner->m_pTransformCom->Move({ 0.f,0.f,RACE_SPEED_PER_SEC }, fTimeDelta);

		/* [ 2�ʵ��� ��� ���� ] */
		m_fTime += fTimeDelta;
		if (m_fTime > 0.5f)
			Exit();
	}

	virtual void Exit() override
	{
		/* [ ���� ���� ] */
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
		/* [ �⺻���� Z�� �̵� ] */
		m_pOwner->m_pTransformCom->Move({ 0.f,0.f,RACE_SPEED_PER_SEC}, fTimeDelta);

		m_pOwner->ShuffleandPop();
		m_pOwner->Fire_HeadBullet(fTimeDelta);
		m_pOwner->Play_Sound("ShotBullet");
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
		m_iTailBulletCount = 0;
		m_fTime = 0;
	}

	virtual void Execute(_float fTimeDelta) override
	{
		//�� �����Ӹ��� ���� ��ġ��ŭ �и�
		m_pOwner->m_pTransformCom->Move({ 0.f,0.f,RACE_SPEED_PER_SEC}, fTimeDelta);

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
		if (m_pOwner->Get_HeadBulletCount() > 4)
		{
			//������ �������� �̾���
			m_pOwner->Set_State(CRaceBoss::IDLE);
			m_pOwner->Set_HeadBulletCountZero();

			//�ѱ� �̹����� �������
			m_pOwner->RestoreTextureID();

			//Ȥ�ó� �Ҹ� �������
			m_pOwner->Stop_Sound("ShotBullet");
		}

		else
			m_pOwner->Set_State(CRaceBoss::SHOTHEADBULLET);
	}

private:
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
	/*    [ �����غ� : �÷��̾� ��ġ�� �����Ÿ��� �����ϸ� �������� ������ϴ� ]   */
	virtual void Enter(_float fTimeDelta) override
	{
		m_fTime = 0.f;
		m_pOwner->m_vSavedRight = *m_pOwner->m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		m_pOwner->m_vSavedUp = *m_pOwner->m_pTransformCom->Get_State(CTransform::STATE_UP);
		m_pOwner->m_vSavedLook = *m_pOwner->m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	}
	virtual void Execute(_float fTimeDelta) override
	{
		m_fTime += fTimeDelta;
		
		/* [ ���� ���� ��ġ ] */
		_float3 vCurrentPos = *m_pOwner->m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		/* [ ���� ��ǥ ��ġ ��� ] */
		_float fUpSpeed = 1000.f;
		_float fGoSpeed = 1000.f;
		_float3 vTargetPos = vCurrentPos;
		vTargetPos.y += fTimeDelta * fUpSpeed;
		vTargetPos.z -= fTimeDelta * fGoSpeed;

		/* [ Lerp�� �ε巴�� ���� �̵� ] */
		_float lerpFactor = 0.2f; // 0.1~0.2 ������ �Ϲ���
		_float3 vSmoothPos = LERP(vCurrentPos, vTargetPos, lerpFactor);

		m_pOwner->m_pTransformCom->TurnCustom({ 1.f, 0.f, 0.f }, 0.1f, fTimeDelta);
		m_pOwner->m_pTransformCom->Set_State(CTransform::STATE_POSITION, vSmoothPos);

		if (m_bPlaySound)
		{
			m_pOwner->Play_Sound("BombStart");
			m_bPlaySound = false;
		}

		// �غ�ð� : 2��
		if (m_fTime > 2.0f)
			Exit();

	}
	virtual void Exit() override
	{
		/* ȸ�� ����ġ�� ���� */
		m_pOwner->m_pTransformCom->Set_State(CTransform::STATE_RIGHT, m_pOwner->m_vSavedRight);
		m_pOwner->m_pTransformCom->Set_State(CTransform::STATE_UP, m_pOwner->m_vSavedUp);
		m_pOwner->m_pTransformCom->Set_State(CTransform::STATE_LOOK, m_pOwner->m_vSavedLook);

		m_pOwner->Set_State(CRaceBoss::BOMBATTACK);

		m_bPlaySound = true;
	}

private:
	_bool m_bPlaySound = { true };
};

class CRBState_BombAttack final : public CRBState
{
public:
	CRBState_BombAttack(CRaceBoss* pOwner)
		:CRBState(pOwner) {}
	virtual ~CRBState_BombAttack() = default;

public: /* [ �հ����� ������ ������ �ϸ� �÷��̾ ���� �����Ѵ� ] */
	virtual void Enter(_float fTimeDelta) override
	{
		m_fTime = 0.f;

		// ������ �հ��� ��ġ�Ѵ�.
		_float3 vCurrentPos = *m_pOwner->m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float fYPos = m_pOwner->m_pPlayerpos->y + 100;
		_float fZPos = m_pOwner->m_pPlayerpos->z + 4000;
		m_pOwner->m_pTransformCom->Set_State(CTransform::STATE_POSITION, { vCurrentPos.x, fYPos, fZPos });

		m_pOwner->SpawnWaterParticle(400.f, -360.f, 360.f);
	}
	virtual void Execute(_float fTimeDelta) override
	{
		m_fTime += fTimeDelta;
		m_fShakeTime += fTimeDelta;

		/* �հ����� �÷��̾ ���� �����Ѵ� */
		_float3 vCurrentPos = *m_pOwner->m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		
		/* ������� ��ƼŬ */
		if (m_pOwner->m_pWaterBoatEffect_01)
		{
			_float3 vWaterPos = vCurrentPos;
			vWaterPos.z -= 500.f;
			vWaterPos.y -= 100.f;

			static_cast<CWaterBoat*>(m_pOwner->m_pWaterBoatEffect_01)->SetPosition(vWaterPos);
			static_cast<CWaterBoat*>(m_pOwner->m_pWaterBoatEffect_02)->SetPosition(vWaterPos);
			static_cast<CWaterBoat*>(m_pOwner->m_pWaterBoatEffect_03)->SetPosition(vWaterPos);
		}

		_float fSpeed = 800.f;
		_float3 vTargerPos = vCurrentPos;
		vTargerPos.z -= fTimeDelta * fSpeed;

		m_pOwner->m_pTransformCom->Set_State(CTransform::STATE_POSITION, vTargerPos);

		m_fSoundTime += fTimeDelta;
		if (m_fSoundTime > 0.2f && m_iSoundCount < 17)
		{
			m_pOwner->Play_Sound("BombAttack");
			m_fSoundTime = 0.f;
			++m_iSoundCount;
		}
		
		/* [ ���� ���� ] */
		m_pOwner->SpawnMultipleTargetAim(fTimeDelta);

		/* �����ε� ī�޶� �����߰���? */
		if (m_fShakeTime > 0.2f)
		{
			if (vCurrentPos.z > m_pOwner->m_pPlayerpos->z)
			{
				m_fShakeTime = 0.f;
				static_cast<CCameraManager*>(m_pOwner->m_pGameInstance->Find_Manager(TEXT("Camera_Manager")))->Shake_Camera(0.4f, 0.4f);
			}
		}

		if (m_pOwner->m_pPlayerpos->z > vCurrentPos.z + 1500)
			Exit();
	}
	virtual void Exit() override
	{
		m_pOwner->Set_State(CRaceBoss::CROSSATTACK);
		
		m_pOwner->Stop_Sound("BombAttack");
		m_fSoundTime = 0.f;
		m_iSoundCount = 0;

		/* [ ����Ʈ�� �ݳ��ϰ� ���ÿ� ] */
		if (m_pOwner->m_pWaterBoatEffect_01)
		{
			static_cast<CWaterBoat*>(m_pOwner->m_pWaterBoatEffect_01)->SetDead();
			static_cast<CWaterBoat*>(m_pOwner->m_pWaterBoatEffect_02)->SetDead();
			static_cast<CWaterBoat*>(m_pOwner->m_pWaterBoatEffect_03)->SetDead();
			m_pOwner->m_pWaterBoatEffect_01 = nullptr;
			m_pOwner->m_pWaterBoatEffect_02 = nullptr;
			m_pOwner->m_pWaterBoatEffect_03 = nullptr;
		}
	}

private:
	_float m_fShakeTime = 0.f;
	_float m_fSoundTime = 0.f;
	_uint m_iSoundCount = 0;
};

class CRBState_CrossAttack final : public CRBState
{
public:
	CRBState_CrossAttack(CRaceBoss* pOwner)
		:CRBState(pOwner) {}
	virtual ~CRBState_CrossAttack() = default;

public:
	virtual void Enter(_float fTimeDelta) override
	{
		m_fTime = 0;
		m_pOwner->m_vSavedRight = *m_pOwner->m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		m_pOwner->m_vSavedUp = *m_pOwner->m_pTransformCom->Get_State(CTransform::STATE_UP);
		m_pOwner->m_vSavedLook = *m_pOwner->m_pTransformCom->Get_State(CTransform::STATE_LOOK);

		m_pOwner->m_pTransformCom->Turn_Immediately({ 0.f, 0.f, 1.f }, RADIAN(90.f));
		static_cast<CCameraManager*>(m_pOwner->m_pGameInstance->Find_Manager(TEXT("Camera_Manager")))->Shake_Camera(1.f, 1.f);
		m_pOwner->SpawnWaterParticle(-400.f, -120.f, 120.f);
	}
	virtual void Execute(_float fTimeDelta) override
	{
		m_fTime += fTimeDelta;

		/* �÷��̾� �ڿ��� ������ �����鼭 �����ΰ���. */
		_float3 vCurrentPos = *m_pOwner->m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		if (m_bPlaySound)
		{
			m_pOwner->Play_Sound("CrossAttack");
			m_bPlaySound = false;
		}
		
		/* ������� ��ƼŬ */
		if (m_pOwner->m_pWaterBoatEffect_01)
		{
			_float3 vWaterPos = vCurrentPos;
			vWaterPos.y -= 100.f;

			static_cast<CWaterBoat*>(m_pOwner->m_pWaterBoatEffect_01)->SetPosition(vWaterPos);
			static_cast<CWaterBoat*>(m_pOwner->m_pWaterBoatEffect_02)->SetPosition(vWaterPos);
			static_cast<CWaterBoat*>(m_pOwner->m_pWaterBoatEffect_03)->SetPosition(vWaterPos);
		}

		m_fSpeed -= fTimeDelta * 1000.f;
		_float3 vTargerPos = vCurrentPos;
		vTargerPos.z += fTimeDelta * m_fSpeed;

		m_pOwner->m_pTransformCom->Set_State(CTransform::STATE_POSITION, vTargerPos);

		if (vCurrentPos.z > m_pOwner->m_pPlayerpos->z)
			m_pOwner->m_pTransformCom->TurnCustom({ 0.f, 0.f, 1.f }, 1.f, fTimeDelta);

		if (m_fTime > 3.f)
			Exit();

	}
	virtual void Exit() override
	{
		m_pOwner->m_pTransformCom->Set_State(CTransform::STATE_RIGHT, m_pOwner->m_vSavedRight);
		m_pOwner->m_pTransformCom->Set_State(CTransform::STATE_UP, m_pOwner->m_vSavedUp);
		m_pOwner->m_pTransformCom->Set_State(CTransform::STATE_LOOK, m_pOwner->m_vSavedLook);

		m_pOwner->Set_State(CRaceBoss::COMEBACK);
		m_bPlaySound = true;

		/* [ ����Ʈ�� �ݳ��ϰ� ���ÿ� ] */
		if (m_pOwner->m_pWaterBoatEffect_01)
		{
			static_cast<CWaterBoat*>(m_pOwner->m_pWaterBoatEffect_01)->SetDead();
			static_cast<CWaterBoat*>(m_pOwner->m_pWaterBoatEffect_02)->SetDead();
			static_cast<CWaterBoat*>(m_pOwner->m_pWaterBoatEffect_03)->SetDead();
			m_pOwner->m_pWaterBoatEffect_01 = nullptr;
			m_pOwner->m_pWaterBoatEffect_02 = nullptr;
			m_pOwner->m_pWaterBoatEffect_03 = nullptr;
		}
	}
	
private:
	_float	m_fSpeed = 5000.f;
	_bool m_bPlaySound = { true };
};
#pragma endregion

#pragma region ��������
class CRBState_MombackReady final : public CRBState
{
public:
	CRBState_MombackReady(CRaceBoss* pOwner)
		:CRBState(pOwner) {}
	virtual ~CRBState_MombackReady() = default;

public:
	/* [ �����ġ�� ���� ���η� ������ ���� �غ� �Ѵ� ] */
	virtual void Enter(_float fTimeDelta) override
	{
		m_fTime = 0.f;
		m_fRotatedAmount = 0.f;
		/* [ ���� ȸ���� ���� ] */
		m_pOwner->m_vSavedRight = *m_pOwner->m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		m_pOwner->m_vSavedUp = *m_pOwner->m_pTransformCom->Get_State(CTransform::STATE_UP);
		m_pOwner->m_vSavedLook = *m_pOwner->m_pTransformCom->Get_State(CTransform::STATE_LOOK);

		/* [ ���� ���� ��ġ ] */
		_float3 vCurrentPos = *m_pOwner->m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float3 vTargetPos = *m_pOwner->m_pPlayerpos;
		m_pOwner->SpawnTargetLine({ vCurrentPos.x - 100 , 1.f, vTargetPos.z + 1000 });
	}
	virtual void Execute(_float fTimeDelta) override
	{
		m_fTime += fTimeDelta;

		/* [ �⺻���� Z�� �̵� ] */
		m_pOwner->m_pTransformCom->Move({ 0.f,0.f,700.f }, fTimeDelta);

		/* [ ���� ���� ��ġ ] */
		_float3 vCurrentPos = *m_pOwner->m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		if (m_fTime < 1.f)
			m_pOwner->m_pTransformCom->Move({ -50.f, -100.f, 0.f }, fTimeDelta);

		if (m_fRotatedAmount < D3DXToRadian(90.f)) 
		{
			float fTurnSpeed = 1.5f; // �ʴ� 1 �����̶�� ����
			float fAngleThisFrame = fTurnSpeed * fTimeDelta;

			// ȸ�� ���� �纸�� ũ�� ����
			if (m_fRotatedAmount + fAngleThisFrame > D3DXToRadian(90.f))
				fAngleThisFrame = D3DXToRadian(90.f) - m_fRotatedAmount;

			m_pOwner->m_pTransformCom->TurnCustom({ 0.f, 0.f, 1.f }, fAngleThisFrame, 1.f);
			m_fRotatedAmount += fAngleThisFrame;

			m_pOwner->Update_Collider_OffSet(m_fRotatedAmount);
		}

		if (m_fTime > 1.7f)
			Exit();

		
		if (m_pOwner->Get_MombackReady_Sound_State())
		{
			m_pOwner->Play_Sound("MombackReady");
			m_pOwner->Set_MombackReady_Sound_False();
		}
	}
	virtual void Exit() override
	{
		_float3 vCurrentPos = *m_pOwner->m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float3 vTargetPos = *m_pOwner->m_pPlayerpos;
		m_pOwner->SpawnTargetLineReverse({ vCurrentPos.x + 200 , 1.f, vTargetPos.z + 1000 });
		m_pOwner->Set_State(CRaceBoss::MOMBACK);
		m_pOwner->Stop_Sound("MombackReady");
	}

private:
	float m_fRotatedAmount = 0.f;
};

class CRBState_Momback final : public CRBState
{
public:
	CRBState_Momback(CRaceBoss* pOwner)
		:CRBState(pOwner) {}
	virtual ~CRBState_Momback() = default;

public:
	virtual void Enter(_float fTimeDelta) override
	{
		m_fTime = 0.f;
		m_fSpeed = 4000.f;
		m_pOwner->SpawnWaterParticle(400.f, -360.f, 360.f);
		static_cast<CCameraManager*>(m_pOwner->m_pGameInstance->Find_Manager(TEXT("Camera_Manager")))->Shake_Camera(1.f, 1.f);


		_float3 vCurrentPos = *m_pOwner->m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float3 vTargetPos = *m_pOwner->m_pPlayerpos;
		_float3 vHitBoxPos = { vCurrentPos.x - 100 , 1.f, vTargetPos.z + 1000 };
		_float3 vHitBoxScale = { 250.f, 100.f, 4500.f };
		m_pOwner->SpawnHitBox(vHitBoxPos, vHitBoxScale, TEXT("Cube"), 1.5f, false);
	}
	virtual void Execute(_float fTimeDelta) override
	{
		m_fTime += fTimeDelta;

		/* �÷��̾�� �����Ѵ�. */
		_float3 vCurrentPos = *m_pOwner->m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		/* ������� ��ƼŬ */
		if (m_pOwner->m_pWaterBoatEffect_01)
		{
			_float3 vWaterPos = vCurrentPos;
			vWaterPos.y -= 100.f;

			static_cast<CWaterBoat*>(m_pOwner->m_pWaterBoatEffect_01)->SetPosition(vWaterPos);
			static_cast<CWaterBoat*>(m_pOwner->m_pWaterBoatEffect_02)->SetPosition(vWaterPos);
			static_cast<CWaterBoat*>(m_pOwner->m_pWaterBoatEffect_03)->SetPosition(vWaterPos);
		}

		if (m_bPlaySound)
		{
			m_pOwner->Play_Sound("CrossAttack");
			m_bPlaySound = false;
		}

		m_fSpeed -= fTimeDelta * 1500.f;
		_float3 vTargerPos = vCurrentPos;
		vTargerPos.z -= fTimeDelta * m_fSpeed;

		m_pOwner->m_pTransformCom->Set_State(CTransform::STATE_POSITION, vTargerPos);

		if (m_pOwner->m_pPlayerpos->z > vCurrentPos.z + 1500)
			Exit();
	}
	virtual void Exit() override
	{
		/* [ ����Ʈ �ִ�? ������ ���� ] */
		if (m_pOwner->m_pWaterBoatEffect_01)
		{
			static_cast<CWaterBoat*>(m_pOwner->m_pWaterBoatEffect_01)->SetDead();
			static_cast<CWaterBoat*>(m_pOwner->m_pWaterBoatEffect_02)->SetDead();
			static_cast<CWaterBoat*>(m_pOwner->m_pWaterBoatEffect_03)->SetDead();
			m_pOwner->m_pWaterBoatEffect_01 = nullptr;
			m_pOwner->m_pWaterBoatEffect_02 = nullptr;
			m_pOwner->m_pWaterBoatEffect_03 = nullptr;
		}

		_float3 vCurrentPos = *m_pOwner->m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float3 vTargetPos = *m_pOwner->m_pPlayerpos;
		_float3 vHitBoxPos = { vCurrentPos.x + 200 , 1.f, vTargetPos.z + 1000 };
		_float3 vHitBoxScale = { 250.f, 100.f, 4500.f };
		m_pOwner->SpawnHitBox(vHitBoxPos, vHitBoxScale, TEXT("Cube"), 1.5f, false);

		m_pOwner->Set_State(CRaceBoss::MOMBACKREVERSE);
		m_pOwner->Stop_Sound("CrossAttack");
		m_bPlaySound = true;
	}

private:
	_float m_fSpeed = 4000.f;
	_bool m_bPlaySound = { true };
};

class CRBState_MombackReverse final : public CRBState
{
public:
	CRBState_MombackReverse(CRaceBoss* pOwner)
		:CRBState(pOwner) {}
	virtual ~CRBState_MombackReverse() = default;

public:
	virtual void Enter(_float fTimeDelta) override
	{
		m_fTime = 0.f;
		m_fSpeed = 5000.f;
		/* ������ ���� */
		_float3 vCurrentPos = *m_pOwner->m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float3 vTargetPos = *m_pOwner->m_pPlayerpos;
		_float3 vNewPos = { vCurrentPos.x + 200.f , vCurrentPos.y,  vTargetPos.z - 200.f };
		m_pOwner->m_pTransformCom->Set_State(CTransform::STATE_POSITION, vNewPos);
		
		m_pOwner->SpawnWaterParticle(400.f, -360.f, 360.f);
		static_cast<CCameraManager*>(m_pOwner->m_pGameInstance->Find_Manager(TEXT("Camera_Manager")))->Shake_Camera(1.f, 1.f);
	}
	virtual void Execute(_float fTimeDelta) override
	{
		m_fTime += fTimeDelta;

		/* �÷��̾�� �����Ѵ�. */
		_float3 vCurrentPos = *m_pOwner->m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		/* ������� ��ƼŬ */
		if (m_pOwner->m_pWaterBoatEffect_01)
		{
			_float3 vWaterPos = vCurrentPos;
			vWaterPos.y -= 100.f;

			static_cast<CWaterBoat*>(m_pOwner->m_pWaterBoatEffect_01)->SetPosition(vWaterPos);
			static_cast<CWaterBoat*>(m_pOwner->m_pWaterBoatEffect_02)->SetPosition(vWaterPos);
			static_cast<CWaterBoat*>(m_pOwner->m_pWaterBoatEffect_03)->SetPosition(vWaterPos);
		}

		m_fSpeed -= fTimeDelta * 1000.f;
		_float3 vTargerPos = vCurrentPos;
		vTargerPos.z += fTimeDelta * m_fSpeed;

		m_pOwner->m_pTransformCom->Set_State(CTransform::STATE_POSITION, vTargerPos);
		
		if (m_bPlaySound)
		{
			m_pOwner->Play_Sound("CrossAttack");
			m_bPlaySound = false;
		}

		if (m_fTime > 2.f)
			Exit();
	}
	virtual void Exit() override
	{
		m_pOwner->Set_State(CRaceBoss::COMEBACK);

		/* [ ����Ʈ�� �ݳ��ϰ� ���ÿ� ] */
		if (m_pOwner->m_pWaterBoatEffect_01)
		{
			static_cast<CWaterBoat*>(m_pOwner->m_pWaterBoatEffect_01)->SetDead();
			static_cast<CWaterBoat*>(m_pOwner->m_pWaterBoatEffect_02)->SetDead();
			static_cast<CWaterBoat*>(m_pOwner->m_pWaterBoatEffect_03)->SetDead();
			m_pOwner->m_pWaterBoatEffect_01 = nullptr;
			m_pOwner->m_pWaterBoatEffect_02 = nullptr;
			m_pOwner->m_pWaterBoatEffect_03 = nullptr;
		}

		/* [ ȸ���� ���� ] */
		m_pOwner->m_pTransformCom->Set_State(CTransform::STATE_RIGHT, m_pOwner->m_vSavedRight);
		m_pOwner->m_pTransformCom->Set_State(CTransform::STATE_UP, m_pOwner->m_vSavedUp);
		m_pOwner->m_pTransformCom->Set_State(CTransform::STATE_LOOK, m_pOwner->m_vSavedLook);

		m_pOwner->Update_Collider_OffSet(0.f);

		m_pOwner->Stop_Sound("CrossAttack");
		m_bPlaySound = true;
	}

private:
	_float m_fSpeed = 5000.f;
	_bool m_bPlaySound = true;
};
#pragma endregion

class CRBState_Comeback final : public CRBState
{
public:
	CRBState_Comeback(CRaceBoss* pOwner)
		:CRBState(pOwner) {}
	virtual ~CRBState_Comeback() = default;

public:
	/*     [ ���� � �������̵簣�� �������� ���ƿ��� �Ѵ�. ]      */
	virtual void Enter(_float fTimeDelta) override
	{
		/* ���� ���� ���� �� �� ȣ��Ǵ°� ������ �־����ϴ� */
		m_pOwner->Set_MombackReady_Sound_True();

		/* [ �������� ����ġ ] */
		_float3 vReturnPos = *m_pOwner->m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float fYPos = m_pOwner->m_pPlayerpos->y + 500;
		_float fZPos = m_pOwner->m_pPlayerpos->z + 1200;
		m_pOwner->m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 450.f, fYPos, fZPos });
	}
	virtual void Execute(_float fTimeDelta) override
	{
		_float3 vCurrentPos = *m_pOwner->m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		_float fYPos = m_pOwner->m_pPlayerpos->y + 150;
		_float fZPos = m_pOwner->m_pPlayerpos->z + 1200;

		_float3 vTargetPos = { 450.f, fYPos, fZPos };

		_float fFollowSpeed = 5.f;
		_float t = fTimeDelta * fFollowSpeed;

		_float3 vResultPos = LERP(vCurrentPos, vTargetPos, t);

		m_pOwner->m_pTransformCom->Set_State(CTransform::STATE_POSITION, vResultPos);


		// ���� ��ġ�� ��ǥ ��ġ�� �Ÿ�
		_float3 Length = vTargetPos - vCurrentPos;
		_float fDist = D3DXVec3Length(&Length);

		if (fDist <= 120.f)
		{
			// �̵� �Ϸ�!
			Exit();
		}

	}
	virtual void Exit() override
	{
		m_pOwner->Set_State(CRaceBoss::IDLE);
	}
};

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
		m_pOwner->m_bChangeLevel = true;
	}
	virtual void Execute(_float fTimeDelta) override
	{
		m_fSpeedY *= powf(2.0f, fTimeDelta);
		m_pOwner->m_pTransformCom->Move({ 0.f,m_fSpeedY,100.f }, fTimeDelta);

		/* �Ѿ� ��� ���� ���� ������ �߻� ���� �̹����� �����Ǵ� ��찡 ����*/
		m_pOwner->RestoreTextureID();

	}
	virtual void Exit() override
	{
		m_pOwner->m_pTransformCom->Rotation_Reset();

		/* [ ����Ʈ �ִ�? ������ ���� ] */
		if (m_pOwner->m_pWaterBoatEffect_01)
		{
			static_cast<CWaterBoat*>(m_pOwner->m_pWaterBoatEffect_01)->SetDead();
			static_cast<CWaterBoat*>(m_pOwner->m_pWaterBoatEffect_02)->SetDead();
			static_cast<CWaterBoat*>(m_pOwner->m_pWaterBoatEffect_03)->SetDead();
			m_pOwner->m_pWaterBoatEffect_01 = nullptr;
			m_pOwner->m_pWaterBoatEffect_02 = nullptr;
			m_pOwner->m_pWaterBoatEffect_03 = nullptr;
		}
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
		m_fTime = 0.f;

		/* [ ����Ʈ �ִ�? ������ ���� ] */
		if (m_pOwner->m_pWaterBoatEffect_01)
		{
			static_cast<CWaterBoat*>(m_pOwner->m_pWaterBoatEffect_01)->SetDead();
			static_cast<CWaterBoat*>(m_pOwner->m_pWaterBoatEffect_02)->SetDead();
			static_cast<CWaterBoat*>(m_pOwner->m_pWaterBoatEffect_03)->SetDead();
			m_pOwner->m_pWaterBoatEffect_01 = nullptr;
			m_pOwner->m_pWaterBoatEffect_02 = nullptr;
			m_pOwner->m_pWaterBoatEffect_03 = nullptr;
		}
	}
	virtual void Execute(_float fTimeDelta) override
	{
		//�� �����Ӹ��� ���� ��ġ��ŭ �и� +  ���� �Ʒ��� ������ ��������
		m_pOwner->m_pTransformCom->Move({ -5.f,-100.f,RACE_SPEED_PER_SEC }, fTimeDelta);

		/* ������� ��ƼŬ */
		if (m_pOwner->m_pWaterBoatEffect_01)
		{
			_float3 vCurrentPos = *m_pOwner->m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			_float3 vWaterPos = vCurrentPos;
			vWaterPos.y -= 100.f;

			static_cast<CWaterBoat*>(m_pOwner->m_pWaterBoatEffect_01)->SetPosition(vWaterPos);
			static_cast<CWaterBoat*>(m_pOwner->m_pWaterBoatEffect_02)->SetPosition(vWaterPos);
			static_cast<CWaterBoat*>(m_pOwner->m_pWaterBoatEffect_03)->SetPosition(vWaterPos);
		}

		if (m_pOwner->m_pTransformCom->Get_State(CTransform::STATE_POSITION)->y <= 0.f && !m_bDoOnce)
		{
			m_pOwner->SpawnDieParticle(-10000.f);
			m_bDoOnce = true;
		}
		
		if (m_pOwner->m_pTransformCom->Get_State(CTransform::STATE_POSITION)->y <= -150.f)
		{
			m_fTime += fTimeDelta;

			if(m_fTime > 3.f)
				Exit();
		}
	}
	virtual void Exit() override
	{
		m_pOwner->m_bDead = TRUE;

		/* [ ����Ʈ�� �ݳ��ϰ� ���ÿ� ] */
		if (m_pOwner->m_pWaterBoatEffect_01)
		{
			static_cast<CWaterBoat*>(m_pOwner->m_pWaterBoatEffect_01)->SetDead();
			static_cast<CWaterBoat*>(m_pOwner->m_pWaterBoatEffect_02)->SetDead();
			static_cast<CWaterBoat*>(m_pOwner->m_pWaterBoatEffect_03)->SetDead();
			m_pOwner->m_pWaterBoatEffect_01 = nullptr;
			m_pOwner->m_pWaterBoatEffect_02 = nullptr;
			m_pOwner->m_pWaterBoatEffect_03 = nullptr;
		}
	}
private:
	_bool m_bDoOnce{};
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