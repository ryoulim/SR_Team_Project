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

public: // ��ã�� �� ����Ƽ��
	virtual void PlayerDistance();
	virtual void CalculateVectorToPlayer();
	virtual bool IsPlayerDetected();
	virtual void Render_DebugFOV();
	const char*	 GetMonsterStateName(CMonster::MODE eState);

public: //���º�ȯ
	virtual void MonsterTick(_float fTimeDelta);

public: //�׼�
	virtual void DoIdle(_float dt);
	virtual void DoBattle(_float dt);
	virtual void DoReturn(_float dt);
	virtual void SetRandomDirection();

public: //���� �⺻����
	virtual void AttackPattern(_float dt);
	virtual void ChasePlayer(_float dt);

public: // �浹�Լ�
	virtual _float3		CalculateEffectPos();
	virtual void		On_Collision(_uint MyColliderID, _uint OtherColliderID) override {};

public: // �����
	void	ToggleDebugMode() { m_bDebug = !m_bDebug; }

public: // ���� ����
	float GetRandomFloat(float min, float max)
	{
		if (min > max) std::swap(min, max);
		uniform_real_distribution<float> dist(min, max);
		return dist(m_Gen);
	}
	int GetRandomInt(int min, int max)
	{
		if (min > max) std::swap(min, max);
		std::uniform_int_distribution<int> dist(min, max); // min �̻� max ���� ����
		return dist(m_Gen);
	}

public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free();

#pragma region �ִϸ��̼�


	/************************/
protected: // ���� �ִϸ��̼� �뵵 
	virtual HRESULT Ready_Textures() { return S_OK; }	// PURE | �ؽ��� ������Ʈ�� �ε�, *�θ� �Ϲݸ� �뵵�� �ۼ��ϰ� ������ �������̵� �ϱ�?
	virtual HRESULT Set_TextureType();					// enum�� uint������ �ű�� �۾�(�׳� �Լ��� ��)
	virtual HRESULT Animate_Monster(_float fTimeDelta);	// �ִϸ��̼� ���� (���� �Լ��� ��ġ��??)
	void	Compute_ViewAngle();						// ����-�÷��̾� �� �ü� ���� �� ��� 
	virtual HRESULT Set_Animation() { return S_OK; }	// PURE | �� ���� ���� ���¿� ���� �ִϸ��̼� �ƽ� ������ ���� �ʿ� 

protected:
	_uint	m_iState = {};					// �ִϸ��̼� ���� ����(����, �̵� ...)
	_uint	m_iDegree = {};					// �ִϸ��̼� �ü� �� ����
	_float	m_fPlayersViewAngle = {};		// �ü� ��
	_bool	m_bCW = {true};					// Clockwise?
	_float	m_fDivOffset = {45.f};			// �� ���� �ɰ��� �ִ� �̹������� (����: 22.5��, �Ϲ�(�⺻): 45��) 
	_bool	m_isReadyMonster = { false };	// �ؽ��� �غ� �� �� ���� �⺻ ���������� �������� 

	typedef _uint STATE;					// �ؽ��� ������Ʈ (��ġ���ʾƿ�)
	typedef _uint VIEWDEGREE;
	typedef unordered_map<VIEWDEGREE, CTexture*> TEXTURE_DEGREE;
	unordered_map<STATE, TEXTURE_DEGREE> m_pTextureMap;		// 1Key: ���� , 2key: ����, value: CTexture
	/************************/									// ���� m_pTextureMap[m_iState][m_iDegree]->Bind_Resourceȣ�� ��
																// �Ű������� m_fAnimationFrame ����

#pragma endregion
		

protected: //�⺻����
	LEVEL				m_eLevelID = { LEVEL_END };
	const _tchar*		m_szTextureID = { nullptr };  // ���� �Է�������..
	const _tchar*		m_szBufferType = { nullptr };

protected: //������Ʈ
	CTexture*	m_pTextureCom = { nullptr };
	CVIBuffer*	m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CGravity*	m_pGravityCom = { nullptr };

protected: //�浹	
	CCollider* m_pCollider = { nullptr };

protected: //�Ӽ�
	_int	m_iHP			= 100;
	_int	m_iMaxHP		= 100;
	_int	m_iAttackPower	= 10;
	_int	m_iDefense		= 5;
	_float	m_fSpeed		= 60.f;
	_float3	m_vScale		= { 0.f, 0.f, 0.f };
	_float3 m_vPosition		= { 0.f, 0.f, 0.f };
	MODE	m_eState		= MODE::MODE_END;

	EIdlePhase	m_eIdlePhase = EIdlePhase::IDLE_WAIT;

protected: //�����
	steady_clock::time_point g_LastLogTime = steady_clock::now();

protected: //�μӼ�
	vector<string>	m_vDropItems	= {};
	string			m_strDialogue	= "Grrrr...";
	string			m_strSound		= "SoundFilePath";

protected: //�÷��̾�
	CGameObject*	m_pTargetPlayer	= nullptr;

protected: //����Ƽ��
	_float3			m_vDirection		= { 0.f, 0.f, -1.f };
	_float3			m_vToPlayer			= { 0.f, 0.f, 0.f };
	_float			m_fCurDistance		= 0;
	_float			m_fDetectiveDistance= 0;
	_float			m_fIdleTime			= 0;
	_float			m_fMaxIdleTime		= 0;


protected: // ���� ���� ��������
	random_device m_Rd;
	mt19937 m_Gen{ std::random_device{}() };
	uniform_real_distribution<float> m_Dist;


protected:
	// ��ȸ �̵� ����
	_float  m_fWanderTime = 0.f;
	_float  m_fWanderElapsed = 0.f;
	_float3 m_vReturnPos = { 0.f, 0.f, 0.f };

	// ���� ����
	_float  m_fIdleWaitTime = 3.0f;
	_float  m_fIdleWaitElapsed = 0.f;
	
	// ���� ��ٿ�
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
