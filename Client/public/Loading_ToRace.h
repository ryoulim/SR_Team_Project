// 내 클래스 이름 : Loading_ToRace
// 부모 클래스 이름 : LoadingCutscene

#pragma once
#include "LoadingCutscene.h"

BEGIN(Client)

class CLoading_ToRace final : public CLoadingCutscene
{
private:
	CLoading_ToRace(LPDIRECT3DDEVICE9 pGraphic_Device);
	CLoading_ToRace(const CLoading_ToRace& Prototype);
	virtual ~CLoading_ToRace() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	HRESULT Render_Background();
	
private:
	_bool m_bVehicleSoundTrigger{ false };

public:
	static CLoading_ToRace* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END