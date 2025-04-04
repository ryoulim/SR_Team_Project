// 내 클래스 이름 : Skull
// 부모 클래스 이름 : UI

#pragma once
#include "UI.h"

BEGIN(Client)

class CSkull final : public CUI
{
private:
	CSkull(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSkull(const CSkull& Prototype);
	virtual ~CSkull() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	EVENT Update(const _float3& vPosition, _float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	void	TimeReset() {
		m_fTimeAcc = 0.f;
	}

private:
	static _float4x4	m_Orthomat;
	_float				m_fTimeAcc{};
public:
	static CSkull* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END