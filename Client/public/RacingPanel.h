// 내 클래스 이름 : RacingPanel
// 부모 클래스 이름 : UI

#pragma once
#include "UI.h"

BEGIN(Client)

class CRacingPanel final : public CUI
{
private:
	CRacingPanel(LPDIRECT3DDEVICE9 pGraphic_Device);
	CRacingPanel(const CRacingPanel& Prototype);
	virtual ~CRacingPanel() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Ready_Components(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_TargetSpeed(_int iTargetSpeed) { 
		m_iTargetSpeed = iTargetSpeed; 
		if (iTargetSpeed > 300)
			m_iTargetSpeed = 300 + _int((iTargetSpeed - 300) * 0.3f); // 계기판 최대 속도 이상일 때 속도 증가량을 좀 감소시켰음
	}

private:
	void Render_Boosts();
	void Render_Arrow();
	void Calculate_ArrowAngle();
	void Acc_CurSpeed(_float fTimeDelta);

private:
	class CFont_Racing* m_pFont_Racing = { nullptr };
	_int	m_iBoostCnt = {};
	_int	m_iCurSpeed = {};
	_int	m_iTargetSpeed = {};
	_int	m_iOrigSpeed = {};
	_float	m_fArrowAngle = {};
	CTransform* m_pArrowTransformCom = { nullptr };

public:
	static CRacingPanel* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END