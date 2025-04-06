#pragma once

#include "Client_Defines.h"
#include "Pawn.h"
#include "GameInstance.h"
#include "RaceBossBullet.h"

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
	enum STATE { ENTRANCE, IDLE, SHOTREADY, SHOTHEADBULLET, SHOTTAILBULLET, LEAVE, NON };

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
	void Action(_float fTimeDelta);
	HRESULT Fire_Bullet(CRaceBossBullet::RBULLETTYPE eType);

private:
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer* m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CCollider* m_pCollider = { nullptr };
	
	STATE		m_eState = { NON };
	LEVEL		m_eLevelID = { LEVEL_END };
	_bool		m_bDead = { false };
	_float3		m_vScale = {};
	_uint		m_iHp = {};
	_float		m_fTime = {};
	_uint		m_iBulletCount = {};
	_float3		m_vBulletDiretion = {};

public:
	static CRaceBoss* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
