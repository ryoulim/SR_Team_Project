// 내 클래스 이름 : PlayerOnBoat
// 부모 클래스 이름 : Pawn

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

private:
	class CCameraManager* m_pCameraManager = { nullptr };
	
	CTransform* m_pCameraTransform = { nullptr };

	_float		m_fStartPosX = {};
	_float		m_fTime = {};

private:
	void			Key_Input(_float fTimeDelta);
	void			Update_Camera_Link();
	_float			LerpToHole(_float StartPos, _float TargetPos, _float fTimeDelta);


public:
	static CPlayerOnBoat* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END

