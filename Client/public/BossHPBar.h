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
	HRESULT Render_FirstTime();
	HRESULT Render_Ttakkeun_I();
	HRESULT Render_RacingBoss();

public:
	void Set_BossHP(_int* pBossHP) { m_pBossHP[0] = pBossHP; }
	void Set_BossHP_T2(_int* pBossHP) { m_pBossHP[1] = pBossHP; }
	void Set_BossMaxHP(_int iBossMaxHP) {
		m_iBossMaxHP = iBossMaxHP; 
	}
	void Set_RenderStart() { m_eIsFirstTime = FIRST_RENDER; }

private:
	array<_int*, 2>	m_pBossHP = { nullptr };
	_int			m_iBossMaxHP = {};
	_float3			m_vBarSize = {};
	_float3			m_vBackSize = {};
	_bool			m_bIsFirst = { false };
	enum RENDERSTATE { DONT_RENDER, FIRST_RENDER, DONE };
	RENDERSTATE		m_eIsFirstTime = { DONT_RENDER };
	_float			m_fFirstTimeAnim = { 0.f };

public:
	static CBossHPBar* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END