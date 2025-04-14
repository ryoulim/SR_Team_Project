#pragma once

#include "Bullet.h"

class CRaceBossBomb final : public CBullet
{
public:
	enum RBOMBTTYPE { FIRST, SECOND, THIRD, FOURTH, FIFTH, NON };

public:
	typedef struct tagRaceBossBombDesc : public CBullet::DESC
	{
		_bool bAnimation = false;
		_float3 vLook;
		COLLIDER_GROUP ColliderGroup;
		//LERP를 위한 시작점과 끝점
		_float3 fStartPoint;
		_float3 fEndPoint;
	}DESC;

private:
	CRaceBossBomb(LPDIRECT3DDEVICE9 pGraphic_Device);
	CRaceBossBomb(const CRaceBossBomb& Prototype);
	virtual ~CRaceBossBomb() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;

	HRESULT SetUp_RenderState();
	virtual HRESULT Render() override;
	HRESULT Release_RenderState();

	virtual void On_Collision(_uint MyColliderID, _uint OtherColliderID) override;

	_bool GetbDie() { return m_bDie; }
	void SetbDie(_bool bDie) { m_bDie = bDie; }
	void SetbDead(_bool bDead) { m_bDead = bDead; }

private:
	virtual HRESULT Ready_Components(void* pArg) override;
	
private:
	_float m_fScale = { 1.f };
	_float m_fSpeed = {};
	_float3 m_vStartPos = {};
	_float m_fTimeAcc = {};
	
	_bool m_bDie = false;
	CGameObject* m_pMissile = nullptr;
public:
	static CRaceBossBomb* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

