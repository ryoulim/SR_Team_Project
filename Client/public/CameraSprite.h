#pragma once
#include "Effect.h"

BEGIN(Client)

class CCameraSprite final : public CEffect
{
public:
	enum eEffectType { GUNFIRE , BULLETTRACER , NONE };
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
	CCameraSprite(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCameraSprite(const CCameraSprite& Prototype);
	virtual ~CCameraSprite() = default;

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
	static CCameraSprite* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();

public:
	void	isActive(bool _bActive) { m_bActive = _bActive; }

private:
	bool				m_bActive = false;
	eEffectType			m_eEffectType = NONE;
	
private:
	_float				m_fX = {};
	_float				m_fY = {};
	_float				m_fSizeX = {};
	_float				m_fSizeY = {};

	_float4x4			m_ViewMatrix = {};
	_float4x4			m_ProjMatrix = {};

	_float4x4			m_OldViewMatrix = {};
	_float4x4			m_OldProjMatrix = {};

};

END