#pragma once
#include "Statue.h"

BEGIN(Client)

class CMombackLine final : public CStatue
{
private:
	CMombackLine(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMombackLine(const CMombackLine& Prototype);
	virtual ~CMombackLine() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;

	HRESULT SetUp_RenderState();
	virtual HRESULT Render() override;
	HRESULT Release_RenderState();


private:
	CGameObject* m_pPlayer = { nullptr };

	CGameObject* m_pMissile = nullptr;
	_bool		m_bDoOnce = { false };
	_bool		m_bDead = { false };
	_bool		m_bFrame = { false };
public:
	static CMombackLine* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END


