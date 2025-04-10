// 내 클래스 이름 : Loading_ToUnderground
// 부모 클래스 이름 : LoadingCutscene

#pragma once
#include "LoadingCutscene.h"

BEGIN(Client)

class CLoading_ToUnderground final : public CLoadingCutscene
{
private:
	CLoading_ToUnderground(LPDIRECT3DDEVICE9 pGraphic_Device);
	CLoading_ToUnderground(const CLoading_ToUnderground& Prototype);
	virtual ~CLoading_ToUnderground() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	HRESULT Render_Background();

public:
	static CLoading_ToUnderground* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END