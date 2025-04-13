// 내 클래스 이름 : GarbageBag
// 부모 클래스 이름 : Interactive_Block

#pragma once
#include "Interactive_Block.h"

BEGIN(Client)

class CGarbageBag final : public CInteractive_Block
{
public:
	typedef struct tagGarbageBagDesc : public CInteractive_Block::DESC
	{

	}DESC;

private:
	CGarbageBag(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGarbageBag(const CGarbageBag& Prototype);
	virtual ~CGarbageBag() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void On_Collision(_uint MyColliderID, _uint OtherColliderID) override;

private:
	virtual HRESULT Ready_Components(void* pArg)override;
	void Create_Item();

private:
	CSound_Core* m_pBGM = { nullptr };

public:
	static CGarbageBag* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END