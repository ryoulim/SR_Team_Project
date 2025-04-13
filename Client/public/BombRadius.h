#pragma once
#include "Statue.h"

BEGIN(Client)

class CBombRadius final : public CStatue
{
private:
	CBombRadius(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBombRadius(const CBombRadius& Prototype);
	virtual ~CBombRadius() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;

	HRESULT SetUp_RenderState();
	virtual HRESULT Render() override;
	HRESULT Release_RenderState();


public:
	void SpawnMissile(_float3 _Position);

private:
	CGameObject* m_pPlayer = { nullptr };

	CGameObject* m_pMissile = nullptr;
	_bool		m_bDoOnce = { false };
	_bool		m_bDead = { false };
public:
	static CBombRadius* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END


