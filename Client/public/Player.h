// 내 클래스 이름 : Player
// 부모 클래스 이름 : Pawn

#pragma once
#include "Pawn.h"

BEGIN(Client)

class CPlayer final : public CPawn
{
private:
	CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPlayer(const CPlayer& Prototype);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	
	virtual void On_Collision(_uint MyColliderID, _uint OtherColliderID) override;

private:
	CGravity*					m_pGravityCom = { nullptr };
	class CCameraManager*		m_pCameraManager {};
	CTransform*					m_pCameraTransform{};
	_bool						m_bDash{};

	vector<class CWeapon*>		m_Weapons;
	_int						m_iCurWeaponIndex{};
	_int						m_iMaxWeaponIndex{2};

	// 대쉬
	_float3						m_vPrePosition{};
	_float3						m_vDashDirection{};
	_float						m_fDashTimer{};
	_float						m_fDashSpeed{}; // 대쉬 스피드 저장용
	
	// 사다리
	_bool						m_bOnLadder{};

	//히트 관련
	_bool						m_bOnHit{};
	_float						m_fOnHitTimer{};

	//카드 키 갖고있음?
	_bool						m_bHaveCardkey{ false };

	// 저스트 회피
	_byte						m_byJustDodgeFlag{};
	_float						m_fJustDodgeTimer{};
	_float						m_fDashUnappliedDistance{};// 저스트 회피중 대쉬거리 계산
private:
	virtual HRESULT Ready_Components(void* pArg);

	void			Add_Weapons();
	void			Key_Input(_float fTimeDelta);
	void			Init_Camera_Link();
	void			Update_Camera_Link();
	void			Update_Dash(_float fTimeDelta);
	void			Ladder(_float fTimeDelta);
	void			On_Hit(_int iDamage);
	void			On_Just_Dodge();

public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END