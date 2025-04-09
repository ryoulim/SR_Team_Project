#pragma once
#include "Client_Defines.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "FXMgr.h"
#include <iostream>
#include <chrono>
#include <random>

BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
class CTransform;
class CShader;
END

BEGIN(Client)

using namespace std;
using namespace std::chrono;

class CMonster abstract : public CGameObject
{
public:
	enum MODE { MODE_IDLE, MODE_READY, MODE_BATTLE, MODE_DETECTIVE, MODE_RETURN, MODE_DEAD, MODE_END };
	enum EIdlePhase { IDLE_MOVE, IDLE_WAIT, IDLE_TURN };


public:
	typedef struct tagMonsterDesc : public CTransform::DESC
	{
		LEVEL		eLevel;
		_float3		vPosition;
		_float3		vReturnPos;
		_int		iNums = 0;
		bool		vActive = false;
		_float		fDetectiveDistance;
		_float		fAttackDistance;
	}DESC;

protected:
	CMonster(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMonster(const CMonster& Prototype);
	virtual ~CMonster() = default;

protected:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
public: //상태변환	
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	void		Render_Skull(_bool bOn);

protected:
	virtual HRESULT SetUp_RenderState();
	virtual HRESULT BillboardShaderRender();
	virtual HRESULT Render();
	virtual HRESULT Release_RenderState();

	void FrameUpdate(_float timeDelta, _float _MaxFrame, _float fSpeed, _bool isLoop);

protected:
	virtual HRESULT Ready_Components(void* pArg);


public: // 겟셋을 남용하는 행위
	_int* Get_HP() { return &m_iHP; }
	_int Get_MaxHP() { return m_iMaxHP; }

protected: // 길찾기 및 디텍티브
	virtual void PlayerDistance();
	virtual void CalculateVectorToPlayer();
	virtual bool IsPlayerDetected();
	virtual void Render_DebugFOV();
	const char* GetMonsterStateName(CMonster::MODE eState);
	virtual _bool Raycast_Player();

protected: //상태변환
	virtual void MonsterTick(_float fTimeDelta);

protected: //액션
	virtual void DoIdle(_float dt);
	virtual void DoBattle(_float dt);
	virtual void DoReturn(_float dt);
	virtual void DoReady(_float dt);
	virtual void DoDead(_float dt);
	virtual void SetRandomDirection();

protected: //몬스터 기본패턴
	virtual void AttackPattern(_float dt);
	virtual void ChasePlayer(_float dt);

protected: // 충돌함수
	virtual _float3		CalculateEffectPos();
	virtual void		On_Collision(_uint MyColliderID, _uint OtherColliderID) override;

public: // 디버깅
	void	ToggleDebugMode() { m_bDebug = !m_bDebug; }

protected: // 난수 생성
	float GetRandomFloat(float min, float max)
	{
		if (min > max) std::swap(min, max);
		uniform_real_distribution<float> dist(min, max);
		return dist(m_Gen);
	}
	int GetRandomInt(int min, int max)
	{
		if (min > max) std::swap(min, max);
		std::uniform_int_distribution<int> dist(min, max); // min 이상 max 이하 정수
		return dist(m_Gen);
	}

protected:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free();

#pragma region 애니메이션


	/************************/
protected: // 이하 애니메이션 용도 
	virtual HRESULT Ready_Textures() { return S_OK; }	// PURE | 텍스쳐 컴포넌트들 로딩, *부모에 일반몹 용도로 작성하고 보스만 오버라이딩 하기?
	virtual HRESULT Set_Animation() { return S_OK; }	// PURE | 각 몬스터 별로 상태에 따라 애니메이션 맥스 프레임 설정 필요 
	virtual HRESULT Animate_Monster(_float fTimeDelta);	// PURE | 애니메이션 설정 (위의 함수랑 겹치나??)
	HRESULT Set_TextureType();					// enum을 uint변수로 옮기는 작업(그냥 함수로 뺌)
	void	Compute_ViewAngle();						// 몬스터-플레이어 간 시선 각도 차 계산
	void	Resize_Texture(_float fSizePercent);

protected:
	_uint	m_iState = {};					// 애니메이션 종류 선택(공격, 이동 ...)
	_uint	m_iDegree = {};					// 애니메이션 시선 각 선택
	_float	m_fPlayersViewAngle = {};		// 시선 각
	_bool	m_bCW = { true };					// Clockwise?
	_float	m_fDivOffset = { 45.f };			// 몇 도로 쪼개져 있는 이미지인지 (보스: 22.5도, 일반(기본): 45도) 
	_bool	m_isReadyMonster = { false };	// 텍스쳐 준비 할 때 까지 기본 렌더링으로 돌리려고 
	_float	m_fDeadBodyCounter = {};		// 시체 남아있는 시간 

	typedef _uint STATE;					// 텍스쳐 컴포넌트 (해치지않아요)
	typedef _uint VIEWDEGREE;
	typedef unordered_map<VIEWDEGREE, CTexture*> TEXTURE_DEGREE;
	unordered_map<STATE, TEXTURE_DEGREE> m_pTextureMap;		// 1Key: 종류 , 2key: 각도, value: CTexture
	/************************/									// 이후 m_pTextureMap[m_iState][m_iDegree]->Bind_Resource호출 시
																// 매개변수로 m_fAnimationFrame 전달

#pragma endregion


protected: //기본정보
	LEVEL				m_eLevelID = { LEVEL_END };
	const _tchar* m_szTextureID = { nullptr };  // 직접 입력할지도..
	const _tchar* m_szBufferType = { nullptr };

protected: //컴포넌트
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer* m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CGravity* m_pGravityCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };
	CSoundController* m_pSoundCom{ nullptr };

protected: //충돌	
	CCollider* m_pCollider = { nullptr };
	CCollider* m_pHeadCollider = { nullptr };

protected: //속성
	_int	m_iNum = 0;
	_int	m_iHP = 100;
	_int	m_iMaxHP = 100;
	_int	m_iAttackPower = 10;
	_int	m_iDefense = 5;
	_float	m_fSpeed = 60.f;
	_float3	m_vScale = { 0.f, 0.f, 0.f };
	_float3 m_vPosition = { 0.f, 0.f, 0.f };
	MODE	m_eState = MODE::MODE_END;

	EIdlePhase	m_eIdlePhase = EIdlePhase::IDLE_WAIT;

protected: //디버깅
	steady_clock::time_point g_LastLogTime = steady_clock::now();

protected: //부속성
	vector<string>	m_vDropItems = {};
	string			m_strDialogue = "Grrrr...";
	string			m_strSound = "SoundFilePath";

protected: //플레이어
	CGameObject* m_pTargetPlayer = nullptr;

protected: //디텍티브
	_float3			m_vDirection = { 0.f, 0.f, -1.f };
	_float3			m_vToPlayer = { 0.f, 0.f, 0.f };
	_float			m_fCurDistance = 0;
	_float			m_fDetectiveDistance = 0;
	_float			m_fIdleTime = 0;
	_float			m_fMaxIdleTime = 0;
	_float			m_fAttackDistance = 0;


protected: // 랜덤 난수 생성변수
	random_device m_Rd;
	mt19937 m_Gen{ std::random_device{}() };
	uniform_real_distribution<float> m_Dist;

protected: // 애니메이션
	_float	m_fAnimationFrame = 0;
	_float	m_fAnimationMaxFrame = 0;
	_float	m_fAnimationSpeed = 0;

protected:
	// 배회 이동 관련
	_float  m_fShaderTime = 0.f;
	_float  m_fWanderTime = 0.f;
	_float  m_fWanderElapsed = 0.f;
	_float3 m_vReturnPos = { 0.f, 0.f, 0.f };

	// 멈춤 관련
	_float  m_fIdleWaitTime = 3.0f;
	_float  m_fIdleWaitElapsed = 0.f;
	_bool	m_bRotateAnimation = true;

	// 공격 쿨다운
	_bool	m_bCoolingDown = false;
	_float	m_fCooldownTime = 0.f;
	_float	m_fCooldownDuration = 0.f;
	_float	m_fAttackTimer = 0.f;
	_int	m_iRandom = 0;


protected: // 작동변수
	_bool			m_bDead = false;
	_bool			m_bActive = false;

protected: // 디버깅
	_bool			m_bDebug = false;

protected: // 일반 몬스터 행동 용 변수
	_bool			m_bFoundPlayer = { false };
	_bool			m_isReadyToAttack = { false };
	_float			m_fRaycastTicker = {};
	_float			m_fBulletCooldownElapsed = {};
	_float			m_fBulletCooldown = {};
	_float			m_fAttackTime = {};
	_bool			m_bKnockBack = false;

protected: // 몬스터 해골 띄우기용
	_bool		m_bSkullActive{};
	class CSkull* m_pSkull{ nullptr };
	_float3		m_vSkullOffset{};

protected: // 일반 몬스터용 충돌 함수
	void	On_Collision_NormalMonster(_uint MyColliderID, _uint OtherColliderID);
	void	Collision_With_Weapon();
	void	Collision_With_Block();

protected: // 일반 몬스터용 함수들
	virtual _bool IsMonsterAbleToAttack();
	virtual void Debug_Output();
	virtual void State_Change_IDLE(_float dt);
	virtual void State_Change_DETECTIVE(_float dt);
	virtual void State_Change_READY(_float dt);
	virtual void State_Change_BATTLE(_float dt);

protected:
	_uint	m_iHeadMultiplier = 1;	// 헤드샷 여부
};

END
