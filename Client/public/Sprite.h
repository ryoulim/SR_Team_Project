#pragma once
#include "Effect.h"

BEGIN(Client)

class CSprite final : public CEffect
{
public:
	typedef struct tagSpriteDesc : public CEffect::DESC
	{
		const TCHAR*	szTextureTag;
		_float	fMaxFrame;
		bool	bLoop;
	}DESC;

private:
	CSprite(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSprite(const CSprite& Prototype);
	virtual ~CSprite() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CSprite* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();

private:
	bool		m_bLoop = false;
};

END