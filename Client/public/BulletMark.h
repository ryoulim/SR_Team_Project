#pragma once
#include "Effect.h"

BEGIN(Client)

class CBulletMark final : public CEffect
{
public:
	typedef struct tagSpriteDesc : public CEffect::DESC
	{
		const TCHAR* szTextureTag;
		_float3 vLook;
		_float	fMaxFrame;
		_int	iType;
		bool	bLoop;
	}DESC;

private:
	CBulletMark(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBulletMark(const CBulletMark& Prototype);
	virtual ~CBulletMark() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CBulletMark* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();

private:
	bool		m_bLoop = false;
	_int		m_iType = 0;
	_float3		m_vLook = {0.f, 0.f, 0.f};
};

END