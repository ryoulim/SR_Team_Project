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
	enum STATE { ENTRANCE, SHOTREADY, SHOTHEADBULLET, SHOTTAILBULLET, LEAVE, NON };
	enum MUZZLEPOS { LSIDE, LMIDDLE, MIDDLE, RMIDDLE, RSIDE, POSEND };

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

	void Set_StartState(STATE eState) {
		m_eState = eState;
	}

private:
	HRESULT Ready_Components(void* pArg);
	void Action(_float fTimeDelta);
	HRESULT Fire_Bullet(CRaceBossBullet::RBULLETTYPE eType, MUZZLEPOS ePos);
	_float3 Calc_Muzzle_Position(MUZZLEPOS eMuzzle);

	void Init_Skull();
	void Update_Skull(_float fTimeDelta);
	// 이 위치에 스컬을 온 해라, off는 m_bSkullActive를 끄시오
	void Render_Skull(MUZZLEPOS eMuzzle);

private:
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer* m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CCollider* m_pCollider = { nullptr };
	
	CTransform* m_pPlayerTransformCom = { nullptr };
	const _float3* m_pPlayerpos = { nullptr };

	STATE		m_eState = { NON };
	LEVEL		m_eLevelID = { LEVEL_END };
	_bool		m_bDead = { false };
	_float3		m_vScale = {};
	_uint		m_iHp = {};
	_float		m_fTime = {};
	_uint		m_iBulletCount = {};
	_float3		m_vBulletDiretion = {};
	MUZZLEPOS	m_ePos = { POSEND };
	_float3		m_vLerpStartPos = { 0.f, 250.f, 0.f };
	_float3		m_vLerpEndPos = { 450.f, 250.f, 1200.f };

	///// 해골바가지
	class CSkull* m_pSkull = { nullptr };
	_float3 m_vSkullPos{};
	MUZZLEPOS	m_eSkullMuzzlePos{ POSEND };
	_bool m_bSkullActive{};

public:
	static CRaceBoss* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
