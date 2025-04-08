// 내 클래스 이름 : Generator
// 부모 클래스 이름 : Interactive_Block

#pragma once
#include "Interactive_Block.h"

BEGIN(Client)

class CGenerator final : public CInteractive_Block
{
public:
	typedef struct tagGeneratorDesc : public CInteractive_Block::DESC
	{

	}DESC;

private:
	CGenerator(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGenerator(const CGenerator& Prototype);
	virtual ~CGenerator() = default;

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
	void Im_Broken(_float fTimeDelta);
	void Start_CutScene();

private:
	_float m_fMaxFrame = {};
	_float m_fMinFrame = {};
	_bool  m_bUpFrame = { true };
	_bool  m_bBroken = { false };
	_bool m_bCutSceneEnd1{ false };

	//_float	m_fCameraTimer{};

public:
	static CGenerator* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END
