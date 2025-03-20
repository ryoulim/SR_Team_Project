// 내 클래스 이름 : Weapon_Chaingun
// 부모 클래스 이름 : Weapon

#include "Weapon_Chaingun.h"
#define INITPOS {170.f,-205.f,0.1f}
#define HEADPOS {170.f - 35.f, -205.f + 72.5f, 0.1f}

CWeapon_Chaingun::CWeapon_Chaingun(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CWeapon{ pGraphic_Device }
{
}

CWeapon_Chaingun::CWeapon_Chaingun(const CWeapon_Chaingun& Prototype)
	: CWeapon(Prototype)
{
}

HRESULT CWeapon_Chaingun::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWeapon_Chaingun::Initialize(void* pArg)
{
	DESC Desc{};
	m_fTextureNum = 0.f;
	Desc.fSpeedPerSec = 600.f;
	m_szTextureID = TEXT("Weapon_Chaingun");

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	return S_OK;
}

void CWeapon_Chaingun::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CWeapon_Chaingun::Update(_float fTimeDelta)
{
	Update_Frame(fTimeDelta);
	return __super::Update(fTimeDelta);
}

void CWeapon_Chaingun::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CWeapon_Chaingun::Render()
{
	Body_Render();
	if (m_eState == ST_W_ATK ||
		m_eState == ST_S_ATK)
		__super::Render();

	return S_OK;
}

void CWeapon_Chaingun::Walk(_float fTimeDelta)
{
	if (m_eState == ST_IDLE)
		Set_State(ST_WALK);
	else if (m_eState != ST_WALK)
		return;

#define WALKX 3.f
#define WALKY 2.f
	switch (MOTION(4, 0.175f))
	{
	case 0:
		m_pBodyTransformCom->Move({ -WALKX,-WALKY, 0.f });
		break;
	case 1:
		m_pBodyTransformCom->Move({ +WALKX,+WALKY, 0.f });
		break;
	case 2:
		m_pBodyTransformCom->Move({ +WALKX,-WALKY, 0.f });
		break;
	case 3:
		m_pBodyTransformCom->Move({ -WALKX,+WALKY, 0.f });
		break;
	default:
		break;
	}
}
void CWeapon_Chaingun::Opening(_float fTimeDelta)
{
	m_pBodyTransformCom->Quaternion_Revolution(_float3(0.f, 0.f, 1.f), m_vCenter, -RADIAN(700.f) * fTimeDelta);


	if (m_fMotionTimer > 0.15f)
	{
		m_pBodyTransformCom->Rotation_Reset();
		Set_State(ST_IDLE);
	}
}

void CWeapon_Chaingun::Set_State(STATE State)
{
	if (m_eState == State)
		return;

	m_eState = State;
	m_fMotionTimer = 0.f;
	m_pBodyTransformCom->Set_State(CTransform::STATE_POSITION, INITPOS);

	switch (State)
	{
	case ST_IDLE:
		m_eState = ST_IDLE;
		m_fTextureNum = 1.f;
		m_fStartFrmae = 1.f;
		m_fEndFrame = 1.f;
		m_iBodynum = 15;
		break;
	case ST_OPENING:
		m_iBodynum = 15;
		m_vCenter = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		m_vCenter.y = -FWINCY * 0.7f;
		m_pBodyTransformCom->Quaternion_Revolution({ 0.f,0.f,1.f }, m_vCenter, RADIAN(90.f));
		break;
	case ST_WALK:
		m_eState = ST_WALK;
		m_iBodynum = 15;
		break;
	case ST_W_ATK: // 진짜 발사
		m_eState = ST_W_ATK;
		m_fTextureNum = 8.f;
		m_fStartFrmae = 8.f;
		m_fEndFrame = 14.f;
		m_fFrameSpeed = 40.f;
		m_iBodynum = 0;
		break;
	case ST_S_ATK: // 예열
		m_iBodynum = 0;
		m_eState = ST_S_ATK;
		m_fTextureNum = 1.f;
		m_fStartFrmae = 1.f;
		m_fEndFrame = 7.f;
		m_fFrameSpeed = 20.f;
		break;
	case ST_RELOAD: // 기관총에 장전따윈 없다
		break;
	case ST_ENDING:
		break;
	default:
		break;
	}
}

void CWeapon_Chaingun::Key_Input()
{
	if (MOUSE_DOWN(DIMK_LBUTTON))
	{
		Set_State(ST_W_ATK);
	}
	if (MOUSE_DOWN(DIMK_RBUTTON))
	{
		Set_State(ST_S_ATK);
	}
}

HRESULT CWeapon_Chaingun::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	/* For.Com_BodyTransform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_BodyTransform"), reinterpret_cast<CComponent**>(&m_pBodyTransformCom))))
		return E_FAIL;

	_float3 vSize{};
	m_pTextureCom->Get_TextureSize(0, &vSize);
	m_pBodyTransformCom->Scaling(vSize*1.25f);
	m_pBodyTransformCom->Set_State(CTransform::STATE_POSITION,INITPOS);

	m_pTextureCom->Get_TextureSize(1, &vSize);
	m_pTransformCom->Scaling(vSize * 1.25f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, HEADPOS);
	
	return S_OK;
}

void CWeapon_Chaingun::Weak_Attack(_float fTimeDelta)
{
	// 총 발사
	Update_Frame(fTimeDelta);

	if (m_fMotionTimer < 1.f)
	{
		m_eState = ST_S_ATK;
		m_fTextureNum = 1.f;
		m_fStartFrmae = 1.f;
		m_fEndFrame = 7.f;
		m_fFrameSpeed = 20.f;
		return;
	}

#define AMPX 6.f
#define AMPY 6.f
#define AMPSPEED 80.f

	_float3 vInter = { sinf(m_fMotionTimer * AMPSPEED) * AMPX, -sinf(m_fMotionTimer * AMPSPEED) * AMPY, 0.f };

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(HEADPOS) + vInter);
	m_pBodyTransformCom->Set_State(CTransform::STATE_POSITION, _float3(INITPOS) + vInter);

	if (MOUSE_UP(DIMK_LBUTTON))
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, HEADPOS);
		m_pBodyTransformCom->Set_State(CTransform::STATE_POSITION, INITPOS);
		Set_State(ST_IDLE);
	}
}

void CWeapon_Chaingun::Strong_Attack(_float fTimeDelta)
{
	// 총열 예열
	Update_Frame(fTimeDelta);

	if (m_fMotionTimer > 1.f && MOUSE_PRESSING(DIMK_LBUTTON))
	{
		m_eState = ST_W_ATK;
		m_fTextureNum = 8.f;
		m_fStartFrmae = 8.f;
		m_fEndFrame = 14.f;
		m_fFrameSpeed = 40.f;
		return;
	}
	
	
	if (MOUSE_UP(DIMK_RBUTTON))
		Set_State(ST_IDLE);
}

void CWeapon_Chaingun::Ending(_float fTimeDelta)
{
}

void CWeapon_Chaingun::Body_Render()
{
	m_pTextureCom->Bind_Resource(m_iBodynum);

	m_pBodyTransformCom->Bind_Resource();

	m_pVIBufferCom->Bind_Buffers();

	m_pVIBufferCom->Render();
}

CWeapon_Chaingun* CWeapon_Chaingun::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CWeapon_Chaingun* pInstance = new CWeapon_Chaingun(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CWeapon_Chaingun");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWeapon_Chaingun::Clone(void* pArg)
{
	CWeapon_Chaingun* pInstance = new CWeapon_Chaingun(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CWeapon_Chaingun");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWeapon_Chaingun::Free()
{
	Safe_Release(m_pBodyTransformCom);
	__super::Free();
}