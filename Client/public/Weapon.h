#pragma once

#include "Client_Defines.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "CameraManager.h"

#define MOTION(Num,Duration) _uint(m_fMotionTimer / Duration) % Num

BEGIN(Client)

class CWeapon abstract : public CGameObject
{
public:
	enum STATE { ST_IDLE, ST_WALK, ST_OPENING, ST_W_ATK, ST_S_ATK, ST_RELOAD, ST_ENDING, ST_END };
	enum TYPE { // 여기 순서를 콜라이더와 맞추지 않는다면 아주 피곤한 일이 벌어질것
		LOVERBOY = CI_LOVERBOY,
		CHAINGUN,
		DISPERSER_SHELL,
		DISPERSER_GRENADE,
		PENETRATOR,
		BOWLINGBOMB,
		IONBOW,
		CLUSTERPUCK,
		AMMO_END
	};
public:
	typedef struct tagAmmoInfo
	{
		TYPE  eType;
		_uint iCurAmmo;
		_uint iMaxAmmo;
		_uint iReloadedAmmo;
	}AMMOINFO;

	typedef struct tagWeaponDesc : public CTransform::DESC
	{
		CTransform* pPlayerTransform;
		_float3 vInitPos;
		LEVEL	eLevelID;
	}DESC;

	virtual const AMMOINFO* Get_Info() {
		return &m_tAmmoInfo;
	}

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
	virtual void Walk(_float fTimeDelta);

	virtual void Set_State(STATE State) PURE;
	virtual void Key_Input();
	void Replenish_Ammo(_uint iNumber);

protected:
	virtual HRESULT Ready_Components(void* pArg);
	_bool Update_Frame(_float fTimeDelta);

	void Setup_RenderState();
	void Release_RenderState();
	void Picking_Object();
	void Mouse_Over();

protected:
	LEVEL	m_eLevelID{};
	const _tchar* m_szTextureID = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer* m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };

	CTransform* m_pPlayerTransform{ nullptr };
	CCameraManager* m_pCameraManager{ nullptr };
	CTransform* m_pCameraTransform{ nullptr };
	CSoundController* m_pSoundCom{ nullptr };

	static _uint		m_iPrePickedColliderID;
	static _uint		m_iPickedColliderID;
	static CCollider* m_pPrePickedCollider;
	static CCollider* m_pPickedCollider;

	AMMOINFO m_tAmmoInfo{};
	_float m_fFrameSpeed{};
	_float m_fStartFrmae{};
	_float m_fEndFrame{};
	_float m_fMotionTimer{};
	_float m_fWalkTimer{};
	_float m_fAlpha{ 1.f };

	STATE m_eState{ ST_IDLE };

	_float3 m_vImagePosition[ST_END];
	_float3 m_vCenter{};
	_float3 m_vMovingPos{};

	//발사용
	_float	m_fRayLength{1000.f};

protected:
	void Action(_float fTimeDelta);
	void Idle();
	virtual void Create_Bullet();

private:
	virtual void Opening(_float fTimeDelta) PURE;
	virtual void Weak_Attack(_float fTimeDelta) PURE;
	virtual void Strong_Attack(_float fTimeDelta) PURE;
	virtual void Reload(_float fTimeDelta) PURE;
	virtual void Ending(_float fTimeDelta) PURE;

public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free();
};

END