// 내 클래스 이름 : Loading_ToIn
// 부모 클래스 이름 : LoadingCutscene

#pragma once
#include "LoadingCutscene.h"

BEGIN(Client)

class CLoading_ToIn final : public CLoadingCutscene
{
private:
	CLoading_ToIn(LPDIRECT3DDEVICE9 pGraphic_Device);
	CLoading_ToIn(const CLoading_ToIn& Prototype);
	virtual ~CLoading_ToIn() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	HRESULT Render_Background();

private:
	_bool	m_bSoundTrigger[2]{ false, false };

public:
	static CLoading_ToIn* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END