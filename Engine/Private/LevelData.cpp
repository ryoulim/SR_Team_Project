#include "LevelData.h"

CLevelData::CLevelData()
{
}

HRESULT CLevelData::Add_Data(const _wstring& strDataTag, void* pData)
{
    if (nullptr != Find_Data(strDataTag))
        return E_FAIL;

    m_pData.emplace(strDataTag, pData);
    return S_OK;
}

void* CLevelData::Find_Data(const _wstring& strDataTag)
{
    auto Iter = m_pData.find(strDataTag);
    if (Iter == m_pData.end())
        return nullptr;

    return Iter->second;
}

CLevelData* CLevelData::Create()
{
	return  new CLevelData();
}

void CLevelData::Free()
{
    __super::Free();

    for (auto& Pair : m_pData)
    {
        free(Pair.second);
        Pair.second = nullptr;
    }
    m_pData.clear();
}
