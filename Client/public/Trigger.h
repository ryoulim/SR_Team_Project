// 내 클래스 이름 : Trigger
// 부모 클래스 이름 : GameObject

#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "GameInstance.h"

BEGIN(Engine)
class CVIBuffer;
class CTransform;
class CCollider;
END


BEGIN(Client)

class CTrigger final : public CGameObject
{
public:
	typedef struct tagTriggerDesc : public CTransform::DESC
	{
		_float3 vInitPos;
		_float3 vScale;
		_float3 vAngle;
		_wstring LayerTag;

	}DESC;

private:
	CTrigger(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTrigger(const CTrigger& Prototype);
	virtual ~CTrigger() = default;

public:
	virtual HRESULT Initialize_Prototype() override;	
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:


private:
	CTransform* m_pTransformCom = { nullptr };
	CCollider* m_pColliderCom = { nullptr };

	_bool m_bDead = { false };

private:
	list<CGameObject*> m_pTargets;

private:
	virtual HRESULT Ready_Components(void* pArg);
	virtual void On_Collision(_uint MyColliderID, _uint OtherColliderID)override;

public:
	static CTrigger* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END