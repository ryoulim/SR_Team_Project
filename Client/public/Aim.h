// 내 클래스 이름 : Aim
// 부모 클래스 이름 : UI

#pragma once
#include "UI.h"

BEGIN(Client)

class CAim final : public CUI
{
public:
	typedef struct tagAimDesc : public CUI::DESC
	{

	}DESC;

private:
	CAim(LPDIRECT3DDEVICE9 pGraphic_Device);
	CAim(const CAim& Prototype);
	virtual ~CAim() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CAim* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END