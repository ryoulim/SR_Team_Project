// 내 클래스 이름 : Ladder
// 부모 클래스 이름 : Map

#pragma once
#include "Map.h"

BEGIN(Client)

class CLadder final : public CMap
{
public:
	typedef struct tagLadderDesc : public CMap::DESC
	{

	}DESC;

private:
	CLadder(LPDIRECT3DDEVICE9 pGraphic_Device);
	CLadder(const CLadder& Prototype);
	virtual ~CLadder() = default;

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
	static CLadder* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END