// 내 클래스 이름 : RaceAim
// 부모 클래스 이름 : UI

#pragma once
#include "UI.h"

BEGIN(Client)

class CRaceAim final : public CUI
{
private:
	CRaceAim(LPDIRECT3DDEVICE9 pGraphic_Device);
	CRaceAim(const CRaceAim& Prototype);
	virtual ~CRaceAim() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	EVENT Update(const _float3& vPosition, _float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CRaceAim* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END