#pragma once

#include "Statue.h"

BEGIN(Engine)
class CCollider;
END

class CRaceBossRazer final : public CStatue
{
public:
	enum RBOMBTTYPE { FIRST, SECOND, THIRD, FOURTH, FIFTH, NON };

public:
	typedef struct tagRaceBossRazerDesc : public CStatue::DESC
	{
		_uint iColliderID;
		COLLIDER_GROUP ColliderGroup;
	}DESC;

private:
	CRaceBossRazer(LPDIRECT3DDEVICE9 pGraphic_Device);
	CRaceBossRazer(const CRaceBossRazer& Prototype);
	virtual ~CRaceBossRazer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void On_Collision(_uint MyColliderID, _uint OtherColliderID) override;

private:
	virtual HRESULT Ready_Components(void* pArg) override;

private:
	CCollider* m_pCollider = { nullptr };
	_float3 m_vQu = {};
	
public:
	static CRaceBossRazer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

