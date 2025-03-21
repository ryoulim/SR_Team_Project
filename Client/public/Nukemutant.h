// �� Ŭ���� �̸� : TestMonster
// �θ� Ŭ���� �̸� : Monster

#pragma once
#include "Monster.h"

BEGIN(Client)

class CNukemutant final : public CMonster
{
public:
	typedef struct tagTestMonsterDesc : public CMonster::DESC
	{
		//���� ���� Ư��
	}DESC;

private:
	CNukemutant(LPDIRECT3DDEVICE9 pGraphic_Device);
	CNukemutant(const CNukemutant& Prototype);
	virtual ~CNukemutant() = default;

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
	static CNukemutant* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END