// 내 클래스 이름 : DebugMode
// 부모 클래스 이름 : GameObject

#include "DebugMode.h"

CDebugMode::CDebugMode(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CDebugMode::CDebugMode(const CDebugMode& Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CDebugMode::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDebugMode::Initialize(void* pArg)
{
	m_rcFPS = { g_iWinSizeX - 90, 0, g_iWinSizeX, 40 };
	return S_OK;
}

EVENT CDebugMode::Update(_float fTimeDelta)
{
 	m_fTimeAcc += fTimeDelta;
	m_iFPSConter++;
	if (m_fTimeAcc >= 1.f)
	{
		m_strFPS = TEXT("FPS :");
		m_strFPS += to_wstring(m_iFPSConter);
		m_fTimeAcc = 0.f;
		m_iFPSConter = 0;
	}

	return EVN_NONE;
}

void CDebugMode::Late_Update(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this)))
		return;
}


HRESULT CDebugMode::Render()
{
  	return m_pGameInstance->Draw_Font(m_strFPS, &m_rcFPS);
}

CDebugMode* CDebugMode::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CDebugMode* pInstance = new CDebugMode(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDebugMode");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDebugMode::Clone(void* pArg)
{
	CDebugMode* pInstance = new CDebugMode(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CDebugMode");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDebugMode::Free()
{
	__super::Free();
}