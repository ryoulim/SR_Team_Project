// 내 클래스 이름 : LoadingMenu
// 부모 클래스 이름 : UI

#pragma once
#include "UI.h"

BEGIN(Client)

class CLoadingMenu final : public CUI
{
public:
	typedef struct tagLoadingMenuDesc : public CUI::DESC
	{

	}DESC;
	enum LOADERTEXTURES { ROTATEANIM, LOADERBARBACK, LOADERBAR };

private:
	CLoadingMenu(LPDIRECT3DDEVICE9 pGraphic_Device);
	CLoadingMenu(const CLoadingMenu& Prototype);
	virtual ~CLoadingMenu() = default;

private:
	HRESULT Ready_LoadingComponents();

private:
	vector<CTexture*>	m_pTextureComForLoading = { nullptr };
	vector<CVIBuffer*>	m_pVIBufferComForLoading = { nullptr };
	vector<CTransform*> m_pTransformComForLoading = { nullptr };

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CLoadingMenu* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END