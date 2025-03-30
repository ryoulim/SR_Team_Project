#pragma once
#include "Base.h"

BEGIN(Engine)
// 각종 매니저를 관리하기 위한 상위 클래스
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