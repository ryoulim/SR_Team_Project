// 내 클래스 이름 : InteractPromptUI
// 부모 클래스 이름 : UI

#pragma once
#include "UI.h"

BEGIN(Client)

class CInteractPromptUI final : public CUI
{
public:
	typedef struct tagInteractPromptUIDesc : public CUI::DESC
	{

	}DESC;

private:
	CInteractPromptUI(LPDIRECT3DDEVICE9 pGraphic_Device);
	CInteractPromptUI(const CInteractPromptUI& Prototype);
	virtual ~CInteractPromptUI() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	virtual HRESULT Ready_Components(void* pArg)override;

public:
	static CInteractPromptUI* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END