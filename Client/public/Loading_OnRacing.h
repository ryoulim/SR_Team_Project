// �� Ŭ���� �̸� : Loading_OnRacing
// �θ� Ŭ���� �̸� : LoadingCutscene

#pragma once
#include "LoadingCutscene.h"

BEGIN(Client)

class CLoading_OnRacing final : public CLoadingCutscene
{
private:
	CLoading_OnRacing(LPDIRECT3DDEVICE9 pGraphic_Device);
	CLoading_OnRacing(const CLoading_OnRacing& Prototype);
	virtual ~CLoading_OnRacing() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	HRESULT Render_Background();

public:
	static CLoading_OnRacing* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END