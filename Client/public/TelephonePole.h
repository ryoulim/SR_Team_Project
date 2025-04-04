// 내 클래스 이름 : TelephonePole
// 부모 클래스 이름 : Map

#pragma once
#include "Map.h"

BEGIN(Client)

class CTelephonePole final : public CMap
{
public:
	typedef struct tagTelephonePoleDesc : public CMap::DESC
	{

	}DESC;

private:
	CTelephonePole(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTelephonePole(const CTelephonePole& Prototype);
	virtual ~CTelephonePole() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CTelephonePole* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END