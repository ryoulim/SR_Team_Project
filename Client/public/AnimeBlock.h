// 내 클래스 이름 : AnimeBlock
// 부모 클래스 이름 : Map

#pragma once

#include "Map.h"

BEGIN(Client)

class CAnimeBlock : public CMap
{
public:
	typedef struct tagAnimeBlockDesc : public CMap::DESC
	{

	}DESC;

private:
	CAnimeBlock(LPDIRECT3DDEVICE9 pGraphic_Device);
	CAnimeBlock(const CAnimeBlock& Prototype);
	virtual ~CAnimeBlock() = default;

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
	static CAnimeBlock* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END