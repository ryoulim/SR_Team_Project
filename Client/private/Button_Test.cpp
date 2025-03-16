// 내 클래스 이름 : Button_Test
// 부모 클래스 이름 : Button

#include "Button_Test.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CButton_Test::CButton_Test(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CButton{ pGraphic_Device }
{
}

CButton_Test::CButton_Test(const CButton_Test& Prototype)
	: CButton(Prototype)
{
}

HRESULT CButton_Test::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CButton_Test::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_LOGO;
	m_szTextureID = TEXT("Button_Test");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CButton_Test::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CButton_Test::Update(_float fTimeDelta)
{

	POINT			ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);


	if (PtInRect(&m_tRect, ptMouse))
		RENDER_ITEMDIALOG("menu picked", g_iWinSizeY / 2.f - 20.f);

	return __super::Update(fTimeDelta);
}

void CButton_Test::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CButton_Test::Render()
{
	return __super::Render();
}

CButton_Test* CButton_Test::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CButton_Test* pInstance = new CButton_Test(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CButton_Test");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CButton_Test::Clone(void* pArg)
{
	CButton_Test* pInstance = new CButton_Test(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CButton_Test");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CButton_Test::Free()
{
	__super::Free();
}