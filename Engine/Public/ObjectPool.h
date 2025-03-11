#pragma once

#include "Base.h"

/* ��ü���� ��Ƴ��´�. */

BEGIN(Engine)

class CGameObject;
class CObjectPool final : public CBase
{
private:
	CObjectPool();
	virtual ~CObjectPool() = default;

public:
	CGameObject* Active(_uint& _Out_ iNum);
	_uint DeActive(CGameObject* ptr);

private:
	forward_list<CGameObject*> m_Objectlist;
	_uint					   m_iSize{};

public:
	static CObjectPool* Create();
	virtual void Free();
};

END