// �� Ŭ���� �̸� : Player
// �θ� Ŭ���� �̸� : Pawn

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

	// �뽬
	_float3						m_vPrePosition{};
	_float3						m_vDashDirection{};
	_float						m_fDashTimer{};
	_float						m_fDashSpeed{}; // �뽬 ���ǵ� �����
	
	// ��ٸ�
	_bool						m_bOnLadder{};

	//��Ʈ ����
	_bool						m_bOnHit{};
	_float						m_fOnHitTimer{};

	//ī�� Ű ��������?
	_bool						m_bHaveCardkey{ false };

	// ����Ʈ ȸ��
	_byte						m_byJustDodgeFlag{};
	_float						m_fJustDodgeTimer{};
	_float						m_fDashUnappliedDistance{};// ����Ʈ ȸ���� �뽬�Ÿ� ���
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