// 내 클래스 이름 : MyCube
// 부모 클래스 이름 : Statue

#pragma once
#include "Statue.h"

BEGIN(Client)

class CMyCube final : public CStatue
{
public:
	typedef struct tagMyCubeDesc : public CStatue::DESC
	{
		_float3 vLook;
	}DESC;

private:
	CMyCube(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMyCube(const CMyCube& Prototype);
	virtual ~CMyCube() = default;

public:
	virtual HRESULT Reset(void* pArg) override;
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	_float				m_fTimeAcc{};
	virtual HRESULT Ready_Components(void* pArg);

public:
	static CMyCube* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END