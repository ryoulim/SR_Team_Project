// 내 클래스 이름 : PlayerOnBoat
// 부모 클래스 이름 : Pawn

#pragma once
#include "Pawn.h"

BEGIN(Client)
class CPBState;
END

BEGIN(Client)

class CPlayerOnBoat final : public CPawn
{
public:
	enum STATE { DECEL, NORMAL, LERP, ACCEL, NON };

public:
	typedef struct tagPlayerOnBoatDesc : public CPawn::DESC
	{
		_float			fMouseSensor;
	}DESC;

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

	//상태패턴
	void			Set_State(STATE eState);
	void			Go_Straight(_float fTimeDelta);
	void			Key_Input(_float fTimeDelta);
	_float			Compute_StartPosX();
	_float			Compute_CurPosZ();
	void			Move_Lerp(_float fStartPosX, _float fTime);
	void			StoreSlidingSpeed(_float fSpeed);
	_float			GetSlidingSpeed();

private:
	class CCameraManager*	m_pCameraManager = { nullptr };
	CTransform*				m_pCameraTransform = { nullptr };
	STATE					m_ePreState = { NON };
	STATE					m_eCurState = { NORMAL };
	CPBState*				m_pCurState = { nullptr };
	CPBState*				m_pState[NON] = { nullptr };
	_float					m_fSlidingSpeed = {};

private:
	void			Init_Camera_Link();
	void			Update_Camera_Link();
	
public:
	static CPlayerOnBoat* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END

