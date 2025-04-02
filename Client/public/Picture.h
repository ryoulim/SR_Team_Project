// �� Ŭ���� �̸� : Picture
// �θ� Ŭ���� �̸� : Interactive_Block

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
	_bool			m_bBroken{ FALSE };
public:
	static CPicture* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END