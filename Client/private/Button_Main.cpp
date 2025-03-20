// 내 클래스 이름 : Button_Main
// 부모 클래스 이름 : UI

#include "Button_Main.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "UI_Manager.h"

CButton_Main::CButton_Main(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI{ pGraphic_Device }
{
}

CButton_Main::CButton_Main(const CButton_Main& Prototype)
	: CUI(Prototype)
{
}

HRESULT CButton_Main::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CButton_Main::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_LOGO;
	m_szTextureID = TEXT("Button_Main");
	m_szBufferType = TEXT("Rect");

	//if (FAILED(__super::Initialize(pArg)))
	//	return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, _wstring(TEXT("Prototype_Component_VIBuffer_")) + m_szBufferType,
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), pArg)))
		return E_FAIL;

	if (pArg != nullptr)
	{
		DESC* pDesc = static_cast<DESC*>(pArg);
		m_eAlign = pDesc->eAlign;
		m_vPos = pDesc->vInitPos;
		m_vPos.z = 0.f;
		m_vSize = pDesc->vScale;
		m_vSize.z = 1.f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vPos);
		m_pTransformCom->Scaling(m_vSize);
		_float posx = m_vPos.x + g_iWinSizeX * 0.5f;
		_float posy = -(m_vPos.y - g_iWinSizeY * 0.5f);


		//m_tRect.left = LONG(posx);
		//m_tRect.top = LONG(posy - (m_vSize.y * 0.5f));
		//m_tRect.right = m_tRect.left + m_vSize.x;
		//m_tRect.bottom = m_tRect.top + m_vSize.y;
		Initialize_ButtonRect();
		/*__super::Update_Rect();*/
	}
	m_fAnimationFrame = 0.f;
	m_fDepth = 5.f;
	return S_OK;
}

void CButton_Main::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CButton_Main::Update(_float fTimeDelta)
{

	/*RENDER_ITEMDIALOG("menu picked", g_iWinSizeY / 2.f - 20.f);*/
	if (m_bNextLevel)
		m_fAnimationFrame += fTimeDelta*3.f;
	__super::Update(fTimeDelta);
	return EVN_NONE;
}

void CButton_Main::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CButton_Main::Render()
{
	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	_float2 vPos = { m_pTransformCom->Get_State(CTransform::STATE_POSITION)->x , m_pTransformCom->Get_State(CTransform::STATE_POSITION)->y };


	Pick_Button();


	return S_OK;
 
}

void CButton_Main::Initialize_ButtonRect()
{
	_float posx = m_vPos.x + g_iWinSizeX * 0.5f;
	_float posy = -(m_vPos.y - g_iWinSizeY * 0.5f);


	m_tButton[GAMESTART].left = LONG(posx);
	m_tButton[GAMESTART].top = LONG(posy - (m_vSize.y * 0.5f));
	m_tButton[GAMESTART].right = LONG(m_tButton[GAMESTART].left + m_vSize.x);
	m_tButton[GAMESTART].bottom = LONG(m_tButton[GAMESTART].top + m_vSize.y);

	m_tButton[OPTIONS].left = LONG(posx);
	m_tButton[OPTIONS].top = LONG(posy - (m_vSize.y * 0.5f) + 50.f);
	m_tButton[OPTIONS].right = LONG(m_tButton[OPTIONS].left + m_vSize.x);
	m_tButton[OPTIONS].bottom = LONG(m_tButton[OPTIONS].top + m_vSize.y);

	m_tButton[QUIT].left = LONG(posx);
	m_tButton[QUIT].top = LONG(posy - (m_vSize.y * 0.5f) + 100.f);
	m_tButton[QUIT].right  = LONG(m_tButton[QUIT].left + m_vSize.x);
	m_tButton[QUIT].bottom = LONG(m_tButton[QUIT].top + m_vSize.y);

}

HRESULT CButton_Main::Pick_Button()
{
	//140 20
	POINT			ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	//RENDER_TEXT_BOL("NEW GAME", 280.f - g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f - 460.f, 0.9f);
	//RENDER_TEXT_BOL("OPTIONS", 280.f - g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f - 510.f, 0.9f);
	//RENDER_TEXT_BOL("QUIT", 280.f - g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f - 560.f, 0.9f);


 	if (PtInRect(&m_tButton[GAMESTART], ptMouse))
	{
		RENDER_TEXT_BOL("NEW GAME", 280.f - g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f - 460.f, 0.9f);
		if (MOUSE_DOWN(DIMK_LBUTTON))
		{
			CUI_Manager::Get_Instance(m_pGameInstance)->Fade_Out();
			m_bNextLevel = true;

		}
	}
	else
	{
		RENDER_TEXT_BOL_DARK("NEW GAME", 280.f - g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f - 460.f, 0.9f);
	}

	if (PtInRect(&m_tButton[OPTIONS], ptMouse))
	{
		RENDER_TEXT_BOL("OPTIONS", 280.f - g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f - 510.f, 0.9f);
		if (MOUSE_DOWN(DIMK_LBUTTON))
		{
			/* 여기에 옵션 창 입력 */
			/* UI매니저에 추가? */
		}
	}
	else
	{
		RENDER_TEXT_BOL_DARK("OPTIONS", 280.f - g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f - 510.f, 0.9f);
	}

	if (PtInRect(&m_tButton[QUIT], ptMouse))
	{
		if (MOUSE_DOWN(DIMK_LBUTTON))
		{
			PostQuitMessage(0);
		}
		else
		{
			RENDER_TEXT_BOL("QUIT", 280.f - g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f - 560.f, 0.9f);
		}
	}
	else
	{
		RENDER_TEXT_BOL_DARK("QUIT", 280.f - g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f - 560.f, 0.9f);
	}

	if (m_fAnimationFrame > 1.7f)
	{
		if (FAILED(m_pGameInstance->Change_Level(LEVEL_LOADING,
			CLevel_Loading::Create(m_pGraphic_Device, LEVEL_GAMEPLAY))))
			return E_FAIL;
	}
	return S_OK;
}

CButton_Main* CButton_Main::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CButton_Main* pInstance = new CButton_Main(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CButton_Main");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CButton_Main::Clone(void* pArg)
{
	CButton_Main* pInstance = new CButton_Main(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CButton_Main");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CButton_Main::Free()
{
	__super::Free();
}