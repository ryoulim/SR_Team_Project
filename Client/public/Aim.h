// 내 클래스 이름 : Aim
// 부모 클래스 이름 : UI

#pragma once
#include "UI.h"
#include "Weapon.h"

BEGIN(Client)

class CAim final : public CUI
{
public:
	typedef struct tagAimDesc : public CUI::DESC
	{

	}DESC;

private:
	CAim(LPDIRECT3DDEVICE9 pGraphic_Device);
	CAim(const CAim& Prototype);
	virtual ~CAim() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void	Calc_Magazine(_uint iMax, _uint iCur);
	void	Set_Ammo(const CWeapon::AMMOINFO* ammoInfo) { m_pAmmoInfo = ammoInfo; }

private:
	_float	m_fMaskingDist = {1.f};
	_uint	m_iMaxMagazine = {};
	_uint	m_iCurMagazine = {};
	const CWeapon::AMMOINFO* m_pAmmoInfo = { nullptr };


public:
	static CAim* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END