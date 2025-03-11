#include "ObjectPool.h"
#include "GameObject.h"

CObjectPool::CObjectPool()
{
}

CGameObject* CObjectPool::Active(_uint& _Out_ iNum)
{
	if (m_Objectlist.empty())
	{
		MSG_BOX("Failed to Active: CObjectPool");
		return nullptr;
	}

	CGameObject* pReturn = m_Objectlist.front();
	m_Objectlist.pop_front();
	iNum = --m_iSize;
	
	return pReturn;
}

_uint CObjectPool::DeActive(class CGameObject* ptr)
{
	m_Objectlist.push_front(ptr);
	return ++m_iSize;
}

CObjectPool* CObjectPool::Create()
{
	return new CObjectPool;
}

void CObjectPool::Free()
{
	__super::Free();
	for (auto& Obj : m_Objectlist)
		Safe_Release(Obj);
	m_Objectlist.clear();
}