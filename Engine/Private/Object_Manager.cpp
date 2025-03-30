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
	/* �纻��ü�� ������Ʈ �Ŵ����� �߰��Ѵ�. */
	/* ���� �Ŵ������� �����Ͽ� ����!! */
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

HRESULT CObject_Manager::Push_GameObject(CGameObject* pObject, _uint iLevelIndex, const _wstring& strLayerTag)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);

	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create();

		pLayer->Add_GameObject(pObject);

		m_pLayers[iLevelIndex].emplace(strLayerTag, pLayer);
	}
	else
		pLayer->Add_GameObject(pObject);

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
	auto pLayer = Find_Layer(iLevelIndex, strLayerTag);
	if (!pLayer)
		return nullptr;
	return pLayer->Find_Object(iVectorIndex);
}

list<CGameObject*>* CObject_Manager::Find_Objects(_uint iLevelIndex, const _wstring& strLayerTag)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);
	if (nullptr == pLayer)
		return nullptr;
	else
		return pLayer->Get_Objects();
}

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
	// �¿�, ����, ����/���Ÿ� ����� ����
	m_Frustum[0] = { viewProj._14 + viewProj._11, viewProj._24 + viewProj._21, viewProj._34 + viewProj._31, viewProj._44 + viewProj._41 }; // Left
	m_Frustum[1] = { viewProj._14 - viewProj._11, viewProj._24 - viewProj._21, viewProj._34 - viewProj._31, viewProj._44 - viewProj._41 }; // Right
	m_Frustum[2] = { viewProj._14 - viewProj._12, viewProj._24 - viewProj._22, viewProj._34 - viewProj._32, viewProj._44 - viewProj._42 }; // Top
	m_Frustum[3] = { viewProj._14 + viewProj._12, viewProj._24 + viewProj._22, viewProj._34 + viewProj._32, viewProj._44 + viewProj._42 }; // Bottom
	m_Frustum[4] = { viewProj._14 + viewProj._13, viewProj._24 + viewProj._23, viewProj._34 + viewProj._33, viewProj._44 + viewProj._43 }; // Near
	m_Frustum[5] = { viewProj._14 - viewProj._13, viewProj._24 - viewProj._23, viewProj._34 - viewProj._33, viewProj._44 - viewProj._43 }; // Far

	// ����ȭ
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

	Safe_Delete_Array(m_pLayers);

	Safe_Release(m_pGameInstance);
}
