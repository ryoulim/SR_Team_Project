// 내 클래스 이름 : Player
// 부모 클래스 이름 : Pawn

#pragma once
#include "Pawn.h"

BEGIN(Client)

class CPlayer final : public CPawn
{
public:
	typedef struct tagPlayerDesc : public CPawn::DESC
	{
		_float			fMouseSensor;
	}DESC;
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
	class CCameraManager*		m_pCameraManager {};
	CTransform*					m_pCameraTransform{};
	_float						m_fMouseSensor{};
	_bool						m_bBouseFixMod{};
	_bool						m_bFpsMode{TRUE};

	vector<class CWeapon*>		m_Weapons;
	_uint						m_iCurWeaponIndex{};

private:
	void			Key_Input(_float fTimeDelta);
	void			Mouse_Move();
	void			Mouse_Fix();
	void			Update_Camera_Link();

public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END