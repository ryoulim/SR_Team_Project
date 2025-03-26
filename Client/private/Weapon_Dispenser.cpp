// ³» Å¬·¡½º ÀÌ¸§ : Weapon_Dispenser
// ºÎ¸ð Å¬·¡½º ÀÌ¸§ : Weapon

#include "Weapon_Dispenser.h"

#define INITPOS {350.f,-200.f,0.1f}
#define GRENADEMODE 30

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
	Desc.fSpeedPerSec = 2600.f;
	m_szTextureID = TEXT("Weapon_Dispenser");
	m_vMovingPos = INITPOS;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

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
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 0.f,0.f,0.f });
		m_eState = ST_RELOAD;
		m_fTextureNum = 16.f;
		m_fStartFrmae = 16.f;
		m_fEndFrame = 29.f;
		m_fFrameSpeed = 20.f;
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
	if (MOUSE_DOWN(DIMK_LBUTTON))
	{
		Set_State(ST_W_ATK);
	}
	if (MOUSE_DOWN(DIMK_RBUTTON))
	{
		m_bGrenadeMode = !m_bGrenadeMode;
		Set_State(ST_ENDING); // ¸ðµå º¯°æÀÓ
	}
	if (KEY_DOWN(DIK_R))
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
	Set_State(ST_OPENING);
}

void CWeapon_Dispenser::Create_Bullet()
{
	// ÆøÅº»Ñ¸®±â
	if (m_bGrenadeMode)
	{

	}
	// ¼¦°Ç
	else
	{

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
