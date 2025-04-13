#pragma once
#include "Statue.h"

BEGIN(Client)

class CRazerRadius final : public CStatue
{
private:
	CRazerRadius(LPDIRECT3DDEVICE9 pGraphic_Device);
	CRazerRadius(const CRazerRadius& Prototype);
	virtual ~CRazerRadius() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CGameObject* m_pPlayer = { nullptr };

public:
	static CRazerRadius* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END


