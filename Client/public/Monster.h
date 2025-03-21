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

public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free();

public: // 길찾기 및 디텍티브
	virtual void PlayerDistance();

protected:
	virtual HRESULT Ready_Textures() { return S_OK; }
	virtual HRESULT Animate_Monster();
	virtual HRESULT Set_TextureType() { return S_OK; }
	void	Compute_ViewAngle();
		

protected: //기본정보
	LEVEL				m_eLevelID = { LEVEL_END };
	const _tchar*		m_szTextureID = { nullptr };
	const _tchar*		m_szBufferType = { nullptr };

protected: //컴포넌트
	CTexture*	m_pTextureCom = { nullptr };
	CVIBuffer*	m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };

	typedef _uint STATE;
	typedef _uint VIEWDEGREE;
	typedef unordered_map<VIEWDEGREE, CTexture*> TEXTURE_DEGREE;
	unordered_map<STATE, TEXTURE_DEGREE> m_pTextureMap;

protected: //충돌	
	CCollider* m_pCollider = { nullptr };

protected: //속성
	_int	m_iHP			= 100;
	_int	m_iAttackPower	= 10;
	_int	m_iDefense		= 5;
	_float	m_fSpeed		= 1.0f;
	_float3	m_vScale		= { 0.f, 0.f, 0.f };
	_float3 m_vPosition		= { 0.f, 0.f, 0.f };
	_uint	m_iState		= {};
	_uint	m_iDegree		= {};
	_float	m_fPlayersViewAngle = {};
	MODE	m_eBehavior		= MODE::MODE_END;

protected: //부속성
	vector<string>	m_vDropItems	= {};
	string			m_strDialogue	= "Grrrr...";
	string			m_strSound		= "SoundFilePath";

protected: //플레이어
	CGameObject*	m_pTargetPlayer	= nullptr;

protected: //기능성 수치
	_float			m_fPlayerDistance = 0;


};

END
