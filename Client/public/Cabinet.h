#pragma once
// 내 클래스 이름 : Cabinet
// 부모 클래스 이름 : Statue

#pragma once
#include "Statue.h"

BEGIN(Client)

class CCabinet final : public CStatue
{
public:
	typedef struct tagCabinetDesc : public CStatue::DESC
	{

	}DESC;

private:
	CCabinet(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCabinet(const CCabinet& Prototype);
	virtual ~CCabinet() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	HRESULT Ready_Components(void* pArg);

public:
	static CCabinet* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END
