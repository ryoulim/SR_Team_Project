// 내 클래스 이름 : TestMonster
// 부모 클래스 이름 : Monster

#pragma once
#include "Monster.h"

BEGIN(Client)

class CTestMonster final : public CMonster
{
public:
	typedef struct tagTestMonsterDesc : public CMonster::DESC
	{

	}DESC;

private:
	CTestMonster(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTestMonster(const CTestMonster& Prototype);
	virtual ~CTestMonster() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	virtual HRESULT Ready_Components(void* pArg);

public:
	static CTestMonster* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END