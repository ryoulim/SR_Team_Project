// 내 클래스 이름 : FirePlug
// 부모 클래스 이름 : Interactive_Block

#pragma once
#include "Interactive_Block.h"

BEGIN(Client)

class CFirePlug final : public CInteractive_Block
{
public:
	typedef struct tagFirePlugDesc : public CInteractive_Block::DESC
	{

	}DESC;

private:
	CFirePlug(LPDIRECT3DDEVICE9 pGraphic_Device);
	CFirePlug(const CFirePlug& Prototype);
	virtual ~CFirePlug() = default;

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
	void Spawn_HydroPump();
	void Im_Broken();

private:
	CSound_Core* m_pBGM = { nullptr };

private:
	_bool m_bBroken = { false };

public:
	static CFirePlug* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END
