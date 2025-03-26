// 내 클래스 이름 : InvisibleBlock
// 부모 클래스 이름 : Block

#pragma once
#include "Map.h"

BEGIN(Client)

class CInvisibleBlock final : public CMap
{
public:
	typedef struct tagInvisibleBlockDesc : public CMap::DESC
	{

	}DESC;

private:
	CInvisibleBlock(LPDIRECT3DDEVICE9 pGraphic_Device);
	CInvisibleBlock(const CInvisibleBlock& Prototype);
	virtual ~CInvisibleBlock() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	virtual HRESULT Ready_Components(void* pArg) override;

public:
	static CInvisibleBlock* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END
