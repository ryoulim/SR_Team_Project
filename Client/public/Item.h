#pragma once

#include "Client_Defines.h"
#include "GameInstance.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
class CTransform;
class CCollider;
END

BEGIN(Client)

class CItem : public CGameObject
{
public:
	typedef struct tagItemDesc : public CTransform::DESC
	{
		const _tchar* szTextureID;
		_float3 vInitPos;
		_float3 vScale;
		_float	fTextureNum;
		COLLIDER_ID eColID;
		LEVEL	eLevelID;
	}DESC;

protected:
	CItem(LPDIRECT3DDEVICE9 pGraphic_Device);
	CItem(const CItem& Prototype);
	virtual ~CItem() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void On_Collision(_uint MyColliderID, _uint OtherColliderID) override;

protected:
	virtual HRESULT Ready_Components(void* pArg);

protected:
	LEVEL m_eLevelID = { LEVEL_END };
	const _tchar* m_szTextureID = { nullptr };
	const _tchar* m_szBufferType = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer* m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CCollider* m_pCollider = { nullptr };
	bool m_bDead = { false };
	COLLIDER_ID m_eColID = { COLLIDER_ID::CI_END };
	_float m_fStdHeight = {};

public:
	static CItem* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();
};

END
