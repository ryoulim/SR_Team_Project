#pragma once

#include "Engine_Defines.h"

/* ��� Ŭ�������� �θ� �Ǵ� Ŭ������. */
/* ���۷���ī��Ʈ�� �����ϱ����� ����� �ο��Ѵ�. (AddRef, Release) */
BEGIN(Engine)

class ENGINE_DLL CBase
{
protected:
	CBase();
	virtual ~CBase() = default;

public:
	/* ���۷��� ī��Ʈ�� ������Ų��. */
	unsigned int AddRef();

	/* ���۷��� ī��Ʈ�� ���ҽ�Ų��. or �����Ѵ�. */
	unsigned int Release();

protected:
	unsigned int		m_iRefCnt = { };

public:
	virtual void Free();

};

END


