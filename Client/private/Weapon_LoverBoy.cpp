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
	Desc.fSpeedPerSec = 700.f;
	m_szTextureID = TEXT("Weapon_LoverBoy");

	Positioning();

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
	return __super::Update(fTimeDelta);
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
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vImagePosition[m_eState]);

	switch (State)
	{
	case ST_IDLE:
		m_eState = ST_IDLE;
		m_fTextureNum = 0.f;
		m_fStartFrmae = 0.f;
		m_fEndFrame = 0.f;
		break;
	case ST_OPENING:
		m_fTextureNum = 13.f;
		//m_pTransformCom->Quaternion_Rotation(RADIAN(_float3(0.f, 0.f, -00.f)));
		break;
	case ST_W_ATK:
		m_fFrameSpeed = 30.f;
		m_eState = ST_W_ATK;
		m_fTextureNum = 0.f;
		m_fStartFrmae = 0.f;
		m_fEndFrame = 6.f;
		break;
	case ST_S_ATK:
		break;
	case ST_RELOAD:
		m_fFrameSpeed = 15.f;
		m_eState = ST_RELOAD;
		m_fTextureNum = 6.f;
		m_fStartFrmae = 7.f;
		m_fEndFrame = 12.f;
		break;
	case ST_ENDING:
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
	m_pTransformCom->Quaternion_Revolution(_float3(0.f, 0.f, 1.f), m_vCenter, RADIAN(400.f) * fTimeDelta);
	m_vCenter.y -= 700.f * fTimeDelta;
	m_pTransformCom->Go_Down(fTimeDelta);

	if (m_fMotionTimer > 0.5f)
	{
		Set_State(ST_IDLE);
		m_pTransformCom->Rotation_Reset();
	}
}

void CWeapon_LoverBoy::Opening(_float fTimeDelta)
{
	//m_pTransformCom->Quaternion_Revolution(_float3(0.f, 0.f, 1.f), _float3(400.f, -300.f, 0.1f),RADIAN(2.f));
}

void CWeapon_LoverBoy::Walk(_float fTimeDelta)
{
}

void CWeapon_LoverBoy::Positioning()
{
	m_vImagePosition[ST_IDLE] = {320.f,-105.f,0.1f};
	m_vImagePosition[ST_OPENING] = { 320.f,-100.f,0.1f };
	m_vImagePosition[ST_WALK] = { 320.f,-105.f,0.1f };
	m_vImagePosition[ST_W_ATK] = { 320.f,-105.f,0.1f };
	m_vImagePosition[ST_S_ATK] = { 320.f,-105.f,0.1f };
	m_vImagePosition[ST_RELOAD] = { 320.f,-105.f,0.1f };
	m_vImagePosition[ST_ENDING] = { 320.f,-105.f,0.1f };
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