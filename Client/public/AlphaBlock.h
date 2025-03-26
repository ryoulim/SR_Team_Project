// 내 클래스 이름 : AlphaBlock
// 부모 클래스 이름 : Map

#pragma once
#include "Map.h"

BEGIN(Client)

class CAlphaBlock final : public CMap
{
public:
	typedef struct tagAlphaBlockDesc : public CMap::DESC
	{

	}DESC;

private:
	CAlphaBlock(LPDIRECT3DDEVICE9 pGraphic_Device);
	CAlphaBlock(const CAlphaBlock& Prototype);
	virtual ~CAlphaBlock() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CAlphaBlock* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END