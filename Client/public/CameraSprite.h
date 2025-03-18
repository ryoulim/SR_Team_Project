#pragma once
#include "Effect.h"

BEGIN(Client)

class CCameraSprite final : public CEffect
{
public:
	typedef struct tagSpriteDesc : public CEffect::DESC
	{
		const TCHAR* szTextureTag;
		_float	fMaxFrame;
		_float	fSceenX;
		_float	fSceenY;
		_float	fSceenZ;
		bool	bActive;
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
	virtual HRESULT Render() override;

public:
	static CCameraSprite* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();

public:
	void	isActive(bool _bActive) { m_bActive = _bActive; }

private:
	_float		m_fMaxFrame = 0;
	bool		m_bActive = false;

private:
	_float		m_fScreenX = 0;
	_float		m_fScreenY = 0;
	_float		m_fScreenZ = 0;
};

END