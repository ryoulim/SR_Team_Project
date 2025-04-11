#pragma once

#include "Client_Defines.h"
#include "Pawn.h"
#include "GameInstance.h"
#include "RaceBossBullet.h"
#include "RaceBossBomb.h"
#include "BombRadius.h"

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
	enum STATE { WAITFORPLAYER, ENTRANCE, IDLE,
		SHOTREADY, SHOTHEADBULLET, SHOTTAILBULLET,
		READYBOMB, DRAWINGRADIUS, BOMBING, COMEBACK, 
		LEAVE, NON };

	// 콜라이더 떄문에 아이디랑 통일해 두겠습니다, 부위파괴 로직때문에 순서 좀만 조정하겠습니다.
	enum MUZZLEPOS { LSIDE = CI_RACEBOSS_MUZZLE1, LMIDDLE, RMIDDLE, RSIDE, MIDDLE, POSEND = 5 };

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

	void Set_StartState(STATE eState);

	_float GetRandomFloat(float lowBound, float highBound);

	virtual void On_Collision(_uint MyColliderID, _uint OtherColliderID) override;

public:
	_float3		GetVelocityPerSecond(_float fTimeDelta) const {
		const _float3& vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		return (vPos - m_vPrePos) / fTimeDelta;
	}
	_bool		IsDead() { 
		return m_bDead; 
	}
private:
	HRESULT Ready_Components(void* pArg);
	void ReadyForState();
	HRESULT Fire_Bullet(CRaceBossBullet::RBULLETTYPE eType, MUZZLEPOS ePos, _float fTimeDelta);
	HRESULT Fire_Bomb(_float fTimeDelta);
	HRESULT Fire_Bomb2();
	HRESULT Fire_Bomb3();

	

	HRESULT Set_BombRadius();
	_bool Fire_Bomb4(_uint iBombIndex, _float fTime);
	_float3 Calc_Muzzle_Position(MUZZLEPOS eMuzzle);
	void ShuffleandPop();
	_float3 CatmulRomPos(_float3& v0, _float3& vStartPos, _float3& vEndPos, _float3& v3, _float fTimeAcc);

	//플레이어를 맞추려면 어디로 쏴야하는지 플레이어 Z 이동속도에 따라 예측해주는 놈 
	_float3 CalcBulletLook(const _float3& vBulletStartPos, _float fBulletSpeed, _float fTimeDelta);

	void Init_Skull();
	void Update_Skull(_float fTimeDelta);
	_bool Judge_Skull(const _float3& vColliderPos, _float vColliderRadius, _float fTimedelta);
	void Render_Skull(MUZZLEPOS eMuzzlePos);
	HRESULT Draw_BombRadius(_float3 vBombingPos);

private:
	friend class CRBState_WaitPlayer;
	friend class CRBState_Entrance;
	friend class CRBState_IDLE;
	friend class CRBState_ReadyShot;
	friend class CRBState_ShotHeadBullet;
	friend class CRBState_ShotTailBullet;
	friend class CRBState_ReadyBombing;
	friend class CRBState_DrawingRadius;
	friend class CRBState_Bombing;
	friend class CRBState_Comeback;
	friend class CRBState_Leave;

	STATE					m_eState = { NON };
	STATE					m_ePreState = { NON };
	STATE					m_eCurState = { ENTRANCE };
	class CRBState* m_pCurState = { nullptr };
	class CRBState* m_pState[NON] = { nullptr };

	void Set_State(STATE eState);
	void Go_Straight(_float fTimeDelta);
	void Go_Backward(_float fTimeDelta);
	void Go_Up(_float fTimeDelta);
	void Go_Right(_float fTimeDelta);
	_float Compute_PosZ();
	_float Compute_PozY();
	void MoveCatMullRom(_float3& v0, _float3& vStartPos, _float3& vEndPos, _float3& v3, _float fTimeAcc);
	void Fire_HeadBullet(_float fTimeDelta);
	void Fire_TailBullet(_float fTimeDelta);
	_uint Get_HeadBulletCount();
	void Set_HeadBulletCountZero();
	//void SelectAndDrawRadius();
	void Bombing(_float fTimeDelta);
	_bool Comeback(_float fTimeDelta);
	bool m_bDown = { false };
	
private:
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer* m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };

	vector<CCollider*> m_ColliderComs = { nullptr };
	
	class CPlayerOnBoat* m_pPlayer = { nullptr };
	const _float3* m_pPlayerpos = { nullptr };

	_float3		m_vPrePos{}; // 이전 프레임의 포지션
	LEVEL		m_eLevelID = { LEVEL_END };
	_bool		m_bDead = { false };
	_bool		m_bPartDead[5]{};
	_float3		m_vScale = {};
	_float		m_fTime = {};
	_uint		m_iHeadBulletCount = {};
	_float3		m_vBulletDiretion = {};
	MUZZLEPOS	m_ePos = { POSEND };
	vector<MUZZLEPOS> m_VecBulletPos;

	typedef struct tagBombPosdata
	{
		_float fPosX;
		_float fPosZ;
	}BOMBDATA;

	vector<BOMBDATA> m_vecBombPos;

	_float		m_fBombPosX = {};
	_float		m_fBombPosX2[2] = {};
	_float		m_fBombPosZ	= {};
	_bool		m_bFireBomb[4] = { false };


	///// 해골바가지
	class CSkull* m_pSkull = { nullptr };
	_float3 m_vSkullPos{};
	MUZZLEPOS	m_eSkullMuzzlePos{ POSEND };
	_bool m_bSkullActive{};


private: // 보스 피격과 부위파괴 관련
	_int		m_iHp = {400}; // 총체력
	_int		m_iMuzzleHp[5]{ 80 ,80 ,80 ,80 ,80 }; // 부위별 체력인데 마지막이 센터
	_uint		m_iTextureID[5]{1,1,1,1,2};
		
	void On_Hit(MUZZLEPOS HitPos, _int iDamage);

public:
	static CRaceBoss* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
