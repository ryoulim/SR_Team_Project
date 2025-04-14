// 내 클래스 이름 : Loading_Ending
// 부모 클래스 이름 : LoadingCutscene

#include "Loading_Ending.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CLoading_Ending::CLoading_Ending(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLoadingCutscene{ pGraphic_Device }
{
}

CLoading_Ending::CLoading_Ending(const CLoading_Ending& Prototype)
	: CLoadingCutscene(Prototype)
{
}

HRESULT CLoading_Ending::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLoading_Ending::Initialize(void* pArg)
{
	m_szTextureID = TEXT("Loading_Ending");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Get_TextureSize(0, &m_vSize)))
		return E_FAIL;

	m_vSize.x *= g_iWinSizeY / m_vSize.y; m_vSize.y = g_iWinSizeY;
	m_pTransformCom->Scaling(m_vSize);

	m_fDepth = UI_PRIORITY;

	return S_OK;
}


void CLoading_Ending::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CLoading_Ending::Update(_float fTimeDelta)
{
	if (KEY_DOWN(DIK_RETURN))
	{
		m_fTextureNum += 1.f;
		if (m_fTextureNum > 8.f)
			m_fTextureNum = 8.f;
	}

	if (KEY_DOWN(DIK_SPACE))
	{
		if (m_fTextureNum < 8.f)
		{
			m_fTextureNum = 8.f;
			// 게임 종료?
		}
		if (m_fTextureNum > 7.f)
			PostQuitMessage(0);

	}
	return CUI::Update(fTimeDelta);
}

void CLoading_Ending::Late_Update(_float fTimeDelta)
{
	CUI::Late_Update(fTimeDelta);
}

HRESULT CLoading_Ending::Render()
{
	Render_Background();
	CUI_Manager::Get_Instance()->Render_Text("Press Enter ...", CFont::MEDIUMBLUE, CFont::CENTER, 0.f, g_iWinSizeY * -0.5f + 30.f, 0.8f);
	return S_OK;
}

HRESULT CLoading_Ending::Render_Background()
{
	if (FAILED(m_pTextureCom->Bind_Resource(_int(m_fTextureNum))))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;
	return S_OK;
}

CLoading_Ending* CLoading_Ending::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLoading_Ending* pInstance = new CLoading_Ending(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLoading_Ending");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLoading_Ending::Clone(void* pArg)
{
	CLoading_Ending* pInstance = new CLoading_Ending(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CLoading_Ending");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoading_Ending::Free()
{
	__super::Free();
}