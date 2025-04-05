// 내 클래스 이름 : PhotoFrame
// 부모 클래스 이름 : Map

#pragma once
#include "Map.h"

BEGIN(Client)

class CPhotoFrame final : public CMap
{
public:
	typedef struct tagPhotoFrameDesc : public CMap::DESC
	{

	}DESC;

private:
	CPhotoFrame(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPhotoFrame(const CPhotoFrame& Prototype);
	virtual ~CPhotoFrame() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	virtual HRESULT Ready_Components(void* pArg) override;
	virtual void On_Collision(_uint MyColliderID, _uint OtherColliderID)override;

private:
	_bool m_bBroken = { false };

public:
	static CPhotoFrame* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END