#pragma once

#include "Client_Defines.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "UI_Manager.h"

BEGIN(Client)

class CWeapon abstract : public CGameObject
{
public:
	enum STATE { ST_IDLE, ST_OPENING,ST_WALK, ST_W_ATK, ST_S_ATK, ST_RELOAD, ST_END };
public:
	typedef struct tagWeaponDesc : public CTransform::DESC
	{
		_float3 vInitPos;
	}DESC;

protected:
	CWeapon(LPDIRECT3DDEVICE9 pGraphic_Device);
	CWeapon(const CWeapon& Prototype);
	virtual ~CWeapon() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void Set_State(STATE State) PURE;
	_uint	Get_Ammo() { return m_iAmmo; }

protected:
	virtual HRESULT Ready_Components(void* pArg);
	void Update_Frame(_float fTimeDelta);

protected:
	const _tchar* m_szTextureID = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer* m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };

	vector<_float3> m_vImagePosition;

	_uint m_iAmmo{};
	_float m_fFrameSpeed{};
	_float m_fStartFrmae{};
	_float m_fEndFrame{};
	_float m_fMotionTimer{};

	STATE m_eState{};

protected:
	void Action(_float fTimeDelta);
	void Idle();

private:
	virtual void Opening() PURE;
	virtual void Weak_Attack() PURE;
	virtual void Strong_Attack() PURE;

virtual void Reload() PURE; public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free();
};

END
