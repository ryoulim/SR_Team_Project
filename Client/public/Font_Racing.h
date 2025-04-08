// 내 클래스 이름 : Font_Racing
// 부모 클래스 이름 : Font

#pragma once
#include "Font.h"

BEGIN(Client)

class CFont_Racing final : public CFont
{
private:
	CFont_Racing(LPDIRECT3DDEVICE9 pGraphic_Device);
	CFont_Racing(const CFont_Racing& Prototype);
	virtual ~CFont_Racing() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual HRESULT imsi_Render_Text(const string& _Text, _float3 vPos);
	HRESULT Render_Number(const _uint _Val);

public:
	static CFont_Racing* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END