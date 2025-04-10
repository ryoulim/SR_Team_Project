#pragma once

#include "Client_Defines.h"
#include "GameInstance.h"
#include "GameObject.h"

BEGIN(Client)

class CPawn abstract : public CGameObject
{
public:
	enum TYPE {COMMON,BOAT};
	typedef struct tagPawnDesc : public CTransform::DESC
	{
		_float3 vInitPos;
		_float3 vScale;
		LEVEL	eLevelID;
	}DESC;
public:
	typedef struct tagPlayerInfo {
		_int	iHP;
		_int	iArmor;
	}INFO;
protected:
	CPawn(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPawn(const CPawn& Prototype);
	virtual ~CPawn() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	void Link_Player_Data(const CPawn& Other) { m_tInfo = Other.m_tInfo; }
	TYPE Get_Type() { return m_eType; }
	virtual void Set_Level(LEVEL ID);
	void Set_Active(_bool isActive) {
		m_bActive = isActive;
	}
protected:
	virtual HRESULT Ready_Components(void* pArg);
	void Change_Level();

protected:
	TYPE m_eType{};
	LEVEL m_eLevelID = { LEVEL_END };
	const _tchar* m_szTextureID = { nullptr };
	const _tchar* m_szBufferType = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer* m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CCollider* m_pCollider = { nullptr };
	CCollider* m_pPrePosCollider{ nullptr };
	_bool	m_bActive{ TRUE };
	_bool	m_bFogAnimation{ false };

	_float						m_fInitSpeed{};

	INFO						m_tInfo{};

	//히트 관련
	_bool						m_bOnHit{};
	_float						m_fOnHitTimer{};

	//_bool	m_bDead{ FALSE };

protected:
	virtual void		On_Hit(_int iDamage);

public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free();
};

END
