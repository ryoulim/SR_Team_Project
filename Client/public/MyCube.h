// �� Ŭ���� �̸� : MyCube
// �θ� Ŭ���� �̸� : Statue

#pragma once
#include "Statue.h"

BEGIN(Client)

class CMyCube final : public CStatue
{
public:
	typedef struct tagMyCubeDesc : public CStatue::DESC
	{

	}DESC;

private:
	CMyCube(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMyCube(const CMyCube& Prototype);
	virtual ~CMyCube() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CMyCube* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END