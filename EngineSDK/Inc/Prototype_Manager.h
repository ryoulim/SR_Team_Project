#pragma once

#include "VIBuffer_Canopy.h"
#include "VIBuffer_Signboard.h"
#include "VIBuffer_Cabinet.h"
#include "VIBuffer_Stall.h"
#include "VIBuffer_Computer.h"
#include "VIBuffer_Trapezoid.h"
#include "VIBuffer_Rect.h"
#include "VIBuffer_Cube.h"
#include "VIBuffer_CubeEx.h"
#include "VIBuffer_Circle.h"
#include "VIBuffer_TriangularPillar.h"
#include "VIBuffer_BuildingH.h"
#include "VIBuffer_BuildingW.h"
#include "VIBuffer_BuildingV.h"
#include "VIBuffer_BuildingU.h"
#include "VIBuffer_RaceGate.h"
#include "VIBuffer_RaceCylinder.h"
#include "VIBuffer_RaceBoss.h"

#include "VIBuffer_Terrain.h"
#include "VIBuffer_RaceTerrain.h"
#include "VIBuffer_RaceLandscape.h"

#include "Transform.h"
#include "Texture.h"
#include "Gravity.h"

#include "Collider_AABB_Cube.h"
#include "Collider_OBB_Cube.h"
#include "Collider_Capsule.h"
#include "Collider_Sphere.h"
#include "Collider_Line.h"
#include "Collider_Rect.h"

#include "SoundController.h"
#include "Sound_Core.h"

/* 1. 원형객체를 보관한다. */
/* 1_1. 원형객체의 타입( CGameObject, CComponent )에 크게 영향을 받지 않는다. */
/* 1_2. 보관할때 레벨별( + 모든레벨) 로 구분하여 저장할거야. */

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