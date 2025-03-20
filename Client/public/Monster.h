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
		MODE_WAIT, MODE_ATTACK, MODE_DETECTIVE , MODE_END
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

public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free();

public: // ��ã�� �� ����Ƽ��
	virtual void PlayerDistance();
		

protected: //�⺻����
	LEVEL				m_eLevelID = { LEVEL_END };
	const _tchar*		m_szTextureID = { nullptr };
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


};

END
