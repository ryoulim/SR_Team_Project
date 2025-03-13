#pragma once

#include "Base.h"

/* ��ü���� ��Ƴ��´�. */

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

	class CGameObject* Find_Object(_uint iVectorIndex);
	vector<CGameObject*>& Get_Objects() {
		return m_GameObjects;
	}

private:
	vector<class CGameObject*>			m_GameObjects;

public:
	static CLayer* Create();
	virtual void Free();
};

END