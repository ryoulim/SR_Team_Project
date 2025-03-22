#include "Object_Manager.h"
#include "Layer.h"

#include "GameInstance.h"
#include "GameObject.h"
#include "ObjectPool.h"
#include "Collider.h"

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

HRESULT CObject_Manager::Add_GameObjectReturn
(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, CGameObject** ppOut, void* pArg)
{
	CGameObject* pGameObject = dynamic_cast<CGameObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_GAMEOBJECT, iPrototypeLevelIndex, strPrototypeTag, pArg));
	if (nullptr == pGameObject)
		return E_FAIL;

	*ppOut = pGameObject;

	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);

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

list<CGameObject*>& CObject_Manager::Find_Objects(_uint iLevelIndex, const _wstring& strLayerTag)
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
	if(FAILED(pGameObject->Reset(pArg)))
		return 0;

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

_uint CObject_Manager::Deactive_Object(const _wstring& strObjectTag, class CGameObject* pObject)
{
	auto ObjPool = Find_Object_Pool(strObjectTag);
	_uint iReturn = ObjPool->DeActive(pObject);
	return iReturn;
}

#ifndef _COLLIDER_MANAGER
void CObject_Manager::Intersect(_uint iLevelIndex, const _wstring& strLayerTag1, const _wstring& strLayerTag2)
{
	auto Layer1 = Find_Layer(iLevelIndex, strLayerTag1);
	auto Layer2 = Find_Layer(iLevelIndex, strLayerTag2);
	
	if (Layer1 == nullptr || Layer2 == nullptr)
		return;

	auto GroupA = Layer1->Get_Objects();
	auto GroupB = Layer2->Get_Objects();

	CCollider* pCollider1{ nullptr };
	CCollider* pCollider2{ nullptr };

	for (auto& Obj1 : GroupA)
	{
		pCollider1 = static_cast<CCollider*>(Obj1->Find_Component(TEXT("Com_Collider")));
		for (auto& Obj2 : GroupB)
		{
			pCollider2 = static_cast<CCollider*>(Obj2->Find_Component(TEXT("Com_Collider")));

			if (pCollider1->Check_Intersect(pCollider2))
			{
				Obj1->On_Collision(Obj2, strLayerTag2);
			}
		}
	}
}

_bool CObject_Manager::Raycast(const _float3& rayOrigin, const _float3& rayDir, _uint iLevelIndex, const _wstring& strLayerTag)
{
	auto Layer = Find_Layer(iLevelIndex, strLayerTag);

	if (Layer == nullptr)
		return FALSE;

	auto& GroupA = Layer->Get_Objects();

	_bool bResult{};

	for (auto& Obj1 : GroupA)
	{
		auto pCollider = static_cast<CCollider*>(Obj1->Find_Component(TEXT("Com_Collider")));

		bResult |= pCollider->RayCasting(rayOrigin, rayDir);
	}

	return bResult;
}

_bool CObject_Manager::Raycast_Downward(const _float3& rayOrigin, _uint iLevelIndex, const _wstring& strLayerTag)
{
	auto Layer = Find_Layer(iLevelIndex, strLayerTag);

	if (Layer == nullptr)
		return FALSE;

	auto& GroupA = Layer->Get_Objects();

	_bool bResult{};

	for (auto& Obj1 : GroupA)
	{
		auto pCollider = static_cast<CCollider*>(Obj1->Find_Component(TEXT("Com_Collider")));

		bResult |= pCollider->RayCast_Downward(rayOrigin);
	}

	return bResult;
}
#endif // !COLLIDER_MANAGER


_bool CObject_Manager::IsPointInFrustum(const _float3& Point)
{
	for (int i = 0; i < 6; i++)
	{
		if (D3DXPlaneDotCoord(&m_Frustum[i], &Point) < 0)
			return false;
	}
	return true;
}

void CObject_Manager::Update_Frustum(const _float4x4& viewProj)
{
	// 좌우, 상하, 근접/원거리 평면을 추출
	m_Frustum[0] = { viewProj._14 + viewProj._11, viewProj._24 + viewProj._21, viewProj._34 + viewProj._31, viewProj._44 + viewProj._41 }; // Left
	m_Frustum[1] = { viewProj._14 - viewProj._11, viewProj._24 - viewProj._21, viewProj._34 - viewProj._31, viewProj._44 - viewProj._41 }; // Right
	m_Frustum[2] = { viewProj._14 - viewProj._12, viewProj._24 - viewProj._22, viewProj._34 - viewProj._32, viewProj._44 - viewProj._42 }; // Top
	m_Frustum[3] = { viewProj._14 + viewProj._12, viewProj._24 + viewProj._22, viewProj._34 + viewProj._32, viewProj._44 + viewProj._42 }; // Bottom
	m_Frustum[4] = { viewProj._14 + viewProj._13, viewProj._24 + viewProj._23, viewProj._34 + viewProj._33, viewProj._44 + viewProj._43 }; // Near
	m_Frustum[5] = { viewProj._14 - viewProj._13, viewProj._24 - viewProj._23, viewProj._34 - viewProj._33, viewProj._44 - viewProj._43 }; // Far

	// 정규화
	for (int i = 0; i < 6; i++)
		D3DXPlaneNormalize(&m_Frustum[i], &m_Frustum[i]);
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
