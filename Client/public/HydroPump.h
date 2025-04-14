// 내 클래스 이름 : HydroPump
// 부모 클래스 이름 : Effect

#pragma once
#include "Effect.h"

BEGIN(Client)

class CHydroPump final : public CEffect
{
public:
	typedef struct tagHydroPumpDesc : public CEffect::DESC
	{
		_float fMaxFrame;
		_float fAniSpeed;
		_float m_fDeadTime;
		LEVEL eLevelID;
	}DESC;

private:
	CHydroPump(LPDIRECT3DDEVICE9 pGraphic_Device);
	CHydroPump(const CHydroPump& Prototype);
	virtual ~CHydroPump() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	
private:
	_float m_fDeadTime = {};
	_float m_fTimeAcc = {};


private:
	class CCameraManager* m_pCameraManager = { nullptr };
	CSound_Core* m_pBGM = { nullptr };

public:
	static CHydroPump* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END