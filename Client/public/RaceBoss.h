#pragma once

#include "Client_Defines.h"
#include "Pawn.h"
#include "GameInstance.h"
#include "RaceBossBullet.h"
#include "RaceBossBomb.h"
#include "BombRadius.h"
#include "MombackLine.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer;
class CTransform;
class CCollider;
class CSoundController;
END

BEGIN(Client)

class CRaceBoss final : public CGameObject
{
public:
	enum STATE { WAITFORPLAYER, ENTRANCE, IDLE,
		SHOTREADY, SHOTHEADBULLET, SHOTTAILBULLET,
		READYBOMB, DRAWINGBOMBRADIUS, BOMBING, COMEBACK,
		BOMBATTACK,CROSSATTACK,
		MOMBACK,MOMBACKREADY,MOMBACKREVERSE,
		CLOSE_TO_PLAYER, 
		LEAVE, DEAD, NON };

	// �ݶ��̴� ������ ���̵�� ������ �ΰڽ��ϴ�, �����ı� ���������� ���� ���� �����ϰڽ��ϴ�.
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
	void Add_Collider();
	
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
		return m_eCurState == DEAD;
	}
private:

	// ������Ʈ ĳ���ͷ� �̾��ִ� ����׿�
	const char* Debug_State(STATE eState);

	HRESULT Ready_Components(void* pArg);
	void ReadyForState();

	HRESULT Fire_Bullet(CRaceBossBullet::RBULLETTYPE eType, MUZZLEPOS ePos, _float fTimeDelta);
	void RestoreTextureID();
	
private:
	/* [ ���ݷ��� �������� ] */
	HRESULT SpawnTargetAim(_float3 _vAimPosition);
	HRESULT SpawnTargetLine(_float3 _vAimPosition);
	HRESULT SpawnTargetLineReverse(_float3 _vLinePosition);
	HRESULT SpawnMultipleTargetAim(_float _fTimedelta);
	void SpawnWaterParticle(_float fWaterSpeed, _float _fMin, _float _fMax);
	void SpawnDieParticle(_float fWaterSpeed);

	void SpawnHitBox(const _float3& _Position, const _float3& _Scale, const TCHAR* _szTextureTag, _float MaxTime, _bool _HitDead);

	//HRESULT Set_BombRadius();
	//_bool Fire_Bomb4(_uint iBombIndex, _float fTime);
	_float3 Calc_Muzzle_Position(MUZZLEPOS eMuzzle);
	void ShuffleandPop();
	_float3 CatmulRomPos(_float3& v0, _float3& vStartPos, _float3& vEndPos, _float3& v3, _float fTimeAcc);

	//�÷��̾ ���߷��� ���� �����ϴ��� �÷��̾� Z �̵��ӵ��� ���� �������ִ� �� 
	_float3 CalcBulletLook(const _float3& vBulletStartPos, _float fBulletSpeed, _float fTimeDelta);

	void Init_Skull();
	void Update_Skull(_float fTimeDelta);
	_bool Judge_Skull(const _float3& vColliderPos, _float vColliderRadius, _float fTimedelta);
	void Render_Skull(MUZZLEPOS eMuzzlePos);

private:
	friend class CRBState_WaitPlayer;
	friend class CRBState_Entrance;
	friend class CRBState_IDLE;
	friend class CRBState_ReadyShot;
	friend class CRBState_ShotHeadBullet;
	friend class CRBState_ShotTailBullet;
	friend class CRBState_ReadyBombing;
	friend class CRBState_BombAttack;
	friend class CRBState_CrossAttack;
	friend class CRBState_Comeback;
	friend class CRBState_Leave;
	friend class CRBState_Dead;
	friend class CRBState_MombackReady;
	friend class CRBState_Momback;
	friend class CRBState_MombackReverse;
	friend class CRBState_CloseToPlayer;

	STATE					m_ePreState = { NON };
	STATE					m_eCurState = { ENTRANCE };
	class CRBState* m_pCurState = { nullptr };
	class CRBState* m_pState[NON] = { nullptr };

	void Set_State(STATE eState);
	void Go_Straight(_float fTimeDelta);
	void Go_Backward(_float fTimeDelta);
	void Go_Up(_float fTimeDelta);
	void Go_Right(_float fTimeDelta);
	void Play_Sound(const string& strTag);
	void Stop_Sound(const string& strTag);
	void Set_MombackReady_Sound_False();
	void Set_MombackReady_Sound_True();
	_bool Get_MombackReady_Sound_State();

	_float Compute_PosZ();
	_float Compute_PozY();
	void MoveCatMullRom(_float3& v0, _float3& vStartPos, _float3& vEndPos, _float3& v3, _float fTimeAcc);
	void Fire_HeadBullet(_float fTimeDelta);
	void Fire_TailBullet(_float fTimeDelta);
	_uint Get_HeadBulletCount();
	void Set_HeadBulletCountZero();
	void Update_Collider_OffSet(_float ZRot);
	
	
private:
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer* m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CSoundController* m_pSoundCom = { nullptr };

private:
	CGameObject* m_pWaterBoatEffect_01 = nullptr;
	CGameObject* m_pWaterBoatEffect_02 = nullptr;
	CGameObject* m_pWaterBoatEffect_03 = nullptr;

	CGameObject* m_pBossHitBox = nullptr;

	vector<CCollider*> m_ColliderComs = { nullptr };
	
	class CPlayerOnBoat* m_pPlayer = { nullptr };
	const _float3* m_pPlayerpos = { nullptr };

	_float3		m_vPrePos{}; // ���� �������� ������
	LEVEL		m_eLevelID = { LEVEL_END };
	_bool		m_bDead = { false };
	_bool		m_bPartDead[5]{};
	_bool		m_bChangeLevel{};
	_bool		m_bMombackReadySoundPlay = { true };
	_float3		m_vScale = {};
	_float		m_fBombTime = {};
	_float		m_fDieTime = {};
	_uint		m_iHeadBulletCount = {};
	_float3		m_vBulletDiretion = {};
	MUZZLEPOS	m_ePos = { POSEND };
	vector<MUZZLEPOS> m_VecBulletPos;
	vector<_float> m_vecRazerPos;


private:
	_float3 m_vSavedRight, m_vSavedUp, m_vSavedLook;

	///// �ذ�ٰ���
	class CSkull* m_pSkull = { nullptr };
	_float3 m_vSkullPos{};
	MUZZLEPOS	m_eSkullMuzzlePos{ POSEND };
	_bool m_bSkullActive{};


private: // ���� �ǰݰ� �����ı� ����
	_int		m_iHp = {250}; // ��ü��
	_int		m_iMuzzleHp[5]{ 50 ,50 ,50 ,50 ,50 }; // ������ ü���ε� �������� ����
	_uint		m_iTextureID[5]{6,6,6,6,9};
		
	void On_Hit(MUZZLEPOS HitPos, _int iDamage);

public:
	_int* Get_HP() { return &m_iHp; }

public:
	static CRaceBoss* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
