// 내 클래스 이름 : BossBridge
// 부모 클래스 이름 : Map

#pragma once
#include "Map.h"

BEGIN(Client)

class CBossBridge final : public CMap
{
public:
	typedef struct tagBossBridgeDesc : public CMap::DESC
	{

	}DESC;

private:
	CBossBridge(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBossBridge(const CBossBridge& Prototype);
	virtual ~CBossBridge() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	_bool m_bTrigger{};
	_bool m_bDoOnce{};
	_bool m_bCutSceneEnd[3] {};

	_float m_fTimeaAcc{};
	_float m_fFallTime = {};

	class CCameraManager* m_pCameraManager{ nullptr };
	CSoundController* m_pSoundCom{ nullptr };

private:
	virtual HRESULT Ready_Components(void* pArg) override;
	virtual void On_Trigger() override;

	void BossMap_CutScene1();
	void BossMap_CutScene2();
public:
	static CBossBridge* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END

