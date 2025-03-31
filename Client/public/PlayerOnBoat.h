// �� Ŭ���� �̸� : PlayerOnBoat
// �θ� Ŭ���� �̸� : Pawn

#pragma once
#include "Pawn.h"

BEGIN(Client)

class CPlayerOnBoat final : public CPawn
{
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
private:
	class CCameraManager* m_pCameraManager = { nullptr };
	CTransform* m_pCameraTransform = { nullptr };

private:
	void			Key_Input(_float fTimeDelta);
	void			Init_Camera_Link();
	void			Update_Camera_Link();

public:
	static CPlayerOnBoat* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END

