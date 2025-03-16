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
		bool	bLoop;
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

private:
	_float		m_fMaxFrame = 0;
	bool		m_bLoop = false;
};

END