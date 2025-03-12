// 내 클래스 이름 : Common_Shape
// 부모 클래스 이름 : Statue

#pragma once
#include "Statue.h"

BEGIN(Client)

class CCommon_Shape final : public CStatue
{
public:
	typedef struct tagCommon_ShapeDesc : public CStatue::DESC
	{
		LEVEL eLevelID;
		const _tchar* szTextureID;
		const _tchar* szBufferType;
	}DESC;

private:
	CCommon_Shape(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCommon_Shape(const CCommon_Shape& Prototype);
	virtual ~CCommon_Shape() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CCommon_Shape* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END