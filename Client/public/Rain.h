#pragma once
#include "PSystem.h"

//스노우 파티클은 포지션이 아닌 바운딩 박스로 위치를 지정한다.

class CRain : public CPSystem 
{
public:
	typedef struct tagRainDesc
	{
		_float3 vMin;
		_float3 vMax;
	}DESC;
public:	
	CRain(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName);
	CRain(const CPSystem& Prototype);
	virtual ~CRain() = default;

	HRESULT Ready_Components();
	virtual void resetParticle(Attribute* attribute);
	virtual EVENT Update(_float timeDelta);
	virtual HRESULT Render() override;

	virtual HRESULT Initialize(void* pArg)override;

public:
	static CRain* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();

};

