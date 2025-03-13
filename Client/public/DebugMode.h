// 내 클래스 이름 : DebugMode
// 부모 클래스 이름 : GameObject

#pragma once
#include "GameObject.h"
#include "Client_Defines.h"
#include "GameInstance.h"

BEGIN(Client)

class CDebugMode final : public CGameObject
{
private:
	CDebugMode(LPDIRECT3DDEVICE9 pGraphic_Device);
	CDebugMode(const CDebugMode& Prototype);
	virtual ~CDebugMode() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render() override;


private:
	_float  m_fTimeAcc{};
	_int	m_iFPSConter{};
	_wstring m_strFPS{};
	RECT	 m_rcFPS{};

public:
	static CDebugMode* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END