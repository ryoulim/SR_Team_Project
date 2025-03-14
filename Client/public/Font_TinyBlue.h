// 내 클래스 이름 : Font_ItemDialog
// 부모 클래스 이름 : Font

#pragma once
#include "Font.h"

BEGIN(Client)

class CFont_TinyBlue final : public CFont
{
private:
	CFont_TinyBlue(LPDIRECT3DDEVICE9 pGraphic_Device);
	CFont_TinyBlue(const CFont_TinyBlue& Prototype);
	virtual ~CFont_TinyBlue() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;


public:
	static CFont_TinyBlue* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END