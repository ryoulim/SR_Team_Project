#include "Layer.h"
#include "GameObject.h"
#include "GameInstance.h"

CLayer::CLayer()
	:m_pGameInstance(CGameInstance::Get_Instance())
{
}

HRESULT CLayer::Add_GameObject(CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_GameObjects.push_back(pGameObject);

	return S_OK;
}

void CLayer::Priority_Update(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
	{
		if (nullptr != pGameObject)
			pGameObject->Priority_Update(fTimeDelta);
	}
		
}

void CLayer::Update(_float fTimeDelta)
{
	EVENT ObjEvent{};
	for (auto Iter = m_GameObjects.begin();
		Iter != m_GameObjects.end();)
	{
		if (nullptr != *Iter)
		{
			ObjEvent = (*Iter)->Update(fTimeDelta);
			if (EVN_NONE != ObjEvent)
			{
				if (EVN_DEAD == ObjEvent)
				{
					// 지금 오브젝트가 죽으면 콜라이더가 있는지 검사하게 되어있음 나중에 수정or최적화 할 수도 있음
					m_pGameInstance->Delete_Collider(*Iter);
					Safe_Release(*Iter);
				}
				Iter = m_GameObjects.erase(Iter);
			}
			else
				Iter++;
		}
	}
}

void CLayer::Late_Update(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
	{
		if (nullptr != pGameObject)
			pGameObject->Late_Update(fTimeDelta);

	}
}

CGameObject* CLayer::Find_Object(_uint iIndex)
{
	if (iIndex >= m_GameObjects.size())
		return nullptr;
	
	auto Iter = m_GameObjects.begin();

	for (_uint i = 0; i < iIndex; ++i)
		Iter++;

	return *Iter;
}

CLayer* CLayer::Create()
{
	return new CLayer();
}

void CLayer::Free()
{
	__super::Free();

	for (auto& pGameObject : m_GameObjects)
		Safe_Release(pGameObject);
	m_GameObjects.clear();
}