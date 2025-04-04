#pragma once

#include "Client_Defines.h"
#include "Pawn.h"
#include "GameInstance.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer;
class CTransform;
class CCollider;
END

BEGIN(Client)

class CRaceBoss final : public CGameObject
{
public:
	typedef struct tagRaceBossDesc : public CTransform::DESC
	{
		LEVEL	eLevelID;
	}DESC;

private:
	CRaceBoss(LPDIRECT3DDEVICE9 pGraphic_Device);
	CRaceBoss(const CRaceBoss& Prototype);
	virtual ~CRaceBoss() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	
	void Set_LevelID(LEVEL ID) {
		m_eLevelID = ID;
	}

private:
	HRESULT Ready_Components(void* pArg);
	void Attack();

private:
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer* m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CCollider* m_pCollider = { nullptr };
	
	LEVEL		m_eLevelID = { LEVEL_END };
	_bool		m_bDead = { false };
	_float3		m_vScale = {};
	_uint		m_iHp = {};
	_float		m_fTime = {};

public:
	static CRaceBoss* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
