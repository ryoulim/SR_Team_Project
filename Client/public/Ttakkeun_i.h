// 내 클래스 이름 : TestMonster
// 부모 클래스 이름 : Monster

#pragma once
#include "Monster.h"

BEGIN(Client)

class CTtakkeun_i final : public CMonster
{
public:
	typedef struct tagTestMonsterDesc : public CMonster::DESC
	{
		//몬스터 고유 특성
	}DESC;

	enum MONSTER_STATE { STATE_WALK, STATE_FLY, STATE_FLY_ATTACK, STATE_JUMP, STATE_MISSILE, STATE_BOMB, STATE_STAY, STATE_END };
	enum STATE_MAXFRAME { MAX_WALK = 12, MAX_FLY = 1, MAX_FLY_ATTACK = 2, MAX_GROUNDATK = 2, MAX_JUMP = 5 };
	enum FLY_DIR { DOWN = 0, LEFT = 1, RIGHT = 2, UP = 3, DIR_END = 4};
	enum BOSS_DEGREE { D0, D22, D45, D67, D90, D112, D135, D157, D180, D_END };


private:
	CTtakkeun_i(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTtakkeun_i(const CTtakkeun_i& Prototype);
	virtual ~CTtakkeun_i() = default;

private:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	virtual HRESULT Ready_Components(void* pArg);
	virtual HRESULT Ready_Textures();
	virtual HRESULT Set_Animation();
	virtual HRESULT Animate_Monster(_float fTimeDelta);


private: // 히트박스
	void SpawnJumpHitBox(const _float3& _Position, const TCHAR* _szTextureTag, _float MaxTime, _bool _HitDead);
	void SpawnFireHitBox(const _float3& _Position, const TCHAR* _szTextureTag, _float MaxTime, _bool _HitDead);


private: //해당객체의 몬스터 패턴
	virtual void DoIdle(_float dt);
	virtual void DoBattle(_float dt);
	virtual void AttackPattern(_float dt);

private: //따끈이 패턴
	void BasicAttackSet(_float dt);
	void MissileAttack(_float dt);
	void SpawnAttack(_float dt);
	void FireAttack(_float dt);
	void BounceBall(_float dt);
	void JumpAttack(_float dt);
	void FlyAttack(_float dt);

private: //서포팅 함수들
	void SpawnBounce();
	void SpawnDeacon(_float3 vPos);
	void SpawnDeaconEffect(_float3 vPos);
	void SpawnMissile(_float dt);
	void SpawnGuidMissile();
	void StartCooldown(_float dt, _float fMin, _float fMax);
	_float3 Rotate_Y(_float3 vDir, float rad);
	_float3 Rotate_Z(_float3 vDir, float rad);

public: // 컷씬시작
	void SetCutScene() { m_bCutScene = true; }
	void CutSceneAction(_float dt);
	_bool GetbRender() { return m_bRender; }

private: //따끈이 전용 이펙트
	void FlyEffect();

private: //행동 오버라이딩
	void DoReturn(_float dt);



private:
	MONSTER_STATE	m_eCurMonsterState = { MONSTER_STATE::STATE_WALK };
	MONSTER_STATE	m_ePrevMonsterState = { MONSTER_STATE::STATE_WALK };
	FLY_DIR			m_eCurFlyingDirection = { UP };
	FLY_DIR			m_ePrevFlyingDirection = { UP };

private: //단발성 변수
	_bool			m_bDoOnce = false;
	_bool			m_bDoPatternLeft = false;
	_bool			m_bDoPatternRight = false;

private: //공중패턴
	_bool			m_bFlyAttack = false;
	_float			m_fFlyAttack = 0.f;
	_float			m_fSpawnMissile = 0.f;
	_int			m_iMissileCount = 0;

private: //이펙트
	CGameObject*	m_pBossEffect = nullptr;
	CGameObject*	m_pBossHitBox = nullptr;

private: //카메라
	class CCameraManager* m_pCamera = nullptr;

private: //점프패턴
	_bool			m_bJumpStart = false;
	_bool			m_bJumpEnd = false;
	_bool			m_bIsFly = false;
	_bool			m_bIsLink = false;
	_float			m_fFallSpeed = 0.f;
	_int			m_iJumpCount = 0;

private: //바운스볼 패턴
	_float			m_fBounceTime = 0.f;
	_int			m_iBounceCount = 0;

private: // 컷씬시작
	_bool			m_bRender = false;
	_bool			m_bCutScene = false;
	_bool			m_bCutSceneEnd = false;
	_float			m_bCutSceneTime = 0.f;

private: //데드 변수
	_float			m_fCallTimer = 0.f;
	_float			m_fTotalTime = 0.f;
	_bool			m_bDie = false;

private:
	virtual void On_Collision(_uint MyColliderID, _uint OtherColliderID) override;

public:
	static CTtakkeun_i* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END