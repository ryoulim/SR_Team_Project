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
public: //���º�ȯ	
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


public: // �ټ��� �����ϴ� ����
	_int* Get_HP() { return &m_iHP; }
	_int Get_MaxHP() { return m_iMaxHP; }

protected: // ��ã�� �� ����Ƽ��
	virtual void PlayerDistance();
	virtual void CalculateVectorToPlayer();
	virtual bool IsPlayerDetected();
	virtual void Render_DebugFOV();
	const char* GetMonsterStateName(CMonster::MODE eState);
	virtual _bool Raycast_Player();

protected: //���º�ȯ
	virtual void MonsterTick(_float fTimeDelta);

protected: //�׼�
	virtual void DoIdle(_float dt);
	virtual void DoBattle(_float dt);
	virtual void DoReturn(_float dt);
	virtual void DoReady(_float dt);
	virtual void DoDead(_float dt);
	virtual void SetRandomDirection();

protected: //���� �⺻����
	virtual void AttackPattern(_float dt);
	virtual void ChasePlayer(_float dt);

protected: // �浹�Լ�
	virtual _float3		CalculateEffectPos();
	virtual void		On_Collision(_uint MyColliderID, _uint OtherColliderID) override;

public: // �����
	void	ToggleDebugMode() { m_bDebug = !m_bDebug; }

protected: // ���� ����
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

protected:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free();

#pragma region �ִϸ��̼�


	/************************/
protected: // ���� �ִϸ��̼� �뵵 
	virtual HRESULT Ready_Textures() { return S_OK; }	// PURE | �ؽ��� ������Ʈ�� �ε�, *�θ� �Ϲݸ� �뵵�� �ۼ��ϰ� ������ �������̵� �ϱ�?
	virtual HRESULT Set_Animation() { return S_OK; }	// PURE | �� ���� ���� ���¿� ���� �ִϸ��̼� �ƽ� ������ ���� �ʿ� 
	virtual HRESULT Animate_Monster(_float fTimeDelta);	// PURE | �ִϸ��̼� ���� (���� �Լ��� ��ġ��??)
	HRESULT Set_TextureType();					// enum�� uint������ �ű�� �۾�(�׳� �Լ��� ��)
	void	Compute_ViewAngle();						// ����-�÷��̾� �� �ü� ���� �� ���
	void	Resize_Texture(_float fSizePercent);

protected:
	_uint	m_iState = {};					// �ִϸ��̼� ���� ����(����, �̵� ...)
	_uint	m_iDegree = {};					// �ִϸ��̼� �ü� �� ����
	_float	m_fPlayersViewAngle = {};		// �ü� ��
	_bool	m_bCW = { true };					// Clockwise?
	_float	m_fDivOffset = { 45.f };			// �� ���� �ɰ��� �ִ� �̹������� (����: 22.5��, �Ϲ�(�⺻): 45��) 
	_bool	m_isReadyMonster = { false };	// �ؽ��� �غ� �� �� ���� �⺻ ���������� �������� 
	_float	m_fDeadBodyCounter = {};		// ��ü �����ִ� �ð� 

	typedef _uint STATE;					// �ؽ��� ������Ʈ (��ġ���ʾƿ�)
	typedef _uint VIEWDEGREE;
	typedef unordered_map<VIEWDEGREE, CTexture*> TEXTURE_DEGREE;
	unordered_map<STATE, TEXTURE_DEGREE> m_pTextureMap;		// 1Key: ���� , 2key: ����, value: CTexture
	/************************/									// ���� m_pTextureMap[m_iState][m_iDegree]->Bind_Resourceȣ�� ��
																// �Ű������� m_fAnimationFrame ����

#pragma endregion


protected: //�⺻����
	LEVEL				m_eLevelID = { LEVEL_END };
	const _tchar* m_szTextureID = { nullptr };  // ���� �Է�������..
	const _tchar* m_szBufferType = { nullptr };

protected: //������Ʈ
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer* m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CGravity* m_pGravityCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };
	CSoundController* m_pSoundCom{ nullptr };

protected: //�浹	
	CCollider* m_pCollider = { nullptr };
	CCollider* m_pHeadCollider = { nullptr };

protected: //�Ӽ�
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

protected: //�����
	steady_clock::time_point g_LastLogTime = steady_clock::now();

protected: //�μӼ�
	vector<string>	m_vDropItems = {};
	string			m_strDialogue = "Grrrr...";
	string			m_strSound = "SoundFilePath";

protected: //�÷��̾�
	CGameObject* m_pTargetPlayer = nullptr;

protected: //����Ƽ��
	_float3			m_vDirection = { 0.f, 0.f, -1.f };
	_float3			m_vToPlayer = { 0.f, 0.f, 0.f };
	_float			m_fCurDistance = 0;
	_float			m_fDetectiveDistance = 0;
	_float			m_fIdleTime = 0;
	_float			m_fMaxIdleTime = 0;
	_float			m_fAttackDistance = 0;


protected: // ���� ���� ��������
	random_device m_Rd;
	mt19937 m_Gen{ std::random_device{}() };
	uniform_real_distribution<float> m_Dist;

protected: // �ִϸ��̼�
	_float	m_fAnimationFrame = 0;
	_float	m_fAnimationMaxFrame = 0;
	_float	m_fAnimationSpeed = 0;

protected:
	// ��ȸ �̵� ����
	_float  m_fShaderTime = 0.f;
	_float  m_fWanderTime = 0.f;
	_float  m_fWanderElapsed = 0.f;
	_float3 m_vReturnPos = { 0.f, 0.f, 0.f };

	// ���� ����
	_float  m_fIdleWaitTime = 3.0f;
	_float  m_fIdleWaitElapsed = 0.f;
	_bool	m_bRotateAnimation = true;

	// ���� ��ٿ�
	_bool	m_bCoolingDown = false;
	_float	m_fCooldownTime = 0.f;
	_float	m_fCooldownDuration = 0.f;
	_float	m_fAttackTimer = 0.f;
	_int	m_iRandom = 0;


protected: // �۵�����
	_bool			m_bDead = false;
	_bool			m_bActive = false;

protected: // �����
	_bool			m_bDebug = false;

protected: // �Ϲ� ���� �ൿ �� ����
	_bool			m_bFoundPlayer = { false };
	_bool			m_isReadyToAttack = { false };
	_float			m_fRaycastTicker = {};
	_float			m_fBulletCooldownElapsed = {};
	_float			m_fBulletCooldown = {};
	_float			m_fAttackTime = {};
	_bool			m_bKnockBack = false;

protected: // ���� �ذ� �����
	_bool		m_bSkullActive{};
	class CSkull* m_pSkull{ nullptr };
	_float3		m_vSkullOffset{};

protected: // �Ϲ� ���Ϳ� �浹 �Լ�
	void	On_Collision_NormalMonster(_uint MyColliderID, _uint OtherColliderID);
	void	Collision_With_Weapon();
	void	Collision_With_Block();

protected: // �Ϲ� ���Ϳ� �Լ���
	virtual _bool IsMonsterAbleToAttack();
	virtual void Debug_Output();
	virtual void State_Change_IDLE(_float dt);
	virtual void State_Change_DETECTIVE(_float dt);
	virtual void State_Change_READY(_float dt);
	virtual void State_Change_BATTLE(_float dt);

protected:
	_uint	m_iHeadMultiplier = 1;	// ��弦 ����
};

END
