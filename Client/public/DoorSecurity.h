// 내 클래스 이름 : DoorSecurity
// 부모 클래스 이름 : Interactive_Block

#pragma once
#include "Interactive_Block.h"

BEGIN(Client)

class CDoorSecurity final : public CInteractive_Block
{
public:
	enum STATE { USUAL, LOCK, OPEN, ST_END };
public:
	typedef struct tagDoorSecurityDesc : public CInteractive_Block::DESC
	{

	}DESC;

private:
	CDoorSecurity(LPDIRECT3DDEVICE9 pGraphic_Device);
	CDoorSecurity(const CDoorSecurity& Prototype);
	virtual ~CDoorSecurity() = default;

public:
	_bool Get_IsPicked() {
		return m_bPicked;
	}

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void On_Collision(_uint MyColliderID, _uint OtherColliderID)override;

private:
	virtual HRESULT Ready_Components(void* pArg)override;
	void Move_Frame(_float fTimeDelta);
	HRESULT Link_Door();

private:
	_float m_fMaxFrame = {};
	_float m_fLockTimeAcc = {};
	_float m_fOpenTimeAcc = {};
	_bool  m_bPicked = { false };

private:
	class CGameObject* m_pInteractPromptUI = { nullptr };
	class CGameObject* m_pDoor = { nullptr };
	class CPlayer* m_pPlayer = { nullptr };
	CSoundController* m_pSoundCom = { nullptr };

	STATE m_eState = { USUAL };

public:
	static CDoorSecurity* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END
