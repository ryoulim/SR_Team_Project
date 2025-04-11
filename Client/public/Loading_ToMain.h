// 내 클래스 이름 : Loading_ToIn
// 부모 클래스 이름 : LoadingCutscene

#pragma once
#include "LoadingCutscene.h"

BEGIN(Client)

class CLoading_ToMain final : public CLoadingCutscene
{
public:
	enum TEXNUM { TEX_ANIM = 0, TEX_BARBACK = 8, TEX_BAR = 9, TEX_BACKGROUND = 10 };

private:
	CLoading_ToMain(LPDIRECT3DDEVICE9 pGraphic_Device);
	CLoading_ToMain(const CLoading_ToMain& Prototype);
	virtual ~CLoading_ToMain() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	HRESULT Initialize_LoadingAnims();
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	HRESULT Render_Background();
	HRESULT Render_LoadingAnimation();
	HRESULT Render_LoadingBar();
	void	Render_LoadingText();

private:
	CTransform* m_pAnimationTransformCom = { nullptr };
	CTransform* m_pBarTransformCom = { nullptr };

public:
	static CLoading_ToMain* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END