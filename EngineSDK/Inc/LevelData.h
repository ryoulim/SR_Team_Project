#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CLevelData final : public CBase
{
private:
	CLevelData();
	virtual ~CLevelData() = default;

public:
	HRESULT Add_Data(const _wstring& strDataTag, void* pData);
	void*   Find_Data(const _wstring& strDataTag);

private:
	unordered_map<_wstring, void*> m_pData;

public:
	static CLevelData* Create();
	virtual void Free() override;
};

END