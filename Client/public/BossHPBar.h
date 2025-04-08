// 내 클래스 이름 : BossHPBar
// 부모 클래스 이름 : UI

#pragma once
#include "UI.h"

BEGIN(Client)

class CBossHPBar final : public CUI
{
public:
	typedef struct tagBossHPBarDesc : public CUI::DESC
	{

	}DESC;

private:
	CBossHPBar(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBossHPBar(const CBossHPBar& Prototype);
	virtual ~CBossHPBar() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Ready_Components(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	void Render_FirstTime();


public:
	void Set_BossHP(_int* pBossHP) { m_pBossHP = pBossHP; }
	void Set_BossMaxHP(_int iBossMaxHP) { m_iBossMaxHP = iBossMaxHP; }

private:
	_int	m_iBossMaxHP = {};
	_int*	m_pBossHP = { nullptr };
	_float3 m_vBarSize = {};
	_float3 m_vBackSize = {};

public:
	static CBossHPBar* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END