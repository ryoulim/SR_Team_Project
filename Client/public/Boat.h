// 내 클래스 이름 : Boat
// 부모 클래스 이름 : Monster

#pragma once
#include "Monster.h"

BEGIN(Client)

class CBoat final : public CMonster
{
public:
	typedef struct tagBoatDesc : public CMonster::DESC
	{

	}DESC;
	enum BOAT_DEGREE { D0, D22, D45, D67, D90, D112, D135, D157, D180, D_END };
private:
	CBoat(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBoat(const CBoat& Prototype);
	virtual ~CBoat() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	virtual HRESULT Ready_Components(void* pArg)override;
	virtual HRESULT Ready_Textures()override;

public:
	static CBoat* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END
