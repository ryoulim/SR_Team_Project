#include "Object_Manager.h"
#include "Layer.h"

#include "GameInstance.h"
#include "GameObject.h"
#include "ObjectPool.h"

CObject_Manager::CObject_Manager()
	: m_pGameInstance { CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CObject_Manager::Initialize(_uint iNumLevels)
{
	m_pLayers = new LAYERS[iNumLevels];

	m_iNumLevels = iNumLevels;

    return S_OK;
}

HRESULT CObject_Manager::Add_GameObject(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg)
{
	/* 사본객체를 오브젝트 매니져에 추가한다. */
	/* 원형 매니져에게 복제하여 내놔!! */
	CGameObject*		pGameObject = dynamic_cast<CGameObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_GAMEOBJECT, iPrototypeLevelIndex, strPrototypeTag, pArg));
	if (nullptr == pGameObject)
		return E_FAIL;

	CLayer*		pLayer = Find_Layer(iLevelIndex, strLayerTag);

	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create();

		pLayer->Add_GameObject(pGameObject);

		m_pLayers[iLevelIndex].emplace(strLayerTag, pLayer);
	}
	else
		pLayer->Add_GameObject(pGameObject);

	return S_OK;
}

void CObject_Manager::Priority_Update(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
			Pair.second->Priority_Update(fTimeDelta);
	}
}

void CObject_Manager::Update(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
			Pair.second->Update(fTimeDelta);
	}
}

void CObject_Manager::Late_Update(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
			Pair.second->Late_Update(fTimeDelta);
	}
}

void CObject_Manager::Clear(_uint iLevelIndex)
{
	if (iLevelIndex >= m_iNumLevels)
		return;

	for (auto& Pair : m_pLayers[iLevelIndex])
		Safe_Release(Pair.second);

	m_pLayers[iLevelIndex].clear();
}

CGameObject* CObject_Manager::Find_Object(_uint iLevelIndex, const _wstring& strLayerTag, _uint iVectorIndex)
{
	return Find_Layer(iLevelIndex, strLayerTag)->Find_Object(iVectorIndex);
}

vector<CGameObject*>& CObject_Manager::Find_Objects(_uint iLevelIndex, const _wstring& strLayerTag)
{
	return Find_Layer(iLevelIndex, strLayerTag)->Get_Objects();
}

HRESULT CObject_Manager::Create_Object_Pool(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, const _wstring& strObjectTag, _uint iPoolSize)
{
	CObjectPool* pObjectPool = CObjectPool::Create();

	for (_uint i = 0; i < iPoolSize; ++i)
	{
		CGameObject* pGameObject = dynamic_cast<CGameObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_GAMEOBJECT, iPrototypeLevelIndex, strPrototypeTag));
		if (nullptr == pGameObject)
			return E_FAIL;
		pObjectPool->DeActive(pGameObject);
	}

	m_ObjectPool.emplace(strObjectTag, pObjectPool);

	return S_OK;
}

HRESULT CObject_Manager::Release_Object_Pool(const _wstring& strObjectTag)
{
	auto Iter = m_ObjectPool.find(strObjectTag);
	if (Iter == m_ObjectPool.end())
		return E_FAIL;

	m_ObjectPool.erase(Iter);

	return S_OK;
}

_uint CObject_Manager::Active_Object(const _wstring& strObjectTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg)
{
	auto ObjPool = Find_Object_Pool(strObjectTag);
	_uint iReturn{};
	CGameObject* pGameObject = ObjPool->Active(iReturn);
	pGameObject->Initialize(pArg);

	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);

	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create();

		pLayer->Add_GameObject(pGameObject);

		m_pLayers[iLevelIndex].emplace(strLayerTag, pLayer);
	}
	else
		pLayer->Add_GameObject(pGameObject);

	return iReturn;
}

_uint CObject_Manager::Deactive_Object()
{
	return E_NOTIMPL;
}

CLayer* CObject_Manager::Find_Layer(_uint iLevelIndex, const _wstring& strLayerTag)
{
	auto	iter = m_pLayers[iLevelIndex].find(strLayerTag);
	if (iter == m_pLayers[iLevelIndex].end())
		return nullptr;

	return iter->second;
}

CObjectPool* CObject_Manager::Find_Object_Pool(const _wstring& strObjectTag)
{
	auto	iter = m_ObjectPool.find(strObjectTag);
	if (iter == m_ObjectPool.end())
		return nullptr;

	return iter->second;
}

CObject_Manager* CObject_Manager::Create(_uint iNumLevels)
{
	CObject_Manager* pInstance = new CObject_Manager();

	if (FAILED(pInstance->Initialize(iNumLevels)))
	{
		MSG_BOX("Failed to Created : CObject_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CObject_Manager::Free()
{
    __super::Free();

	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
			Safe_Release(Pair.second);
		m_pLayers[i].clear();
	}
	for (auto& Pair : m_ObjectPool)
		Safe_Release(Pair.second);
	m_ObjectPool.clear();

	Safe_Delete_Array(m_pLayers);

	Safe_Release(m_pGameInstance);
}
