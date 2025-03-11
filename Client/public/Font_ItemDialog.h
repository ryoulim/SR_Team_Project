// 내 클래스 이름 : Font_ItemDialog
// 부모 클래스 이름 : Font

#pragma once
#include "Font.h"

BEGIN(Client)

class CFont_ItemDialog final : public CFont
{
public:
	typedef struct tagFont_ItemDialogDesc : public CFont::DESC
	{

	}DESC;

private:
	CFont_ItemDialog(LPDIRECT3DDEVICE9 pGraphic_Device);
	CFont_ItemDialog(const CFont_ItemDialog& Prototype);
	virtual ~CFont_ItemDialog() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	virtual _uint	Find_TextureNum() override { return 0; };

public:
	static CFont_ItemDialog* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END