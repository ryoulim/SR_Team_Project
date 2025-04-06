// 내 클래스 이름 : TrashCan
// 부모 클래스 이름 : Interactive_Block

#pragma once
#include "Interactive_Block.h"

BEGIN(Client)

class CTrashCan final : public CInteractive_Block
{
public:
	typedef struct tagTrashCanDesc : public CInteractive_Block::DESC
	{

	}DESC;

private:
	CTrashCan(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTrashCan(const CTrashCan& Prototype);
	virtual ~CTrashCan() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CTrashCan* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END