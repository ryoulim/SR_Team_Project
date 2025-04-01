// 내 클래스 이름 : Water
// 부모 클래스 이름 : AnimeRect

#pragma once
#include "AnimeRect.h"

BEGIN(Client)

class CWater final : public CAnimeRect
{
public:
	typedef struct tagWaterDesc : public CAnimeRect::DESC
	{

	}DESC;

private:
	CWater(LPDIRECT3DDEVICE9 pGraphic_Device);
	CWater(const CWater& Prototype);
	virtual ~CWater() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CWater* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END
