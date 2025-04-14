// 내 클래스 이름 : Picture
// 부모 클래스 이름 : Interactive_Block

#pragma once
#include "Interactive_Block.h"

BEGIN(Client)

class CPicture final : public CInteractive_Block
{
private:
	CPicture(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPicture(const CPicture& Prototype);
	virtual ~CPicture() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void On_Collision(_uint MyColliderID, _uint OtherColliderID);

private:
	virtual HRESULT Ready_Components(void* pArg)override;

private:
	_bool			m_bBroken{ FALSE };
	CSound_Core*	m_pBGM = { nullptr };

public:
	static CPicture* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END