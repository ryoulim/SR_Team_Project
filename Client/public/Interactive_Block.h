// 내 클래스 이름 : Interactive_Block
// 부모 클래스 이름 : Map

#pragma once
#include "Map.h"

BEGIN(Client)

class CInteractive_Block final : public CMap
{
public:
	typedef struct tagInteractive_BlockDesc : public CMap::DESC
	{

	}DESC;

private:
	CInteractive_Block(LPDIRECT3DDEVICE9 pGraphic_Device);
	CInteractive_Block(const CInteractive_Block& Prototype);
	virtual ~CInteractive_Block() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CInteractive_Block* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END