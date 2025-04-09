// 내 클래스 이름 : Door
// 부모 클래스 이름 : Interactive_Block

#pragma once
#include "Interactive_Block.h"

BEGIN(Client)

class CDoor final : public CInteractive_Block
{
public:
	typedef struct tagDoorDesc : public CInteractive_Block::DESC
	{

	}DESC;

private:
	CDoor(LPDIRECT3DDEVICE9 pGraphic_Device);
	CDoor(const CDoor& Prototype);
	virtual ~CDoor() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void On_Collision(_uint MyColliderID, _uint OtherColliderID)override;

public:
	void Security_Off() {
		m_bSecurity = false; }

private:
	virtual HRESULT Ready_Components(void* pArg)override;
	void Open_The_Door(_float fTimeDelta);

private:
	class CGameObject* m_pInteractPromptUI = { nullptr };
	_bool m_bOpen = { false };
	_bool m_bSecurity = { false };
	_bool m_bPicked = { false };
	_float m_fTimeAcc = {};

public:
	static CDoor* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END