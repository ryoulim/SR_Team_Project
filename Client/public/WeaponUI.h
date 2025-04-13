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
	void Add_Weapon() { ++m_iMaxWeaponIndex; }
	void Set_Render(_bool bRender) { m_bRender = bRender; m_fRenderTime = 0.f; m_fSizeMul = 1.f; }

private:
	HRESULT Update_WeaponSettings();
	HRESULT Render_WeaponUI();
	HRESULT Set_Transform(_int iTexNum);

private:
	_float	m_fPosX = g_iWinSizeX * 0.5f;
	_float3	m_vSelectPos = _float3{ g_iWinSizeX * 0.5f - 80.f, 0.f, 0.99f };
	_float3 m_vSecondPos = _float3{ g_iWinSizeX * 0.5f - 60.f, -60.f, 0.99f };
	_float3 m_vThirdPos = _float3{ g_iWinSizeX * 0.5f - 60.f, 60.f, 0.99f };
	_bool   m_bRender = false;
	vector<class CWeapon*>* m_pWeapons;
	_int	m_iCurWeaponIndex = 0;
	_int	m_iMaxWeaponIndex = 0;
	_float	m_fRenderTime = 0.f;
	_float  m_fSizeMul = 1.f;

public:
	static CWeaponUI* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END