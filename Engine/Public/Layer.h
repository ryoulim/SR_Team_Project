#pragma once

#include "Base.h"

/* 객체들을 모아놓는다. */

BEGIN(Engine)

class CLayer final : public CBase
{
private:
	CLayer();
	virtual ~CLayer() = default;

public:
	HRESULT Add_GameObject(class CGameObject* pGameObject);
	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);

	class CGameObject* Find_Object(_uint iIndex);
	list<CGameObject*>& Get_Objects() {
		return m_GameObjects;
	}

private:
	list<class CGameObject*>			m_GameObjects;

public:
	static CLayer* Create();
	virtual void Free();
};

END