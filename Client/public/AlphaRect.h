// 내 클래스 이름 : AlphaRect
// 부모 클래스 이름 : Map

#pragma once
#include "Map.h"

BEGIN(Client)

class CAlphaRect final : public CMap
{
public:
	typedef struct tagAlphaRectDesc : public CMap::DESC
	{

	}DESC;

private:
	CAlphaRect(LPDIRECT3DDEVICE9 pGraphic_Device);
	CAlphaRect(const CAlphaRect& Prototype);
	virtual ~CAlphaRect() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual HRESULT Ready_Components(void* pArg);

public:
	static CAlphaRect* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END