#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject(const CGameObject& Prototype);
	virtual ~CGameObject() = default;

public:
	virtual HRESULT Reset(void* pArg);
	/* 원형이 호출하는 함수. 오래걸릴 수 있는 초기화작업 */
	virtual HRESULT Initialize_Prototype();
	/* 사본이 호출하는 함수. */
	virtual HRESULT Initialize(void* pArg);

	virtual void Priority_Update(_float fTimeDelta);
	virtual EVENT Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

	void FrameUpdate(float timeDelta, float _MaxFrame, bool isLoop);


	class CComponent* Find_Component(const _wstring& strComponentTag);
	virtual void On_Collision(CGameObject* pCollisionedObject, const _wstring& strLayerTag) {};

protected:
	LPDIRECT3DDEVICE9				m_pGraphic_Device = { nullptr };
	class CGameInstance*			m_pGameInstance = { nullptr };

protected:
	unordered_map<_wstring, class CComponent*>	m_Components;
	_float							m_fTextureNum = {};

protected:
	HRESULT Add_Component(_uint iLevelIndex, const _wstring& strPrototypeTag, const _wstring& strComponentTag, CComponent** ppOut, void* pArg = nullptr);
public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free();

protected:
	float m_fAnimationFrame = 0;
	float m_fAnimationMaxFrame = 0;
};

END