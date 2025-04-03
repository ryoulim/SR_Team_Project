#pragma once
#include "PSystem.h"

//����� ��ƼŬ�� �������� �ƴ� �ٿ�� �ڽ��� ��ġ�� �����Ѵ�.

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

