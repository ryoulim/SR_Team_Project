// 내 클래스 이름 : Button
// 부모 클래스 이름 : UI

#include "Button.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "UI_Manager.h"

CButton::CButton(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI{ pGraphic_Device }
{
}

CButton::CButton(const CButton& Prototype)
	: CUI(Prototype)
{
}

HRESULT CButton::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CButton::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_LOGO;
	m_szTextureID = TEXT("Button");
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
		__super::Update_Rect();
	}


	return S_OK;
}

void CButton::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CButton::Update(_float fTimeDelta)
{

	
		/*RENDER_ITEMDIALOG("menu picked", g_iWinSizeY / 2.f - 20.f);*/

	__super::Update(fTimeDelta);
	return EVN_NONE;
}

void CButton::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CButton::Render()
{
	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	_float2 vPos = { m_pTransformCom->Get_State(CTransform::STATE_POSITION)->x , m_pTransformCom->Get_State(CTransform::STATE_POSITION)->y };
	
	// 작은 글씨 너비 여백 출력 오류 있음 *** 수정필요 
	RENDER_TEXT_BOL("NEW GAME",		280.f - g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f - 460.f, 0.6f);
	RENDER_TEXT_BOL("CONT INUE",	280.f - g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f - 490.f, 0.6f);
	RENDER_TEXT_BOL("OPT IONS",		280.f - g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f - 520.f, 0.6f);
	RENDER_TEXT_BOL("READ ME !",	280.f - g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f - 550.f, 0.6f);
	RENDER_TEXT_BOL("QU IT",		280.f - g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f - 580.f, 0.6f);

	//140 20
	POINT			ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);


	if (PtInRect(&m_tRect, ptMouse))
	{
		CUI_Manager::Get_Instance(m_pGameInstance)->Render_Text(
			"New game P icked !",
			CFont::BIGORANGE,
			CFont::LEFT,
			0.f,
			0.f);
		if (KEY_DOWN(VK_LBUTTON))
		{
			if (FAILED(m_pGameInstance->Change_Level(LEVEL_LOADING,
				CLevel_Loading::Create(m_pGraphic_Device, LEVEL_GAMEPLAY))))
				return E_FAIL;
		}
	}

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;
	return S_OK;
 
}

CButton* CButton::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CButton* pInstance = new CButton(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CButton::Clone(void* pArg)
{
	CButton* pInstance = new CButton(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CButton::Free()
{
	__super::Free();
}