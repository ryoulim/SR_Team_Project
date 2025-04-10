// 내 클래스 이름 : LoadingCutscene
// 부모 클래스 이름 : UI

#pragma once
#include "UI.h"

BEGIN(Client)
/* 부모가 되는 컷씬 인터페이스임 */
class CLoadingCutscene : public CUI 
{
public:
	typedef struct tagLoadingCutsceneDesc : public CUI::DESC
	{

	}DESC;

protected:
	CLoadingCutscene(LPDIRECT3DDEVICE9 pGraphic_Device);
	CLoadingCutscene(const CLoadingCutscene& Prototype);
	virtual ~CLoadingCutscene() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Ready_Components(void* pArg);
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_LoadingGauge(const _float percent) { m_fCurLoadingGauge = percent; }
	_bool IsLoadingComplete() { return m_isReadyToChangeLevel; }

protected:
	_float		m_fLoadingGauge{ 0.01f };
	_float		m_fCurLoadingGauge{ 0.f };
	_bool		m_isLoadingFinished{ false };
	_float 		m_fFinished_WaitingTime{0.f};
	_bool		m_isReadyToChangeLevel{ false };

public:
	static CLoadingCutscene* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END