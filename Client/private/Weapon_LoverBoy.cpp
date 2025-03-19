// 내 클래스 이름 : Weapon_LoverBoy
// 부모 클래스 이름 : Weapon

#include "Weapon_LoverBoy.h"

CWeapon_LoverBoy::CWeapon_LoverBoy(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CWeapon{ pGraphic_Device }
{
}

CWeapon_LoverBoy::CWeapon_LoverBoy(const CWeapon_LoverBoy& Prototype)
	: CWeapon(Prototype)
{
}

HRESULT CWeapon_LoverBoy::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWeapon_LoverBoy::Initialize(void* pArg)
{
	DESC Desc{};
	m_fTextureNum = 0.f;
	Desc.fSpeedPerSec = 600.f;
	m_szTextureID = TEXT("Weapon_LoverBoy");

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	return S_OK;
}

void CWeapon_LoverBoy::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CWeapon_LoverBoy::Update(_float fTimeDelta)
{	

	if (KEY_DOWN('4'))
	{
		UPDATE_AMMO(CAmmo::LOVERBOY);
	}

	__super::Update(fTimeDelta);

	return EVN_NONE;
}

void CWeapon_LoverBoy::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CWeapon_LoverBoy::Render()
{
	if (m_eState == ST_S_ATK)
	{
		Left_Hand_Render();
	}
	return __super::Render();
}

void CWeapon_LoverBoy::Set_State(STATE State)
{
	if (m_eState == State)
		return;

	m_eState = State;
	m_fMotionTimer = 0.f;

	switch (State)
	{
	case ST_IDLE:
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 320.f,-105.f,0.1f });
		m_eState = ST_IDLE;
		m_fTextureNum = 0.f;
		m_fStartFrmae = 0.f;
		m_fEndFrame = 0.f;
		break;
	case ST_OPENING:
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 219.f,-321.f,0.1f });
		m_fTextureNum = 13.f;
		m_vCenter = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		m_vCenter.y = -FWINCY * 0.7f;
		m_pTransformCom->Quaternion_Revolution({ 0.f,0.f,1.f }, m_vCenter, RADIAN(90.f));
		break;
	case ST_WALK:
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 320.f,-105.f,0.1f });
		break;
	case ST_W_ATK:
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 320.f,-105.f,0.1f });
		m_fFrameSpeed = 27.f;
		m_eState = ST_W_ATK;
		m_fTextureNum = 0.f;
		m_fStartFrmae = 0.f;
		m_fEndFrame = 6.f;
		break;
	case ST_S_ATK:
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 320.f,-105.f,0.1f });
		break;
	case ST_RELOAD:
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 320.f,-105.f,0.1f });
		m_fFrameSpeed = 15.f;
		m_eState = ST_RELOAD;
		m_fTextureNum = 6.f;
		m_fStartFrmae = 7.f;
		m_fEndFrame = 12.f;
		break;
	case ST_ENDING:
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 320.f,-105.f,0.1f });
		m_eState = ST_ENDING;
		m_fTextureNum = 12.f;
		m_vCenter = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		m_vCenter.y = -FWINCY*1.4f;
		break;
	default:
		break;
	}

	_float3 vScale{};
	m_pTextureCom->Get_TextureSize(static_cast<_uint>(m_fTextureNum), &vScale);
	m_pTransformCom->Scaling(vScale);
}

void CWeapon_LoverBoy::Weak_Attack(_float fTimeDelta)
{
	if (Update_Frame(fTimeDelta))
		Set_State(ST_IDLE);
}

void CWeapon_LoverBoy::Strong_Attack(_float fTimeDelta)
{
}

void CWeapon_LoverBoy::Reload(_float fTimeDelta)
{
	if (Update_Frame(fTimeDelta))
		Set_State(ST_ENDING);
}

void CWeapon_LoverBoy::Ending(_float fTimeDelta)
{
	m_pTransformCom->Quaternion_Revolution(_float3(0.f, 0.f, 1.f), m_vCenter, RADIAN(350.f) * fTimeDelta);
	m_vCenter.y -= 600.f * fTimeDelta;
	m_pTransformCom->Go_Down(fTimeDelta);

	if (m_fMotionTimer > 0.8f)
	{
		m_pTransformCom->Rotation_Reset();
		Set_State(ST_OPENING);
	}
}

void CWeapon_LoverBoy::Opening(_float fTimeDelta)
{
	m_pTransformCom->Quaternion_Revolution(_float3(0.f, 0.f, 1.f), m_vCenter, -RADIAN(350.f) * fTimeDelta);
	//m_vCenter.y -= 600.f * fTimeDelta;
	//m_pTransformCom->Go_Up(fTimeDelta);

	if (m_fMotionTimer > 0.3f)
	{
		m_pTransformCom->Rotation_Reset();
		Set_State(ST_IDLE);
	}
}

void CWeapon_LoverBoy::Walk(_float fTimeDelta)
{
#define WALKX 1.5f
#define WALKY 1.f
	switch (MOTION(4, 0.35f))
	{
	case 0:
		m_pTransformCom->Move({ -WALKX,-WALKY,0.f });
		break;
	case 1:
		m_pTransformCom->Move({ +WALKX,+WALKY,0.f });
		break;
	case 2:
		m_pTransformCom->Move({ +WALKX,-WALKY,0.f });
		break;
	case 3: 
		m_pTransformCom->Move({ -WALKX,+WALKY,0.f });
		break;
	default:
		break;
	}		
}

void CWeapon_LoverBoy::Left_Hand_Render()
{
	m_LeftHand.pTextureCom->Bind_Resource(0);
}

HRESULT CWeapon_LoverBoy::Ready_Components(void* pArg)
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, _wstring(TEXT("Prototype_Component_Texture_LeftHand")),
		TEXT("Com_Left_Texture"), reinterpret_cast<CComponent**>(&m_LeftHand.pTextureCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, _wstring(TEXT("Prototype_Component_VIBuffer_Rect")),
		TEXT("Com_Left_VIBuffer"), reinterpret_cast<CComponent**>(&m_LeftHand.pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Left_Transform"), reinterpret_cast<CComponent**>(&m_LeftHand.pTransformCom))))
		return E_FAIL;

	return __super::Ready_Components(pArg);
}

CWeapon_LoverBoy* CWeapon_LoverBoy::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CWeapon_LoverBoy* pInstance = new CWeapon_LoverBoy(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CWeapon_LoverBoy");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWeapon_LoverBoy::Clone(void* pArg)
{
	CWeapon_LoverBoy* pInstance = new CWeapon_LoverBoy(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CWeapon_LoverBoy");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWeapon_LoverBoy::Free()
{
	__super::Free();

	Safe_Release(m_LeftHand.pTextureCom);
	Safe_Release(m_LeftHand.pTransformCom);
	Safe_Release(m_LeftHand.pVIBufferCom);
}