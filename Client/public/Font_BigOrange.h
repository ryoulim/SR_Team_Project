// 내 클래스 이름 : Font_ItemDialog
// 부모 클래스 이름 : Font

#pragma once
#include "Font.h"

BEGIN(Client)

class CFont_BigOrange final : public CFont
{
private:
	CFont_BigOrange(LPDIRECT3DDEVICE9 pGraphic_Device);
	CFont_BigOrange(const CFont_BigOrange& Prototype);
	virtual ~CFont_BigOrange() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	// 텍스트 출력 용, 매개변수 wstring 아님에 주의
	virtual HRESULT Render_Text(const string& _text, FONTALIGN _align, _float _posX, _float _posY, _float vSizeMul = 1.f);
	// 숫자 출력 용
	virtual HRESULT Render_Text(const _int _val, FONTALIGN _align, _float _posX, _float _posY, _float vSizeMul = 1.f);


public:
	static CFont_BigOrange* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END