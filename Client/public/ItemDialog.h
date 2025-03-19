// �� Ŭ���� �̸� : ItemDialog
// �θ� Ŭ���� �̸� : UI

#pragma once
#include "UI.h"

BEGIN(Client)

class CItemDialog final : public CUI
{
public:
	typedef struct tagItemDialogDesc : public CUI::DESC
	{

	}DESC;

private:
	CItemDialog(LPDIRECT3DDEVICE9 pGraphic_Device);
	CItemDialog(const CItemDialog& Prototype);
	virtual ~CItemDialog() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CItemDialog* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END