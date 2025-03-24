#include "Collider_Manager.h"
#include "GameObject.h"
#include "Collider.h"

CCollider_Manager::CCollider_Manager()
{
}

HRESULT CCollider_Manager::Initialize(_uint iNumGroups)
{
    m_iNumGroups = iNumGroups;
    m_pColliders = new list<CCollider*>[iNumGroups];

    return S_OK;
}

HRESULT CCollider_Manager::Add_Collider(CCollider* pCollider, _uint iColliderGroupID)
{
    if (iColliderGroupID >= m_iNumGroups)
        return E_FAIL;

    m_pColliders[iColliderGroupID].push_back(pCollider);
    Safe_AddRef(pCollider);

    return S_OK;
}

void CCollider_Manager::Delete_Collider(const CGameObject* pOwner)
{
    for (_uint i = 0; i < m_iNumGroups; ++i)
    {
        for (auto Iter = m_pColliders[i].begin();
            Iter != m_pColliders[i].end();)
        {
            if ((*Iter)->Get_Owner() == pOwner)
            {
                Safe_Release(*Iter);
                Iter = m_pColliders[i].erase(Iter);
                continue;
            }
            Iter++;
        }
    }
}

void CCollider_Manager::Clear()
{
    for (_uint i = 0; i < m_iNumGroups; ++i)
    {
        for (auto Collider : m_pColliders[i])
        {
            if (Safe_Release(Collider))
                int a = 1;
        }
        m_pColliders[i].clear();
    }
}

void CCollider_Manager::Intersect(_uint iColliderGroupID1, _uint iColliderGroupID2)
{
    if (iColliderGroupID1 >= m_iNumGroups ||
        iColliderGroupID2 >= m_iNumGroups)
        return;

    for (auto& pCollider1 : m_pColliders[iColliderGroupID1])
    {
        for (auto& pCollider2 : m_pColliders[iColliderGroupID2])
        {
            if (pCollider1->Check_Intersect(pCollider2))
            {
                pCollider1->Get_Owner()->On_Collision(iColliderGroupID1, iColliderGroupID2);
                pCollider2->Get_Owner()->On_Collision(iColliderGroupID2, iColliderGroupID1);
            }
        }
    }

}

_bool CCollider_Manager::Raycast(const _float3& rayOrigin, const _float3& rayDir, _uint iColliderGroupID)
{
    if (iColliderGroupID >= m_iNumGroups)
        return FALSE;

    _bool bResult{};

    for (auto& pCollider : m_pColliders[iColliderGroupID])
    {
        bResult |= pCollider->RayCasting(rayOrigin, rayDir);
    }

    return bResult;
}

_float CCollider_Manager::Raycast_Downward(const _float3& rayOrigin, _uint iColliderGroupID)
{
    if (iColliderGroupID >= m_iNumGroups)
        return FALSE;

    _float fResult{0};

    for (auto& pCollider : m_pColliders[iColliderGroupID])
    {
        if (pCollider->RayCast_Downward(rayOrigin))
        {
            fResult = max(fResult, CCollider::Get_Last_Collision_Pos().y);
        }
    }

    return fResult;
}

CCollider_Manager* CCollider_Manager::Create(_uint iNumGroups)
{
    CCollider_Manager* pInstance = new CCollider_Manager();

    if (FAILED(pInstance->Initialize(iNumGroups)))
    {
        MSG_BOX("Failed to Created : CCollider_Manager");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CCollider_Manager::Free()
{
    __super::Free();
    Clear();
    Safe_Delete_Array(m_pColliders);
}
