#pragma once
#include "Base.h"

BEGIN(Engine)
// ���� �Ŵ����� �����ϱ� ���� ���� Ŭ����
class CManagement : public CBase
{
private:
	CManagement();
	virtual ~CManagement() = default;

public:
	HRESULT Add_Manager(const _wstring& strManagertag, CBase* pManager);
	CBase* Find_Manager(const _wstring& strManagertag) const;

private:
	unordered_map<_wstring, CBase*> m_Managers;

public:
	static CManagement* Create();
	virtual void Free() override;
};

END