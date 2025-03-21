// �� Ŭ���� �̸� : TestMonster
// �θ� Ŭ���� �̸� : Monster

#pragma once
#include "Monster.h"

BEGIN(Client)

class CCultist final : public CMonster
{
public:
	typedef struct tagTestMonsterDesc : public CMonster::DESC
	{
		//���� ���� Ư��
	}DESC;

private:
	CCultist(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCultist(const CCultist& Prototype);
	virtual ~CCultist() = default;

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
	virtual void On_Collision(CGameObject* pCollisionedObject, const _wstring& strLayerTag);

public:
	static CCultist* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END