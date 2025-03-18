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
	Desc.vInitPos = {310.f,-312.f,0.1f};
	//Desc.fSpeedPerSec = 1.f;

	m_szTextureID = TEXT("Weapon_LoverBoy");

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	Positioning();

	Set_State(ST_IDLE);

	return S_OK;
}

void CWeapon_LoverBoy::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CWeapon_LoverBoy::Update(_float fTimeDelta)
{	
	//if (KEY_DOWN('1'))
	//{
	//	if (m_fTextureNum == 0.f)
	//	{
	//		m_fTextureNum = 15.f;
	//	}
	//	m_fTextureNum--;
	//	m_pTransformCom->Set_State(CTransform::STATE_POSITION, ImagePosition[(int)m_fTextureNum]);
	//}
	//if (KEY_DOWN('2'))
	//{
	//	m_fTextureNum++;
	//	if (m_fTextureNum > 14.f)
	//	{
	//		m_fTextureNum = 0.f;
	//	}
	//	m_pTransformCom->Set_State(CTransform::STATE_POSITION, ImagePosition[(int)m_fTextureNum]);
	//}
	//if (KEY_PRESSING(VK_LEFT))
	//{
	//	m_pTransformCom->Go_Left(1.f);
	//}
	//if (KEY_PRESSING(VK_UP))
	//{
	//	m_pTransformCom->Go_Up(1.f);

	//}
	//if (KEY_PRESSING(VK_RIGHT))
	//{
	//	m_pTransformCom->Go_Right(1.f);

	//}
	//if (KEY_PRESSING(VK_DOWN))
	//{
	//	m_pTransformCom->Go_Down(1.f);
	//}
	//if (KEY_DOWN(VK_RETURN))
	//{
	//	ImagePosition[(int)m_fTextureNum] = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	//}

	__super::Update(fTimeDelta);

	return EVN_NONE;
}

void CWeapon_LoverBoy::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CWeapon_LoverBoy::Render()
{
	//RECT rc = { g_iWinSizeX - 200, 40, g_iWinSizeX, 80 };

	//_wstring Buf{TEXT("FrameNum : ")};
	//Buf += to_wstring((_uint)m_fTextureNum);
	//m_pGameInstance->Draw_Font(Buf, &rc);

	//Buf = TEXT("POS : { ");
	//Buf += to_wstring((int)m_pTransformCom->Get_State(CTransform::STATE_POSITION)->x);
	//Buf += TEXT(", ");
	//Buf += to_wstring((int)m_pTransformCom->Get_State(CTransform::STATE_POSITION)->y);
	//Buf += TEXT(", ");
	//Buf += to_wstring((int)m_pTransformCom->Get_State(CTransform::STATE_POSITION)->z);
	//Buf += TEXT(" }");
	//rc = { g_iWinSizeX - 250, 80, g_iWinSizeX, 120 };
	//m_pGameInstance->Draw_Font(Buf, &rc);

	return __super::Render();
}

void CWeapon_LoverBoy::Set_State(STATE State)
{
	m_eState = State;

	switch (State)
	{
	case ST_IDLE:
		m_eState = ST_IDLE;
		m_fTextureNum = 0.f;
		m_fStartFrmae = 0.f;
		m_fEndFrame = 0.f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vImagePosition[(_uint)m_fTextureNum]);
		break;
	case ST_OPENING:
		break;
	case ST_W_ATK:
		m_fFrameSpeed = 30.f;
		m_eState = ST_W_ATK;
		m_fTextureNum = 0.f;
		m_fStartFrmae = 0.f;
		m_fEndFrame = 5.f;
		break;
	case ST_S_ATK:
		break;
	case ST_RELOAD:
		m_fFrameSpeed = 20.f;
		m_eState = ST_RELOAD;
		m_fTextureNum = 6.f;
		m_fStartFrmae = 6.f;
		m_fEndFrame = 14.f;
		break;
	default:
		break;
	}
}

void CWeapon_LoverBoy::Weak_Attack()
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vImagePosition[(_uint)m_fTextureNum]);
}

void CWeapon_LoverBoy::Strong_Attack()
{
}

void CWeapon_LoverBoy::Reload()
{
}

void CWeapon_LoverBoy::Opening()
{
	//m_pTransformCom->
}

void CWeapon_LoverBoy::Positioning()
{
	m_vImagePosition.resize(15);

	m_vImagePosition[0] = { 310,-312 ,0.1f };
	m_vImagePosition[1] = { 333,-364 ,0.1f };
	m_vImagePosition[2] = { 375,-286 ,0.1f };
	m_vImagePosition[3] = { 398,-253 ,0.1f };
	m_vImagePosition[4] = { 385,-305 ,0.1f };
	m_vImagePosition[5] = { 324,-300 ,0.1f };
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
}