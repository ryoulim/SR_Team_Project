#pragma once

#include "VIBuffer_Cabinet.h"
#include "VIBuffer_Trapezoid.h"
#include "VIBuffer_Terrain.h"
#include "VIBuffer_Rect.h"
#include "VIBuffer_Cube.h"
#include "Transform.h"
#include "Texture.h"
#include "Gravity.h"

/* 1. ������ü�� �����Ѵ�. */
/* 1_1. ������ü�� Ÿ��( CGameObject, CComponent )�� ũ�� ������ ���� �ʴ´�. */
/* 1_2. �����Ҷ� ������( + ��緹��) �� �����Ͽ� �����Ұž�. */

BEGIN(Engine)

class CPrototype_Manager final : public CBase
{
private:
	CPrototype_Manager();
	virtual ~CPrototype_Manager() = default;

public:
	HRESULT Initialize(_uint iNumLevels);
	HRESULT Add_Prototype(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, class CBase* pPrototype);
	CBase* Clone_Prototype(PROTOTYPE ePrototypeType, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg);
	void Clear(_uint iLevelIndex);
private:
	_uint										m_iNumLevels = {};
	typedef unordered_map<_wstring, class CBase*>	PROTOTYPES;
	PROTOTYPES*			m_pPrototypes = { nullptr };

private:
	class CBase* Find_Prototype(_uint iLevelIndex, const _wstring& strPrototypeTag);

public:
	static CPrototype_Manager* Create(_uint iNumLevels);
	virtual void Free() override;
};

END