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
					// ���� ������Ʈ�� ������ �ݶ��̴��� �ִ��� �˻��ϰ� �Ǿ����� ���߿� ����or����ȭ �� ���� ����
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