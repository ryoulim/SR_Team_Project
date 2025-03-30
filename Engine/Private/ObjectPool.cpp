#include "ObjectPool.h"
#include "GameObject.h"

CObjectPool::CObjectPool(_uint iPoolSize)
	:m_pGameInstance(CGameInstance::Get_Instance())
	, m_iPoolSize{ iPoolSize }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CObjectPool::Initialize(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg)
{
	m_Objects.reserve(m_iPoolSize);
	for (_uint i = 0; i < m_iPoolSize; ++i)
	{
		m_Objects.push_back(static_cast<CGameObject*>(m_pGameInstance->Clone_Prototype(
			PROTOTYPE::TYPE_GAMEOBJECT, iPrototypeLevelIndex, strPrototypeTag, pArg)));
	}
	return S_OK;
}

void CObjectPool::Active_Object(_uint iLevelIndex, const _wstring& strLayertag, void* pArg)
{
	Safe_AddRef(m_Objects[m_iCurIndex]);

	m_pGameInstance->Push_GameObject(m_Objects[m_iCurIndex], iLevelIndex, strLayertag);
	m_Objects[m_iCurIndex++]->Reset(pArg);


	if (m_iCurIndex >= m_iPoolSize)
		m_iCurIndex = 0;
}

CObjectPool* CObjectPool::Create(_uint iPoolSize, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg)
{
	CObjectPool* pInstance = new CObjectPool(iPoolSize);

	if (FAILED(pInstance->Initialize(iPrototypeLevelIndex, strPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Created : CObjectPool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CObjectPool::Free()
{
	__super::Free();
	Safe_Release(m_pGameInstance);
	for (auto Obj : m_Objects)
		Safe_Release(Obj);
	m_Objects.clear();
}
