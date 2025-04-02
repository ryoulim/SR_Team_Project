// 내 클래스 이름 : Armor
// 부모 클래스 이름 : UI

#pragma once
#include "UI.h"
#include "Player.h"

BEGIN(Client)

class CArmor final : public CUI
{
public:
	enum ARMORSTATUS { ARMOR0, ARMOR1, ARMOR2, ARMOREND };
private:
	CArmor(LPDIRECT3DDEVICE9 pGraphic_Device);
	CArmor(const CArmor& Prototype);
	virtual ~CArmor() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Ready_Components(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;


private:
	_uint*					m_pArmor = {};
	_uint					m_uiArmor = {};
	ARMORSTATUS				m_eArmorType = { ARMOR0 };
	const CPlayer::INFO*	m_pPlayerInfo = { nullptr };

public:
	void	Set_Info(const CPlayer::INFO* pInfo) { m_pPlayerInfo = pInfo; }

public:
	static CArmor* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END