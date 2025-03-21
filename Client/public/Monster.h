#pragma once
#include "Client_Defines.h"
#include "GameInstance.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
class CTransform;
END

BEGIN(Client)

class CMonster abstract : public CGameObject
{
public:
	enum MODE
	{
		MODE_WAIT, MODE_ATTACK, MODE_DETECTIVE, MODE_END
	};


public:
	typedef struct tagMonsterDesc : public CTransform::DESC
	{
		_float3		vPosition;
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

protected: // �浹�Լ�
	virtual _float3		CalculateEffectPos();
	virtual void		On_Collision(CGameObject* pCollisionedObject, const _wstring& strLayerTag) {};

public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free();


/************************/
protected: // ���� �ִϸ��̼� �뵵 
	virtual HRESULT Ready_Textures() { return S_OK; }	// �ؽ��� ������Ʈ�� �ε�, *�θ� �Ϲݸ� �뵵�� �ۼ��ϰ� ������ �������̵� �ϱ�?
	void	Compute_ViewAngle();			// ����-�÷��̾� �� �ü� ���� �� ��� 
	virtual HRESULT Set_TextureType() { return S_OK; }	// enum�� uint������ �ű�� �۾�(�׳� �Լ��� ��)
	virtual HRESULT Animate_Monster();		// �ִϸ��̼� ���� (���� �Լ��� ��ġ��??)

protected:
	_uint	m_iState = {};					// �ִϸ��̼� ���� ����(����, �̵� ...)
	_uint	m_iDegree = {};					// �ִϸ��̼� �ü� �� ����
	_float	m_fPlayersViewAngle = {};		// �ü� ��

	typedef _uint STATE;		// �ؽ��� ������Ʈ (��ġ���ʾƿ�)
	typedef _uint VIEWDEGREE;
	typedef unordered_map<VIEWDEGREE, CTexture*> TEXTURE_DEGREE;
	unordered_map<STATE, TEXTURE_DEGREE> m_pTextureMap;		// 1Key: ���� , 2key: ����, value: CTexture
/************************/									// ���� m_pTextureMap[m_iState][m_iDegree]->Bind_Resourceȣ�� ��
															// �Ű������� m_fAnimationFrame ����
		

protected: //�⺻����
	LEVEL				m_eLevelID = { LEVEL_END };
	const _tchar*		m_szTextureID = { nullptr };  // ���� �Է�������..
	const _tchar*		m_szBufferType = { nullptr };

protected: //������Ʈ
	CTexture*	m_pTextureCom = { nullptr };
	CVIBuffer*	m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };




protected: //�浹	
	CCollider* m_pCollider = { nullptr };

protected: //�Ӽ�
	_int	m_iHP			= 100;
	_int	m_iAttackPower	= 10;
	_int	m_iDefense		= 5;
	_float	m_fSpeed		= 1.0f;
	_float3	m_vScale		= { 0.f, 0.f, 0.f };
	_float3 m_vPosition		= { 0.f, 0.f, 0.f };
	MODE	m_eBehavior		= MODE::MODE_END;

protected: //�μӼ�
	vector<string>	m_vDropItems	= {};
	string			m_strDialogue	= "Grrrr...";
	string			m_strSound		= "SoundFilePath";

protected: //�÷��̾�
	CGameObject*	m_pTargetPlayer	= nullptr;

protected: //��ɼ� ��ġ
	_float			m_fPlayerDistance = 0;

protected:
	bool			m_bDead = false;



};

END
