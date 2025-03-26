// 내 클래스 이름 : Lava
// 부모 클래스 이름 : AnimeRect

#pragma once
#include "AnimeRect.h"

BEGIN(Client)

class CLava final : public CAnimeRect
{
public:
	typedef struct tagLavaDesc : public CAnimeRect::DESC
	{

	}DESC;

private:
	CLava(LPDIRECT3DDEVICE9 pGraphic_Device);
	CLava(const CLava& Prototype);
	virtual ~CLava() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CLava* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END