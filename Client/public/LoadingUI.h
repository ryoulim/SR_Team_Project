// 내 클래스 이름 : LoadingUI
// 부모 클래스 이름 : UI

#pragma once
#include "UI.h"

BEGIN(Client)

class CLoadingUI final : public CUI
{
public:
	typedef struct tagLoadingUIDesc : public CUI::DESC
	{
		LEVEL	eNextLevel;
		LEVEL	eCurLevel;
	}DESC;

private:
	CLoadingUI(LPDIRECT3DDEVICE9 pGraphic_Device);
	CLoadingUI(const CLoadingUI& Prototype);
	virtual ~CLoadingUI() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	HRESULT	Initialize_LoadingCutscene(void* pArg);
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	class CLoadingCutscene* m_pLoadingCutscene = { nullptr };
	LEVEL					m_eNextLevel = LEVEL_END;
	LEVEL					m_eCurLevel = LEVEL_END;
	_float					m_fLoadingGauge{ 0.1f }; //필요한진 모름
	_float					m_fCurLoadingGauge{};
	_bool					m_isFinished = { false };

public:
	_bool IsLoadingComplete();
	void Set_LoadingGauge(const _float percent);

public:
	static CLoadingUI* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END