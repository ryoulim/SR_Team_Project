// 내 클래스 이름 : ItemDialog
// 부모 클래스 이름 : UI

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
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	typedef struct tagDialog
	{
		string strMessage;
		_float fElapsedTime = {};
	}DIALOG;

	queue<DIALOG> m_DialogQueue;

	_float	m_fDisplayTime = {};
	_float	m_fElapsedTime = {};
	_float	m_fPosX = {};
	_float	m_fPosY = {};

public:
	void Insert_DialogQueue(const string& strMsg);

public:
	static CItemDialog* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END