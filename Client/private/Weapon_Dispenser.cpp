// ³» Å¬·¡½º ÀÌ¸§ : Weapon_Dispenser
// ºÎ¸ð Å¬·¡½º ÀÌ¸§ : Weapon

#include "Weapon_Dispenser.h"
#include "UI_Manager.h"
#include "GrenadeBullet.h"
#include "FXMgr.h"

#define INITPOS {350.f,-150.f,0.1f}
#define GRENADEMODE 30

#define m_tShellInfo m_tAmmoInfo

CWeapon_Dispenser::CWeapon_Dispenser(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CWeapon{ pGraphic_Device }
{
}

CWeapon_Dispenser::CWeapon_Dispenser(const CWeapon_Dispenser& Prototype)
	: CWeapon(Prototype)
{
}

HRESULT CWeapon_Dispenser::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWeapon_Dispenser::Initialize(void* pArg)
{
	DESC Desc{};
	m_fTextureNum = 0.f;
	m_szTextureID = TEXT("Weapon_Dispenser");
	m_vMovingPos = INITPOS;
	
	///////
	m_tShellInfo.eType = DISPERSER_SHELL;
	m_tShellInfo.iCurAmmo = 50;
	m_tShellInfo.iMaxAmmo = 6;
	m_tShellInfo.iReloadedAmmo = 6;
	///////
	
	///////
	m_tGrenadeInfo.eType = DISPERSER_GRENADE;
	m_tGrenadeInfo.iCurAmmo = 40;
	m_tGrenadeInfo.iMaxAmmo = 6;
	m_tGrenadeInfo.iReloadedAmmo = 6;
	///////

	m_pCurAmmo = &m_tShellInfo;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_fRayLength = 1000.f;

	m_pSoundCom->SetVolume(0.6f);
	m_pSoundCom->SetVolume("apart_001", 0.5f);

	return S_OK;

}

void CWeapon_Dispenser::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CWeapon_Dispenser::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CWeapon_Dispenser::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CWeapon_Dispenser::Render()
{
	return __super::Render();
}

const CWeapon::AMMOINFO* CWeapon_Dispenser::Get_Info()
{
	if (m_bGrenadeMode)
		return &m_tGrenadeInfo;
	else
		return &m_tShellInfo;
}

HRESULT CWeapon_Dispenser::Ready_Components(void* pArg)
{
	__super::Ready_Components(pArg);

	/* For.Com_Sound */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Sound_Dispenser"),
		TEXT("Com_Sound"), reinterpret_cast<CComponent**>(&m_pSoundCom))))
		return E_FAIL;

	return S_OK;
}

void CWeapon_Dispenser::Set_State(STATE State)
{
	if (m_eState == State)
		return;

	m_eState = State;
	m_fMotionTimer = 0.f;

	switch (State)
	{
	case ST_IDLE:
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3 INITPOS);
		m_fStartFrmae = 0.f;
		m_fEndFrame = 0.f;
		m_fTextureNum = 0.f;
		break;
	case ST_OPENING:
		m_fStartFrmae = 0.f;
		m_fEndFrame = 0.f;
		m_fTextureNum = 0.f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3 INITPOS + _float3{ 0.f,-400.f,0.f });
		break;
	case ST_WALK:
		m_eState = ST_WALK;
		m_fTextureNum = 0.f;
		m_fStartFrmae = 0.f;
		m_fTextureNum = 0.f;
		break;
	case ST_W_ATK: // °ø°Ý
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, INITPOS);
		m_eState = ST_W_ATK;
		m_fFrameSpeed = 35.f;
		m_fTextureNum = 0.f;
		m_fStartFrmae = 0.f;
		m_fEndFrame = 5.f;
		break;
	case ST_S_ATK: // ÆßÇÎ(ÀÚµ¿)
	{
		m_pSoundCom->Play("reload_1");
		_float3 vPos = INITPOS;
		vPos.x -= 40.f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
		m_eState = ST_S_ATK;
		m_fTextureNum = 6.f;
		m_fStartFrmae = 6.f;
		m_fEndFrame = 15.f;
		m_fFrameSpeed = 20.f;
		break;
	}
	case ST_RELOAD:
	{
		m_pSoundCom->Play("apart_001");
		m_pSoundCom->Play("insert_001");
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 0.f,-50.f,0.f });
		m_eState = ST_RELOAD;
		m_fTextureNum = 16.f;
		m_fStartFrmae = 16.f;
		m_fEndFrame = 29.f;
		m_fFrameSpeed = 25.f;
		m_pCurAmmo->iReloadedAmmo = m_pCurAmmo->iMaxAmmo;
		break;
	}
	} // Switch

	if (m_bGrenadeMode)
	{
		m_fTextureNum += GRENADEMODE;
		m_fStartFrmae += GRENADEMODE;
		m_fEndFrame += GRENADEMODE;
	}

	_float3 vScale{};
	m_pTextureCom->Get_TextureSize(static_cast<_uint>(m_fTextureNum), &vScale);
	m_pTransformCom->Scaling(vScale);
}

void CWeapon_Dispenser::Key_Input()
{
	if (m_eState >= ST_OPENING)
		return;

	if (MOUSE_DOWN(DIMK_LBUTTON) && m_pCurAmmo->iCurAmmo > 0)
	{
		if (m_pCurAmmo->iReloadedAmmo)
		{
			Set_State(CWeapon::ST_W_ATK);
			Create_Bullet();
			m_pCurAmmo->iReloadedAmmo--;
			m_pCurAmmo->iCurAmmo--;
		}
		else
			Set_State(CWeapon::ST_RELOAD);
	}
	if (MOUSE_DOWN(DIMK_RBUTTON) && m_pCurAmmo->iCurAmmo > 0)
	{
		m_bGrenadeMode = !m_bGrenadeMode;

		// ¿©±â¼­ ÃÑ ¹Ù²Þ
		m_pCurAmmo->iCurAmmo -= m_pCurAmmo->iReloadedAmmo;
		m_pCurAmmo->iReloadedAmmo = m_pCurAmmo->iMaxAmmo;
		if (m_pCurAmmo->iCurAmmo < 0)
			m_pCurAmmo->iCurAmmo = 0;

		if (m_bGrenadeMode)
			m_pCurAmmo = &m_tGrenadeInfo;
		else
			m_pCurAmmo = &m_tShellInfo;

		CUI_Manager::Get_Instance()->Change_Weapon(m_pCurAmmo, 1);
		Set_State(ST_ENDING); // ¸ðµå º¯°æÀÓ
	}
	if (KEY_DOWN(DIK_R) && m_pCurAmmo->iCurAmmo > 0)
	{
		Set_State(ST_RELOAD);
	}
}

void CWeapon_Dispenser::Opening(_float fTimeDelta)
{
	m_pTransformCom->Go_Up(fTimeDelta);

	if (m_fMotionTimer > 0.15f)
	{
		//m_pTransformCom->Rotation_Reset();
		Set_State(ST_IDLE);
	}
}

void CWeapon_Dispenser::Weak_Attack(_float fTimeDelta)
{	
	// ½î°í Àá±ñ ´ë±â
	if (m_bTrigger)
	{
		if (m_fMotionTimer > 0.3f)
		{
			m_bTrigger = FALSE;
			Set_State(ST_S_ATK);
		}
	}
	else
	{
		if (Update_Frame(fTimeDelta))
			m_bTrigger = TRUE;
	}
}

void CWeapon_Dispenser::Strong_Attack(_float fTimeDelta) // ÆßÇÎ
{
	if (Update_Frame(fTimeDelta))
		Set_State(ST_IDLE);
}

void CWeapon_Dispenser::Reload(_float fTimeDelta)
{
	if (m_bTrigger)
	{
		if (Update_Frame_Reverse(fTimeDelta))
		{
			m_bTrigger = FALSE;
			Set_State(ST_IDLE);
		}

	}
	else
	{
		if (Update_Frame(fTimeDelta))
		{
			m_fTextureNum = m_fEndFrame + 0.9f;
			m_bTrigger = TRUE;
		}
	}
}

void CWeapon_Dispenser::Ending(_float fTimeDelta)
{
	Set_State(ST_RELOAD);
}

void CWeapon_Dispenser::Create_Bullet()
{
	// ÆøÅº»Ñ¸®±â
	if (m_bGrenadeMode)
	{
		m_pSoundCom->Play("glaunch");
		FX_MGR->SpawnShotGunFire(_float3{ 750.f, 450.f, 0.1f }, LEVEL_GAMEPLAY);
		CAMERA_MANAGER->StartRecoil(1.5f);
		_float3 vLook = m_pPlayerTransform->Get_State(CTransform::STATE_LOOK)->Normalize();
		_float3 vRight = m_pPlayerTransform->Get_State(CTransform::STATE_RIGHT)->Normalize();

		CGrenadeBullet::DESC BulletDesc{};
		BulletDesc.fSpeedPerSec = 300.f;
		BulletDesc.vScale = { 6.f,6.f,6.f };
		BulletDesc.vPosition = *m_pPlayerTransform->Get_State(CTransform::STATE_POSITION) 
			+ _float3{0.f,12.f,0.f} + vRight * 5.f + vLook * 5.f;
		BulletDesc.vLook = vLook;
		BulletDesc.ColliderGroup = CG_PBULLET;
		BulletDesc.fInitJumpPower = 30.f;
		BulletDesc.fTimeLimit = 10.f;

		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_GrenadeBullet"),
			LEVEL_STATIC, TEXT("Layer_Bullet"), &BulletDesc)))
			return;
	}
	// ¼¦°Ç
	else
	{
		m_pSoundCom->Play("fire_1_4");
		FX_MGR->SpawnShotGunFire(_float3{ 750.f, 450.f, 0.1f }, LEVEL_GAMEPLAY);
		FX_MGR->SpawnShotGunTracer(_float3{ 700.f, 400.f, 0.9f }, LEVEL_GAMEPLAY);
		CAMERA_MANAGER->StartRecoil(1.5f);

		_float3 pPos = *m_pCameraTransform->Get_State(CTransform::STATE_POSITION);
		_float3 pLook = *m_pCameraTransform->Get_State(CTransform::STATE_LOOK);
		_uint iColliderID{};

		_float3 vOffset{};
		for (_uint i = 0; i < 10; ++i)
		{
			vOffset = {
				m_pGameInstance->RandomFloat(-0.06f, 0.06f),
				m_pGameInstance->RandomFloat(-0.06f, 0.06f),
				m_pGameInstance->RandomFloat(-0.06f, 0.06f)
			};
			auto pPickedObj = m_pGameInstance->Raycast(pPos, pLook + vOffset, m_fRayLength, { CG_BLOCK,CG_MONSTER,CG_MONSTER_HEAD,CG_INTERACTIVE,CG_MBULLET }, iColliderID);
			if (pPickedObj)
			{
				pPickedObj->Get_Owner()->On_Collision(iColliderID, m_tShellInfo.eType);
			}
		}
	}
}

_bool CWeapon_Dispenser::Update_Frame_Reverse(_float fTimeDelta)
{
	m_fTextureNum -= m_fFrameSpeed * fTimeDelta;
	if (m_fStartFrmae > m_fTextureNum)
	{
		m_fTextureNum = m_fEndFrame;
		return TRUE;
	}
	return FALSE;

}

CWeapon_Dispenser* CWeapon_Dispenser::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CWeapon_Dispenser* pInstance = new CWeapon_Dispenser(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CWeapon_Dispenser");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWeapon_Dispenser::Clone(void* pArg)
{
	CWeapon_Dispenser* pInstance = new CWeapon_Dispenser(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CWeapon_Dispenser");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWeapon_Dispenser::Free()
{
	__super::Free();
}
