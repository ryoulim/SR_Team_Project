// 내 클래스 이름 : TestMonster
// 부모 클래스 이름 : Monster

#pragma once
#include "Monster.h"

BEGIN(Client)

class CArchangel final : public CMonster
{
public:
	typedef struct tagTestMonsterDesc : public CMonster::DESC
	{
		//몬스터 고유 특성
	}DESC;

private:
	CArchangel(LPDIRECT3DDEVICE9 pGraphic_Device);
	CArchangel(const CArchangel& Prototype);
	virtual ~CArchangel() = default;

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
	static CArchangel* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END