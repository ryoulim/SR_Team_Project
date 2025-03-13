#pragma once
#include "Base.h"

BEGIN(Engine)

class CcsvReader final : public CBase
{
private:
	CcsvReader();
	virtual ~CcsvReader() = default;

public:
	HRESULT Readcsv(const _wstring& strcsvPath, class CLevelData* pDatas);

private:
	class CGameInstance* m_pGameInstance = { nullptr };

private:
	_wstring Parsing_csv(wstringstream& linestream);
	bool IsNum(const _wstring& strCell);
	//void Change_Data(const _wstring& strCell, _Out_ _int* pOutInt, _Out_ _float* pOutfloat);

public:
	static CcsvReader* Create();
	virtual void Free()override;
};

END