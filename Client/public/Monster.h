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
END

BEGIN(Client)

using namespace std;
using namespace std::chrono;

class CMonster abstract : public CGameObject
{
public:
	enum MODE{	MODE_IDLE, MODE_ATTACK, MODE_BATTLE, MODE_DETECTIVE, MODE_RETURN, MODE_END  };
	enum EIdlePhase { IDLE_MOVE, IDLE_WAIT, IDLE_TURN };


public:
	typedef struct tagMonsterDesc : public CTransform::DESC
	{
		_float3		vPosition;
		_float3		vReturnPos;
		bool		vActive = false;
	}DESC;

protected:
	CMonster(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMonster(const CMonster& Prototype);
	virtual ~CMonster() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;

public:
	virtual HRESULT SetUp_RenderState();
	virtual HRESULT Render();
	virtual HRESULT Release_RenderState();

protected:
	virtual HRESULT Ready_Components(void* pArg);

public: // 길찾기 및 디텍티브
	virtual void PlayerDistance();
	virtual void CalculateVectorToPlayer();
	virtual bool IsPlayerDetected();
	virtual void Render_DebugFOV();
	const char*	 GetMonsterStateName(CMonster::MODE eState);

public: //상태변환
	virtual void MonsterTick(_float fTimeDelta);

public: //액션
	virtual void DoIdle(_float dt);
	virtual void DoBattle(_float dt);
	virtual void DoReturn(_float dt);
	virtual void SetRandomDirection();

public: //몬스터 기본패턴
	virtual void AttackPattern(_float dt);
	virtual void ChasePlayer(_float dt);

public: // 충돌함수
	virtual _float3		CalculateEffectPos();
	virtual void		On_Collision(_uint MyColliderID, _uint OtherColliderID) override {};

public: // 디버깅
	void	ToggleDebugMode() { m_bDebug = !m_bDebug; }

public: // 난수 생성
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

public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free();

#pragma region 애니메이션


	/************************/
protected: // 이하 애니메이션 용도 
	virtual HRESULT Ready_Textures() { return S_OK; }	// PURE | 텍스쳐 컴포넌트들 로딩, *부모에 일반몹 용도로 작성하고 보스만 오버라이딩 하기?
	virtual HRESULT Set_TextureType();					// enum을 uint변수로 옮기는 작업(그냥 함수로 뺌)
	virtual HRESULT Animate_Monster(_float fTimeDelta);	// 애니메이션 설정 (위의 함수랑 겹치나??)
	void	Compute_ViewAngle();						// 몬스터-플레이어 간 시선 각도 차 계산 
	virtual HRESULT Set_Animation() { return S_OK; }	// PURE | 각 몬스터 별로 상태에 따라 애니메이션 맥스 프레임 설정 필요 

protected:
	_uint	m_iState = {};					// 애니메이션 종류 선택(공격, 이동 ...)
	_uint	m_iDegree = {};					// 애니메이션 시선 각 선택
	_float	m_fPlayersViewAngle = {};		// 시선 각
	_bool	m_bCW = {true};					// Clockwise?
	_float	m_fDivOffset = {45.f};			// 몇 도로 쪼개져 있는 이미지인지 (보스: 22.5도, 일반(기본): 45도) 
	_bool	m_isReadyMonster = { false };	// 텍스쳐 준비 할 때 까지 기본 렌더링으로 돌리려고 

	typedef _uint STATE;					// 텍스쳐 컴포넌트 (해치지않아요)
	typedef _uint VIEWDEGREE;
	typedef unordered_map<VIEWDEGREE, CTexture*> TEXTURE_DEGREE;
	unordered_map<STATE, TEXTURE_DEGREE> m_pTextureMap;		// 1Key: 종류 , 2key: 각도, value: CTexture
	/************************/									// 이후 m_pTextureMap[m_iState][m_iDegree]->Bind_Resource호출 시
																// 매개변수로 m_fAnimationFrame 전달

#pragma endregion
		

protected: //기본정보
	LEVEL				m_eLevelID = { LEVEL_END };
	const _tchar*		m_szTextureID = { nullptr };  // 직접 입력할지도..
	const _tchar*		m_szBufferType = { nullptr };

protected: //컴포넌트
	CTexture*	m_pTextureCom = { nullptr };
	CVIBuffer*	m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CGravity*	m_pGravityCom = { nullptr };

protected: //충돌	
	CCollider* m_pCollider = { nullptr };

protected: //속성
	_int	m_iHP			= 100;
	_int	m_iMaxHP		= 100;
	_int	m_iAttackPower	= 10;
	_int	m_iDefense		= 5;
	_float	m_fSpeed		= 60.f;
	_float3	m_vScale		= { 0.f, 0.f, 0.f };
	_float3 m_vPosition		= { 0.f, 0.f, 0.f };
	MODE	m_eState		= MODE::MODE_END;

	EIdlePhase	m_eIdlePhase = EIdlePhase::IDLE_WAIT;

protected: //디버깅
	steady_clock::time_point g_LastLogTime = steady_clock::now();

protected: //부속성
	vector<string>	m_vDropItems	= {};
	string			m_strDialogue	= "Grrrr...";
	string			m_strSound		= "SoundFilePath";

protected: //플레이어
	CGameObject*	m_pTargetPlayer	= nullptr;

protected: //디텍티브
	_float3			m_vDirection		= { 0.f, 0.f, -1.f };
	_float3			m_vToPlayer			= { 0.f, 0.f, 0.f };
	_float			m_fCurDistance		= 0;
	_float			m_fDetectiveDistance= 0;
	_float			m_fIdleTime			= 0;
	_float			m_fMaxIdleTime		= 0;


protected: // 랜덤 난수 생성변수
	random_device m_Rd;
	mt19937 m_Gen{ std::random_device{}() };
	uniform_real_distribution<float> m_Dist;


protected:
	// 배회 이동 관련
	_float  m_fWanderTime = 0.f;
	_float  m_fWanderElapsed = 0.f;
	_float3 m_vReturnPos = { 0.f, 0.f, 0.f };

	// 멈춤 관련
	_float  m_fIdleWaitTime = 3.0f;
	_float  m_fIdleWaitElapsed = 0.f;
	
	// 공격 쿨다운
	_bool	m_bCoolingDown = false;
	_float	m_fCooldownTime = 0.f;
	_float	m_fCooldownDuration = 0.f;
	_float	m_fAttackTimer = 0.f;
	_int	m_iRandom = 0;


protected:
	bool			m_bDead = false;
	bool			m_bActive = false;
	bool			m_bDebug = false;
	bool			m_bRotateAnimation = true;


};

END
