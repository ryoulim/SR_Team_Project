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
	enum STATE { WAITFORPLAYER, ENTRANCE,
		READYBOMB, DRAWINGRADIUS, BOMBING, COMEBACK, 
		SHOTREADY, SHOTHEADBULLET, SHOTTAILBULLET, 
		LEAVE, NON };

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

public:
	_float3		GetVelocityPerSecond(_float fTimeDelta) const {
		const _float3& vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		return (vPos - m_vPrePos) / fTimeDelta;
	}
private:
	HRESULT Ready_Components(void* pArg);
	void ReadyForState();
	void Action(_float fTimeDelta);
	HRESULT Fire_Bullet(CRaceBossBullet::RBULLETTYPE eType, MUZZLEPOS ePos, _float fTimeDelta);
	_float3 Calc_Muzzle_Position(MUZZLEPOS eMuzzle);
	void ShuffleandPop();
	_float3 CatmulRomPos(_float3& v0, _float3& vStartPos, _float3& vEndPos, _float3& v3, _float fTimeAcc);

	//플레이어를 맞추려면 어디로 쏴야하는지 플레이어 Z 이동속도에 따라 예측해주는 놈 
	_float3 CalcBulletLook(const _float3& vBulletStartPos, _float fBulletSpeed, _float fTimeDelta);

	void Init_Skull();
	void Update_Skull(_float fTimeDelta);
	_bool Judge_Skull(const _float3& vColliderPos, _float vColliderRadius, _float fTimedelta);
	void Render_Skull(MUZZLEPOS eMuzzlePos);

private:
	friend class CRBState_ReadyShot;
	friend class CRBState_ShotHeadBullet;
	friend class CRBState_ShotTailBullet;
	friend class CRBState_ReadyBombing;
	friend class CRBState_DrawingRadius;
	friend class CRBState_Bombing;
	friend class CRBState_Comeback;

	STATE					m_ePreState = { NON };
	STATE					m_eCurState = { ENTRANCE };
	class CRBState* m_pCurState = { nullptr };
	class CRBState* m_pState[NON] = { nullptr };

private:
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer* m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };

	vector<CCollider*> m_ColliderComs = { nullptr };
	
	class CPlayerOnBoat* m_pPlayer = { nullptr };
	const _float3* m_pPlayerpos = { nullptr };

	_float3		m_vPrePos{}; // 이전 프레임의 포지션

	STATE		m_eState = { NON };
	LEVEL		m_eLevelID = { LEVEL_END };
	_bool		m_bDead = { false };
	_float3		m_vScale = {};
	_uint		m_iHp = {};
	_float		m_fTime = {};
	_uint		m_iTailBulletCount = {};
	_uint		m_iHeadBulletCount = {};
	_float3		m_vBulletDiretion = {};
	MUZZLEPOS	m_ePos = { POSEND };
	_float3		m_vLerpStartPos = { 0.f, 250.f, 0.f };
	_float3		m_vLerpEndPos = { 450.f, 250.f, 1200.f };
	vector<MUZZLEPOS> m_VecBulletPos;

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
