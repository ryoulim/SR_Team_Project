// �� Ŭ���� �̸� : Player
// �θ� Ŭ���� �̸� : Pawn

#pragma once
#include "Pawn.h"
#include "UI_Manager.h"

BEGIN(Client)

class CPlayer final : public CPawn
{
public:
	typedef struct tagPlayerInfo {
		_int	iHP;
		_int	iArmor;
	}INFO;
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
	INFO						m_tInfo{};

	vector<class CWeapon*>		m_Weapons;
	_int						m_iCurWeaponIndex{};
	_int						m_iMaxWeaponIndex{2};

	// �뽬
	_float3						m_vPrePosition{};
	_float3						m_vDashDirection{};
	_float						m_fDashTimer{};

private:
	virtual HRESULT Ready_Components(void* pArg);

	void			Add_Weapons();
	void			Key_Input(_float fTimeDelta);
	void			Init_Camera_Link();
	void			Update_Camera_Link();
	void			Update_Dash(_float fTimeDelta);

public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END