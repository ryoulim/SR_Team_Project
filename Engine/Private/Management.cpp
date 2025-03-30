#include "Management.h"

CManagement::CManagement()
{
}

HRESULT CManagement::Add_Manager(const _wstring& strManagertag, CBase* pManager)
{
    if (Find_Manager(strManagertag))
        return E_FAIL;

    m_Managers.emplace(strManagertag, pManager);

    return S_OK;
}

CBase* CManagement::Find_Manager(const _wstring& strManagertag) const
{
    auto Iter = m_Managers.find(strManagertag);
    if(Iter == m_Managers.end())
        return nullptr;

    return Iter->second;
}

CManagement* CManagement::Create()
{
    return new CManagement();
}

void CManagement::Free()
{
    __super::Free();

    for (auto& Pair : m_Managers)
        Safe_Release(Pair.second);
    m_Managers.clear();
}
