#pragma once
#include "Effect.h"

BEGIN(Client)

class CScreenSprite final : public CEffect
{
public:
	enum eEffectType { HIT , HEAL , NONE};
public:
	typedef struct tagSpriteDesc : public CEffect::DESC
	{
		const TCHAR* szTextureTag;
		_float	fMaxFrame;
		_float	fAniSpeed;
		bool	bRandom;
		bool	bActive;

		eEffectType eEffectType;

	}DESC;

private:
	CScreenSprite(LPDIRECT3DDEVICE9 pGraphic_Device);
	CScreenSprite(const CScreenSprite& Prototype);
	virtual ~CScreenSprite() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;

public: //·»´õ¸µ
	virtual HRESULT SetUp_RenderState();
	virtual HRESULT Render();
	virtual HRESULT Release_RenderState();

public:
	static CScreenSprite* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();

public:
	void	isActive(bool _bActive) {}

private:
	_int				m_iRandom = 0;
	eEffectType			m_eEffectType = NONE;
	float				m_fAccTime = 1.0f;
	float				m_fFadeSpeed = 0.5f;

private:
	_float				m_fX = {};
	_float				m_fY = {};
	_float				m_fSizeX = {};
	_float				m_fSizeY = {};
	_bool				m_bDead = false;
};

END