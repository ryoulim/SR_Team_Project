#pragma once
#include "PSystem.h"

//스노우 파티클은 포지션이 아닌 바운딩 박스로 위치를 지정한다.

class CDust : public CPSystem
{
public:
	CDust(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName);
	CDust(const CPSystem& Prototype);
	virtual ~CDust() = default;

	HRESULT Ready_Components();
	virtual void resetParticle(Attribute* attribute);
	virtual EVENT Update(_float timeDelta);
	virtual HRESULT Render() override;

	virtual HRESULT Initialize(void* pArg)override;

public:
	static CDust* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName, int numparticles, _float3 MinBox, _float3 MaxBox);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();

};

