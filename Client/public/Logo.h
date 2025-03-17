// 내 클래스 이름 : Logo
// 부모 클래스 이름 : UI

#pragma once
#include "UI.h"

BEGIN(Client)

class CLogo final : public CUI
{
public:
	typedef struct tagLogoDesc : public CUI::DESC
	{

	}DESC;

private:
	CLogo(LPDIRECT3DDEVICE9 pGraphic_Device);
	CLogo(const CLogo& Prototype);
	virtual ~CLogo() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Component_For_Shadow(void* pArg);

private:
	CTexture* m_pTextureCom_For_Shadow = { nullptr };
	CVIBuffer* m_pVIBufferCom_For_Shadow = { nullptr };

public:
	static CLogo* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END
