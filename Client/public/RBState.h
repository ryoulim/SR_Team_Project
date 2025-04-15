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
	//상태 진입 시 한 번 호출
	virtual void Enter(_float fTimeDelta) PURE;

	//해당 상태일 때 계속 호출
	virtual void Execute(_float fTimeDelta) PURE;

	//상태 퇴장 시 한 번 호출
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
		//매 프레임마다 일정 수치만큼 밀림
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
		vStartPos = { -1000.f, 1000.f,    0.f };  // 시작점
		vEndPos = { 450.f,  250.f, 1200.f };  // 끝점
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
		//랜덤한 패턴으로 이어짐
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

		//2000보다 거리가 가까울 떄는 매 프레임마다 일정 수치만큼 밀림
		if (fDiffToPlayer < 2000.f)
			m_pOwner->m_pTransformCom->Move({ 0.f,0.f,RACE_SPEED_PER_SEC }, fTimeDelta);


		// 플레이어와 너무 멀어졌으니 가까워져라
		if (fDiffToPlayer > 2500.f)
			m_pOwner->Set_State(CRaceBoss::CLOSE_TO_PLAYER);

		if (m_pOwner->m_pPlayerpos->z > -1500.f &&
			fDiffToPlayer - fScaleZ*2.f < 0.f)
			m_pOwner->m_pPlayer->Set_StartState(CPlayerOnBoat::AWAY_FROM_BOSS);

		if (m_pOwner->m_pPlayerpos->z > -3500.f &&	m_pOwner->m_pPlayerpos->z < -2500.f)
			m_pOwner->Set_State(CRaceBoss::ENTRANCE);

		///////////////////////////////////////
		// 플레이어 웜홀 중 플레이어를 밀치는 패턴 발동되서 거리기반으로 수정 했습니다.
		///////////////////////////////////////

		/* [ 플레이어의 상태가 노말일 때만 패턴이 유효하게 하라 ] */
		//if (m_pOwner->m_pPlayer->GetState() == CPlayerOnBoat::NORMAL)
		//{
		//	if (m_pOwner->m_bChangeLevel)
		//	{
		//		m_pOwner->m_bChangeLevel = false;
		//		m_pOwner->Set_State(CRaceBoss::ENTRANCE);
		//	}

		if (m_pOwner->m_pPlayerpos->z > -2500)
		{
			if (m_fTime > 0.3f) // 살짝의 텀 주기
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

#pragma region 총쏘는 패턴
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
		/* [ 기본적인 Z축 이동 ] */
		m_pOwner->m_pTransformCom->Move({ 0.f,0.f,RACE_SPEED_PER_SEC }, fTimeDelta);

		/* [ 2초동안 대기 상태 ] */
		m_fTime += fTimeDelta;
		if (m_fTime > 0.5f)
			Exit();
	}

	virtual void Exit() override
	{
		/* [ 공격 시작 ] */
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
		/* [ 기본적인 Z축 이동 ] */
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
		//매 프레임마다 일정 수치만큼 밀림
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
			//랜덤한 패턴으로 이어짐
			m_pOwner->Set_State(CRaceBoss::IDLE);
			m_pOwner->Set_HeadBulletCountZero();

			//총구 이미지를 원래대로
			m_pOwner->RestoreTextureID();

			//혹시나 소리 남을까봐
			m_pOwner->Stop_Sound("ShotBullet");
		}

		else
			m_pOwner->Set_State(CRaceBoss::SHOTHEADBULLET);
	}

private:
	_uint m_iTailBulletCount = {};
};

#pragma endregion

#pragma region 폭격패턴
class CRBState_ReadyBombing final : public CRBState
{
public:
	CRBState_ReadyBombing(CRaceBoss* pOwner)
		:CRBState(pOwner) {}
	virtual ~CRBState_ReadyBombing() = default;

public: 
	/*    [ 폭격준비 : 플레이어 위치와 일정거리를 유지하면 공중으로 사라집니다 ]   */
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
		
		/* [ 보스 현재 위치 ] */
		_float3 vCurrentPos = *m_pOwner->m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		/* [ 보스 목표 위치 계산 ] */
		_float fUpSpeed = 1000.f;
		_float fGoSpeed = 1000.f;
		_float3 vTargetPos = vCurrentPos;
		vTargetPos.y += fTimeDelta * fUpSpeed;
		vTargetPos.z -= fTimeDelta * fGoSpeed;

		/* [ Lerp로 부드럽게 보간 이동 ] */
		_float lerpFactor = 0.2f; // 0.1~0.2 정도가 일반적
		_float3 vSmoothPos = LERP(vCurrentPos, vTargetPos, lerpFactor);

		m_pOwner->m_pTransformCom->TurnCustom({ 1.f, 0.f, 0.f }, 0.1f, fTimeDelta);
		m_pOwner->m_pTransformCom->Set_State(CTransform::STATE_POSITION, vSmoothPos);

		if (m_bPlaySound)
		{
			m_pOwner->Play_Sound("BombStart");
			m_bPlaySound = false;
		}

		// 준비시간 : 2초
		if (m_fTime > 2.0f)
			Exit();

	}
	virtual void Exit() override
	{
		/* 회전 원위치용 로직 */
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

public: /* [ 먼곳에서 무차별 폭격을 하며 플레이어를 향해 돌진한다 ] */
	virtual void Enter(_float fTimeDelta) override
	{
		m_fTime = 0.f;

		// 보스를 먼곳에 배치한다.
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

		/* 먼곳에서 플레이어를 향해 돌진한다 */
		_float3 vCurrentPos = *m_pOwner->m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		
		/* 따라오는 파티클 */
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
		
		/* [ 폭격 로직 ] */
		m_pOwner->SpawnMultipleTargetAim(fTimeDelta);

		/* 폭격인데 카메라가 흔들려야겠지? */
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

		/* [ 이펙트를 반납하고 가시오 ] */
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

		/* 플레이어 뒤에서 반으로 가르면서 앞으로간다. */
		_float3 vCurrentPos = *m_pOwner->m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		if (m_bPlaySound)
		{
			m_pOwner->Play_Sound("CrossAttack");
			m_bPlaySound = false;
		}
		
		/* 따라오는 파티클 */
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

		/* [ 이펙트를 반납하고 가시오 ] */
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

#pragma region 몸박패턴
class CRBState_MombackReady final : public CRBState
{
public:
	CRBState_MombackReady(CRaceBoss* pOwner)
		:CRBState(pOwner) {}
	virtual ~CRBState_MombackReady() = default;

public:
	/* [ 몸통박치기 전에 가로로 뒤집고 돌진 준비를 한다 ] */
	virtual void Enter(_float fTimeDelta) override
	{
		m_fTime = 0.f;
		m_fRotatedAmount = 0.f;
		/* [ 이전 회전값 저장 ] */
		m_pOwner->m_vSavedRight = *m_pOwner->m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		m_pOwner->m_vSavedUp = *m_pOwner->m_pTransformCom->Get_State(CTransform::STATE_UP);
		m_pOwner->m_vSavedLook = *m_pOwner->m_pTransformCom->Get_State(CTransform::STATE_LOOK);

		/* [ 보스 현재 위치 ] */
		_float3 vCurrentPos = *m_pOwner->m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float3 vTargetPos = *m_pOwner->m_pPlayerpos;
		m_pOwner->SpawnTargetLine({ vCurrentPos.x - 100 , 1.f, vTargetPos.z + 1000 });
	}
	virtual void Execute(_float fTimeDelta) override
	{
		m_fTime += fTimeDelta;

		/* [ 기본적인 Z축 이동 ] */
		m_pOwner->m_pTransformCom->Move({ 0.f,0.f,700.f }, fTimeDelta);

		/* [ 보스 현재 위치 ] */
		_float3 vCurrentPos = *m_pOwner->m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		if (m_fTime < 1.f)
			m_pOwner->m_pTransformCom->Move({ -50.f, -100.f, 0.f }, fTimeDelta);

		if (m_fRotatedAmount < D3DXToRadian(90.f)) 
		{
			float fTurnSpeed = 1.5f; // 초당 1 라디안이라고 가정
			float fAngleThisFrame = fTurnSpeed * fTimeDelta;

			// 회전 남은 양보다 크면 조절
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

		/* 플레이어에게 돌진한다. */
		_float3 vCurrentPos = *m_pOwner->m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		/* 따라오는 파티클 */
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
		/* [ 이펙트 있니? 있으면 내놔 ] */
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
		/* 포지션 정비 */
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

		/* 플레이어에게 돌진한다. */
		_float3 vCurrentPos = *m_pOwner->m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		/* 따라오는 파티클 */
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

		/* [ 이펙트를 반납하고 가시오 ] */
		if (m_pOwner->m_pWaterBoatEffect_01)
		{
			static_cast<CWaterBoat*>(m_pOwner->m_pWaterBoatEffect_01)->SetDead();
			static_cast<CWaterBoat*>(m_pOwner->m_pWaterBoatEffect_02)->SetDead();
			static_cast<CWaterBoat*>(m_pOwner->m_pWaterBoatEffect_03)->SetDead();
			m_pOwner->m_pWaterBoatEffect_01 = nullptr;
			m_pOwner->m_pWaterBoatEffect_02 = nullptr;
			m_pOwner->m_pWaterBoatEffect_03 = nullptr;
		}

		/* [ 회전값 복구 ] */
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
	/*     [ 현재 어떤 포지션이든간에 원점으로 돌아오게 한다. ]      */
	virtual void Enter(_float fTimeDelta) override
	{
		/* 몸박 레디 사운드 두 번 호출되는거 떄문에 넣었습니다 */
		m_pOwner->Set_MombackReady_Sound_True();

		/* [ 포지션을 원위치 ] */
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


		// 현재 위치와 목표 위치의 거리
		_float3 Length = vTargetPos - vCurrentPos;
		_float fDist = D3DXVec3Length(&Length);

		if (fDist <= 120.f)
		{
			// 이동 완료!
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

		/* 총알 쏘기 패턴 도중 떠나면 발사 상태 이미지로 고정되는 경우가 있음*/
		m_pOwner->RestoreTextureID();

	}
	virtual void Exit() override
	{
		m_pOwner->m_pTransformCom->Rotation_Reset();

		/* [ 이펙트 있니? 있으면 내놔 ] */
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

		/* [ 이펙트 있니? 있으면 내놔 ] */
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
		//매 프레임마다 일정 수치만큼 밀림 +  왼쪽 아래로 꾸준히 떨어져라
		m_pOwner->m_pTransformCom->Move({ -5.f,-100.f,RACE_SPEED_PER_SEC }, fTimeDelta);

		/* 따라오는 파티클 */
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

		/* [ 이펙트를 반납하고 가시오 ] */
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
		// 속도계산
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
			fDiffToPlayer < 750.f) // 750 까지 가까워짐
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