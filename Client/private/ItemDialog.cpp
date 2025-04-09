// 내 클래스 이름 : ItemDialog
// 부모 클래스 이름 : UI

#include "ItemDialog.h"
#include "GameInstance.h"
#include <UI_Manager.h>

CItemDialog::CItemDialog(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI{ pGraphic_Device }
{
}

CItemDialog::CItemDialog(const CItemDialog& Prototype)
	: CUI(Prototype)
{
}

HRESULT CItemDialog::Initialize_Prototype()
{
	//Render_Text("armor fragment x4", CFont::LEFT, CFont::MEDIUMBLUE, -(g_iWinSizeX / 2.f) + 20.f, g_iWinSizeY / 2.f - 20.f);
	return S_OK;
}

HRESULT CItemDialog::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_STATIC;
	m_fPosX = -(g_iWinSizeX / 2.f) + 20.f;
	m_fPosY = g_iWinSizeY / 2.f;
	m_fDisplayTime = 3.f;

	Ready_ShaderComponent();

	return S_OK;
}

void CItemDialog::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CItemDialog::Update(_float fTimeDelta)
{
	queue<DIALOG> tempQueue;
	while (!m_DialogQueue.empty())
	{
		auto tDialog = m_DialogQueue.front();
		m_DialogQueue.pop();
		tDialog.fElapsedTime += fTimeDelta;
		if (tDialog.fElapsedTime < m_fDisplayTime)
		{
			tempQueue.push(tDialog);
		}
	}
	m_DialogQueue = tempQueue;
	return EVN_NONE;
}

void CItemDialog::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	if (m_pGameInstance->Get_CurrentLevelIndex() == LEVEL_GAMEPLAY)
		m_fPosY = g_iWinSizeY / 2.f - 60.f;
}

HRESULT CItemDialog::Render()
{
	int iIndex = 1;

	queue<DIALOG> tempQueue = m_DialogQueue;
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	while (!tempQueue.empty())
	{
		auto tDialog = tempQueue.front();
		// 세로로 길게 쓰기 싫어서 지피티한테 맡긴 한줄짜리 삼항연산자 코드니까 일부러 이해하려 할 필요 없음,,,
		_float fOpacity = (tDialog.fElapsedTime <= 0.3f) ? pow(tDialog.fElapsedTime / 0.3f, 2.0f) : (tDialog.fElapsedTime <= 2.7f) ? 1.0f : (tDialog.fElapsedTime <= 3.0f) ? 1.0f - pow((tDialog.fElapsedTime - 2.7f) / 0.5f, 2.0f) : 0.0f;
		m_pShaderCom->SetFloat("opacity", fOpacity);
		CUI_Manager::Get_Instance()->Render_Text(tDialog.strMessage, CFont::MEDIUMBLUE, CFont::LEFT,
												m_fPosX, m_fPosY - 22.f * iIndex++, 1.f, m_pShaderCom);
		tempQueue.pop();
	}
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return S_OK;
}

void CItemDialog::Insert_DialogQueue(const string& strMsg)
{
	DIALOG tDialog{};
	tDialog.strMessage = strMsg;
	tDialog.fElapsedTime = 0.f;
	m_DialogQueue.push(tDialog);
}

CItemDialog* CItemDialog::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CItemDialog* pInstance = new CItemDialog(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CItemDialog");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CItemDialog::Clone(void* pArg)
{
	CItemDialog* pInstance = new CItemDialog(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CItemDialog");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CItemDialog::Free()
{
	__super::Free();
}