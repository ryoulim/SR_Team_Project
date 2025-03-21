#pragma once
#include "Effect.h"

BEGIN(Client)

class CFlatform final : public CEffect
{
public:
	typedef struct tagSpriteDesc : public CEffect::DESC
	{
		const TCHAR* szTextureTag;
		_float	fMaxFrame;
		bool	bLoop;
	}DESC;

private:
	CFlatform(LPDIRECT3DDEVICE9 pGraphic_Device);
	CFlatform(const CFlatform& Prototype);
	virtual ~CFlatform() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CFlatform* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();

private:
	_float		m_fMaxFrame = 0;
	bool		m_bLoop = false;
};

END