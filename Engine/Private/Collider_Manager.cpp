#include "Collider_Manager.h"
#include "GameObject.h"
#include "Collider.h"
#include "Gravity.h"

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
            Safe_Release(Collider);
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
                pCollider1->Get_Owner()->On_Collision(pCollider1->Get_ID(), pCollider2->Get_ID());
                pCollider2->Get_Owner()->On_Collision(pCollider2->Get_ID(), pCollider1->Get_ID());
            }
        }
    }

}

_bool CCollider_Manager::RaycastBetweenPoints(const _float3& Point1, const _float3& Point2, _uint ColliderGroupID)
{
    _float3 vRayDir = Point2 - Point1;
    _float fRayLength = vRayDir.Length();
    vRayDir.Normalize();

    for (auto& pCollider : m_pColliders[ColliderGroupID])
    {
        // 범위 밖에 있음
        auto Length = (pCollider->Get_Pos() - Point1).Length();
        if (Length > fRayLength)
            continue;

        if (pCollider->RayCasting(Point1, vRayDir))
            return TRUE;
    }
    return FALSE;
}

CGameObject* CCollider_Manager::Raycast(const _float3& rayOrigin, const _float3& rayDir, _float rayLength, const initializer_list<_uint>& ColliderGroupIDs, _uint& _Out_ ColliderID)
{
    CGameObject* pCurObj{nullptr};
    _float3 vCurPos{}, vCurNormal{};
    _float fCurLength{}, fResult{ rayLength };

    for (auto& ID : ColliderGroupIDs)
    {
        if (ID >= m_iNumGroups)
            return nullptr;

        for (auto& pCollider : m_pColliders[ID])
        {
            // 범위 밖에 있음
            auto Length = (pCollider->Get_Pos() - rayOrigin).Length();
            if (Length > rayLength + pCollider->Get_MaxLength())
                continue;

            if (pCollider->RayCasting(rayOrigin, rayDir))
            {
                fCurLength = (CCollider::m_vLast_Collision_Pos - rayOrigin).Length();
                if (fCurLength < fResult)
                {
                    fResult = fCurLength;
                    vCurNormal = CCollider::m_vLast_Collision_Depth;
                    vCurPos = CCollider::m_vLast_Collision_Pos;
                    pCurObj = pCollider->m_pOwner;
                    ColliderID = pCollider->Get_ID();
                }
                else
                {
                    CCollider::m_vLast_Collision_Depth = vCurNormal;
                    CCollider::m_vLast_Collision_Pos = vCurPos;
                }
            }
        }
    }

    return pCurObj;
}

_float CCollider_Manager::Raycast_Downward(const _float3& rayOrigin, _uint iColliderGroupID)
{
    if (iColliderGroupID >= m_iNumGroups)
        return FALSE;

    _float3 vResultNormal{};
    _float fResult{ GRV_MIN_HEIGHT },fvCurHeight{};

    for (auto& pCollider : m_pColliders[iColliderGroupID])
    {
        if (pCollider->RayCast_Downward(rayOrigin))
        {
            fvCurHeight = CCollider::Get_Last_Collision_Pos().y;

            if (fResult < fvCurHeight)
            {
                fResult = fvCurHeight;
                vResultNormal = CCollider::m_vLast_Collision_Depth;
            }
            else
            {
                CCollider::m_vLast_Collision_Depth = vResultNormal;
            }
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
