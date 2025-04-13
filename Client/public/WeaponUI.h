#pragma once
#include "UI.h"

BEGIN(Client)

class CWeaponUI final : public CUI
{
private:
	CWeaponUI(LPDIRECT3DDEVICE9 pGraphic_Device);
	CWeaponUI(const CWeaponUI& Prototype);
	virtual ~CWeaponUI() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Ready_Components(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_Weapons(const vector<class CWeapon*>* pWeapons) { m_pWeapons = const_cast<vector<class CWeapon*>*>(pWeapons); }
	void Set_CurWeaponIndex(_int iIndex) { m_iCurWeaponIndex = iIndex; }	

private:
	HRESULT Update_WeaponSettings();

private:
	_float	m_fPosX = g_iWinSizeX * 0.5f;
	_float	m_fSelectPosY = 0.f;
	_float  m_fSecondPosY = 0.f;
	_float  m_fThirdPosY = 0.f;
	_bool   m_bRender = false;
	vector<class CWeapon*>* m_pWeapons;
	_int	m_iCurWeaponIndex = 0;

public:
	static CWeaponUI* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END