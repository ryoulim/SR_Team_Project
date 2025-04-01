#pragma once
#include "Statue.h"

BEGIN(Client)

class CRaceLandscape final : public CStatue
{
private:
	CRaceLandscape(LPDIRECT3DDEVICE9 pGraphic_Device);
	CRaceLandscape(const CRaceLandscape& Prototype);
	virtual ~CRaceLandscape() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Surface_Plane();

public:
	static CRaceLandscape* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END

