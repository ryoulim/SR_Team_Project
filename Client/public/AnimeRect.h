// 내 클래스 이름 : AnimeRect
// 부모 클래스 이름 : BackGround

#pragma once
#include "BackGround.h"

BEGIN(Client)

class CAnimeRect final : public CBackGround
{
public:
	typedef struct tagAnimeRectDesc : public CBackGround::DESC
	{

	}DESC;

private:
	CAnimeRect(LPDIRECT3DDEVICE9 pGraphic_Device);
	CAnimeRect(const CAnimeRect& Prototype);
	virtual ~CAnimeRect() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CAnimeRect* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END
