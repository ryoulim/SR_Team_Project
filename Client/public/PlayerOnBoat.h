// 내 클래스 이름 : PlayerOnBoat
// 부모 클래스 이름 : Pawn

#pragma once
#include "Pawn.h"

BEGIN(Client)

class CPlayerOnBoat final : public CPawn
{
public:
	enum STATE { DECEL, NORMAL, LERP, ACCEL, AWAY_FROM_BOSS, NON };

private:
	CPlayerOnBoat(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPlayerOnBoat(const CPlayerOnBoat& Prototype);
	virtual ~CPlayerOnBoat() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void On_Collision(_uint MyColliderID, _uint OtherColliderID) override;
	virtual void On_Hit(_int iDamage) override;

	_float3		GetVelocityPerSecond(_float fTimeDelta) const {
		const _float3& vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		return (vPos - m_vPrePos) / fTimeDelta;
	}
	void Set_StartState(STATE eState);
	void Set_RaceBossTransform(CTransform* BossTransform);
	STATE GetState() { return m_eCurState; }
private:
	virtual HRESULT Ready_Components(void* pArg)override;
	void Update_Frame(_float fTimeDelta);
	void Update_Aim(_float fTimeDelta);

private:
	friend class CPBState_Accel;
	friend class CPBState_Decel;
	friend class CPBState_Lerp;
	friend class CPBState_Normal;
	friend class CPBState_Away_From_Boss;

	//상태패턴
	void			Set_State(STATE eState);
	void			Go_Straight(_float fTimeDelta);
	void			Key_Input(_float fTimeDelta);
	void			Key_Input2(_float fTimeDelta);
	_float			Compute_StartPosX();
	_float			Compute_CurPosZ();
	void			Move_Lerp(_float fStartPosX, _float fTime);
	void			StoreSlidingSpeed(_float fSpeed);
	_float			GetSlidingSpeed();
	void			Set_Speed(_float fSpeed);
	void			SpawnWaterParticle(_float fWaterSpeed);

	void			Create_Bullet();
	void			Tilt(_bool Right);
	void			Init_Aim();

private:
	class CShader* m_pShaderCom = { nullptr };

	class CCameraManager*	m_pCameraManager = { nullptr };
	// 3인칭 카메라를 소유하게 하고, 카메라에 댐핑 함수를 만들자
	class CTPS_Camera*		m_pTPS_Camera = { nullptr };
	//CTransform*				m_pCameraTransform = { nullptr };

	CGameObject*			m_pWaterBoatEffect_01 = nullptr;
	CGameObject*			m_pWaterBoatEffect_02 = nullptr;
	CGameObject*			m_pWaterBoatEffect_03 = nullptr;

	_float3					m_vPrePos{}; // 이전 프레임의 포지션

	STATE					m_ePreState = { NON };
	STATE					m_eCurState = { NORMAL };
	class CPBState*			m_pCurState = { nullptr };
	class CPBState*			m_pState[NON] = { nullptr };
	_float					m_fSlidingSpeed = {};
	_float					m_fWaterSpeed = {};

	// 플레이어 움직임과 기울기에 감속 넣기 위함
	_float					m_fSpeedRatio{1.f};
	_float					m_fKeyTimer{};
	_float					m_fCurrentLean{};

	// 차체의 단계적인 기울어짐
	_float					m_fAngleTimer{};
	_byte 					m_byTiltIndex {3};

	// 총알 쿨타임
	_float					m_fBulletTimer{};

	// 에임관련
	class CRaceAim*			m_pAim {};
	CTransform*				m_pBossTransform{};
	_float					m_fBossHalfScaleZ{};
	_bool					m_bAimRender{ TRUE };
private:
	void			Init_Camera_Link();
	void			Update_Camera_Link();
	
public:
	static CPlayerOnBoat* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END

