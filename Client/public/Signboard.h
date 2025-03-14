#pragma once

// 내 클래스 이름 : Signboard
// 부모 클래스 이름 : Statue

#pragma once
#include "Statue.h"

BEGIN(Client)

class CSignboard final : public CStatue
{
public:
	typedef struct tagSignboardDesc : public CStatue::DESC
	{

	}DESC;

private:
	CSignboard(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSignboard(const CSignboard& Prototype);
	virtual ~CSignboard() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CSignboard* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END

