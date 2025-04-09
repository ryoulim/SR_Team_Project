#pragma once
#include "Effect.h"

BEGIN(Client)

class CRaceSprite final : public CEffect
{
public:
	typedef struct tagSpriteDesc : public CEffect::DESC
	{
		const TCHAR* szTextureTag;
		_float3	vPosOffset;
		_float	fStartFrame = 0.f;
		_float	fMaxFrame;
		bool	bLoop;
	}DESC;

private:
	CRaceSprite(LPDIRECT3DDEVICE9 pGraphic_Device);
	CRaceSprite(const CRaceSprite& Prototype);
	virtual ~CRaceSprite() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CRaceSprite* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();

private:
	bool		m_bLoop = false;
	_float3		m_vPosOffset = { 0.f, 0.f, 0.f };
};

END