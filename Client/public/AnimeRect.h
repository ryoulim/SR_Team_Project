// 내 클래스 이름 : AnimeRect
// 부모 클래스 이름 : Map

#pragma once
#include "Map.h"

BEGIN(Client)

class CAnimeRect : public CMap
{
public:
	typedef struct tagAnimeRectDesc : public CMap::DESC
	{

	}DESC;

protected:
	CAnimeRect(LPDIRECT3DDEVICE9 pGraphic_Device);
	CAnimeRect(const CAnimeRect& Prototype);
	virtual ~CAnimeRect() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	_float m_fMaxFrame = {};

protected:
	void Move_Frame(_float fTimeDelta);

public:
	static CAnimeRect* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END
